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

target_sources(${PROJECT_NAME} PRIVATE
        PluginEditor.cpp
        PluginProcessor.cpp
        PluginEditor.h
        PluginProcessor.h
        KeyboardComponent.cpp
        KeyboardComponent.h
        PianoRollComponent.cpp
        PianoRollComponent.h
        GridComponent.cpp
        GridComponent.h
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/lib)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/lib/JUCE/modules)

target_compile_definitions(${PROJECT_NAME}
        PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
        JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED=1
        )

target_link_libraries(${PROJECT_NAME} PRIVATE
        ${PROJECT_NAME}_data
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
