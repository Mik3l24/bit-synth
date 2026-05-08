cmake_minimum_required(VERSION 3.15)
project(bitprocessing VERSION 0.0.2)

set(CMAKE_CXX_STANDARD 17)

#set(Boost_DEBUG ON)
#[[set(BOOST_ROOT ${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET})
set(Boost_NO_SYSTEM_PATHS ON)
set(Boost_NO_BOOST_CMAKE  ON) # additional set this one]]

find_package(Boost 1.83 REQUIRED)
find_package(JUCE CONFIG REQUIRED)



#juce_add_gui_app(bitprocessing
#    # VERSION ...                       # Set this if the app version is different to the project version
#    # ICON_BIG ...                      # ICON_* arguments specify a path to an image file to use as an icon
#    # ICON_SMALL ...
#    # DOCUMENT_EXTENSIONS ...           # Specify file extensions that should be associated with this app
#    COMPANY_NAME "Mik3l24"                  # Specify the name of the app's author
#    PRODUCT_NAME "bitprocessing")

juce_add_plugin(
    bitprocessing
    # VERSION ...                       # Set this if the app version is different to the project version
    # ICON_BIG ...                      # ICON_* arguments specify a path to an image file to use as an icon
    # ICON_SMALL ...
    # DOCUMENT_EXTENSIONS ...           # Specify file extensions that should be associated with this app
    COMPANY_NAME "Mik3l24"                  # Specify the name of the app's author
    PRODUCT_NAME "bitprocessing"
    PLUGIN_NAME "Bit Synth"
    PLUGIN_CODE "Bool"
    FORMATS "Standalone" "VST3"
    IS_SYNTH TRUE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT FALSE
    VST3_CATEGORIES "Synth"
)


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

set(BITPROCESSING_SOURCES_DSP
    dsp/bitprocessing/BitIO.cpp
    dsp/bitprocessing/GateNode.cpp
    dsp/bitprocessing/Oscillator.cpp
    dsp/bitprocessing/BitMixChannel.cpp
    dsp/BitSynthVoice.cpp
)

set(BITPROCESSING_SOURCES_SYNTH
    synth_management/BitSynthesizer.cpp
    synth_management/SynthStateManager.cpp
    SynthAudioProcessor.cpp
)

set(BITPROCESSING_SOURCES_UI
    ui/Theme.cpp
    ui/SourceConnector.cpp
    ui/TargetConnector.cpp
    ui/SynthElement.cpp
    ui/OscillatorParameters.cpp
    ui/MixChannelParameters.cpp
    ui/Gate.cpp
    ui/ElementAdderButton.cpp
    ui/ElementPicker.cpp
    ui/StructureEditor.cpp
    ui/TopBar.cpp
    ui/SynthEditor.cpp
)

set(BITPROCESSING_SOURCES
    ${BITPROCESSING_SOURCES_DSP}
    ${BITPROCESSING_SOURCES_SYNTH}
    ${BITPROCESSING_SOURCES_UI}
)

target_sources(bitprocessing
    PRIVATE
        Main.cpp
        ${BITPROCESSING_SOURCES}
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
        JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:bitprocessing,JUCE_VERSION>"
        JUCE_USE_FLAC=0
        JUCE_USE_OGGVORBIS=0
        ${DEBUG_DEFS})

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
        juce::juce_gui_basics
        juce::juce_audio_utils
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)

target_link_libraries(bitprocessing
    LINK_PUBLIC
        ${Boost_LIBRARIES}
)

# --- Lattice generator executable section ---

juce_add_console_app(bitprocessing_lattice
    COMPANY_NAME "Mik3l24"
    PRODUCT_NAME "Bit Synth Lattice Generator"
)

target_sources(bitprocessing_lattice
    PRIVATE
        benchmark/LatticeGenerator.cpp
        synth_management/SynthStateManager.cpp
)

include_directories(bitprocessing_lattice
    ${Boost_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_definitions(bitprocessing_lattice
    PRIVATE
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:bitprocessing_lattice,JUCE_PRODUCT_NAME>"
        JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:bitprocessing_lattice,JUCE_VERSION>"
        JUCE_USE_FLAC=0
        JUCE_USE_OGGVORBIS=0
        ${DEBUG_DEFS}
)

target_link_libraries(bitprocessing_lattice
    PRIVATE
        juce::juce_audio_utils
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
        ${Boost_LIBRARIES}
)


# --- Benchmark executable section ---

juce_add_console_app(bitprocessing_benchmark
    COMPANY_NAME "Mik3l24"
    PRODUCT_NAME "Bit Synth Benchmark"
)

target_sources(bitprocessing_benchmark
    PRIVATE
        "benchmark/Main.cpp"
        "benchmark/Benchmark.cpp"
        ${BITPROCESSING_SOURCES}
)


include_directories(bitprocessing_benchmark
    ${Boost_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_definitions(bitprocessing_benchmark
    PRIVATE
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:bitprocessing_benchmark,JUCE_PRODUCT_NAME>"
        JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:bitprocessing_benchmark,JUCE_VERSION>"
        JUCE_USE_FLAC=0
        JUCE_USE_OGGVORBIS=0
        ${DEBUG_DEFS}
)

target_link_libraries(bitprocessing_benchmark
    PRIVATE
        juce::juce_audio_utils
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
        ${Boost_LIBRARIES}
)

# Run benchmarks:
enable_testing()

set(BENCHMARK_PATCH_DIR ${CMAKE_CURRENT_SOURCE_DIR}/benchmark/patches)
set(BENCHMARK_PATCH_FILES
    "basic_xor.xml"
    "lattice_forward_4_4x4_4_xor.xml"
    "lattice_backward_4_4x4_4_xor.xml"
    "lattice_forward_16x32_xor.xml"
    "lattice_backward_16x32_xor.xml"
)

add_custom_target(run_benchmarks)
add_dependencies(run_benchmarks bitprocessing_benchmark)


foreach(PATCH_FILE IN LISTS BENCHMARK_PATCH_FILES)
    add_test(NAME "Benchmark_${PATCH_FILE}"
        COMMAND $<TARGET_FILE:bitprocessing_benchmark> ${BENCHMARK_PATCH_DIR}/${PATCH_FILE}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
    add_custom_command(TARGET run_benchmarks
        POST_BUILD
        COMMAND $<TARGET_FILE:bitprocessing_benchmark> ${BENCHMARK_PATCH_DIR}/${PATCH_FILE}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
endforeach(PATCH_FILE)

