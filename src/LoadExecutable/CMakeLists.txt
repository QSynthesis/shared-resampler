project(LoadExecutable)

file(GLOB _src *.h *.cpp)

add_library(${PROJECT_NAME} STATIC ${_src})

target_include_directories(${PROJECT_NAME} PUBLIC .)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)