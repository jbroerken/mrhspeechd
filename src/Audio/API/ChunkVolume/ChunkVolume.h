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

#ifndef ChunkVolume_h
#define ChunkVolume_h

// C / C++

// External

// Project
#include "../../SpeechChecker.h"
#include "../../../Configuration.h"


class ChunkVolume : public SpeechChecker
{
public:

    //*************************************************************************************
    // Constructor / Destructor
    //*************************************************************************************

    /**
     *  Default constructor.
     *
     *  \param c_Configuration The configuration to setup with.
     */

    ChunkVolume(Configuration::ChunkVolume const& c_Configuration);

    /**
     *  Default destructor.
     */

    ~ChunkVolume() noexcept;

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

    bool IsSpeech(AudioBuffer::AudioChunk const& v_Chunk) override;

private:

    //*************************************************************************************
    // Data
    //*************************************************************************************

    MRH_Sfloat32 f32_MinVolume;
    MRH_Sfloat32 f32_MinSamples;

protected:

};

#endif /* ChunkVolume_h */
