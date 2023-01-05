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

#ifndef AudioBuffer_h
#define AudioBuffer_h

// C / C++
#include <deque>
#include <vector>

// External
#include <MRH_Typedefs.h>

// Project

// Pre-defined
#define MRH_AUDIO_BUFFER_CHANNELS 1


class AudioBuffer
{
public:

    //*************************************************************************************
    // Destructor
    //*************************************************************************************

    typedef std::vector<MRH_Sint16> AudioChunk;

    //*************************************************************************************
    // Constructor / Destructor
    //*************************************************************************************

    /**
     *  Default constructor.
     *
     *  \param u32_KHz The KHz of the audio stored.
     *  \param dq_Chunk The initial audio chunks.
     */

    AudioBuffer(MRH_Uint32 u32_KHz,
                std::deque<AudioChunk> dq_Chunk = {}) noexcept
    {
        Reset(u32_KHz, dq_Chunk);
    }

    /**
     *  Default destructor.
     */

    ~AudioBuffer() noexcept
    {}

    //*************************************************************************************
    // Reset
    //*************************************************************************************

    /**
     *  Reset the audio buffer.
     *
     *  \param u32_KHz The KHz of the audio stored.
     *  \param dq_Chunk The initial audio chunks.
     */

    void Reset(MRH_Uint32 u32_KHz, std::deque<AudioChunk> dq_Chunk = {}) noexcept
    {
        this->u32_KHz = u32_KHz;
        this->dq_Chunk.swap(dq_Chunk);

        dq_Chunk.clear();
    }

    /**
     *  Reset the audio buffer.
     *
     *  \param c_Buffer The buffer to reset with. The buffer data is consumed.
     */

    void Reset(AudioBuffer& c_Buffer) noexcept
    {
        Reset(c_Buffer.u32_KHz, c_Buffer.dq_Chunk);
    }

    /**
     *  Clear the audio buffer.
     */

    void Clear() noexcept
    {
        dq_Chunk.clear();
    }

    //*************************************************************************************
    // Add
    //*************************************************************************************

    /**
     *  Add audio chunks to the buffer.
     *
     *  \param dq_Chunk The chunks to add. The deque is consumed.
     */

    void Add(std::deque<AudioChunk>& dq_Chunk) noexcept
    {
        if (dq_Chunk.empty() == false)
        {
            Append(dq_Chunk, this->dq_Chunk);
        }
    }

    /**
     *  Add a audio chunk to the buffer.
     *
     *  \param v_Chunk The audio chunk to add. The chunk is consumed.
     *  \param b_ToFront If the buffer should be added in the front or back.
     */

    void Add(AudioChunk& v_Chunk, bool b_ToFront) noexcept
    {
        if (v_Chunk.empty() == true)
        {
            return;
        }

        if (b_ToFront == true)
        {
            dq_Chunk.emplace_front();
            dq_Chunk.front().swap(v_Chunk);
        }
        else
        {
            dq_Chunk.emplace_back();
            dq_Chunk.back().swap(v_Chunk);
        }
    }

    //*************************************************************************************
    // Retrieve
    //*************************************************************************************

    /**
     *  Retrieve all currently stored audio chunks.
     *
     *  \param dq_Chunk The chunks to append to.
     */

    void Retrieve(std::deque<AudioChunk>& dq_Chunk) noexcept
    {
        if (this->dq_Chunk.empty() == false)
        {
            Append(this->dq_Chunk, dq_Chunk);
        }
    }

    /**
     *  Retrieve a stored audio chunk.
     *
     *  \param v_Chunk The chunk to retrieve. The chunk is removed from the buffer.
     *
     *  \return true if a chunk was retrieved, false if not.
     */

    bool Retrieve(AudioChunk& v_Chunk) noexcept
    {
        if (dq_Chunk.empty() == true)
        {
            return false;
        }

        dq_Chunk.front().swap(v_Chunk);
        dq_Chunk.pop_front();

        return true;
    }

    //*************************************************************************************
    // Getters
    //*************************************************************************************

    /**
     *  Get the audio buffer KHz.
     *
     *  \return The audio buffer KHz.
     */

    MRH_Uint32 GetKHz() const noexcept
    {
        return u32_KHz;
    }

    /**
     *  Get the amount of audio chunks.
     *
     *  \return The amount of audio chunks.
     */

    size_t GetChunkCount() noexcept
    {
        return dq_Chunk.size();
    }

private:

    //*************************************************************************************
    // Append
    //*************************************************************************************

    /**
     *  Append a given chunk deque to the enf of a target deque.
     *
     *  \param dq_Src The chunk deque to append. The deque is consumed.
     *  \param dq_Dst The chunk deque to append to.
     */

    inline void Append(std::deque<AudioChunk>& dq_Src, std::deque<AudioChunk>& dq_Dst)
    {
        std::move(dq_Src.begin(), dq_Src.end(), std::back_inserter(dq_Dst));
        dq_Src.clear();
    }

    //*************************************************************************************
    // Data
    //*************************************************************************************

    std::deque<AudioChunk> dq_Chunk;
    MRH_Uint32 u32_KHz;

protected:

};

#endif /* AudioBuffer_h */
