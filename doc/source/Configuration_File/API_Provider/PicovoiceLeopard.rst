*******************************
Picovoice Leopard Configuration
*******************************
Picovoice Leopard is used to transcribe speech input.

Picovoice Leopard Block
-----------------------
The PicovoiceLeopard block stores the following values:

.. list-table::
    :header-rows: 1

    * - Key
      - Description
    * - AccessKeyPath
      - The full path to the picovoice access key to use.
    * - ModelDirectoryPath
      - The full path to the locale model directory.
    * - ModelFileName
      - The name of the model file to use.
        
        
Example
-------
The following example shows default Picovoice Leopard settings found in the 
configuration file:

.. code-block:: c

    <PicovoiceLeopards>{
        <AccessKeyPath></usr/share/mrh/speechd/picovoice/accesskey.conf>
        <ModelDirectoryPath></usr/share/mrh/speechd/picovoice/>
        <ModelFileName><leopard_params.pv>
    }
    
