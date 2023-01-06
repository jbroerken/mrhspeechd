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

// C / C++
#include <cstring>

// External
#include <libmrhbf.h>

// Project
#include "./Configuration.h"
#include "./Logger.h"

// Pre-defined
#ifndef MRH_SPEECHD_CONFIGURATION_PATH
    #define MRH_SPEECHD_CONFIGURATION_PATH "/usr/share/mrh/speechd/speechd.conf"
#endif

namespace
{
    enum Identifier
    {
        // Block Name
        BLOCK_SERVICE = 0,
        BLOCK_API = 1,
        BLOCK_SDL2_RECORDER = 2,
        BLOCK_SDL2_PLAYER = 3,
        BLOCK_CHUNK_VOLUME = 4,
        BLOCK_PICOVOICE_COBRA = 5,
        BLOCK_GOOGLE_CLOUD_TTS = 6,
        BLOCK_GOOGLE_CLOUD_STT = 7,
        BLOCK_PICOVOICE_LEOPARD,

        // Service Key
        SERVICE_SOCKET_PATH,

        // API Key
        API_RECORDING_API,
        API_PLAYBACK_API,
        API_SPEECH_CHECK_API,
        API_TTS_API,
        API_STT_API,

        // SDL2 Recorder Key
        SDL2_RECORDER_DEVICE_NAME,
        SDL2_RECORDER_KHZ,
        SDL2_RECORDER_SAMPLES_PER_FRAME,
        SDL2_RECORDER_TRAILING_FRAME_SIZE,
        SDL2_RECORDER_AMPLIFICATION,

        // SDL2 Player Key
        SDL2_PLAYER_DEVICE_NAME,
        SDL2_PLAYER_SAMPLES_PER_FRAME,

        // Chunk Volume Key
        CHUNK_VOLUME_MIN_VOLUME,
        CHUNK_VOLUME_MIN_SAMPLES,

        // Picovoice Cobra Key
        PICOVOICE_COBRA_ACCESS_KEY_PATH,
        PICOVOICE_COBRA_MIN_CONFIDENCE,

        // Google Cloud TTS Key
        GOOGLE_CLOUD_TTS_BCP_DIRECTORY_PATH,
        GOOGLE_CLOUD_TTS_BCP_FILE_NAME,
        GOOGLE_CLOUD_TTS_VOICE_GENDER,
        GOOGLE_CLOUD_TTS_KHZ,
        GOOGLE_CLOUD_TTS_CHUNK_SAMPLES,

        // Google Cloud STT Key
        GOOGLE_CLOUD_STT_BCP_DIRECTORY_PATH,
        GOOGLE_CLOUD_STT_BCP_FILE_NAME,

        // Picovoice Leopard Key
        PICOVOICE_LEOPARD_ACCESS_KEY_PATH,
        PICOVOICE_LEOPARD_MODEL_DIRECTORY_PATH,
        PICOVOICE_LEOPARD_MODEL_FILE_NAME,

        // Bounds
        IDENTIFIER_MAX = PICOVOICE_LEOPARD_MODEL_FILE_NAME,

        IDENTIFIER_COUNT = IDENTIFIER_MAX + 1
    };

    const char* p_Identifier[IDENTIFIER_COUNT] =
    {
        // Block Name
        "Service",
        "API",
        "SDL2Recorder",
        "SDL2Player",
        "ChunkVolume",
        "PicovoiceCobra",
        "GoogleCloudTTS",
        "GoogleCloudSTT",
        "PicovoiceLeopard",

        // Service
        "SocketPath",

        // API Key
        "Recording",
        "Playback",
        "SpeechCheck",
        "TTS",
        "STT",

        // SDL2 Recorder Key
        "DeviceName",
        "KHz",
        "SamplesPerFrame",
        "TrailingFrameSize",
        "Amplification",

        // SDL2 Player Key
        "DeviceName",
        "SamplesPerFrame",

        // Chunk Volume Key
        "MinVolume",
        "MinSamples",

        // Picovoice Cobra Key
        "AccessKeyPath",
        "MinConfidence",

        // Google Cloud TTS Key
        "BCPDirectoryPath",
        "BCPFileName",
        "VoiceGender",
        "KHz",
        "ChunkSamples",

        // Google Cloud STT Key
        "BCPDirectoryPath",
        "BCPFileName",

        // Picovoice Leopard Key
        "AccessKeyPath",
        "ModelDirectoryPath",
        "ModelFileName"
    };
}


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

