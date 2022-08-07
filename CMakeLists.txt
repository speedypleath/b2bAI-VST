cmake_minimum_required(VERSION 3.23)
project(b2bAI-VST
        VERSION 0.1.0
        LANGUAGES CXX
        DESCRIPTION "VST plugin")

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_OSX_ARCHITECTURES arm64;x86_64)

#JUCE
add_subdirectory(lib/JUCE EXCLUDE_FROM_ALL)

juce_add_module(lib/foleys_gui_magic)
juce_add_binary_data(${PROJECT_NAME}_data
        SOURCES
        ${CMAKE_SOURCE_DIR}/res/magic.xml)

set_property(GLOBAL PROPERTY JUCE_COPY_PLUGIN_AFTER_BUILD TRUE)

option(JUCE_ENABLE_MODULE_SOURCE_GROUPS "Enable Module Source Groups" ON)
set_property(GLOBAL PROPERTY USE_FOLDERS YES)

# Company settings
set_directory_properties(PROPERTIES JUCE_COMPANY_COPYRIGHT "GNU GENERAL PUBLIC LICENSE Version 3")
set_directory_properties(PROPERTIES JUCE_COMPANY_NAME "brahman")
set_directory_properties(PROPERTIES JUCE_COMPANY_WEBSITE "https://github.com/speedypleath/b2bAI-VST")
set_directory_properties(PROPERTIES JUCE_COMPANY_EMAIL "gheorgheandrei13@gmail.com")

# Plugin
add_subdirectory(plugin)
