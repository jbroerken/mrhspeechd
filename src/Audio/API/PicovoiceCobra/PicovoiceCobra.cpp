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
#include <fstream>

// External

// Project
#include "./PicovoiceCobra.h"

// Pre-defined
#if PICOVOICE_COBRA_LOG_EXTENDED > 0
    #define PICOVOICE_COBRA_LOG(X) Logger::Singleton().Log(Logger::INFO, X, "PicovoiceCobra.cpp", __LINE__)
#else
    #define PICOVOICE_COBRA_LOG(X)
#endif


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

PicovoiceCobra::PicovoiceCobra(Configuration::PicovoiceCobra const& c_Configuration) : SpeechChecker("Picovoice Cobra"),
                                                                                       p_Handle(NULL),
                                                                                       f32_MinConfidence(c_Configuration.f32_MinConfidence)
{
    std::ifstream f_File(c_Configuration.s_AccessKeyPath);
    std::string s_AccessKey;

    if (f_File.is_open() == false)
    {
        throw Exception("Failed to open " +
                        c_Configuration.s_AccessKeyPath +
                        " key file!");
    }

    getline(f_File, s_AccessKey);
    f_File.close();

    pv_status_t e_Status = pv_cobra_init(s_AccessKey.c_str(),
                                         &p_Handle);

    if (e_Status != PV_STATUS_SUCCESS)
    {
        throw Exception(pv_status_to_string(e_Status));
    }
}

PicovoiceCobra::~PicovoiceCobra() noexcept
{
    if (p_Handle != NULL)
    {
        pv_cobra_delete(p_Handle);
    }
}

//*************************************************************************************
// Check
//*************************************************************************************

bool PicovoiceCobra::IsSpeech(AudioBuffer::AudioChunk const& v_Chunk)
{
    Logger& c_Logger = Logger::Singleton();
    size_t us_SampleCount = v_Chunk.size();

    if (us_SampleCount == 0)
    {
        PICOVOICE_COBRA_LOG("No samples to process!");
        return false;
    }

    pv_status_t e_Status;
    float f32_Confidence;
    size_t us_Pos = 0;

    while (us_Pos < us_SampleCount)
    {
        // Cobra wants a specific sample rate
        int32_t s32_RequiredSamples = pv_cobra_frame_length();

        PICOVOICE_COBRA_LOG("Required sample count: " +
                            std::to_string(s32_RequiredSamples) +
                            ", available sample count: " +
                            std::to_string(us_SampleCount) +
                            ", current position: " +
                            std::to_string(us_Pos));

        // Got enough samples to check for voice?
        if (s32_RequiredSamples > (us_SampleCount - us_Pos))
        {
            PICOVOICE_COBRA_LOG("Not enough samples!");
            return false;
        }

        // Process
        e_Status = pv_cobra_process(p_Handle, &(v_Chunk[us_Pos]), &f32_Confidence);

        if (e_Status != PV_STATUS_SUCCESS)
        {
            c_Logger.Log(Logger::WARNING, pv_status_to_string(e_Status),
                         "PicovoiceCobra.cpp", __LINE__);
            return false;
        }

        PICOVOICE_COBRA_LOG("Picovoice cobra reports " +
                            std::to_string(f32_Confidence) +
                            " confidence that audio contains speech.");

        if (f32_Confidence >= f32_MinConfidence)
        {
            PICOVOICE_COBRA_LOG("Speech recognized!");
            return true;
        }

        us_Pos += s32_RequiredSamples;
    }

    // Default
    PICOVOICE_COBRA_LOG("No speech recognized in buffer!");
    return false;
}
