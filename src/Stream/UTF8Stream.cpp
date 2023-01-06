/**
 *  Copyright (C) 2021 - 2023 The mrhpsspeech Authors.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

// C / C++
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>
#include <poll.h>
#include <errno.h>
#include <cstring>

// External
#include <libmrhevdata/Version/1/MRH_EvListen_V1.h> // MRH_EVD_L_STRING_BUFFER_MAX

// Project
#include "./UTF8Stream.h"
#include "../Logger.h"

// Pre-defined
#ifndef MRH_SPEECHD_CONNECT_WAIT_S
    #define MRH_SPEECHD_CONNECT_WAIT_S 5
#endif


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

UTF8Stream::UTF8Stream(std::string const& s_SocketPath,
                       std::shared_ptr<DataNotifier>& p_Notifier) : b_Read(true),
                                                                    s_SocketPath(s_SocketPath),
                                                                    i_FD(-1),
                                                                    p_Notifier(p_Notifier)
{
    // Start reader thread
    try
    {
        c_Thread = std::thread(Read, this);
    }
    catch (std::exception& e)
    {
        throw Exception("Failed to start socket read thread: " +
                        std::string(e.what()));
    }

    Logger::Singleton().Log(Logger::INFO, "Started stream.",
                            "UTF8Stream.cpp", __LINE__);
}

UTF8Stream::~UTF8Stream() noexcept
{
    b_Read = false;

    if (i_FD >= 0)
    {
        shutdown(i_FD, SHUT_RDWR);
        close(i_FD);
    }

    c_Thread.join();
}

//*************************************************************************************
// Connection
//*************************************************************************************

void UTF8Stream::Connect()
{
    // Connect to socket
    int i_FD;
    struct sockaddr_un c_Address;
    socklen_t us_AddressLength;

    if ((i_FD = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
    {
        throw Exception("Could not create socket: " +
                        std::string(std::strerror(errno)) +
                        " (" +
                        std::to_string(errno) +
                        ")");
    }

    us_AddressLength = sizeof(c_Address);
    memset(&c_Address, 0, us_AddressLength);
    c_Address.sun_family = AF_UNIX;
    strcpy(c_Address.sun_path, s_SocketPath.c_str());

    if (connect(i_FD, (struct sockaddr*)&c_Address, us_AddressLength) < 0)
    {
        close(i_FD);
        
        throw Exception("Could not connect to socket: " +
                        std::string(std::strerror(errno)) +
                        " (" +
                        std::to_string(errno) +
                        ")");
    }
    else if (fcntl(i_FD, F_SETFL, fcntl(i_FD, F_GETFL, 0) | O_NONBLOCK) < 0)
    {
        shutdown(i_FD, SHUT_RDWR);
        close(i_FD);

        throw Exception("Could not set socket non-blocking: " +
                        std::string(std::strerror(errno)) +
                        " (" +
                        std::to_string(errno) +
                        ")");
    }

    this->i_FD = i_FD;
}

void UTF8Stream::Disconnect() noexcept
{
    if (i_FD < 0)
    {
        return;
    }

    shutdown(i_FD, SHUT_RDWR);
    close(i_FD);

    i_FD = -1;

    Logger::Singleton().Log(Logger::INFO, "Closed connection.",
                            "UTF8Stream.cpp", __LINE__);

    p_Notifier->Notify(true); // Notify for method switch
}

//*************************************************************************************
// Read
//*************************************************************************************

bool UTF8Stream::PollSocket(int i_FD)
{
    struct pollfd c_PollFD;

    c_PollFD.fd = i_FD;
    c_PollFD.events = POLLIN;

    int i_Result = poll(&c_PollFD, (nfds_t)1, -1);

    switch (i_Result)
    {
        case -1:
            throw Exception("Failed to poll socket: " +
                            std::string(std::strerror(errno)) +
                            " (" +
                            std::to_string(errno) +
                            ")!");

        case 0:
            return false;

        default:
            if (c_PollFD.revents & POLLERR || /* Polling error */
                c_PollFD.revents & POLLHUP || /* Other end closed */
                c_PollFD.revents & POLLNVAL)  /* EBADF */
            {
                throw Exception("Failed to poll socket: Returned event " +
                                std::to_string(c_PollFD.revents) +
                                "!");
            }
            return true;
    }
}

ssize_t UTF8Stream::ReadSocket(int i_FD, char* p_Buffer, size_t us_Length)
{
    ssize_t ss_Total = 0;
    ssize_t ss_Result;

    while (ss_Total < us_Length)
    {
        ss_Result = read(i_FD, &(p_Buffer[ss_Total]), us_Length - ss_Total);

        if (ss_Result < 0)
        {
            switch (errno)
            {
#if EAGAIN != EWOULDBLOCK
                case EWOULDBLOCK:
#endif
                case EAGAIN:
                case EINTR:
                {
                    // No data, end
                    return ss_Total;
                }

                default:
                {
                    throw Exception("Could not read socket: " +
                                    std::string(std::strerror(errno)) +
                                    " (" +
                                    std::to_string(errno) +
                                    ")!");
                }
            }
        }
        else if (ss_Result == 0)  // EOF
        {
            throw Exception("Could not read socket: EOF reached!");
        }
        else
        {
            ss_Total += ss_Result;
        }
    }

    return ss_Total;
}

