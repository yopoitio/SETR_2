cmake_minimum_required(VERSION 3.20)

project(project2 VERSION 1.0)

add_subdirectory(src/CMDPROC)

add_executable(exec src/main.c)

list(APPEND EXTRA_LIBS CMD_Lib)
list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/src/CMDPROC")

target_link_libraries(exec PUBLIC ${EXTRA_LIBS})

target_include_directories(exec PUBLIC ${EXTRA_INCLUDES})