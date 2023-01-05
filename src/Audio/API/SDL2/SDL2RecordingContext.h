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
     *  \param u32_KHz The recording KHz.
     *  \param u32_TrailingFrameSizeMax The amount of samples allowed to append with no speech.
     *  \param p_Context The recorder context to manage.
     */

    SDL2RecordingContext(MRH_Uint32 u32_KHz,
                         MRH_Uint32 u32_TrailingFrameSizeMax,
                         MRH_Sfloat32 f32_Amplification,
                         std::shared_ptr<RecorderContext>& p_Context) noexcept : c_Buffer(u32_KHz),
                                                                                 u32_TrailingFrameSizeCurrent(0),
                                                                                 u32_TrailingFrameSizeMax(u32_TrailingFrameSizeMax),
                                                                                 f32_Amplification(f32_Amplification),
                                                                                 u32_DeviceID(MRH_SDL2_AUDIO_DEVICE_ID_INVALID),
                                                                                 p_Context(p_Context)
    {}

    //*************************************************************************************
    // Data
    //*************************************************************************************

    AudioBuffer c_Buffer;

    MRH_Uint32 u32_TrailingFrameSizeCurrent;
    MRH_Uint32 u32_TrailingFrameSizeMax;

    MRH_Sfloat32 f32_Amplification;

    SDL_AudioDeviceID u32_DeviceID;

    std::shared_ptr<RecorderContext> p_Context;
};


#endif /* SDL2RecordingContext_h */
