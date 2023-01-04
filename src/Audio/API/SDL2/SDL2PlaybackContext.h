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

#ifndef SDL2PlaybackContext_h
#define SDL2PlaybackContext_h

// C / C++

// External
#include <SDL2/SDL.h>

// Project
#include "./SDL2Device.h"
#include "../../AudioBuffer.h"


struct SDL2PlaybackContext
{
public:

    //*************************************************************************************
    // Constructor
    //*************************************************************************************

    /**
     *  Default constructor.
     */

    SDL2PlaybackContext() noexcept : c_Buffer(0),
                                     u32_DeviceID(MRH_SDL2_AUDIO_DEVICE_ID_INVALID)
    {}

    //*************************************************************************************
    // Data
    //*************************************************************************************

    AudioBuffer c_Buffer;

    SDL_AudioDeviceID u32_DeviceID;
};


#endif /* SDL2PlaybackContext_h */
