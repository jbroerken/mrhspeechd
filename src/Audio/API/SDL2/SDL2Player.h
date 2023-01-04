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

#ifndef SDL2Player_h
#define SDL2Player_h

// C / C++

// External

// Project
#include "../../Player.h"
#include "./SDL2PlaybackContext.h"
#include "../../../Configuration.h"


class SDL2Player : public Player
{
public:

    //*************************************************************************************
    // Constructor / Destructor
    //*************************************************************************************

    /**
     *  Default constructor.
     *
     *  \param c_Configuration The configuration to setup with.
     */

    SDL2Player(Configuration::SDL2Player const& c_Configuration);

    /**
     *  Default destructor.
     */

    ~SDL2Player() noexcept;

    //*************************************************************************************
    // Playback
    //*************************************************************************************

    /**
     *  Start playback.
     */

    void Start(AudioBuffer& c_Buffer) override;

    /**
     *  Stop playback.
     */

    void Stop() noexcept override;

    //*************************************************************************************
    // Getters
    //*************************************************************************************

    /**
     *  Check if playback is active.
     *
     *  \return true if active, false if not.
     */

    bool GetPlaying() const noexcept override;

private:

    //*************************************************************************************
    // Callback
    //*************************************************************************************

    /**
     *  Audio playback callback.
     *
     *  \param p_Context The callback context.
     *  \param p_Stream The audio stream bytes to write.
     *  \param i_Length The required length to write in bytes.
     */

    static void Callback(void* p_Context, Uint8* p_Stream, int i_Length) noexcept;

    //*************************************************************************************
    // Data
    //*************************************************************************************

    SDL2PlaybackContext* p_Context;

    std::string s_DeviceName;
    MRH_Uint32 u32_SamplesPerFrame;

protected:

};

#endif /* SDL2Player_h */
