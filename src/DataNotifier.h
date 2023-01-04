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

#ifndef DataNotifier_h
#define DataNotifier_h

// C / C++
#include <mutex>
#include <condition_variable>

// External

// Project


class DataNotifier
{
public:

    //*************************************************************************************
    // Constructor / Destructor
    //*************************************************************************************

    /**
     *  Default constructor.
     */

    DataNotifier() noexcept : b_Notified(false)
    {}

    /**
     *  Default destructor.
     */

    ~DataNotifier() noexcept
    {}

    //*************************************************************************************
    // Wait
    //*************************************************************************************

    /**
     *  Wait for a notification.
     */

    void Wait() noexcept
    {
        std::unique_lock<std::mutex> c_Lock(c_Mutex);

        if (b_Notified == false)
        {
            c_Condition.wait(c_Lock);
        }

        b_Notified = false;
    }

    //*************************************************************************************
    // Notify
    //*************************************************************************************

    /**
     *  Notify a waiting thread.
     *
     *  \param b_All If all waiting threads should be notified.
     */

    void Notify(bool b_All) noexcept
    {
        {
            std::lock_guard<std::mutex> c_Guard(c_Mutex);

            b_Notified = true;
        }

        if (b_All == true)
        {
            c_Condition.notify_all();
        }
        else
        {
            c_Condition.notify_one();
        }
    }

private:

    //*************************************************************************************
    // Data
    //*************************************************************************************

    std::mutex c_Mutex;
    std::condition_variable c_Condition;
    bool b_Notified;

protected:

};

#endif /* DataNotifier_h */
