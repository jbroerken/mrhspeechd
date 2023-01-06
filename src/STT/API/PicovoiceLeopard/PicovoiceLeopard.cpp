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
#include <libmrhvt/String/MRH_LocalisedPath.h>

// Project
#include "./PicovoiceLeopard.h"

// Pre-defined
#if PICOVOICE_LEOPARD_LOG_EXTENDED > 0
    #define PICOVOICE_LEOPARD_LOG(X) Logger::Singleton().Log(Logger::INFO, X, "PicovoiceLeopards.cpp", __LINE__)
#else
    #define PICOVOICE_LEOPARD_LOG(X)
#endif


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

PicovoiceLeopard::PicovoiceLeopard(Configuration::PicovoiceLeopard const& c_Configuration) : STT("Picovoice Leopard")
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

    // Get model path based on locale folder
    std::string s_ModelPath = MRH::VT::LocalisedPath::GetPath(c_Configuration.s_ModelDirPath, c_Configuration.s_ModelFileName);

    PICOVOICE_LEOPARD_LOG("Using leopard model at path: " +
                          s_ModelPath);

    // Create handle with given params
    pv_status_t e_Status = pv_leopard_init(s_AccessKey.c_str(),
                                           s_ModelPath.c_str(),
                                           true,
                                           &p_Handle);

    if (e_Status != PV_STATUS_SUCCESS)
    {
        throw Exception("Failed to initialize leopard!");
    }
}

PicovoiceLeopard::~PicovoiceLeopard() noexcept
{
    if (p_Handle != NULL)
    {
        pv_leopard_delete(p_Handle);
    }
}

//*************************************************************************************
// Transcribe
//*************************************************************************************

void PicovoiceLeopard::Transcribe(AudioBuffer& c_Buffer, std::string& s_String)
{
    // Check audio
    if (c_Buffer.GetKHz() != pv_sample_rate())
    {
        throw Exception("Invalid audio sample rate: " +
                        std::to_string(pv_sample_rate()) +
                        " required!");
    }

    // Create full buffer
    size_t us_SampleCount = PrepareAudio(c_Buffer);

    if (us_SampleCount == 0)
    {
        throw Exception("No audio samples to transcribe!");
    }

    PICOVOICE_LEOPARD_LOG("Transcribing audio with " +
                          std::to_string(us_SampleCount) +
                          " samples.");

    // Perform transcription
    // @NOTE: We don't use words or word count, but leopard returns invalid param
    //        if those are not given as parameters
    char* p_Transcript = NULL;
    int32_t s32_WordCount = 0;
    pv_word_t* p_Words = NULL;

    try
    {
        pv_status_t e_Status = pv_leopard_process(p_Handle,
                                                  v_Audio.data(),
                                                  us_SampleCount,
                                                  &p_Transcript,
                                                  &s32_WordCount,
                                                  &p_Words);

        if (e_Status != PV_STATUS_SUCCESS)
        {
            throw Exception("Failed to transcribe speech input: " +
                            std::string(pv_status_to_string(e_Status)));
        }

        s_String = p_Transcript;

        PICOVOICE_LEOPARD_LOG("Transcription result: " +
                              s_String);

        free(p_Transcript);
        free(p_Words);
    }
    catch (Exception& e)
    {
        throw;
    }
}