void UTF8Stream::Read(UTF8Stream* p_Instance) noexcept
{
    Logger& c_Logger = Logger::Singleton();

    // Buffer size matches max string message data
    char p_Buffer[MRH_EVD_L_STRING_BUFFER_MAX] = { '\0' };
    size_t us_BufferPos = 0;

    while (p_Instance->b_Read == true)
    {
        /**
         *  Connect
         */

        if (p_Instance->i_FD < 0)
        {
            // Wait before connecting, so that socket spawns
            std::this_thread::sleep_for(std::chrono::seconds(MRH_SPEECHD_CONNECT_WAIT_S));

            c_Logger.Log(Logger::INFO, "Attempting to connect to socket " +
                                       p_Instance->s_SocketPath +
                                       "...",
                         "UTF8Stream.cpp", __LINE__);

            // Attempt to connect
            try
            {
                // Attempt to connect
                p_Instance->Connect();

                // New connection, notify all
                p_Instance->p_Notifier->Notify(true);
            }
            catch (Exception& e)
            {
                c_Logger.Log(Logger::ERROR, e.what(),
                             "UTF8Stream.cpp", __LINE__);
                continue;
            }
        }

        /**
         *  Read
         */

        try
        {
            // Poll for data first
            if (p_Instance->PollSocket(p_Instance->i_FD) == false)
            {
                continue;
            }

            // Data available, read
            ssize_t ss_Read = p_Instance->ReadSocket(p_Instance->i_FD,
                                                     &(p_Buffer[us_BufferPos]),
                                                     MRH_EVD_L_STRING_BUFFER_MAX - us_BufferPos);

            // Add new byte info
            us_BufferPos += ss_Read;
        }
        catch (Exception& e)
        {
            c_Logger.Log(Logger::ERROR, e.what(),
                         "UTF8Stream.cpp", __LINE__);

            p_Instance->Disconnect();
            continue;
        }

        /**
         *  Add
         */

        // Can we work with the received data?
        if (us_BufferPos == MRH_EVD_L_STRING_BUFFER_MAX) // Buffer full, create now
        {
            p_Instance->p_Notifier->Notify(true);

            {
                std::lock_guard<std::mutex> c_Guard(p_Instance->c_Mutex);

                p_Instance->dq_Read.emplace_back(p_Buffer,
                                                 p_Buffer + us_BufferPos);
            }

            us_BufferPos = 0;
        }
        else if (us_BufferPos > 0) // Data available, find terminator
        {
            // Find buffer end first
            size_t us_StringEnd = 0;

            for (; us_StringEnd < us_BufferPos; ++us_StringEnd)
            {
                if (p_Buffer[us_StringEnd] == '\0')
                {
                    break;
                }
            }

            // No terminator yet?
            if (us_StringEnd == us_BufferPos)
            {
                // Need more data
                continue;
            }

            // Notify, this stream does something
            p_Instance->p_Notifier->Notify(true);

            // Usable message?
            if (us_StringEnd > 0)
            {
                std::lock_guard<std::mutex> c_Guard(p_Instance->c_Mutex);

                p_Instance->dq_Read.emplace_back(p_Buffer,
                                                 p_Buffer + us_StringEnd);
            }

            // Copy following characters to start
            size_t us_Following = us_BufferPos - (us_StringEnd + 1);

            if (us_Following > 0)
            {
                std::memmove(p_Buffer, &(p_Buffer[us_StringEnd + 1]), us_Following);
                std::memset(&(p_Buffer[us_Following]), '\0', us_BufferPos - us_Following);
            }

            // Set to point after last following byte
            us_BufferPos = us_Following;
        }
    }
}

//*************************************************************************************
// Write
//*************************************************************************************

void UTF8Stream::Write(std::string const& s_Message)
{
    if (i_FD < 0)
    {
        throw Exception("Cannot write message while not connected!");
    }
    else if (s_Message.empty() == true)
    {
        throw Exception("Attempted to write empty message!");
    }
    
    size_t us_Total = s_Message.size() + 1; // Write terminator as well if possible!

    char p_Buffer[MRH_EVD_L_STRING_BUFFER_MAX];
    std::strncpy(p_Buffer, s_Message.c_str(), MRH_EVD_L_STRING_BUFFER_MAX);

    if (us_Total <= MRH_EVD_L_STRING_BUFFER_MAX)
    {
        p_Buffer[(us_Total - 1)] = '\0'; // Important, signals end to receiver for short string
    }
    else
    {
        us_Total = MRH_EVD_L_STRING_BUFFER_MAX;
    }

    size_t us_Written = 0;
    ssize_t ss_Result;

    while (us_Written < us_Total)
    {
        // @NOTE: Use MSG_NOSIGNAL, client might close socket
        ss_Result = send(i_FD, &(p_Buffer[us_Written]), (us_Total - us_Written), MSG_NOSIGNAL);

        if (ss_Result >= 0)
        {
            us_Written += ss_Result;
            continue;
        }

        // Failed to write
        switch (errno)
        {
#if EAGAIN != EWOULDBLOCK
            case EWOULDBLOCK:
#endif
            case EAGAIN:
            case EINTR:
            {
                break;
            }

            default: // + EPIPE
            {
                // Close socket, broken
                Disconnect();

                throw Exception("Could not write socket: " +
                                std::string(std::strerror(errno)) +
                                " (" +
                                std::to_string(errno) +
                                ")!");
            }
        }
    }
}

//*************************************************************************************
// Getters
//*************************************************************************************

bool UTF8Stream::IsConnected() noexcept
{
    return i_FD < 0 ? false : true;
}

bool UTF8Stream::GetAvailable() noexcept
{
    std::lock_guard<std::mutex> c_Guard(c_Mutex);

    return !(dq_Read.empty());
}

std::string UTF8Stream::GetMessage()
{
    std::lock_guard<std::mutex> c_Guard(c_Mutex);

    if (dq_Read.empty() == true)
    {
        throw Exception("No messages available!");
    }

    std::string s_String(dq_Read.front());
    dq_Read.pop_front();

    return s_String;
}
