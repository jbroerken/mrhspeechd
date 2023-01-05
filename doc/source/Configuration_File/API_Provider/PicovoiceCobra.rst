*****************************
Picovoice Cobra Configuration
*****************************
Picovoice Cobra is used to recognize speech in a given audio buffer.

Picovoice Cobra Block
----------------------
The PicovoiceCobra block stores the following values:

.. list-table::
    :header-rows: 1

    * - Key
      - Description
    * - AccessKeyPath
      - The full path to the picovoice access key to use.
    * - MinConfidence
      - The minimum confidence in percent required to recognize speech.
        
        
Example
-------
The following example shows default Picovoice Cobra settings found in the 
configuration file:

.. code-block:: c

    <PicovoiceCobra>{
        <AccessKeyPath></usr/share/mrh/speechd/picovoice/accesskey.conf>
        <MinConfidence><0.75>
    }
    
