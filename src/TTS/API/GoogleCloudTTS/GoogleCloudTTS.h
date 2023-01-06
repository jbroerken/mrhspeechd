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

#ifndef GoogleCloudTTS_h
#define GoogleCloudTTS_h

// C / C++

// External

// Project
#include "../../TTS.h"
#include "../../../Configuration.h"


class GoogleCloudTTS : public TTS
{
public:

    //*************************************************************************************
    // Destructor
    //*************************************************************************************

    /**
     *  Default constructor.
     *
     *  \param c_Configuration The configuration to setup with.
     */

    GoogleCloudTTS(Configuration::GoogleCloudTTS const& c_Configuration);

    /**
     *  Default destructor.
     */

    ~GoogleCloudTTS() noexcept;

    //*************************************************************************************
    // Synthesize
    //*************************************************************************************

    /**
     *  Synthesize speech output from a given text string.
     *
     *  \param s_String The speech string to synthesize audio for.
     *  \param c_Buffer The audio buffer to store audio in. The buffer is overwritten.
     */

    void Synthesize(std::string const& s_String, AudioBuffer& c_Buffer) override;

private:

    //*************************************************************************************
    // Data
    //*************************************************************************************

    std::string s_LanguageCode;
    MRH_Uint8 u8_VoiceGender;

    MRH_Uint32 u32_KHz;
    MRH_Uint32 u32_ChunkSamples;

protected:

};

#endif /* GoogleCloudTTS_h */
