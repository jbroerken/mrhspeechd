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

// External

// Project
#include "./CreateTTSAPI.h"
#if MRH_SPEECHD_TTS_API_GGOGLE_CLOUD > 0
#include "./GoogleCloudTTS/GoogleCloudTTS.h"
#endif


//*************************************************************************************
// Requirements
//*************************************************************************************

void CreateTTSAPI::Init(Configuration const& c_Configuration)
{}

void CreateTTSAPI::Destroy(Configuration const& c_Configuration) noexcept
{}

//*************************************************************************************
// TTS API
//*************************************************************************************

std::shared_ptr<TTS> CreateTTSAPI::CreateTTS(Configuration const& c_Configuration)
{
    try
    {
        switch (c_Configuration.c_API.u8_TTSAPI)
        {
#if MRH_SPEECHD_TTS_API_GGOGLE_CLOUD > 0
            case TTS_API_GOOGLE_CLOUD:
                return std::make_shared<GoogleCloudTTS>(c_Configuration.c_GoogleCloudTTS);
#endif
            default:
                throw Exception("Unknown or unsupported TTS API!");
        }
    }
    catch (std::exception& e)
    {
        throw Exception(e.what());
    }
}
