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

#ifndef RecorderContext_h
#define RecorderContext_h

// C / C++
#include <atomic>
#include <memory>

// External

// Project
#include "./SpeechChecker.h"
#include "../DataNotifier.h"


struct RecorderContext
{
public:

    //*************************************************************************************
    // Constructor
    //*************************************************************************************

    /**
     *  Default constructor.
     *
     *  \param p_Notifier The notifier to trigger for new recording data.
     *  \param p_SpeechChecker The speech checker used to detect voice audio.
     */

    RecorderContext(std::shared_ptr<DataNotifier>& p_Notifier,
                    std::shared_ptr<SpeechChecker>& p_SpeechChecker) noexcept : b_SpeechRecorded(false),
                                                                                p_Notifier(p_Notifier),
                                                                                p_SpeechChecker(p_SpeechChecker)
    {}

    //*************************************************************************************
    // Data
    //*************************************************************************************

    std::atomic<bool> b_SpeechRecorded;

    std::shared_ptr<DataNotifier> p_Notifier;
    std::shared_ptr<SpeechChecker> p_SpeechChecker;
};


#endif /* RecorderContext_h */
