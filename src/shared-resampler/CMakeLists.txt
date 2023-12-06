project(shared-resampler)

file(GLOB _src *.h *.cpp)

add_library(${PROJECT_NAME} SHARED ${_src})

target_link_libraries(${PROJECT_NAME} PRIVATE LoadExecutable)

target_include_directories(${PROJECT_NAME} PUBLIC .)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)