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
#include <SDL2/SDL.h>

// Project
#include "./SDL2Recorder.h"

// Pre-defined
#if SDL2_RECORDER_LOG_EXTENDED > 0
    #define SDL2_RECORDER_LOG(X) Logger::Singleton().Log(Logger::INFO, X, "SDL2Recorder.cpp", __LINE__)
#else
    #define SDL2_RECORDER_LOG(X)
#endif


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

SDL2Recorder::SDL2Recorder(Configuration::SDL2Recorder const& c_Configuration,
                           std::shared_ptr<RecorderContext>& p_Context) : Recorder("SDL2 Recorder",
                                                                                   p_Context),
                                                                          s_DeviceName(c_Configuration.s_DeviceName),
                                                                          u32_SamplesPerFrame(c_Configuration.u32_SamplesPerFrame)
{
    this->p_Context = new SDL2RecordingContext(c_Configuration.u32_KHz,
                                               c_Configuration.u32_TrailingFrameSize,
                                               c_Configuration.f32_Amplification,
                                               p_Context);
}

SDL2Recorder::~SDL2Recorder() noexcept
{
    if (p_Context != NULL)
    {
        delete p_Context;
    }
}

//*************************************************************************************
// Recording
//*************************************************************************************

void SDL2Recorder::Start(bool b_Clear)
{
    // Already recording?
    if (GetRecording() == true)
    {
        if (b_Clear == false)
        {
            return;
        }
        else
        {
            Stop();
        }
    }

    // Clear old recording
    p_Context->c_Buffer.Clear();

    // Flag context
    p_Context->p_Context->b_SpeechRecorded = false;
    p_Context->u32_TrailingFrameSizeCurrent = 0;

    // Open playback device if needed
    if (p_Context->u32_DeviceID == MRH_SDL2_AUDIO_DEVICE_ID_INVALID)
    {
        Logger::Singleton().Log(Logger::INFO, "Opening recording device " +
                                              s_DeviceName +
                                              " (KHz: " +
                                              std::to_string(p_Context->c_Buffer.GetKHz()) +
                                              ", Frame Size: " +
                                              std::to_string(u32_SamplesPerFrame) +
                                              ") ...",
                                "SDL2Recorder.cpp", __LINE__);
        SDL_AudioSpec c_Want;
        SDL_AudioSpec c_Have;

        SDL_zero(c_Want);

        c_Want.freq = p_Context->c_Buffer.GetKHz();
        c_Want.format = AUDIO_S16SYS;
        c_Want.channels = MRH_AUDIO_BUFFER_CHANNELS;
        c_Want.samples = u32_SamplesPerFrame;
        c_Want.callback = Callback;
        c_Want.userdata = (void*)p_Context;

        if (s_DeviceName.compare(MRH_SDL2_DEFAULT_DEVICE_NAME) == 0)
        {
            p_Context->u32_DeviceID = SDL_OpenAudioDevice(NULL, 1, &c_Want, &c_Have, SDL_AUDIO_ALLOW_SAMPLES_CHANGE);
        }
        else
        {
            p_Context->u32_DeviceID = SDL_OpenAudioDevice(s_DeviceName.c_str(), 1, &c_Want, &c_Have, SDL_AUDIO_ALLOW_SAMPLES_CHANGE);
        }

        if (p_Context->u32_DeviceID == 0)
        {
            throw Exception("Failed to open recording device: " +
                            std::string(SDL_GetError()));
        }
        else if (c_Have.format != c_Want.format || c_Have.channels != c_Want.channels)
        {
            SDL_CloseAudioDevice(p_Context->u32_DeviceID);
            p_Context->u32_DeviceID = 0;

            throw Exception("Failed to get wanted recording format!");
        }
        else if (c_Have.samples != c_Want.samples)
        {
            Logger::Singleton().Log(Logger::WARNING, "Sample rate changed by SDL2!",
                                    "SDL2Recorder.cpp", __LINE__);
        }

        if (s_DeviceName.compare(MRH_SDL2_DEFAULT_DEVICE_NAME) == 0)
        {
            Logger::Singleton().Log(Logger::INFO, "Opened system default recording device.",
                                    "SDL2Recorder.cpp", __LINE__);
        }
        else
        {
            Logger::Singleton().Log(Logger::INFO, "Opened recording device: " +
                                                  s_DeviceName +
                                                  ".",
                                    "SDL2Recorder.cpp", __LINE__);
        }
    }

    // Start recording
    Logger::Singleton().Log(Logger::INFO, "Started audio recording.",
                            "SDL2Recorder.cpp", __LINE__);

    SDL_PauseAudioDevice(p_Context->u32_DeviceID, 0);
}

