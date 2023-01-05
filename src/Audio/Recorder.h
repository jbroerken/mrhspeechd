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

#ifndef Recorder_h
#define Recorder_h

// C / C++

// External

// Project
#include "./RecorderContext.h"


class Recorder
{
public:

    //*************************************************************************************
    // Destructor
    //*************************************************************************************

    /**
     *  Default destructor.
     */

    virtual ~Recorder() noexcept
    {}

    //*************************************************************************************
    // Recording
    //*************************************************************************************

    /**
     *  Start recording.
     *
     *  \param b_Clear If the current recording should be cleared.
     */

    virtual void Start(bool b_Clear)
    {
        throw Exception("Default Start() function called!");
    }

    /**
     *  Stop recording.
     */

    virtual void Stop() noexcept
    {}

    //*************************************************************************************
    // Getters
    //*************************************************************************************

    /**
     *  Check if recording is active.
     *
     *  \return true if active, false if not.
     */

    virtual bool GetRecording() const noexcept
    {
        return false;
    }

    /**
     *  Check if speech was recorded.
     *
     *  \return true if speech was recorded, false if not.
     */

    bool GetSpeechRecorded() const noexcept
    {
        return p_Context->b_SpeechRecorded;
    }

    /**
     *  Get all currently recorded audio.
     *
     *  \param c_Buffer The audio buffer to store in. The buffer is overwritten.
     */

    virtual void GetRecordedAudio(AudioBuffer& c_Buffer)
    {
        throw Exception("Default GetRecordedAudio() function called!");
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
     *  \param s_Identifier The recorder identifier.
     *  \param p_Context The recorder context to use.
     */

    Recorder(std::string const& s_Identifier,
             std::shared_ptr<RecorderContext>& p_Context) noexcept : s_Identifier(s_Identifier),
                                                                     p_Context(p_Context)
    {
        Logger::Singleton().Log(Logger::INFO, "Created [ " +
                                              s_Identifier +
                                              " ] recorder API.",
                                "Recorder.h", __LINE__);
    }

    //*************************************************************************************
    // Data
    //*************************************************************************************

    std::shared_ptr<RecorderContext> p_Context;
};

#endif /* Recorder_h */
