if (TARGET rpp-imgui-notify)
    return()
endif()

set(IMGUI_NOTIFY_DIR "${RPP_EXTERNALS_DIR}/imgui-notify")

add_library(
    rpp-imgui-notify
    INTERFACE 
)

target_include_directories(
    rpp-imgui-notify
    INTERFACE 
    "${IMGUI_NOTIFY_DIR}"
)

RPPFindPackage(rpp-imgui "Dependencies")

target_link_libraries(
    rpp-imgui-notify
    INTERFACE
    rpp-imgui
)