#########################################################################
#
#  CMAKE
#
#########################################################################

###
#  Minimum Version
#  ---------------
#  The CMake version required.
###
cmake_minimum_required(VERSION 3.1)

###
#  CMake Configuration
#  -------------------
#  Configuration settings for CMake.
#
#  NOTE:
#  These settings have to be applied before the project() setting!
###
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_STANDARD 14)

###
#  CMake Options
#  -------------
#  Options to use when building with CMake.
###
option(AUDIO_API_SDL2 "Enable recording and playback with SDL2" ON)
option(AUDIO_API_PICOVOICE_COBRA "Enable speech recognition with Picovoice Cobra" OFF)

option(STT_API_GOOGLE_CLOUD "Enable speech to text conversion with Google Cloud" ON)
option(STT_API_PICOVOICE_LEOPARD "Enable speech to text conversion with Picovoice Cobra" OFF)

option(TTS_API_GOOGLE_CLOUD "Enable text to speech conversion with Google Cloud" ON)

###
#  Project Info
#  ------------
#  General simple information about our project.
###
project(mrhspeechd VERSION 1.0.0
                   DESCRIPTION "MRH voice speech handling utility"
                   LANGUAGES CXX)

#########################################################################
#
#  PATHS
#
#########################################################################

###
#  Install Paths
#  -------------
#  GNU system paths for installation.
###
include(GNUInstallDirs)

###
#  Source Paths
#  ------------
#  The paths to the source files to use.
#  Add OS specific source files in their own list.
###
set(SRC_DIR_PATH "${CMAKE_SOURCE_DIR}/src/")

set(SRC_LIST_STREAM "${SRC_DIR_PATH}/Stream/UTF8Stream.cpp"
                    "${SRC_DIR_PATH}/Stream/UTF8Stream.h")

set(SRC_LIST_STT "${SRC_DIR_PATH}/STT/API/CreateSTTAPI.cpp"
                 "${SRC_DIR_PATH}/STT/API/CreateSTTAPI.h"
                 "${SRC_DIR_PATH}/STT/API/STTAPI.h"
                 "${SRC_DIR_PATH}/STT/STT.h")

if(STT_API_GOOGLE_CLOUD MATCHES ON)
    set(SRC_LIST_STT ${SRC_LIST_STT}
                     "${SRC_DIR_PATH}/STT/API/GoogleCloudSTT/GoogleCloudSTT.cpp"
                     "${SRC_DIR_PATH}/STT/API/GoogleCloudSTT/GoogleCloudSTT.h")
endif()

if(STT_API_PICOVOICE_LEOPARD MATCHES ON)
    set(SRC_LIST_STT ${SRC_LIST_STT}
                     "${SRC_DIR_PATH}/STT/API/PicovoiceLeopard/PicovoiceLeopard.cpp"
                     "${SRC_DIR_PATH}/STT/API/PicovoiceLeopard/PicovoiceLeopard.h")
endif()

set(SRC_LIST_TTS "${SRC_DIR_PATH}/TTS/API/CreateTTSAPI.cpp"
                 "${SRC_DIR_PATH}/TTS/API/CreateTTSAPI.h"
                 "${SRC_DIR_PATH}/TTS/API/TTSAPI.h"
                 "${SRC_DIR_PATH}/TTS/TTS.h")

if(TTS_API_GOOGLE_CLOUD MATCHES ON)
    set(SRC_LIST_TTS ${SRC_LIST_TTS}
                     "${SRC_DIR_PATH}/TTS/API/GoogleCloudTTS/GoogleCloudTTS.cpp"
                     "${SRC_DIR_PATH}/TTS/API/GoogleCloudTTS/GoogleCloudTTS.h")
endif()