void SDL2Recorder::Stop() noexcept
{
    if (GetRecording() == false)
    {
        return;
    }

    Logger::Singleton().Log(Logger::INFO, "Stopped audio recording.",
                            "SDL2Recorder.cpp", __LINE__);

    SDL_PauseAudioDevice(p_Context->u32_DeviceID, 1);
    SDL_CloseAudioDevice(p_Context->u32_DeviceID);

    p_Context->u32_DeviceID = MRH_SDL2_AUDIO_DEVICE_ID_INVALID;
}

//*************************************************************************************
// Callback
//*************************************************************************************

void SDL2Recorder::Callback(void* p_Context, Uint8* p_Stream, int i_Length) noexcept
{
    SDL2RecordingContext* p_SDL2Context = (SDL2RecordingContext*)p_Context;

    if (i_Length < sizeof(MRH_Sint16))
    {
        return;
    }

    MRH_Sint16* p_Audio = (MRH_Sint16*)p_Stream;
    size_t us_Length = i_Length / sizeof(MRH_Sint16);

    AudioBuffer::AudioChunk v_Chunk(p_Audio,
                                    p_Audio + us_Length);

    SDL2_RECORDER_LOG("Amplifying recorded samples by " +
                      std::to_string(p_SDL2Context->f32_Amplification) +
                      ".");

    for (auto& Sample : v_Chunk)
    {
        MRH_Sint32 s32_Sample = Sample * p_SDL2Context->f32_Amplification;

        if (s32_Sample > INT16_MAX)
        {
            Sample = INT16_MAX;
        }
        else if (s32_Sample < INT16_MIN)
        {
            Sample = INT16_MIN;
        }
        else
        {
            Sample = static_cast<MRH_Sint16>(s32_Sample);
        }
    }

    try
    {
        if (p_SDL2Context->p_Context->p_SpeechChecker->IsSpeech(v_Chunk) == true)
        {
            SDL2_RECORDER_LOG("Speech recognized, adding chunk and resetting trailing sample count.");

            p_SDL2Context->c_Buffer.Add(v_Chunk, false);

            p_SDL2Context->p_Context->b_SpeechRecorded = true;
            p_SDL2Context->u32_TrailingFrameSizeCurrent = 0; // Reset, audio found again

            return;
        }
    }
    catch (Exception& e)
    {
        Logger::Singleton().Log(Logger::ERROR, e.what(),
                                "SDL2Recorder.cpp", __LINE__);
        return;
    }

    if (p_SDL2Context->p_Context->b_SpeechRecorded == false)
    {
        return;
    }

    // Add trailing frames?
    if (p_SDL2Context->u32_TrailingFrameSizeCurrent < p_SDL2Context->u32_TrailingFrameSizeMax)
    {
        p_SDL2Context->c_Buffer.Add(v_Chunk, false);

        p_SDL2Context->u32_TrailingFrameSizeCurrent += us_Length;

        SDL2_RECORDER_LOG("No speech found, add " +
                          std::to_string(us_Length) +
                          " trailing samples (now " +
                          std::to_string(p_SDL2Context->u32_TrailingFrameSizeCurrent) +
                          ")");
        return;
    }

    // Ending, pause recording and notfy of audio
    SDL2_RECORDER_LOG("Recording finished, notifying...");

    SDL_PauseAudioDevice(p_SDL2Context->u32_DeviceID, 1);

    p_SDL2Context->p_Context->p_Notifier->Notify(false);
}

//*************************************************************************************
// Getters
//*************************************************************************************

bool SDL2Recorder::GetRecording() const noexcept
{
    if (SDL_GetAudioDeviceStatus(p_Context->u32_DeviceID) == SDL_AUDIO_PLAYING)
    {
        return true;
    }

    return false;
}

void SDL2Recorder::GetRecordedAudio(AudioBuffer& c_Buffer)
{
    if (GetRecording() == true)
    {
        throw Exception("Cannot get recorded audio while recording!");
    }

    c_Buffer.Reset(p_Context->c_Buffer);
}
