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
#include <execinfo.h>
#include <unistd.h>

// External
#include <spdlog/spdlog.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include "spdlog/sinks/stdout_color_sinks.h"

// Project
#include "./Logger.h"

// Pre-defined
#ifndef MRH_SPEECHD_LOG_FILE_PATH
    #define MRH_SPEECHD_LOG_FILE_PATH "/var/log/mrh/mrhspeechd.log"
#endif
#ifndef MRH_LOGGER_PRINT_CLI
    #define MRH_LOGGER_PRINT_CLI 0
#endif
#define MRH_LOGGER_NAME_FILE "File"
#define MRH_LOGGER_NAME_CONSOLE "Console"
#define MRH_LOGGER_MAX_SIZE_B 1048576 * 5
#define MRH_LOGGER_MAX_FILES 3


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

Logger::Logger() noexcept
{
    // Create loggers
    auto FileLogger = spdlog::rotating_logger_mt(MRH_LOGGER_NAME_FILE,
                                                 MRH_SPEECHD_LOG_FILE_PATH,
                                                 MRH_LOGGER_MAX_SIZE_B,
                                                 MRH_LOGGER_MAX_FILES);
#if MRH_LOGGER_PRINT_CLI > 0
    auto ConsoleLogger = spdlog::stdout_color_mt(MRH_LOGGER_NAME_CONSOLE);
#endif
    
    // Update pattern
    spdlog::set_pattern("[%c] [%l] [Thread %t] %v");
    
    // Set as default
    spdlog::set_default_logger(FileLogger);
    
    // Write all starting at info
    spdlog::flush_on(spdlog::level::info);
}

Logger::~Logger() noexcept
{}

//*************************************************************************************
// Singleton
//*************************************************************************************

Logger& Logger::Singleton() noexcept
{
    static Logger s_Logger;
    return s_Logger;
}

//*************************************************************************************
// Log
//*************************************************************************************

void Logger::Log(LogLevel e_Level, std::string const& s_Message, std::string const& s_File, size_t us_Line) noexcept
{
    // Create log string
    std::string s_Line = "[";
    s_Line += s_File;
    s_Line += " (";
    s_Line += std::to_string(us_Line);
    s_Line += ")]: ";
    s_Line += s_Message;
    
    // Got our message, now log
    Log(e_Level, s_Line);
}

void Logger::Log(LogLevel e_Level, std::string const& s_Message) noexcept
{
    switch (e_Level)
    {
        case WARNING:
            spdlog::get(MRH_LOGGER_NAME_FILE)->warn(s_Message);
#if MRH_LOGGER_PRINT_CLI > 0
            spdlog::get(MRH_LOGGER_NAME_CONSOLE)->warn(s_Message);
#endif
            break;
        case ERROR:
            spdlog::get(MRH_LOGGER_NAME_FILE)->error(s_Message);
#if MRH_LOGGER_PRINT_CLI > 0
            spdlog::get(MRH_LOGGER_NAME_CONSOLE)->error(s_Message);
#endif
            break;
            
        default:
            spdlog::get(MRH_LOGGER_NAME_FILE)->info(s_Message);
#if MRH_LOGGER_PRINT_CLI > 0
            spdlog::get(MRH_LOGGER_NAME_CONSOLE)->info(s_Message);
#endif
            break;
    }
}

//*************************************************************************************
// Backtrace
//*************************************************************************************

void Logger::Backtrace(size_t us_TraceSize, std::string const& s_Message) noexcept
{
    // Retrieve current stack addresses
    char** p_Traceback = NULL;
    void* p_AdressList[us_TraceSize];
    
    us_TraceSize = backtrace(p_AdressList, (int)(sizeof(p_AdressList) / sizeof(void*)));
    
    // Get names
    if (us_TraceSize > 0)
    {
        p_Traceback = backtrace_symbols(p_AdressList, (int)us_TraceSize);
    }
    
    // File head
    Log(ERROR, "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    Log(ERROR, "!!!");
    Log(ERROR, "!!! " + s_Message);
    Log(ERROR, "!!! ");
    Log(ERROR, "!!! Backtrace (Size: " + std::to_string(us_TraceSize) + "):");
    
    // Print traceback stack
    if (us_TraceSize == 0)
    {
        Log(ERROR, "!!! Failed to get backtrace!");
    }
    else
    {
        for (int i = 0; i < us_TraceSize; ++i)
        {
            if (p_Traceback[i] != NULL)
            {
                Log(ERROR, "!!! " + std::string(p_Traceback[i]));
            }
            else
            {
                Log(ERROR, "!!! (null)");
            }
        }
    }
    
    // End
    Log(ERROR, "!!!");
    Log(ERROR, "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
}