set(SRC_LIST_AUDIO "${SRC_DIR_PATH}/Audio/API/ChunkVolume/ChunkVolume.cpp"
                   "${SRC_DIR_PATH}/Audio/API/ChunkVolume/ChunkVolume.h"
                   "${SRC_DIR_PATH}/Audio/API/CreateAudioAPI.cpp"
                   "${SRC_DIR_PATH}/Audio/API/CreateAudioAPI.h"
                   "${SRC_DIR_PATH}/Audio/API/AudioAPI.h"
                   "${SRC_DIR_PATH}/Audio/AudioBuffer.h"
                   "${SRC_DIR_PATH}/Audio/SpeechChecker.h"
                   "${SRC_DIR_PATH}/Audio/Recorder.h"
                   "${SRC_DIR_PATH}/Audio/RecorderContext.h"
                   "${SRC_DIR_PATH}/Audio/Player.h")

if(AUDIO_API_SDL2 MATCHES ON)
    set(SRC_LIST_AUDIO ${SRC_LIST_AUDIO}
                       "${SRC_DIR_PATH}/Audio/API/SDL2/SDL2Recorder.cpp"
                       "${SRC_DIR_PATH}/Audio/API/SDL2/SDL2Recorder.h"
                       "${SRC_DIR_PATH}/Audio/API/SDL2/SDL2RecordingContext.h"
                       "${SRC_DIR_PATH}/Audio/API/SDL2/SDL2Player.cpp"
                       "${SRC_DIR_PATH}/Audio/API/SDL2/SDL2Player.h"
                       "${SRC_DIR_PATH}/Audio/API/SDL2/SDL2PlaybackContext.h"
                       "${SRC_DIR_PATH}/Audio/API/SDL2/SDL2Device.h")
endif()

if(AUDIO_API_PICOVOICE_COBRA MATCHES ON)
    set(SRC_LIST_AUDIO ${SRC_LIST_AUDIO}
                       "${SRC_DIR_PATH}/Audio/API/PicovoiceCobra/PicovoiceCobra.cpp"
                       "${SRC_DIR_PATH}/Audio/API/PicovoiceCobra/PicovoiceCobra.h")
endif()

set(SRC_LIST_BASE "${SRC_DIR_PATH}/Configuration.cpp"
                  "${SRC_DIR_PATH}/Configuration.h"
                  "${SRC_DIR_PATH}/Logger.cpp"
                  "${SRC_DIR_PATH}/Logger.h"
                  "${SRC_DIR_PATH}/DataNotifier.h"
                  "${SRC_DIR_PATH}/Exception.h"
                  "${SRC_DIR_PATH}/Revision.h"
                  "${SRC_DIR_PATH}/Main.cpp")

#########################################################################
#
#  TARGET
#
#########################################################################

###
#  Target
#  ------
#  The target(s) to build.
###
add_executable(mrhspeechd ${SRC_LIST_STREAM}
                          ${SRC_LIST_STT}
                          ${SRC_LIST_TTS}
                          ${SRC_LIST_AUDIO}
                          ${SRC_LIST_BASE})

###
#  Required Libraries
#  ------------------
#  Libraries required by this application.
###
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)

find_package(Threads REQUIRED)
find_library(libmrhbf NAMES mrhbf REQUIRED)
find_library(libmrhvt NAMES mrhvt REQUIRED)

if(AUDIO_API_SDL2 MATCHES ON)
    find_package(SDL2 REQUIRED)
endif()

if(AUDIO_API_PICOVOICE_COBRA MATCHES ON)
    find_library(libpv_cobra NAMES pv_cobra REQUIRED)
endif()

if(STT_API_GOOGLE_CLOUD MATCHES ON)
    find_package(google_cloud_cpp_speech REQUIRED)
endif()

if(STT_API_PICOVOICE_LEOPARD MATCHES ON)
    find_library(libpv_leopard NAMES pv_leopard REQUIRED)
endif()

if(TTS_API_GOOGLE_CLOUD MATCHES ON)
    find_package(google_cloud_cpp_texttospeech REQUIRED)
endif()

