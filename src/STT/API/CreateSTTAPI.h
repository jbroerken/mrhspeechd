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

#ifndef CreateSTTAPI_h
#define CreateSTTAPI_h

// C / C++

// External

// Project
#include "./STTAPI.h"
#include "../STT.h"
#include "../../Configuration.h"


namespace CreateSTTAPI
{
    //*************************************************************************************
    // Requirements
    //*************************************************************************************

    /**
     *  Initialize API requirements.
     *
     *  \param c_Configuration The configuration to initialize with.
     */

    void Init(Configuration const& c_Configuration);

    /**
     *  Destroy API requirements.
     *
     *  \param c_Configuration The configuration to destroy with.
     */

    void Destroy(Configuration const& c_Configuration) noexcept;

    //*************************************************************************************
    // STT API
    //*************************************************************************************

    /**
     *  Create a speech to text transcriber.
     *
     *  \param c_Configuration The configuration to create with.
     *
     *  \return The created speech to text transcriber.
     */

    std::shared_ptr<STT> CreateSTT(Configuration const& c_Configuration);
};


#endif /* CreateSTTAPI_h */
