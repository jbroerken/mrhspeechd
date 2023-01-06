******************************
Google Cloud API Configuration
******************************
The Google Cloud API is used to transcribe speech input and synthesize speech 
output.

.. important::

    The Google Cloud Services Configuration is loaded from the default 
    location, found at **~/.config/gcloud/application_default_credentials.json**


GoogleCloudTTS Block
--------------------
The GoogleCloudTTS block stores the following values:

.. list-table::
    :header-rows: 1

    * - Key
      - Description
    * - BCPDirectoryPath
      - The directory containing all localised directories for
        BCP-47 locale files.
    * - BCPFileName
      - The locale file containing the BCP-47 language code used for
        synthesizing output.
    * - VoiceGender
      - 0 for female, 1 for male.
    * - KHz
      - The KHz of the synthesized audio.
    * - ChunkSamples
      - The number of audio samples in a audio chunk.


GoogleCloudSTT Block
--------------------
The GoogleCloudSTT block stores the following values:

.. list-table::
    :header-rows: 1

    * - Key
      - Description
    * - BCPDirectoryPath
      - The directory containing all localised directories for
        BCP-47 locale files.
    * - BCPFileName
      - The locale file containing the BCP-47 language code used for
        transcribing input.
        

Example
-------
The following example shows default Google Cloud API settings found in the 
configuration file:

.. code-block:: c

    <GoogleCloudTTS>{
        <BCPDirectoryPath></usr/share/mrh/speechd/gcloud/>
        <BCPFileName><locale.conf>
        <VoiceGender><0>
        <KHz><16000>
        <ChunkSamples><2048>
    }

    <GoogleCloudSTT>{
        <BCPDirectoryPath></usr/share/mrh/speechd/gcloud/>
        <BCPFileName><locale.conf>
    }
    
