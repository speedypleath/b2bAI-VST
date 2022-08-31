project(b2bAI)

juce_add_plugin(${PROJECT_NAME}
        DONT_SET_USING_JUCE_NAMESPACE
        PRODUCT_NAME "b2bAI"                                    # The name of the final executable, which can differ from the target name
        VERSION ${CMAKE_PROJECT_VERSION}                        # Version
        FORMATS AU VST3 Standalone                              # The formats to build. Other valid formats are: AAX Unity VST AU AUv3
        IS_SYNTH yes
        NEEDS_MIDI_INPUT yes
        VST3_CATEGORIES "Instrument" "Synth"
        AAX_CATEGORY "AAX_ePlugInCategory_Instrument"
        AU_MAIN_TYPE "kAudioUnitType_MusicDevice"
)

juce_add_binary_data(${PROJECT_NAME}_data
        SOURCES
        Resources/magic.xml)

juce_generate_juce_header(${PROJECT_NAME})

set(B2BAI_SOURCES
        Sources/Constants.h
        Sources/PluginProcessor.h
        Sources/PluginProcessor.cpp
        Sources/KeyboardComponent.h
        Sources/KeyboardComponent.cpp
        Sources/GridComponent.h
        Sources/GridComponent.cpp
        Sources/PianoRollComponent.h
        Sources/PianoRollComponent.cpp
        Sources/NoteRectangle.cpp
        Sources/NoteRectangle.h
        Sources/MidiFileListBox.cpp
        Sources/MidiFileListBox.h
        Sources/SearchBar.h
        Sources/SearchBar.cpp
        Sources/MidiSequence.cpp
        Sources/MidiSequence.h
        Sources/PianoRoll.h
        Sources/PianoRoll.cpp
        )

target_sources(${PROJECT_NAME} PRIVATE ${B2BAI_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${B2BAI_SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/lib)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/lib/JUCE/modules)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/lib/midi_generator)

target_compile_definitions(${PROJECT_NAME}
        PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
        JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED=1
        )

message(STATUS "Libraries: ${note}")

target_link_libraries(${PROJECT_NAME} PRIVATE
        ${PROJECT_NAME}_data
        note
        foleys_gui_magic
        juce::juce_audio_basics
        juce::juce_audio_plugin_client
        juce::juce_audio_processors
        juce::juce_audio_utils
        juce::juce_dsp
        juce::juce_cryptography
        juce::juce_gui_extra
        juce::juce_opengl
        juce::juce_recommended_warning_flags
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags)

target_compile_definitions(${PROJECT_NAME}
        PUBLIC
        # switch the following off in the product to hide editor
        FOLEYS_SHOW_GUI_EDITOR_PALLETTE=1
        FOLEYS_SAVE_EDITED_GUI_IN_PLUGIN_STATE=0
        JUCE_VST3_CAN_REPLACE_VST2=0
        JUCE_WEB_BROWSER=0)

target_include_directories(${PROJECT_NAME} PRIVATE
        ${Boost_INCLUDE_DIR})

message(STATUS "Boost include: " ${Boost_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} LINK_PUBLIC
        ${Boost_LIBRARIES} pybind11::embed)

add_definitions(${Boost_DEFINITIONS})

message(STATUS "Boost libraries: " ${Boost_LIBRARY_DIRS})