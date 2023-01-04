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

#ifndef Player_h
#define Player_h

// C / C++

// External

// Project
#include "./AudioBuffer.h"
#include "../Logger.h"
#include "../Exception.h"


class Player
{
public:

    //*************************************************************************************
    // Destructor
    //*************************************************************************************

    /**
     *  Default destructor.
     */

    virtual ~Player() noexcept
    {}

    //*************************************************************************************
    // Playback
    //*************************************************************************************

    /**
     *  Set the audio buffer to play and start playback.
     *
     *  \param c_Buffer The audio buffer to play.
     */

    virtual void Start(AudioBuffer& c_Buffer)
    {
        throw Exception("Default Start() function called!");
    }

    /**
     *  Stop playback.
     */

    virtual void Stop() noexcept
    {}

    //*************************************************************************************
    // Getters
    //*************************************************************************************

    /**
     *  Check if playback is active.
     *
     *  \return true if active, false if not.
     */

    virtual bool GetPlaying() const noexcept
    {
        return false;
    }

    //*************************************************************************************
    // Data
    //*************************************************************************************

    const std::string s_Identifier;

private:

    //*************************************************************************************
    // Data
    //*************************************************************************************

protected:

    //*************************************************************************************
    // Constructor
    //*************************************************************************************

    /**
     *  Default constructor.
     *
     *  \param s_Identifier The player identifier.
     */

    Player(std::string const& s_Identifier) noexcept : s_Identifier(s_Identifier)
    {
        Logger::Singleton().Log(Logger::INFO, "Created " +
                                              s_Identifier +
                                              " player API.",
                                "Player.h", __LINE__);
    }
};

#endif /* Player_h */
