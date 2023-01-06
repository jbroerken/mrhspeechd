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
#include <google/cloud/speech/v1/cloud_speech.grpc.pb.h>
#include <google/longrunning/operations.grpc.pb.h>
#include <grpcpp/grpcpp.h>
#include <libmrhvt/String/MRH_LocalisedPath.h>

// Project
#include "./GoogleCloudSTT.h"

// Pre-defined
#if GOOGLE_CLOUD_STT_LOG_EXTENDED > 0
    #define GOOGLE_CLOUD_STT_LOG(X) Logger::Singleton().Log(Logger::INFO, X, "GoogleCloudSTT.cpp", __LINE__)
#else
    #define GOOGLE_CLOUD_STT_LOG(X)
#endif
#define GOOGLE_CLOUD_STT_CHANNEL "speech.googleapis.com"

// Namespace
using google::cloud::speech::v1::Speech;
using google::cloud::speech::v1::RecognizeRequest;
using google::cloud::speech::v1::RecognizeResponse;
using google::cloud::speech::v1::RecognitionConfig;
using google::cloud::speech::v1::StreamingRecognitionResult;


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

GoogleCloudSTT::GoogleCloudSTT(Configuration::GoogleCloudSTT const& c_Configuration) : STT("Google Cloud API STT"),
                                                                                       s_LanguageCode("")
{
    std::string s_LocaleFilePath = MRH::VT::LocalisedPath::GetPath(c_Configuration.s_BCPDirPath, c_Configuration.s_BCPFileName);
    std::ifstream f_File(s_LocaleFilePath);

    if (f_File.is_open() == false)
    {
        throw Exception("Failed to open " +
                        s_LocaleFilePath +
                        " STT BCP-47 locale file!");
    }

    getline(f_File, s_LanguageCode);
    f_File.close();

    Logger::Singleton().Log(Logger::INFO, "Set Google Cloud API STT locale to " +
                                          s_LanguageCode,
                            "GoogleCloudSTT.cpp", __LINE__);
}

GoogleCloudSTT::~GoogleCloudSTT() noexcept
{}

//*************************************************************************************
// Transcribe
//*************************************************************************************

void GoogleCloudSTT::Transcribe(AudioBuffer& c_Buffer, std::string& s_String)
{
    // Create full buffer
    size_t us_SampleCount = PrepareAudio(c_Buffer);

    if (us_SampleCount == 0)
    {
        throw Exception("No audio samples to transcribe!");
    }

    GOOGLE_CLOUD_STT_LOG("Transcribing audio with " +
                         std::to_string(us_SampleCount) +
                         " samples, " +
                         std::to_string(c_Buffer.GetKHz()) +
                         " KHz.");

    /**
     *  Credentials Setup
     */

    // @NOTE: Google speech api is accessed as shown here:
    //        https://github.com/GoogleCloudPlatform/cpp-samples/blob/main/speech/api/transcribe.cc

    // Setup google connection with credentials for this request
    auto c_Credentials = grpc::GoogleDefaultCredentials();
    auto c_CloudChannel = grpc::CreateChannel(GOOGLE_CLOUD_STT_CHANNEL, c_Credentials);
    std::unique_ptr<Speech::Stub> p_Speech(Speech::NewStub(c_CloudChannel));

    /**
     *  Create Request
     */

    // Define our request to use for config and audio
    RecognizeRequest c_RecognizeRequest;

    // Set recognition configuration
    auto* p_Config = c_RecognizeRequest.mutable_config();
    p_Config->set_language_code(s_LanguageCode);
    p_Config->set_sample_rate_hertz(c_Buffer.GetKHz());
    p_Config->set_encoding(RecognitionConfig::LINEAR16);
    p_Config->set_profanity_filter(true);
    p_Config->set_audio_channel_count(1); // Always mono

    // Now add the audio
    c_RecognizeRequest.mutable_audio()->set_content(v_Audio.data(),
                                                    us_SampleCount * sizeof(MRH_Sint16)); // Byte len

    /**
     *  Transcribe
     */

    grpc::ClientContext c_Context;
    RecognizeResponse c_RecognizeResponse;
    grpc::Status c_RPCStatus = p_Speech->Recognize(&c_Context,
                                                   c_RecognizeRequest,
                                                   &c_RecognizeResponse);

    if (c_RPCStatus.ok() == false)
    {
        throw Exception("Failed to transcribe: GRPC streamer error: " +
                        c_RPCStatus.error_message());
    }

    /**
     *  Select Transcribed
     */

    // Check all results and grab highest confidence
    float f32_Confidence = -1.f;
    std::string s_Transcipt = "";

    for (int i = 0; i < c_RecognizeResponse.results_size(); ++i)
    {
        const auto& c_Result = c_RecognizeResponse.results(i);

        for (int j = 0; j < c_Result.alternatives_size(); ++j)
        {
            const auto& c_Alternative = c_Result.alternatives(i);

            if (f32_Confidence < c_Alternative.confidence())
            {
                f32_Confidence = c_Alternative.confidence();
                s_Transcipt = c_Alternative.transcript();
            }
        }
    }

    s_String = s_Transcipt;

    GOOGLE_CLOUD_STT_LOG("Transcription result: " +
                         s_String);
}
