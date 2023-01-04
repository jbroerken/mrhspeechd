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

#ifndef UTF8Stream_h
#define UTF8Stream_h

// C / C++
#include <thread>
#include <mutex>
#include <atomic>
#include <memory>
#include <vector>
#include <string>
#include <deque>

// External

// Project
#include "../DataNotifier.h"
#include "../Exception.h"


class UTF8Stream
{
public:

    //*************************************************************************************
    // Destructor
    //*************************************************************************************

    /**
     *  Default constructor.
     *
     *  \param s_SocketPath The full path to the UTF-8 stream socket.
     *  \param p_Notifier The notifier to notify when receiving new messages.
     */

    UTF8Stream(std::string const& s_SocketPath,
               std::shared_ptr<DataNotifier>& p_Notifier);

    /**
     *  Default destructor.
     */

    ~UTF8Stream() noexcept;

    //*************************************************************************************
    // Write
    //*************************************************************************************

    /**
     *  Write to the UTF-8 stream. This function blocks until all data was written. This
     *  function is thread safe.
     *
     *  \param s_Message The message to write to the stream.
     */

    void Write(std::string const& s_Message);

    //*************************************************************************************
    // Getters
    //*************************************************************************************

    /**
     *  Check if the local stream is connected.
     *
     *  \return true if connected, false if not.
     */

    bool IsConnected() noexcept;

    /**
     *  Check if read messages are available.
     *
     *  \return true if available, false if not.
     */

    bool GetAvailable() noexcept;

    /**
     *  Get a read message.
     *
     *  \return A read message.
     */

    std::string GetMessage();

private:

    //*************************************************************************************
    // Connection
    //*************************************************************************************

    /**
     *  Open a new connection and notify.
     */

    void Connect();

    /**
     *  Disconnect the connection and notify.
     */

    void Disconnect() noexcept;

    //*************************************************************************************
    // Read
    //*************************************************************************************

    /**
     *  Poll a file descriptor for available data.
     *
     *  \param i_FD The file descriptor to poll.
     *
     *  \return true if data is available, false if not.
     */

    bool PollSocket(int i_FD);

    /**
     *  Read from a socket.
     *
     *  \param i_FD The socket file descriptor.
     *  \param p_Buffer The buffer to read into.
     *  \param us_Length The available buffer length.
     *
     *  \return The amount of bytes read.
     */

    ssize_t ReadSocket(int i_FD, char* p_Buffer, size_t us_Length);

    /**
     *  Read from the UTF-8 stream.
     *
     *  \param p_Instance The class instance to read with.
     */

    static void Read(UTF8Stream* p_Instance) noexcept;

    //*************************************************************************************
    // Data
    //*************************************************************************************

    std::thread c_Thread;
    std::atomic<bool> b_Read;

    std::string s_SocketPath;
    std::atomic<int> i_FD;

    std::mutex c_Mutex;
    std::deque<std::string> dq_Read;
    std::shared_ptr<DataNotifier> p_Notifier;

protected:

};

#endif /* UTF8Stream_h */
