********
Building
********
The project files are already prepared and include everything except the 
required dependencies. This document lists both dependencies as well as the 
build process to create the binary from source.

Supported Platforms
-------------------
The supported platforms for mrhspeechd are UNIX-likes, primarily 
Debian-based GNU/Linux Distributions like the Rapsberry Pi OS.

Dependencies
------------
This project has the following dependencies:

* mrhshared: https://github.com/jbroerken/mrhshared/
* libmrhbf: https://github.com/jbroerken/libmrhbf/
* libmrhvt: https://github.com/jbroerken/libmrhls/
* spdlog: https://github.com/gabime/spdlog/

Optional Dependencies
---------------------
Optional dependencies are used for API providers to handle recording, 
playback and sound.

.. note::

    The dependencies change depending on the API providers used.


.. toctree::
   :maxdepth: 1

   Dependencies/SDL2
   Dependencies/Picovoice_Cobra
   Dependencies/Picovoice_Leopard
   Dependencies/Google_Cloud_API


Build Tools
-----------
This release includes a CMake script (CMakeLists.txt) for a simplified build 
process. The minimal required version for CMake is 3.1.
Also needed is the GNU C++ Compiler. Full C++14 support is required.

Changing Pre-defined Settings
-----------------------------
Listed in the CMakeLists.txt file are preprocessor macros used to specify 
default file paths and other information. Change these to fit the requirements 
of the target use case.

Build Process
-------------
The build process should be relatively straightforward:

1. Aqquire dependencies.
2. Move into the project "build" folder.
3. Compile Makefiles with the included CMakeLists.txt.
4. Run "make" to compile the binary.
5. Install the compiled binary.
6. Acquire and install any required files from the mrh_os_files 
   (https://github.com/jbroerken/mrh_os_files/) repository.

Shell Commands
--------------
The following shell commands will create makefiles with the provided 
CMakeLists.txt, compile the project with the created makefiles and install the 
binary:

.. code-block::

    cd <Project Root Folder>/build
    cmake ..
    make
    sudo make install
