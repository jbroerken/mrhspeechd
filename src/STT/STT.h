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

#ifndef STT_h
#define STT_h

// C / C++
#include <cstring>

// External

// Project
#include "../Audio/AudioBuffer.h"
#include "../Logger.h"
#include "../Exception.h"


class STT
{
public:

    //*************************************************************************************
    // Destructor
    //*************************************************************************************

    /**
     *  Default destructor.
     */

    virtual ~STT() noexcept
    {}

    //*************************************************************************************
    // Transcribe
    //*************************************************************************************

    /**
     *  Transcribe a string from a audio buffer.
     *
     *  \param c_Buffer The audio buffer to transcribe. The buffer is emptied.
     *  \param s_String The transcribed speech string.
     */

    virtual void Transcribe(AudioBuffer& c_Buffer, std::string& s_String)
    {
        throw Exception("Default Transcribe() function called!");
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

    STT(std::string const& s_Identifier) noexcept : s_Identifier(s_Identifier)
    {
        Logger::Singleton().Log(Logger::INFO, "Created " +
                                              s_Identifier +
                                              " STT API.",
                                "STT.h", __LINE__);
    }

    //*************************************************************************************
    // Audio
    //*************************************************************************************

    /**
     *  Add audio from a audio buffer.
     *
     *  \param c_Buffer The buffer to add. The buffer is consumed.
     *
     *  \return The amount of samples added.
     */

    size_t PrepareAudio(AudioBuffer& c_Buffer) noexcept
    {
        std::deque<AudioBuffer::AudioChunk> dq_Chunk;
        c_Buffer.Retrieve(dq_Chunk);

        size_t us_Pos = 0;

        for (auto& Chunk : dq_Chunk)
        {
            size_t us_Size = Chunk.size();

            if (us_Size == 0)
            {
                continue;
            }
            else if (v_Audio.size() < (us_Pos + us_Size))
            {
                v_Audio.insert(v_Audio.end(),
                               0,
                               ((us_Pos + us_Size) - v_Audio.size()));
            }

            memcpy(&(v_Audio[us_Pos]),
                   &(Chunk[0]),
                   us_Size);

            us_Pos += us_Size;
        }

        return us_Pos;
    }

    //*************************************************************************************
    // Data
    //*************************************************************************************

    std::vector<MRH_Sint16> v_Audio;
};

#endif /* STT_h */
