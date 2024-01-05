cmake_minimum_required(VERSION 3.27)

project(LUMATONE_EDITOR VERSION 0.0.1)

# find_package(juce)
add_subdirectory($ENV{juce_DIR} subprojects/juce)

juce_add_gui_app(
    LumatoneEditor
        PRODUCT_NAME "Lumatone Editor"
        # VERSION "1.2.0"
        # BUNDLE_ID "com.Lumatone.LumatoneEditor"
        # ICON_SMALL "${CMAKE_CURRENT_SOURCE_DIR}/BinaryData/Assets/PNG/AppIcon@1x.png"
        # ICON_BIG "${CMAKE_CURRENT_SOURCE_DIR}/BinaryData/Assets/PNG/AppIcon@3x.png"
        # DOCUMENT_EXTENSIONS "ltn"
        )
juce_generate_juce_header(LumatoneEditor)

set_directory_properties(
    PROPERTIES
        COMPANY_COPYRIGHT "BSD-0"
        COMPANY_NAME "Lumatone"
        COMPANY_WEBSITE "https://www.lumatone.io"
        COMPANY_EMAIL "support@lumatone.io"
)

file(GLOB_RECURSE SourceCode
    CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.h"
    )

if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    file(GLOB_RECURSE LibraryCode CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/Libraries/win64/include/*.h")
    set(CryptoLib "${CMAKE_CURRENT_SOURCE_DIR}/Libraries/win64/bin/libcrypto-1_1-x64.dll")
    set(SSHLib "${CMAKE_CURRENT_SOURCE_DIR}/Libraries/win64/bin/libssh2-x64.dll")
endif()

if (UNIX)
    file(GLOB_RECURSE LibraryCode CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/Libraries/mac/include/*.h")
    file(READ
        CryptoLib
            "${CMAKE_CURRENT_SOURCE_DIR}/Libraries/mac/lib/libgpg-error.dylib"
            "${CMAKE_CURRENT_SOURCE_DIR}/Libraries/mac/lib/libgcrypt.dylib"
        )
    file(READ SSHLib "${CMAKE_CURRENT_SOURCE_DIR}/Libraries/mac/lib/libssh2.dylib")
endif()

target_sources(
    LumatoneEditor
        PRIVATE
            ${SourceCode}
            ${LibraryCode}
            )

target_compile_definitions(LumatoneEditor
    PRIVATE
        # Set to 1 if you are building with licensed fonts
        USE_LICENSED_FONTS=0

        SERVERKEY="debian"
        SERVERPWD="temppwd"

        SERVERHOST1="192.168.6.2"
        SERVERHOST2="192.168.7.2"

        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:LumatoneEditor,JUCE_PRODUCT_NAME>"
        JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:LumatoneEditor,JUCE_VERSION>"
        DONT_SET_USING_JUCE_NAMESPACE=0
    )

if (UNIX)
    target_compile_definitions(LumatoneEditor
        PRIVATE
        HAVE_SYS_SOCKET_H=1
        HAVE_NETINET_IN_H=1
        HAVE_SYS_SELECT_H=1
        HAVE_UNISTD_H=1
        HAVE_ARPA_INET_H=1
        HAVE_SYS_TIME_H=1
        HAVE_POLL_H=1
    )
endif()

file(GLOB_RECURSE LibraryBinaryData
        CONFIGURE_DEPENDS
            "${CMAKE_CURRENT_SOURCE_DIR}/Source/lumatone_editor_library/assets/*"
    )
juce_add_binary_data(LumatoneEditorLibraryAssets SOURCES ${LibraryBinaryData})

file(GLOB_RECURSE EditorBinaryData
        CONFIGURE_DEPENDS
            "${CMAKE_CURRENT_SOURCE_DIR}/BinaryData/*"
    )
juce_add_binary_data(LumatoneEditorAssets SOURCES ${EditorBinaryData})

# juce_add_bundle_resources_directory(LumatoneEditor "${CMAKE_CURRENT_SOURCE_DIR}/Libraries/mac/lib")

target_link_libraries(LumatoneEditor
    PRIVATE
        LumatoneEditorAssets
        juce::juce_gui_extra
        juce::juce_audio_utils
        juce::juce_audio_devices
        juce::juce_opengl

        # ${CryptoLib}
        # ${SSHLib}

    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
    )
