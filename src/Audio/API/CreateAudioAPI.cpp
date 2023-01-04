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

// External

// Project
#include "./CreateAudioAPI.h"
#include "./ChunkVolume/ChunkVolume.h"
#if MRH_SPEECHD_SOUND_IO_API_SDL2 > 0
#include "./SDL2/SDL2Recorder.h"
#include "./SDL2/SDL2Player.h"
#endif
#if MRH_SPEECHD_SPEECH_CHECKER_API_PICOVOICE_COBRA > 0
#include "./PicovoiceCobra/PicovoiceCobra.h"
#endif


//*************************************************************************************
// Requirements
//*************************************************************************************

void CreateAudioAPI::Init(Configuration const& c_Configuration)
{
    auto const& API = c_Configuration.c_API;

#if MRH_SPEECHD_SOUND_IO_API_SDL2 > 0
    if (API.u8_RecordingAPI == RECORDER_API_SDL2 || API.u8_PlaybackAPI == PLAYER_API_SDL2)
    {
        if (SDL_Init(SDL_INIT_AUDIO) < 0)
        {
            throw Exception("Failed to initialize SDL2: " +
                            std::string(SDL_GetError()));
        }
    }
#endif
}

void CreateAudioAPI::Destroy(Configuration const& c_Configuration) noexcept
{
    auto const& API = c_Configuration.c_API;

#if MRH_SPEECHD_SOUND_IO_API_SDL2 > 0
    if (API.u8_RecordingAPI == RECORDER_API_SDL2 || API.u8_PlaybackAPI == PLAYER_API_SDL2)
    {
        SDL_Quit();
    }
#endif
}

//*************************************************************************************
// Recording API
//*************************************************************************************

std::shared_ptr<Recorder> CreateAudioAPI::CreateRecorder(Configuration const& c_Configuration, std::shared_ptr<RecorderContext>& p_Context)
{
    try
    {
        switch (c_Configuration.c_API.u8_RecordingAPI)
        {
#if MRH_SPEECHD_SOUND_IO_API_SDL2 > 0
            case RECORDER_API_SDL2:
                return std::make_shared<SDL2Recorder>(c_Configuration.c_SDL2Recorder,
                                                      p_Context);
#endif
            default:
                throw Exception("Unknown or unsupported recording API!");
        }
    }
    catch (std::exception& e)
    {
        throw Exception(e.what());
    }
}

//*************************************************************************************
// Playback API
//*************************************************************************************

std::shared_ptr<Player> CreateAudioAPI::CreatePlayer(Configuration const& c_Configuration)
{
    try
    {
        switch (c_Configuration.c_API.u8_PlaybackAPI)
        {
#if MRH_SPEECHD_SOUND_IO_API_SDL2 > 0
            case PLAYER_API_SDL2:
                return std::make_shared<SDL2Player>(c_Configuration.c_SDL2Player);
#endif
            default:
                throw Exception("Unknown or unsupported playback API!");
        }
    }
    catch (std::exception& e)
    {
        throw Exception(e.what());
    }
}

//*************************************************************************************
// Speech Check API
//*************************************************************************************

std::shared_ptr<SpeechChecker> CreateAudioAPI::CreateSpeechChecker(Configuration const& c_Configuration)
{
    try
    {
        switch (c_Configuration.c_API.u8_SpeechCheckAPI)
        {
            case SPEECH_CHECKER_API_CHUNK_VOLUME:
                return std::make_shared<ChunkVolume>(c_Configuration.c_ChunkVolume);
#if MRH_SPEECHD_SPEECH_CHECKER_API_PICOVOICE_COBRA > 0
            case SPEECH_CHECKER_API_PICOVOICE_COBRA:
                return std::make_shared<PicovoiceCobra>(c_Configuration.c_PicovoiceCobra);
#endif
            default:
                throw Exception("Unknown or unsupported speech checker API!");
        }

    }
    catch (std::exception& e)
    {
        throw Exception(e.what());
    }
}
