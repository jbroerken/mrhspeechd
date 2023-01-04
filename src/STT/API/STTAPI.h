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

#ifndef STTAPI_h
#define STTAPI_h

// C / C++

// External

// Project


//*************************************************************************************
// TTS API Use Flags
//*************************************************************************************

/**
 *  Google Cloud API
 */

#ifndef MRH_SPEECHD_STT_API_GGOGLE_CLOUD
    #define MRH_SPEECHD_STT_API_GGOGLE_CLOUD 1//0
#endif

/**
 *  Picovoice Leopards
 */

#ifndef MRH_SPEECHD_STT_API_PICOVOICE_LEOPARD
    #define MRH_SPEECHD_STT_API_PICOVOICE_LEOPARD 1//0
#endif

//*************************************************************************************
// API Enumerations
//*************************************************************************************

// @NOTE: Keep #define excluded in list for switch cases
typedef enum
{
    // APIs
    STT_API_GOOGLE_CLOUD = 0,
    STT_API_PICOVOICE_LEOPARD = 1,

    // Bounds
    STT_API_MAX = STT_API_PICOVOICE_LEOPARD,

    STT_API_COUNT = STT_API_MAX + 1

}STTAPI;


#endif /* STTAPI_h */
