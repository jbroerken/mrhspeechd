************
Audio Format
************
mrhspeechd uses a specific format for both recording and playback. This is done 
to allow for maximal compitability with common text to speech and speech to text 
services.

Format
------
The format used by mrhspeechd is the PCM 16-bit format. This format is widely 
supported and works with most common NLP services like Google Cloud.

Channels
--------
Channel support for both recording and playback is limited to Mono.

KHz
---
The audio KHz for recording, playback and synthesizing otuput can be controlled 
by the loaded configuration file. Recommended is 16000 KHz for recording and 
playback.

.. important::

    Some API providers require specific values.