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

#ifndef SpeechChecker_h
#define SpeechChecker_h

// C / C++

// External

// Project
#include "./AudioBuffer.h"
#include "../Logger.h"
#include "../Exception.h"


class SpeechChecker
{
public:

    //*************************************************************************************
    // Destructor
    //*************************************************************************************

    /**
     *  Default destructor.
     */

    virtual ~SpeechChecker() noexcept
    {}

    //*************************************************************************************
    // Check
    //*************************************************************************************

    /**
     *  Check if a audio chunk contains speech.
     *
     *  \param v_Chunk The chunk to check.
     *
     *  \return true if speech was found, false if not.
     */

    virtual bool IsSpeech(AudioBuffer::AudioChunk const& v_Chunk)
    {
        throw Exception("Default IsSpeech() function called!");
    }

    //*************************************************************************************
    // Data
    //*************************************************************************************

    const std::string s_Identifier;

private:

    //*************************************************************************************
    // Data
    //*************************************************************************************

protected:

    //*************************************************************************************
    // Constructor
    //*************************************************************************************

    /**
     *  Default constructor.
     *
     *  \param s_Identifier The recorder identifier.
     */

    SpeechChecker(std::string const& s_Identifier) noexcept : s_Identifier(s_Identifier)
    {
        Logger::Singleton().Log(Logger::INFO, "Created [ " +
                                              s_Identifier +
                                              " ] speech checker API.",
                                "SpeechChecker.h", __LINE__);
    }
};

#endif /* SpeechChecker_h */
