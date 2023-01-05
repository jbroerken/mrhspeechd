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

#ifndef SDL2Recorder_h
#define SDL2Recorder_h

// C / C++

// External

// Project
#include "../../Recorder.h"
#include "./SDL2RecordingContext.h"
#include "../../../Configuration.h"


class SDL2Recorder : public Recorder
{
public:

    //*************************************************************************************
    // Constructor / Destructor
    //*************************************************************************************

    /**
     *  Default constructor.
     *
     *  \param c_Configuration The configuration to setup with.
     *  \param p_Context The recorder context to use.
     */

    SDL2Recorder(Configuration::SDL2Recorder const& c_Configuration,
                 std::shared_ptr<RecorderContext>& p_Context);

    /**
     *  Default destructor.
     */

    ~SDL2Recorder() noexcept;

    //*************************************************************************************
    // Recording
    //*************************************************************************************

    /**
     *  Start recording.
     *
     *  \param b_Clear If the current recording should be cleared.
     */

    void Start(bool b_Clear) override;

    /**
     *  Stop recording.
     */

    void Stop() noexcept override;

    //*************************************************************************************
    // Getters
    //*************************************************************************************

    /**
     *  Check if recording is active.
     *
     *  \return true if active, false if not.
     */

    bool GetRecording() const noexcept override;

    /**
     *  Get all currently recorded audio.
     *
     *  \param c_Buffer The audio buffer to store in. The buffer is overwritten.
     */

    void GetRecordedAudio(AudioBuffer& c_Buffer) override;

private:

    //*************************************************************************************
    // Callback
    //*************************************************************************************

    /**
     *  Audio recording callback.
     *
     *  \param p_Context The callback context.
     *  \param p_Stream The audio stream bytes to write.
     *  \param i_Length The required length to write in bytes.
     */

    static void Callback(void* p_Context, Uint8* p_Stream, int i_Length) noexcept;

    //*************************************************************************************
    // Data
    //*************************************************************************************

    SDL2RecordingContext* p_Context;

    std::string s_DeviceName;
    MRH_Uint32 u32_SamplesPerFrame;
    
protected:

};

#endif /* SDL2Recorder_h */
