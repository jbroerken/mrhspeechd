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

#ifndef TTSAPI_h
#define TTSAPI_h

// C / C++

// External

// Project


//*************************************************************************************
// TTS API Use Flags
//*************************************************************************************

/**
 *  Google Cloud API
 */

#ifndef MRH_SPEECHD_TTS_API_GGOGLE_CLOUD
    #define MRH_SPEECHD_TTS_API_GGOGLE_CLOUD 1//0
#endif

//*************************************************************************************
// API Enumerations
//*************************************************************************************

// @NOTE: Keep #define excluded in list for switch cases
typedef enum
{
    // APIs
    TTS_API_GOOGLE_CLOUD = 0,

    // Bounds
    TTS_API_MAX = TTS_API_GOOGLE_CLOUD,

    TTS_API_COUNT = TTS_API_MAX + 1

}TTSAPI;


#endif /* TTSAPI_h */