target_link_libraries(mrhspeechd PUBLIC Threads::Threads)
target_link_libraries(mrhspeechd PUBLIC mrhbf)
target_link_libraries(mrhspeechd PUBLIC mrhvt)

if(AUDIO_API_SDL2 MATCHES ON)
   target_link_libraries(mrhspeechd PUBLIC ${SDL2_LIBRARIES})
endif()

if(AUDIO_API_PICOVOICE_COBRA MATCHES ON)
    target_link_libraries(mrhspeechd PUBLIC pv_cobra)
endif()

if(STT_API_GOOGLE_CLOUD MATCHES ON)
    target_link_libraries(mrhspeechd PUBLIC google-cloud-cpp::speech)
endif()

if(STT_API_PICOVOICE_LEOPARD MATCHES ON)
    target_link_libraries(mrhspeechd PUBLIC pv_leopard)
endif()

if(TTS_API_GOOGLE_CLOUD MATCHES ON)
    target_link_libraries(mrhspeechd PUBLIC google-cloud-cpp::texttospeech)
endif()

###
#  Source Definitions
#  ------------------
#  Preprocessor source definitions.
###
target_compile_definitions(mrhspeechd PRIVATE MRH_SPEECHD_LOG_FILE_PATH="/var/log/mrh/mrhspeechd.log")
target_compile_definitions(mrhspeechd PRIVATE MRH_LOGGER_PRINT_CLI=0)
target_compile_definitions(mrhspeechd PRIVATE MRH_SPEECHD_CONFIGURATION_PATH="/usr/share/mrh/speechd/speechd.conf")
target_compile_definitions(mrhspeechd PRIVATE MRH_SPEECHD_PID_FILE_PATH="/tmp/mrh/mrhpeechd_PID")
target_compile_definitions(mrhspeechd PRIVATE MRH_SPEECHD_DAEMON_MODE=0)

target_compile_definitions(mrhspeechd PRIVATE CHUNK_VOLUME_LOG_EXTENDED=0)

if(AUDIO_API_SDL2 MATCHES ON)
    target_compile_definitions(mrhspeechd PRIVATE MRH_SPEECHD_SOUND_IO_API_SDL2=1)
    target_compile_definitions(mrhspeechd PRIVATE SDL2_RECORDER_LOG_EXTENDED=0)
    target_compile_definitions(mrhspeechd PRIVATE SDL2_PLAYER_LOG_EXTENDED=0)
endif()

if(AUDIO_API_PICOVOICE_COBRA MATCHES ON)
    target_compile_definitions(mrhspeechd PRIVATE MRH_SPEECHD_SPEECH_CHECKER_API_PICOVOICE_COBRA=1)
    target_compile_definitions(mrhspeechd PRIVATE PICOVOICE_COBRA_LOG_EXTENDED=0)
endif()

if(STT_API_GOOGLE_CLOUD MATCHES ON)
    target_compile_definitions(mrhspeechd PRIVATE MRH_SPEECHD_STT_API_GGOGLE_CLOUD=1)
    target_compile_definitions(mrhspeechd PRIVATE GOOGLE_CLOUD_STT_LOG_EXTENDED=0)
endif()

if(STT_API_PICOVOICE_LEOPARD MATCHES ON)
    target_compile_definitions(mrhspeechd PRIVATE MRH_SPEECHD_STT_API_PICOVOICE_LEOPARD=1)
    target_compile_definitions(mrhspeechd PRIVATE PICOVOICE_LEOPARD_LOG_EXTENDED=0)
endif()

if(TTS_API_GOOGLE_CLOUD MATCHES ON)
    target_compile_definitions(mrhspeechd PRIVATE MRH_SPEECHD_TTS_API_GGOGLE_CLOUD=1)
    target_compile_definitions(mrhspeechd PRIVATE GOOGLE_CLOUD_TTS_LOG_EXTENDED=0)
endif()

###
#  Install
#  -------
#  Application installation.
###
install(TARGETS mrhspeechd
        DESTINATION ${CMAKE_INSTALL_BINDIR})