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

#ifndef PicovoiceLeopard_h
#define PicovoiceLeopard_h

// C / C++

// External
#include <pv_leopard.h>

// Project
#include "../../STT.h"
#include "../../../Configuration.h"


class PicovoiceLeopard : public STT
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

    PicovoiceLeopard(Configuration::PicovoiceLeopard const& c_Configuration);

    /**
     *  Default destructor.
     */

    ~PicovoiceLeopard() noexcept;

    //*************************************************************************************
    // Transcribe
    //*************************************************************************************

    /**
     *  Transcribe a string from a audio buffer.
     *
     *  \param c_Buffer The audio buffer to transcribe. The buffer is emptied.
     *  \param s_String The transcribed speech string.
     */

    void Transcribe(AudioBuffer& c_Buffer, std::string& s_String) override;

private:

    //*************************************************************************************
    // Data
    //*************************************************************************************

    pv_leopard_t* p_Handle;

    std::vector<MRH_Sint16> v_Buffer;

protected:

};

#endif /* PicovoiceLeopard_h */
