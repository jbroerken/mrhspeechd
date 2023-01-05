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

#ifndef AudioAPI_h
#define AudioAPI_h

// C / C++

// External

// Project


//*************************************************************************************
// IO API Use Flags
//*************************************************************************************

/**
 *  SDL2 (I/O)
 */

#ifndef MRH_SPEECHD_SOUND_IO_API_SDL2
    #define MRH_SPEECHD_SOUND_IO_API_SDL2 0
#endif

//*************************************************************************************
// Speech Checker API Use Flags
//*************************************************************************************

/**
 *  Picovoice Cobra
 */

#ifndef MRH_SPEECHD_SPEECH_CHECKER_API_PICOVOICE_COBRA
    #define MRH_SPEECHD_SPEECH_CHECKER_API_PICOVOICE_COBRA 0
#endif

//*************************************************************************************
// API Enumerations
//*************************************************************************************

// @NOTE: Keep #define excluded in list for switch cases
typedef enum
{
    // APIs
    RECORDER_API_SDL2 = 0,

    // Bounds
    RECORDER_API_MAX = RECORDER_API_SDL2,

    RECORDER_API_COUNT = RECORDER_API_MAX + 1

}RecorderAPI;

typedef enum
{
    // APIs
    PLAYER_API_SDL2 = 0,

    // Bounds
    PLAYER_API_MAX = PLAYER_API_SDL2,

    PLAYER_API_COUNT = PLAYER_API_MAX + 1

}PlayerAPI;

typedef enum
{
    // APIs
    SPEECH_CHECKER_API_CHUNK_VOLUME = 0,
    SPEECH_CHECKER_API_PICOVOICE_COBRA = 1,

    // Bounds
    SPEECH_CHECKER_API_MAX = SPEECH_CHECKER_API_PICOVOICE_COBRA,

    SPEECH_CHECKER_API_COUNT = SPEECH_CHECKER_API_MAX + 1

}SpeechCheckerAPI;


#endif /* AudioAPI_h */
