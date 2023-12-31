cmake_minimum_required(VERSION 3.15)
project(bitprocessing VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)

#set(Boost_DEBUG ON)
#[[set(BOOST_ROOT ${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET})
set(Boost_NO_SYSTEM_PATHS ON)
set(Boost_NO_BOOST_CMAKE  ON) # additional set this one]]

find_package(Boost 1.83 REQUIRED)
find_package(JUCE CONFIG REQUIRED)



juce_add_gui_app(bitprocessing
    # VERSION ...                       # Set this if the app version is different to the project version
    # ICON_BIG ...                      # ICON_* arguments specify a path to an image file to use as an icon
    # ICON_SMALL ...
    # DOCUMENT_EXTENSIONS ...           # Specify file extensions that should be associated with this app
    COMPANY_NAME "Mik3l24"                  # Specify the name of the app's author
    PRODUCT_NAME "bitprocessing")

# `juce_generate_juce_header` will create a JuceHeader.h for a given target, which will be generated
# into your build tree. This should be included with `#include <JuceHeader.h>`. The include path for
# this header will be automatically added to the target. The main function of the JuceHeader is to
# include all your JUCE module headers; if you're happy to include module headers directly, you
# probably don't need to call this.

juce_generate_juce_header(bitprocessing)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# `target_sources` adds source files to a target. We pass the target that needs the sources as the
# first argument, then a visibility parameter for the sources which should normally be PRIVATE.
# Finally, we supply a list of source files that will be built into the target. This is a standard
# CMake command.

target_sources(bitprocessing
    PRIVATE
        Main.cpp
        dsp/bitprocessing/BitIO.cpp
        dsp/bitprocessing/GateNode.cpp
        dsp/bitprocessing/Oscillator.cpp
        dsp/bitprocessing/BitMixChannel.cpp
        dsp/BitSynthVoice.cpp
        synth_management/BitSynthesizer.cpp
        ui/Theme.cpp
        ui/SourceConnector.cpp
        ui/TargetConnector.cpp
        ui/OscillatorParameters.cpp
        ui/MixChannelParameters.cpp
        ui/Gate.cpp
        ui/ElementAdderButton.cpp
        ui/ElementPicker.cpp
        ui/StructureEditor.cpp
        SynthAudioSource.cpp
        MainContentComponent.cpp
)

# `target_compile_definitions` adds some preprocessor definitions to our target. In a Projucer
# project, these might be passed in the 'Preprocessor Definitions' field. JUCE modules also make use
# of compile definitions to switch certain features on/off, so if there's a particular feature you
# need that's not on by default, check the module header for the correct flag to set here. These
# definitions will be visible both to your code, and also the JUCE module code, so for new
# definitions, pick unique names that are unlikely to collide! This is a standard CMake command.

target_compile_definitions(bitprocessing
    PRIVATE
        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_gui_app` call
        JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_gui_app` call
        JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:bitprocessing,JUCE_PRODUCT_NAME>"
        JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:bitprocessing,JUCE_VERSION>")

# If your target needs extra binary assets, you can add them here. The first argument is the name of
# a new static library target that will include all the binary resources. There is an optional
# `NAMESPACE` argument that can specify the namespace of the generated binary data class. Finally,
# the SOURCES argument should be followed by a list of source files that should be built into the
# static library. These source files can be of any kind (wav data, images, fonts, icons etc.).
# Conversion to binary-data will happen when your target is built.

# juce_add_binary_data(GuiAppData SOURCES ...)

# `target_link_libraries` links libraries and JUCE modules to other libraries or executables. Here,
# we're linking our executable target to the `juce::juce_gui_extra` module. Inter-module
# dependencies are resolved automatically, so `juce_core`, `juce_events` and so on will also be
# linked automatically. If we'd generated a binary data target above, we would need to link to it
# here too. This is a standard CMake command.

target_link_libraries(bitprocessing
    PRIVATE
        # GuiAppData            # If we'd created a binary data target, we'd link to it here
        juce::juce_gui_extra
        juce::juce_audio_utils
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)

target_link_libraries(bitprocessing
    LINK_PUBLIC
        ${Boost_LIBRARIES}
)
