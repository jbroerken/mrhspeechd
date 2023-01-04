*************
API Providers
*************
mrhspeechd implements recording, playback and sound handling with the help of 
3rd party API providers. Those providers can be chosen based on availability 
and type of action required.

.. important::

    mrhspeechd must be compiled with the API provider dependencies and enabled 
    flags to use the providers.


Recording API Providers
-----------------------
Recording API providers are used for sound recording.

.. list-table::
    :header-rows: 1

    * - ID
      - API
    * - 0
      - SDL2
      
      
Playback API Providers
----------------------
Playback API providers are used for sound playback.

.. list-table::
    :header-rows: 1

    * - ID
      - API
    * - 0
      - SDL2
      
      
Speech Check API Providers
--------------------------
Speech Check API providers are used to analyze recorded audio for speech.

.. list-table::
    :header-rows: 1

    * - ID
      - API
    * - 0
      - Chunk Volume
    * - 1
      - Picovoice Cobra
      

Text to Speech API Providers
----------------------------
Text to Speech API providers are used to synthesize audio from text 
messages.

.. list-table::
    :header-rows: 1

    * - ID
      - API
    * - 0
      - Google Cloud API
      

Speech to Text API Providers
----------------------------
Speech to Text API providers are used to transcribe text from recorded audio.

.. list-table::
    :header-rows: 1

    * - ID
      - API
    * - 0
      - Google Cloud API
    * - 1
      - Picovoice Leopard
      
