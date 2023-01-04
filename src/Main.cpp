/**
 *  Copyright (C) 2023 The mrhspeechd Authors.
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
#include <sys/stat.h>
#include <csignal>
#include <cstring>
#include <fstream>
#include <chrono>

// External

// Project
#include "./Audio/API/CreateAudioAPI.h"
#include "./TTS/API/CreateTTSAPI.h"
#include "./STT/API/CreateSTTAPI.h"
#include "./Stream/UTF8Stream.h"
#include "./Logger.h"
#include "./DataNotifier.h"
#include "./Revision.h"

// Pre-defined
#ifndef MRH_SPEECHD_PID_FILE_PATH
    #define MRH_SPEECHD_PID_FILE_PATH "/tmp/mrh/mrhpeechd_PID"
#endif
#ifndef MRH_SPEECHD_DAEMON_MODE
    #define MRH_SPEECHD_DAEMON_MODE 0
#endif
#define MRH_SPEECHD_SIGNAL_START_RECORDING SIGUSR1
#define MRH_SPEECHD_SIGNAL_STOP_RECORDING SIGUSR2

// Namespace
namespace
{
    // Last signal
    int i_LastSignal = -1;
}


//*************************************************************************************
// Signal Handler
//*************************************************************************************

// Prevent name wrangling
extern "C"
{
    void SignalHandler(int i_Signal)
    {
        switch (i_Signal)
        {
            case SIGILL:
            case SIGTRAP:
            case SIGFPE:
            case SIGABRT:
            case SIGSEGV:
                Logger::Singleton().Backtrace(25, "Caught Signal: " + std::to_string(i_Signal));
                _Exit(i_Signal);
                break;
                
            case SIGTERM:
            case SIGINT:
            case SIGHUP:
            case MRH_SPEECHD_SIGNAL_START_RECORDING:
            case MRH_SPEECHD_SIGNAL_STOP_RECORDING:
                i_LastSignal = i_Signal;
                break;
                
            default:
                Logger::Singleton().Log(Logger::WARNING, "Caught signal: " + std::to_string(i_Signal),
                                        "Main.cpp", __LINE__);
                i_LastSignal = -1;
                break;
        }
    }
}

//*************************************************************************************
// Daemon
//*************************************************************************************

#if MRH_SPEECHD_DAEMON_MODE > 0
static int Daemonize() noexcept
{
    pid_t s32_Pid = 0;

    // First fork to leave parent
    if ((s32_Pid = fork()) < 0)
    {
        return -1;
    }
    else if (s32_Pid > 0)
    {
        // Parent exit
        exit(EXIT_SUCCESS);
    }

    // Set session id
    if (setsid() < 0)
    {
        return -1;
    }

    // Ignore child signal
    signal(SIGCHLD, SIG_IGN);

    // Second fork to attach to proccess 1
    if ((s32_Pid = fork()) < 0)
    {
        return -1;
    }
    else if (s32_Pid > 0)
    {
        // Parent exit
        exit(EXIT_SUCCESS);
    }

    // Set new file permissions
    umask(0);

    // Return to root
    chdir("/");

    // in, out and error to NULL
    stdin = fopen("/dev/null", "r");
    stdout = fopen("/dev/null", "w+");
    stderr = fopen("/dev/null", "w+");

    // Child success
    return 0;
}
#endif

//*************************************************************************************
// PID
//*************************************************************************************

static bool WritePID() noexcept
{
    std::ofstream f_File(MRH_SPEECHD_PID_FILE_PATH, std::ios::trunc);
    
    if (f_File.is_open() == false)
    {
        return false;
    }
    
    f_File << std::to_string(getpid());
    
    f_File.close();
    return true;
}

//*************************************************************************************
// Main
//*************************************************************************************

int main(int argc, const char* argv[])
{
#if MRH_SPEECHD_DAEMON_MODE > 0
    if (Daemonize() < 0)
    {
        return EXIT_FAILURE;
    }
#endif

    // Log Setup
    Logger& c_Logger = Logger::Singleton();
    c_Logger.Log(Logger::INFO, "=============================================", "Main.cpp", __LINE__);
    c_Logger.Log(Logger::INFO, "= Started MRH Speech Daemon (" + std::string(VERSION_NUMBER) + ")", "Main.cpp", __LINE__);
    c_Logger.Log(Logger::INFO, "=============================================", "Main.cpp", __LINE__);
    
    // Install signal handlers
#ifdef _NSIG
    for (int i = 0; i < _NSIG; ++i)
#else
    for (int i = 0; i < NSIG; ++i)
#endif
    {
        std::signal(i, SignalHandler);
    }
    
    // Write current PID
    if (WritePID() == false)
    {
        c_Logger.Log(Logger::ERROR, "Failed to write own process ID!", 
                     "Main.cpp", __LINE__);
        
        return EXIT_FAILURE;
    }

    // Create components
    std::shared_ptr<DataNotifier> p_Notifier;
    std::shared_ptr<SpeechChecker> p_SpeechChecker;
    std::shared_ptr<Recorder> p_Recorder;
    std::shared_ptr<Player> p_Player;
    std::shared_ptr<TTS> p_TTS;
    std::shared_ptr<STT> p_STT;
    std::shared_ptr<UTF8Stream> p_Stream;

    try
    {
        Configuration c_Configuration;

        p_Notifier = std::make_shared<DataNotifier>();
        p_SpeechChecker = CreateAudioAPI::CreateSpeechChecker(c_Configuration);

        std::shared_ptr<RecorderContext> p_RecorderContext = std::make_shared<RecorderContext>(p_Notifier,
                                                                                               p_SpeechChecker);


        p_Recorder = CreateAudioAPI::CreateRecorder(c_Configuration, p_RecorderContext);
        p_Player = CreateAudioAPI::CreatePlayer(c_Configuration);

        p_TTS = CreateTTSAPI::CreateTTS(c_Configuration);
        p_STT = CreateSTTAPI::CreateSTT(c_Configuration);

        p_Stream = std::make_shared<UTF8Stream>(c_Configuration.c_Service.s_SocketPath,
                                                p_Notifier);
    }
    catch (Exception& e)
    {
        c_Logger.Log(Logger::ERROR, "Failed to create components: " +
                                    e.what2(),
                     "Main.cpp", __LINE__);

        return EXIT_FAILURE;
    }

    // Handle audio
    while (i_LastSignal != SIGTERM)
    {
        // Wait for notifications
        p_Notifier->Wait();

        // Are we connected to the service
        if (p_Stream->IsConnected() == false)
        {
            p_Recorder->Stop(); // No reason to record if we cannot send
            continue;
        }

        // Is there somethning to play?
        if (p_Stream->GetAvailable() == true)
        {
            // Stop recording, playback
            p_Recorder->Stop();

            try
            {
                AudioBuffer c_Buffer(0);
                std::string s_String = p_Stream->GetMessage();

                p_TTS->Synthesize(s_String, c_Buffer);
                p_Player->Start(c_Buffer);
            }
            catch (Exception& e)
            {
                c_Logger.Log(Logger::ERROR, "Failed to handle output: " +
                                            e.what2(),
                             "Main.cpp", __LINE__);
            }
        }

        // Was audio recorded to transcribe?
        if (p_Recorder->GetSpeechRecorded() == true && p_Recorder->GetRecording() == false)
        {
            try
            {
                AudioBuffer c_Buffer(0);
                std::string s_String("");

                p_Recorder->GetRecordedAudio(c_Buffer);
                p_STT->Transcribe(c_Buffer, s_String);
                p_Stream->Write(s_String);
            }
            catch (Exception& e)
            {
                c_Logger.Log(Logger::ERROR, "Failed to handle input: " +
                                            e.what2(),
                             "Main.cpp", __LINE__);
            }
        }

        // Was a recording signal received?
        if (i_LastSignal == MRH_SPEECHD_SIGNAL_STOP_RECORDING)
        {
            p_Recorder->Stop();
            i_LastSignal = -1;
        }
        else if (i_LastSignal == MRH_SPEECHD_SIGNAL_START_RECORDING)
        {
            // Only works while not recording or playing!
            if (p_Player->GetPlaying() == false && p_Recorder->GetRecording() == false)
            {
                try
                {
                    p_Recorder->Start(true);
                }
                catch (Exception& e)
                {
                    c_Logger.Log(Logger::ERROR, "Failed to start recording: " +
                                                e.what2(),
                                 "Main.cpp", __LINE__);
                }
            }

            i_LastSignal = -1;
        }
    }
    
    // Finished
    c_Logger.Log(Logger::INFO, "mrhspeechd finished.",
                 "Main.cpp", __LINE__);
    
    return EXIT_SUCCESS;
}
