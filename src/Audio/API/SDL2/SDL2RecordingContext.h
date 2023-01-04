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

#ifndef SDL2RecordingContext_h
#define SDL2RecordingContext_h

// C / C++

// External
#include <SDL2/SDL.h>

// Project
#include "./SDL2Device.h"
#include "../../AudioBuffer.h"
#include "../../RecorderContext.h"


struct SDL2RecordingContext
{
public:

    //*************************************************************************************
    // Constructor
    //*************************************************************************************

    /**
     *  Default constructor.
     *
     *  \param p_Context The recorder context to manage.
     */

    SDL2RecordingContext(std::shared_ptr<RecorderContext>& p_Context) noexcept : c_Buffer(0),
                                                                                 u32_NoSpeechSamples(0),
                                                                                 u32_DeviceID(MRH_SDL2_AUDIO_DEVICE_ID_INVALID),
                                                                                 p_Context(p_Context)
    {}

    //*************************************************************************************
    // Data
    //*************************************************************************************

    AudioBuffer c_Buffer;
    MRH_Uint32 u32_NoSpeechSamples;

    SDL_AudioDeviceID u32_DeviceID;

    std::shared_ptr<RecorderContext> p_Context;
};


#endif /* SDL2RecordingContext_h */
