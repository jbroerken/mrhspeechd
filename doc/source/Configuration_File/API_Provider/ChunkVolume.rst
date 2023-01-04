**************************
Chunk Volume Configuration
**************************
Chunk Volume is used to recognize speech in a given audio buffer by checking 
the current amplitude of the audio samples.

Speech By Volume Block
----------------------
The ChunkVolume block stores the following values:

.. list-table::
    :header-rows: 1

    * - Key
      - Description
    * - MinVolume
      - The minimum volume required of each sample in percent.
    * - MinSamples
      - The minimum amount of samples required to reach the 
        minimum volume.
        
        
Example
-------
The following example shows default Chunk Volume found in the 
configuration file:

.. code-block:: c

    <ChunkVolume>{
        <MinVolume><0.25>
        <MinSamples><0.33>
    }
    