Configuration::Configuration() noexcept
{
    try
    {
        MRH::BF::BlockFile c_File(MRH_SPEECHD_CONFIGURATION_PATH);

        for (auto& Block : c_File.l_Block)
        {
            /**
             *  Service
             */

            if (Block.GetName().compare(p_Identifier[BLOCK_SERVICE]) == 0)
            {
                c_Service.s_SocketPath = Block.GetValue(p_Identifier[SERVICE_SOCKET_PATH]);

                continue;
            }

            /**
             *  API
             */

            if (Block.GetName().compare(p_Identifier[BLOCK_API]) == 0)
            {
                c_API.u8_RecordingAPI = static_cast<MRH_Uint8>(std::stoi(Block.GetValue(p_Identifier[API_RECORDING_API])));
                c_API.u8_PlaybackAPI = static_cast<MRH_Uint8>(std::stoi(Block.GetValue(p_Identifier[API_PLAYBACK_API])));
                c_API.u8_SpeechCheckAPI = static_cast<MRH_Uint8>(std::stoi(Block.GetValue(p_Identifier[API_SPEECH_CHECK_API])));
                c_API.u8_TTSAPI = static_cast<MRH_Uint8>(std::stoi(Block.GetValue(p_Identifier[API_TTS_API])));
                c_API.u8_STTAPI = static_cast<MRH_Uint8>(std::stoi(Block.GetValue(p_Identifier[API_STT_API])));

                continue;
            }

            /**
             *  Recording
             */

#if MRH_SPEECHD_SOUND_IO_API_SDL2 > 0
            if (Block.GetName().compare(p_Identifier[BLOCK_SDL2_RECORDER]) == 0)
            {
                c_SDL2Recorder.s_DeviceName = Block.GetValue(p_Identifier[SDL2_RECORDER_DEVICE_NAME]);
                c_SDL2Recorder.u32_KHz = static_cast<MRH_Uint32>(std::stoull(Block.GetValue(p_Identifier[SDL2_RECORDER_KHZ])));
                c_SDL2Recorder.u32_SamplesPerFrame = static_cast<MRH_Uint32>(std::stoull(Block.GetValue(p_Identifier[SDL2_RECORDER_SAMPLES_PER_FRAME])));
                c_SDL2Recorder.u32_TrailingFrameSize = static_cast<MRH_Uint32>(std::stoull(Block.GetValue(p_Identifier[SDL2_RECORDER_TRAILING_FRAME_SIZE])));
                c_SDL2Recorder.f32_Amplification = std::stof(Block.GetValue(p_Identifier[SDL2_RECORDER_AMPLIFICATION]));

                continue;
            }
#endif
            
            /**
             *  Playback
             */

#if MRH_SPEECHD_SOUND_IO_API_SDL2 > 0
            if (Block.GetName().compare(p_Identifier[BLOCK_SDL2_PLAYER]) == 0)
            {
                c_SDL2Player.s_DeviceName = Block.GetValue(p_Identifier[SDL2_PLAYER_DEVICE_NAME]);
                c_SDL2Player.u32_SamplesPerFrame = static_cast<MRH_Uint32>(std::stoull(Block.GetValue(p_Identifier[SDL2_PLAYER_SAMPLES_PER_FRAME])));

                continue;
            }
#endif

            /**
             *  Speech Check
             */

            if (Block.GetName().compare(p_Identifier[BLOCK_CHUNK_VOLUME]) == 0)
            {
                c_ChunkVolume.f32_MinVolume = std::stof(Block.GetValue(p_Identifier[CHUNK_VOLUME_MIN_VOLUME]));
                c_ChunkVolume.f32_MinVolume = std::stof(Block.GetValue(p_Identifier[CHUNK_VOLUME_MIN_SAMPLES]));

                continue;
            }

#if MRH_SPEECHD_SPEECH_CHECKER_API_PICOVOICE_COBRA > 0
            if (Block.GetName().compare(p_Identifier[BLOCK_PICOVOICE_COBRA]) == 0)
            {
                c_PicovoiceCobra.s_AccessKeyPath = Block.GetValue(p_Identifier[PICOVOICE_COBRA_ACCESS_KEY_PATH]);
                c_PicovoiceCobra.f32_MinConfidence = std::stof(Block.GetValue(p_Identifier[PICOVOICE_COBRA_MIN_CONFIDENCE]));

                continue;
            }
#endif

            /**
             *  TTS
             */

#if MRH_SPEECHD_TTS_API_GGOGLE_CLOUD > 0
            if (Block.GetName().compare(p_Identifier[BLOCK_GOOGLE_CLOUD_TTS]) == 0)
            {
                c_GoogleCloudTTS.s_BCPDirPath = Block.GetValue(p_Identifier[GOOGLE_CLOUD_TTS_BCP_DIRECTORY_PATH]);
                c_GoogleCloudTTS.s_BCPFileName = Block.GetValue(p_Identifier[GOOGLE_CLOUD_TTS_BCP_FILE_NAME]);
                c_GoogleCloudTTS.u8_VoiceGender = static_cast<MRH_Uint8>(std::stoull(Block.GetValue(p_Identifier[GOOGLE_CLOUD_TTS_VOICE_GENDER])));
                c_GoogleCloudTTS.u32_KHz = static_cast<MRH_Uint32>(std::stoull(Block.GetValue(p_Identifier[GOOGLE_CLOUD_TTS_KHZ])));
                c_GoogleCloudTTS.u32_ChunkSamples = static_cast<MRH_Uint32>(std::stoull(Block.GetValue(p_Identifier[GOOGLE_CLOUD_TTS_CHUNK_SAMPLES])));

                continue;
            }
#endif

            /**
             *  STT
             */

#if MRH_SPEECHD_STT_API_GGOGLE_CLOUD > 0
            if (Block.GetName().compare(p_Identifier[BLOCK_GOOGLE_CLOUD_STT]) == 0)
            {
                c_GoogleCloudSTT.s_BCPDirPath = Block.GetValue(p_Identifier[GOOGLE_CLOUD_STT_BCP_DIRECTORY_PATH]);
                c_GoogleCloudSTT.s_BCPFileName = Block.GetValue(p_Identifier[GOOGLE_CLOUD_STT_BCP_FILE_NAME]);

                continue;
            }
#endif

#if MRH_SPEECHD_STT_API_PICOVOICE_LEOPARD > 0
            if (Block.GetName().compare(p_Identifier[BLOCK_PICOVOICE_LEOPARD]) == 0)
            {
                c_PicovoiceLeopard.s_AccessKeyPath = Block.GetValue(p_Identifier[PICOVOICE_LEOPARD_ACCESS_KEY_PATH]);
                c_PicovoiceLeopard.s_ModelDirPath = Block.GetValue(p_Identifier[PICOVOICE_LEOPARD_MODEL_DIRECTORY_PATH]);
                c_PicovoiceLeopard.s_ModelFileName = Block.GetValue(p_Identifier[PICOVOICE_LEOPARD_MODEL_FILE_NAME]);

                continue;
            }
#endif
        }
    }
    catch (std::exception& e)
    {
        Logger::Singleton().Log(Logger::WARNING, "Could not read configuration: " +
                                                 std::string(e.what()),
                                "Configuration.cpp", __LINE__);
    }
}

Configuration::~Configuration() noexcept
{}
