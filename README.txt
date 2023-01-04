#######################
#                     #
#  mrhspeechd ReadMe  #
#                     #
#######################

##
# About
##

The mrhspeechd project provides a standard speech audio handling utility to use 
with the MRH Platform.


##
# Requirements
##

Compilation:
------------
This project is built using CMake. You can find CMake here:

https://cmake.org/

Library Dependencies:
---------------------
This project requires other libraries and headers to function:

Dependency List:
libmrhbf:  https://github.com/jbroerken/libmrhbf/
libmrhvt:  https://github.com/jbroerken/libmrhvt/
mrhshared: https://github.com/jbroerken/mrhshared/
spdlog: https://github.com/gabime/spdlog/

Additional dependencies are required depending on the different APIs used.

For more information about the requirements, check the "Building" section found in the documentation.


##
# Documentation
##

All documentation is build with sphinx-doc using the Read The Docs theme.
To build the documentation, grab the requirements for it:

sphinx-doc:
-----------
https://www.sphinx-doc.org/en/master/

Read The Docs Theme:
--------------------
https://sphinx-rtd-theme.readthedocs.io/en/stable/

##
# Licence
##

This project is licenced under the Apache 2.0 licence. 
Please read the included LICENCE.txt for the exact terms.


##
# Directories
##

This project supplies multiple directories for the development of said project. 
Their names and descriptions are as follows:

Directory List:
build: CMake build directory.
doc: Documentation files.
src: Project source code.