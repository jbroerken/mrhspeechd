*******
Signals
*******
mrhspeechd reacts to process signals to perform actions. Other processes should 
check for matching hotwords or keyboard input, followed by sending the correct 
signals to mrhspeechd to perform a wanted action.

Start Recording
---------------
mrhspeechd will start audio recording once **SIGUSR1** is received and no audio 
is currently playing. 

.. important::

    A recording start while already recording replaces currently recorded 
    audio.
    

Stop Audio
----------
mrhspeechd will stop all audio handling once **SIGUSR2** is received. Current 
audio recording and playback will be stopped.