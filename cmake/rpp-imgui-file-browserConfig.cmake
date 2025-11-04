if (TARGET rpp-imgui-file-browser)
    return()
endif()

set(RPP_IMGUI_FILE_BROWSER_DIR "${RPP_EXTERNALS_DIR}/imgui-file-browser")

RPPFindPackage(rpp-imgui "Dependencies")

set(
    IMGUI_FILE_BROWSER_SOURCE_DIRS
    ${RPP_IMGUI_FILE_BROWSER_DIR}/ImGuiFileDialog.cpp
    ${RPP_IMGUI_FILE_BROWSER_DIR}/ImGuiFileDialog.h
    ${RPP_IMGUI_FILE_BROWSER_DIR}/ImGuiFileDialogConfig.h
)

add_library(
    rpp-imgui-file-browser 
    STATIC
    ${IMGUI_FILE_BROWSER_SOURCE_DIRS}
)

target_link_libraries(
    rpp-imgui-file-browser 
    PUBLIC
    rpp-imgui
)

target_include_directories(
    rpp-imgui-file-browser 
    PUBLIC
    ${RPP_IMGUI_FILE_BROWSER_DIR}
)