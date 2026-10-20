# Includes CPM to easily pull repositories for external dependencies
if (NOT EXISTS "${CMAKE_BINARY_DIR}/cmake/CPM.cmake")
    Message(STATUS "Downloading CPM.cmake to ${CMAKE_BINARY_DIR}/cmake")
    File(DOWNLOAD
        "https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.42.1/CPM.cmake"
        "${CMAKE_BINARY_DIR}/cmake/CPM.cmake"
        EXPECTED_HASH SHA256=f3a6dcc6a04ce9e7f51a127307fa4f699fb2bade357a8eb4c5b45df76e1dc6a5
    )
endif ()
Include("${CMAKE_BINARY_DIR}/cmake/CPM.cmake")

If(${${PROJECT_NAME}_BUILD_TESTS})
    enable_testing()

    CPMAddPackage(
        NAME gtest
        GITHUB_REPOSITORY "google/googletest"
        GIT_TAG main
    )
    Set_Property(TARGET GTest::gtest_main PROPERTY FOLDER "Dependencies")
    Set_Property(TARGET GTest::gmock PROPERTY FOLDER "Dependencies")
    Set_Property(TARGET GTest::gmock_main PROPERTY FOLDER "Dependencies")
Endif()