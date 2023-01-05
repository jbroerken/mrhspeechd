******************
SDL2 Configuration
******************
SDL2 can be used for sound recording and playback.

SDL2Recorder Block
------------------
The SDL2Recorder block stores the following values:

.. list-table::
    :header-rows: 1

    * - Key
      - Description
    * - DeviceName
      - The name of the recording device. **null** uses the default 
        device.
    * - KHz
      - The KHz to record in.
    * - SamplesPerFrame
      - The number of samples for each recording frame.
    * - TrailingFrameSize
      - The number of samples appended if speech has ended before
        recording stops.
    * - Amplification
      - The percentage with which the recorded audio should be amplified.


SDL2Player Block
----------------
The SDL2Player block stores the following values:

.. list-table::
    :header-rows: 1

    * - Key
      - Description
    * - DeviceName
      - The name of the playback device. **null** uses the default 
        device.
    * - SamplesPerFrame
      - The number of samples for each playback frame.
        

Example
-------
The following example shows default SDL2 settings found in the 
configuration file:

.. code-block:: c

    <SDL2Recorder>{
        <DeviceName><null>
        <KHz><16000>
        <SamplesPerFrame><2048>
        <Amplification><1.0>
    }

    <SDL2Player>{
        <DeviceName><null>
        <SamplesPerFrame><2048>
    }
    
