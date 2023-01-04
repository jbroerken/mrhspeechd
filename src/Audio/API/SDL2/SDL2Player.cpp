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

// Project
#include "./SDL2Player.h"

// Pre-defined
#if SDL2_PLAYER_LOG_EXTENDED > 0
    #define SDL2_PLAYER_LOG(X) Logger::Singleton().Log(Logger::INFO, X, "SDL2Player.cpp", __LINE__)
#else
    #define SDL2_PLAYER_LOG(X)
#endif


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

SDL2Player::SDL2Player(Configuration::SDL2Player const& c_Configuration) : Player("SDL2 Player"),
                                                                           p_Context(NULL),
                                                                           s_DeviceName(c_Configuration.s_DeviceName),
                                                                           u32_SamplesPerFrame(c_Configuration.u32_SamplesPerFrame)
{
    p_Context = new SDL2PlaybackContext();
}

SDL2Player::~SDL2Player() noexcept
{
    if (p_Context != NULL)
    {
        delete p_Context;
    }
}

//*************************************************************************************
// Playback
//*************************************************************************************

void SDL2Player::Start(AudioBuffer& c_Buffer)
{
    // Stop old playback first
    Stop();

    // Now Reset buffer with new info
    p_Context->c_Buffer.Reset(c_Buffer);

    // Open playback device if needed
    if (p_Context->u32_DeviceID == MRH_SDL2_AUDIO_DEVICE_ID_INVALID)
    {
        Logger::Singleton().Log(Logger::INFO, "Opening playback device " +
                                              s_DeviceName +
                                              " (KHz: " +
                                              std::to_string(p_Context->c_Buffer.GetKHz()) +
                                              ", Frame Size: " +
                                              std::to_string(u32_SamplesPerFrame) +
                                              ") ...",
                                "SDL2Player.cpp", __LINE__);
        SDL_AudioSpec c_Want;
        SDL_AudioSpec c_Have;

        SDL_zero(c_Want);

        c_Want.freq = c_Buffer.GetKHz();
        c_Want.format = AUDIO_S16SYS;
        c_Want.channels = MRH_AUDIO_BUFFER_CHANNELS;
        c_Want.samples = u32_SamplesPerFrame;
        c_Want.callback = Callback;
        c_Want.userdata = (void*)p_Context;

        if (s_DeviceName.compare(MRH_SDL2_DEFAULT_DEVICE_NAME) == 0)
        {
            p_Context->u32_DeviceID = SDL_OpenAudioDevice(NULL, 0, &c_Want, &c_Have, 0);
        }
        else
        {
            p_Context->u32_DeviceID = SDL_OpenAudioDevice(s_DeviceName.c_str(), 0, &c_Want, &c_Have, 0);
        }

        if (p_Context->u32_DeviceID == 0)
        {
            throw Exception("Failed to open playback device: " +
                            std::string(SDL_GetError()));
        }
        else if (c_Have.format != c_Want.format || c_Have.channels != c_Want.channels)
        {
            SDL_CloseAudioDevice(p_Context->u32_DeviceID);
            p_Context->u32_DeviceID = 0;

            throw Exception("Failed to get wanted playback format!");
        }

        Logger::Singleton().Log(Logger::INFO, "Opened playback device " +
                                              (s_DeviceName.size() > 0 ? s_DeviceName : "<Default>") +
                                              ".",
                                "SDL2Player.cpp", __LINE__);
    }

    // Start playback
    Logger::Singleton().Log(Logger::INFO, "Started audio playback.",
                            "SDL2Player.cpp", __LINE__);

    SDL_PauseAudioDevice(p_Context->u32_DeviceID, 0);
}

void SDL2Player::Stop() noexcept
{
    if (GetPlaying() == false)
    {
        return;
    }

    Logger::Singleton().Log(Logger::INFO, "Stopped audio playback.",
                            "SDL2Player.cpp", __LINE__);

    SDL_PauseAudioDevice(p_Context->u32_DeviceID, 1);
    SDL_CloseAudioDevice(p_Context->u32_DeviceID);

    p_Context->u32_DeviceID = MRH_SDL2_AUDIO_DEVICE_ID_INVALID;
}

//*************************************************************************************
// Callback
//*************************************************************************************

void SDL2Player::Callback(void* p_Context, Uint8* p_Stream, int i_Length) noexcept
{
    SDL2PlaybackContext* p_SDL2Context = (SDL2PlaybackContext*)p_Context;

    // Anything left to play?
    if (p_SDL2Context->c_Buffer.GetChunkCount() == 0)
    {
        // Zero buffer
        memset(p_Stream, 0, i_Length);

        // No samples left, no longer playing
        // @NOTE: PauseAudioDevice locks the audio device!
        SDL_PauseAudioDevice(p_SDL2Context->u32_DeviceID, 1);
        return;
    }

    // Audio remains, copy
    AudioBuffer::AudioChunk v_Chunk;
    size_t us_Written = 0;

    while (us_Written < i_Length)
    {
        // Get first
        if (p_SDL2Context->c_Buffer.Retrieve(v_Chunk) == false)
        {
            // No audio left, zero and return
            memset(&(p_Stream[us_Written]), 0, (i_Length - us_Written));
            return;
        }

        // Get the size required to fill the buffer
        size_t us_ChunkSize = v_Chunk.size();
        size_t us_ToWrite = i_Length - us_Written;

        if (us_ChunkSize < us_ToWrite)
        {
            us_ToWrite = us_ChunkSize;
        }

        // Copy to buffer
        memcpy(&(p_Stream[us_Written]), &(v_Chunk[0]), us_ToWrite);

        // Remember size
        us_Written += us_ToWrite;

        // Re-add remaining buffer?
        if (us_ToWrite < us_ChunkSize)
        {
            v_Chunk.erase(v_Chunk.begin() + us_ToWrite);
            p_SDL2Context->c_Buffer.Add(v_Chunk, true);
        }
    }
}

//*************************************************************************************
// Getters
//*************************************************************************************

bool SDL2Player::GetPlaying() const noexcept
{
    if (SDL_GetAudioDeviceStatus(p_Context->u32_DeviceID) == SDL_AUDIO_PLAYING)
    {
        return true;
    }

    return false;
}
