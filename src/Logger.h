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

#ifndef Logger_h
#define Logger_h

// C / C++
#include <string>

// External

// Project


class Logger
{
public:
    
    //*************************************************************************************
    // Types
    //*************************************************************************************
    
    typedef enum
    {
        INFO = 0,
        WARNING = 1,
        ERROR = 2,
        
        LOG_LEVEL_MAX = 2,
        
        LOG_LEVEL_COUNT = 3
        
    }LogLevel;
    
    //*************************************************************************************
    // Singleton
    //*************************************************************************************
    
    /**
     *  Get the class instance. This function is thread safe.
     *
     *  \return The class instance.
     */
    
    static Logger& Singleton() noexcept;
    
    //*************************************************************************************
    // Log
    //*************************************************************************************
    
    /**
     *  Log a message. This function is thread safe.
     *
     *  \param e_Level The message log level.
     *  \param s_Message The message to log.
     *  \param s_File The source file this log was created from.
     *  \param us_Line The source file line this log was created from.
     */
    
    void Log(LogLevel e_Level, std::string const& s_Message, std::string const& s_File, size_t us_Line) noexcept;
    
    //*************************************************************************************
    // Backtrace
    //*************************************************************************************
    
    /**
     *  Write the program backtrace.
     *
     *  \param us_TraceSize The size of the backtrace.
     *  \param s_Message The message describing the backtrace cause.
     */
    
    void Backtrace(size_t us_TraceSize, std::string const& s_Message) noexcept;
    
private:
    
    //*************************************************************************************
    // Constructor / Destructor
    //*************************************************************************************
    
    /**
     *  Default constructor.
     */
    
    Logger() noexcept;
    
    /**
     *  Default destructor.
     */
    
    ~Logger() noexcept;
    
    //*************************************************************************************
    // Log
    //*************************************************************************************
    
    /**
     *  Log a message. This function is thread safe.
     *
     *  \param e_Level The message log level.
     *  \param s_Message The message to log.
     */
    
    void Log(LogLevel e_Level, std::string const& s_Message) noexcept;
    
    //*************************************************************************************
    // Data
    //*************************************************************************************
    
protected:
    
};

#endif /* Logger_h */
