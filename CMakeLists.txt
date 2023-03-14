cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(OmegaCF-Client)

find_package(Torch REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)



file(GLOB_RECURSE SOURCES "src/*.cpp")
message(WARNING ${SOURCES})
add_executable(OmegaCF-Client ${SOURCES})
target_link_libraries(OmegaCF-Client "${TORCH_LIBRARIES}")
target_link_libraries(OmegaCF-Client python3.10)
target_include_directories(OmegaCF-Client PRIVATE /usr/include/python3.10)


set_property(TARGET OmegaCF-Client PROPERTY CXX_STANDARD 17)










# The following code block is suggested to be used on Windows.
# According to https://github.com/pytorch/pytorch/issues/25457,
# the DLLs need to be copied to avoid memory errors.
if (MSVC)
  file(GLOB TORCH_DLLS "${TORCH_INSTALL_PREFIX}/lib/*.dll")
  add_custom_command(TARGET OmegaCF-Client
                     POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy_if_different
                     ${TORCH_DLLS}
                     $<TARGET_FILE_DIR:OmegaCF-Client>)
endif (MSVC)
