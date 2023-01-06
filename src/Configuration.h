/**
 *  Copyright (C) 2021 - 2023 The mrhpsspeech Authors.
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

#ifndef Configuration_h
#define Configuration_h

// C / C++

// External
#include <MRH_Typedefs.h>

// Project
#include "./Audio/API/AudioAPI.h"
#include "./TTS/API/TTSAPI.h"
#include "./STT/API/STTAPI.h"
#include "./Exception.h"


class Configuration
{
public:

    //*************************************************************************************
    // Configuration Blocks
    //*************************************************************************************

    /**
     *  Service
     */

    struct Service
    {
        std::string s_SocketPath = "/tmp/mrh/mrhpsspeech_audio.sock";
    };

    /**
     *  API
     */

    struct API
    {
        MRH_Uint8 u8_RecordingAPI = 0;
        MRH_Uint8 u8_PlaybackAPI = 0;
        MRH_Uint8 u8_SpeechCheckAPI = 0;
        MRH_Uint8 u8_TTSAPI = 0;
        MRH_Uint8 u8_STTAPI = 0;
    };

    /**
     *  Recording
     */

#if MRH_SPEECHD_SOUND_IO_API_SDL2 > 0
    struct SDL2Recorder
    {
        std::string s_DeviceName = "null";
        MRH_Uint32 u32_KHz = 16000;
        MRH_Uint32 u32_SamplesPerFrame = 2048;
        MRH_Uint32 u32_TrailingFrameSize = 32000;
        MRH_Sfloat32 f32_Amplification = 1.f;
    };
#endif

    /**
     *  Playback
     */

#if MRH_SPEECHD_SOUND_IO_API_SDL2 > 0
    struct SDL2Player
    {
        std::string s_DeviceName = "null";
        MRH_Uint32 u32_SamplesPerFrame = 2048;
    };
#endif

    /**
     *  Speech Check
     */

    struct ChunkVolume
    {
        MRH_Sfloat32 f32_MinVolume = 0.5f;
        MRH_Sfloat32 f32_MinSamples = 0.2f;
    };

#if MRH_SPEECHD_SPEECH_CHECKER_API_PICOVOICE_COBRA > 0
    struct PicovoiceCobra
    {
        std::string s_AccessKeyPath = "/usr/share/mrh/speechd/picovoice/accesskey.conf";
        MRH_Sfloat32 f32_MinConfidence = 0.75f;
    };
#endif

    /**
     *  TTS
     */

#if MRH_SPEECHD_TTS_API_GGOGLE_CLOUD > 0
    struct GoogleCloudTTS
    {
        std::string s_BCPDirPath = "/usr/share/mrh/speechd/gcloud/";
        std::string s_BCPFileName = "locale.conf";
        MRH_Uint8 u8_VoiceGender = 0;
        MRH_Uint32 u32_KHz = 16000;
        MRH_Uint32 u32_ChunkSamples = 2048;
    };
#endif

    /**
     *  STT
     */

#if MRH_SPEECHD_STT_API_GGOGLE_CLOUD > 0
    struct GoogleCloudSTT
    {
        std::string s_BCPDirPath = "/usr/share/mrh/speechd/gcloud/";
        std::string s_BCPFileName = "locale.conf";
    };
#endif

#if MRH_SPEECHD_STT_API_PICOVOICE_LEOPARD > 0
    struct PicovoiceLeopard
    {
        std::string s_AccessKeyPath = "/usr/share/mrh/speechd/picovoice/accesskey.conf";
        std::string s_ModelDirPath = "/usr/share/mrh/speechd/picovoice/";
        std::string s_ModelFileName = "leopard_params.pv";
    };
#endif

    //*************************************************************************************
    // Constructor / Destructor
    //*************************************************************************************

    /**
     *  Default constructor.
     */

    Configuration() noexcept;

    /**
     *  Default destructor.
     */

    ~Configuration() noexcept;

    //*************************************************************************************
    // Data
    //*************************************************************************************

    /**
     *  Service
     */

    Service c_Service;

    /**
     *  API
     */

    API c_API;

    /**
     *  Recording
     */

#if MRH_SPEECHD_SOUND_IO_API_SDL2 > 0
    SDL2Recorder c_SDL2Recorder;
#endif

    /**
     *  Playback
     */

#if MRH_SPEECHD_SOUND_IO_API_SDL2 > 0
    SDL2Player c_SDL2Player;
#endif

    /**
     *  Speech Check
     */

    ChunkVolume c_ChunkVolume;
#if MRH_SPEECHD_SPEECH_CHECKER_API_PICOVOICE_COBRA > 0
    PicovoiceCobra c_PicovoiceCobra;
#endif

    /**
     *  TTS
     */

#if MRH_SPEECHD_TTS_API_GGOGLE_CLOUD > 0
    GoogleCloudTTS c_GoogleCloudTTS;
#endif

    /**
     *  STT
     */

#if MRH_SPEECHD_STT_API_GGOGLE_CLOUD > 0
    GoogleCloudSTT c_GoogleCloudSTT;
#endif

#if MRH_SPEECHD_STT_API_PICOVOICE_LEOPARD > 0
    PicovoiceLeopard c_PicovoiceLeopard;
#endif

private:

    //*************************************************************************************
    // Data
    //**************************************************************************************

protected:

};

#endif /* Configuration_h */
