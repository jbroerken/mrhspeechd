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
#include "./CreateSTTAPI.h"
#if MRH_SPEECHD_STT_API_GGOGLE_CLOUD > 0
#include "./GoogleCloudSTT/GoogleCloudSTT.h"
#endif
#if MRH_SPEECHD_STT_API_PICOVOICE_LEOPARD > 0
#include "./PicovoiceLeopard/PicovoiceLeopard.h"
#endif


//*************************************************************************************
// Requirements
//*************************************************************************************

void CreateSTTAPI::Init(Configuration const& c_Configuration)
{}

void CreateSTTAPI::Destroy(Configuration const& c_Configuration) noexcept
{}

//*************************************************************************************
// STT API
//*************************************************************************************

std::shared_ptr<STT> CreateSTTAPI::CreateSTT(Configuration const& c_Configuration)
{
    try
    {
        switch (c_Configuration.c_API.u8_STTAPI)
        {
#if MRH_SPEECHD_STT_API_GGOGLE_CLOUD > 0
            case STT_API_GOOGLE_CLOUD:
                return std::make_shared<GoogleCloudSTT>(c_Configuration.c_GoogleCloudSTT);
#endif
#if MRH_SPEECHD_STT_API_PICOVOICE_LEOPARD > 0
            case STT_API_PICOVOICE_LEOPARD:
                return std::make_shared<PicovoiceLeopard>(c_Configuration.c_PicovoiceLeopard);
#endif
            default:
                throw Exception("Unknown or unsupported STT API!");
        }
    }
    catch (std::exception& e)
    {
        throw Exception(e.what());
    }
}
