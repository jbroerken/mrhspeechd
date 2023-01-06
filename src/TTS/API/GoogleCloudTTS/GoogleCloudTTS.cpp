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
#include <fstream>

// External
#include <google/cloud/texttospeech/v1/cloud_tts.grpc.pb.h>
#include <google/longrunning/operations.grpc.pb.h>
#include <grpcpp/grpcpp.h>
#include <libmrhvt/String/MRH_LocalisedPath.h>

// Project
#include "./GoogleCloudTTS.h"

// Pre-defined
#if GOOGLE_CLOUD_TTS_LOG_EXTENDED > 0
    #define GOOGLE_CLOUD_TTS_LOG(X) Logger::Singleton().Log(Logger::INFO, X, "GoogleCloudTTS.cpp", __LINE__)
#else
    #define GOOGLE_CLOUD_TTS_LOG(X)
#endif
#define GOOGLE_CLOUD_TTS_CHANNEL "texttospeech.googleapis.com"

// Namespace
using google::cloud::texttospeech::v1::TextToSpeech;
using google::cloud::texttospeech::v1::SynthesizeSpeechRequest;
using google::cloud::texttospeech::v1::SynthesizeSpeechResponse;
using google::cloud::texttospeech::v1::AudioEncoding;
using google::cloud::texttospeech::v1::SsmlVoiceGender;


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

GoogleCloudTTS::GoogleCloudTTS(Configuration::GoogleCloudTTS const& c_Configuration) : TTS("Google Cloud API TTS"),
                                                                                       s_LanguageCode(""),
                                                                                       u8_VoiceGender(c_Configuration.u8_VoiceGender),
                                                                                       u32_KHz(c_Configuration.u32_KHz),
                                                                                       u32_ChunkSamples(c_Configuration.u32_ChunkSamples)
{
    std::string s_LocaleFilePath = MRH::VT::LocalisedPath::GetPath(c_Configuration.s_BCPDirPath, c_Configuration.s_BCPFileName);
    std::ifstream f_File(s_LocaleFilePath);

    if (f_File.is_open() == false)
    {
        throw Exception("Failed to open " +
                        s_LocaleFilePath +
                        " TTS BCP-47 locale file!");
    }

    getline(f_File, s_LanguageCode);
    f_File.close();

    Logger::Singleton().Log(Logger::INFO, "Set Google Cloud API TTS locale to " +
                                          s_LanguageCode,
                            "GoogleCloudTTS.cpp", __LINE__);
}

GoogleCloudTTS::~GoogleCloudTTS() noexcept
{}

//*************************************************************************************
// Synthesize
//*************************************************************************************

void GoogleCloudTTS::Synthesize(std::string const& s_String, AudioBuffer& c_Buffer)
{
    if (s_String.empty() == true)
    {
        throw Exception("Empty string given!");
    }

    SsmlVoiceGender c_VoiceGender = SsmlVoiceGender::FEMALE;

    if (u8_VoiceGender > 0)
    {
        c_VoiceGender = SsmlVoiceGender::MALE;
    }

    /**
     *  Credentials Setup
     */

    // Setup google connection with credentials for this request
    auto c_Credentials = grpc::GoogleDefaultCredentials();
    auto c_CloudChannel = grpc::CreateChannel(GOOGLE_CLOUD_TTS_CHANNEL, c_Credentials);
    std::unique_ptr<TextToSpeech::Stub> p_TextToSpeech(TextToSpeech::NewStub(c_CloudChannel));

    /**
     *  Create request
     */

    // Define our request to use for config and audio
    SynthesizeSpeechRequest c_SynthesizeRequest;

    // Set synthesise configuration
    // @NOTE: Default returned is mono!
    auto* p_AudioConfig = c_SynthesizeRequest.mutable_audio_config();
    p_AudioConfig->set_audio_encoding(AudioEncoding::LINEAR16);
    p_AudioConfig->set_sample_rate_hertz(u32_KHz);

    // Set output voice info
    auto* p_VoiceConfig = c_SynthesizeRequest.mutable_voice();
    p_VoiceConfig->set_ssml_gender(c_VoiceGender);
    p_VoiceConfig->set_language_code(s_LanguageCode);

    // Set the string
    c_SynthesizeRequest.mutable_input()->set_text(s_String);

    /**
     *  Synthesize
     */

    GOOGLE_CLOUD_TTS_LOG("Synthesizing string " +
                         s_String +
                         "...");

    grpc::ClientContext c_Context;
    SynthesizeSpeechResponse c_SynthesizeResponse;
    grpc::Status c_RPCStatus = p_TextToSpeech->SynthesizeSpeech(&c_Context,
                                                                c_SynthesizeRequest,
                                                                &c_SynthesizeResponse);

    if (c_RPCStatus.ok() == false)
    {
        throw Exception("Failed to synthesise: GRPC streamer error: " +
                        c_RPCStatus.error_message());
    }

    /**
     *  Add Synthesized
     */

    // Grab the synth data
    MRH_Sint16* p_Buffer = (MRH_Sint16*)c_SynthesizeResponse.audio_content().data();
    size_t us_Elements;

    if (p_Buffer == NULL || (us_Elements = c_SynthesizeResponse.audio_content().size() / sizeof(MRH_Sint16)) == 0)
    {
        throw Exception("Invalid synthesized audio!");
    }

    GOOGLE_CLOUD_TTS_LOG("Synthesized audio received with " +
                         std::to_string(us_Elements) +
                         " samples.");

    try
    {
        std::deque<AudioBuffer::AudioChunk> dq_Chunk;

        for (size_t i = 0; i < us_Elements; i += u32_ChunkSamples)
        {
            if ((i + u32_ChunkSamples) < us_Elements)
            {
                dq_Chunk.emplace_back(&(p_Buffer[i]),
                                      &(p_Buffer[i]) + u32_ChunkSamples);
            }
            else
            {
                dq_Chunk.emplace_back(&(p_Buffer[i]),
                                      &(p_Buffer[i]) + (us_Elements - i));
            }
        }

        c_Buffer.Reset(u32_KHz, dq_Chunk);
    }
    catch (...)
    {
        throw;
    }
}
