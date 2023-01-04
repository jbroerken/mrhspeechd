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

#ifndef CreateAudioAPI_h
#define CreateAudioAPI_h

// C / C++

// External

// Project
#include "./AudioAPI.h"
#include "../Recorder.h"
#include "../Player.h"
#include "../SpeechChecker.h"
#include "../../Configuration.h"


namespace CreateAudioAPI
{
    //*************************************************************************************
    // Requirements
    //*************************************************************************************

    /**
     *  Initialize API requirements.
     *
     *  \param c_Configuration The configuration to initialize with.
     */

    void Init(Configuration const& c_Configuration);

    /**
     *  Destroy API requirements.
     *
     *  \param c_Configuration The configuration to destroy with.
     */

    void Destroy(Configuration const& c_Configuration) noexcept;

    //*************************************************************************************
    // Recording API
    //*************************************************************************************

    /**
     *  Create a audio recorder.
     *
     *  \param c_Configuration The configuration to create with.
     *  \param p_Context The recording context to use.
     *
     *  \return The created audio recorder.
     */

    std::shared_ptr<Recorder> CreateRecorder(Configuration const& c_Configuration, std::shared_ptr<RecorderContext>& p_Context);

    //*************************************************************************************
    // Playback API
    //*************************************************************************************

    /**
     *  Create a audio player.
     *
     *  \param c_Configuration The configuration to create with.
     *
     *  \return The created audio player.
     */

    std::shared_ptr<Player> CreatePlayer(Configuration const& c_Configuration);

    //*************************************************************************************
    // Speech Check API
    //*************************************************************************************

    /**
     *  Create a speech checker.
     *
     *  \param c_Configuration The configuration to create with.
     *
     *  \return The created speech checker.
     */

    std::shared_ptr<SpeechChecker> CreateSpeechChecker(Configuration const& c_Configuration);
};


#endif /* CreateAudioAPI_h */
