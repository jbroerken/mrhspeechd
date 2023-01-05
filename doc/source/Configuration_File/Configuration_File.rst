******************
Configuration File
******************
The mrhspeechd binary loads a configuration file which contains information 
about the socket to exchange messages on as well as the recording, playback 
and analysis format used.

The configuration file uses the MRH Block File format.

File Structure
--------------
The block file stores all information in individual blocks. The service 
information is found in the **Service** block, the used API in the **API** 
block. Additional API settings are found in their own API blocks.

API Provider Settings Info
--------------------------
.. toctree::
   :maxdepth: 1

   API_Provider/SDL2
   API_Provider/ChunkVolume
   API_Provider/PicovoiceCobra
   API_Provider/PicovoiceLeopard
   API_Provider/GoogleCloud


Service Block
-------------
The Service block stores the following values:

.. list-table::
    :header-rows: 1

    * - Key
      - Description
    * - SocketPath
      - The full path to the service UTF8 stream socket to 
        connect to.
        

API Block
---------
The API block stores the following values:

.. list-table::
    :header-rows: 1

    * - Key
      - Description
    * - Recording
      - The recording API to use.
    * - Playback
      - The playback API to use.
    * - SpeechCheck
      - The audio speech check API to use.
    * - TTS
      - The text to speech API to use.
    * - STT
      - The speech to text API to use.
        

Example
-------
The following example shows a configuration file with default values:

.. code-block:: c

    <MRHBF_1>
    
    <Service>{
        <SocketPath></tmp/mrh/mrhpsspeech_audio.sock>
    }

    <API>{
        <Recording><0>
        <Playback><0>
        <SpeechCheck><0>
        <TTS><0>
        <STT><0>
    }
    
    # API settings...
