macro(ExternalsSetup)
    message("-----------------------------------------")
    message(STATUS "Setting up externals...")

    set(TEMPORARY_RPP_EXTERNALS_DIR "${CMAKE_CURRENT_LIST_DIR}/../externals")
    cmake_path(SET RPP_EXTERNALS_DIR NORMALIZE ${TEMPORARY_RPP_EXTERNALS_DIR} )

    if(NOT EXISTS "${RPP_EXTERNALS_DIR}")
        file(MAKE_DIRECTORY "${RPP_EXTERNALS_DIR}")
        message("Created externals directory: ${RPP_EXTERNALS_DIR}")
    endif()
endmacro()

macro(RPPFindPackage PACKAGE_NAME FOLDER_NAME)
    if (NOT FOLDER_NAME STREQUAL "")
        set(CMAKE_FOLDER "${FOLDER_NAME}")
    endif()

    find_package(${PACKAGE_NAME} REQUIRED PATHS ${CMAKE_MODULE_PATH})
    unset(CMAKE_FOLDER)
endmacro()