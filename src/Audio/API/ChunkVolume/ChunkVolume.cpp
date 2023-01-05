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
#include <string>
#include <fstream>

// External

// Project
#include "./ChunkVolume.h"

// Pre-defined
#if CHUNK_VOLUME_LOG_EXTENDED > 0
    #define CHUNK_VOLUME_LOG(X) Logger::Singleton().Log(Logger::INFO, X, "ChunkVolume.cpp", __LINE__)
#else
    #define CHUNK_VOLUME_LOG(X)
#endif


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

ChunkVolume::ChunkVolume(Configuration::ChunkVolume const& c_Configuration) : SpeechChecker("Chunk Volume"),
                                                                              f32_MinVolume(c_Configuration.f32_MinVolume),
                                                                              f32_MinSamples(c_Configuration.f32_MinSamples)
{}

ChunkVolume::~ChunkVolume() noexcept
{}

//*************************************************************************************
// Check
//*************************************************************************************

bool ChunkVolume::IsSpeech(AudioBuffer::AudioChunk const& v_Chunk)
{
    Logger& c_Logger = Logger::Singleton();
    size_t us_SampleCount = v_Chunk.size();

    if (us_SampleCount == 0)
    {
        CHUNK_VOLUME_LOG("No samples to process!");
        return false;
    }

    MRH_Uint16 u16_MinVolume = (MRH_Uint16)(32768.f * f32_MinVolume);
    size_t us_RequiredSamples = (size_t)((float)(v_Chunk.size()) * f32_MinSamples);
    size_t us_MatchingSamples = 0;

    for (auto& Sample : v_Chunk)
    {
        if (Sample >= u16_MinVolume)
        {
            us_SampleCount += 1;
        }
    }

    if (us_MatchingSamples >= us_RequiredSamples)
    {
        return true;
    }

    CHUNK_VOLUME_LOG("Found " +
                     std::to_string(us_SampleCount) +
                     " matching samples, " +
                     std::to_string(us_RequiredSamples) +
                     " are required.");
    return false;
}
