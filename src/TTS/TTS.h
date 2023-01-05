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

#ifndef TTS_h
#define TTS_h

// C / C++

// External

// Project
#include "../Audio/AudioBuffer.h"
#include "../Logger.h"
#include "../Exception.h"


class TTS
{
public:

    //*************************************************************************************
    // Destructor
    //*************************************************************************************

    /**
     *  Default destructor.
     */

    virtual ~TTS() noexcept
    {}

    //*************************************************************************************
    // Synthesize
    //*************************************************************************************

    /**
     *  Synthesize speech output from a given text string.
     *
     *  \param s_String The speech string to synthesize audio for.
     *  \param c_Buffer The audio buffer to store audio in. The buffer is overwritten.
     */

    virtual void Synthesize(std::string const& s_String, AudioBuffer& c_Buffer)
    {
        throw Exception("Default Synthesize() function called!");
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
     *  \param s_Identifier The player identifier.
     */

    TTS(std::string const& s_Identifier) noexcept : s_Identifier(s_Identifier)
    {
        Logger::Singleton().Log(Logger::INFO, "Created [ " +
                                              s_Identifier +
                                              " ] TTS API.",
                                "TTS.h", __LINE__);
    }
};

#endif /* TTS_h */
