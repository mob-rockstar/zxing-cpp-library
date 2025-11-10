
cmake_minimum_required(VERSION 3.16)
project(zxing_wrapper LANGUAGES CXX)

# Adjust these paths if necessary
set(ZXING_ROOT "C:/dev/zxing-cpp")
set(ZXING_BUILD "C:/dev/zxing-build")

# Add zxing include and library paths
include_directories(${ZXING_ROOT}/core/src)
# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
link_directories(${ZXING_BUILD}/core/Release)

# Create DLL target
add_library(zxing-wrapper SHARED
    src/zxing_wrapper.cpp
    # include/zxing_wrapper.h
)

target_include_directories(zxing-wrapper PRIVATE ${ZXING_ROOT}/core/src)

target_link_directories(zxing-wrapper PRIVATE ${ZXING_BUILD}/core/Release)

# Link to zxing
target_link_libraries(zxing-wrapper PRIVATE zxing)

# Compiler settings
target_compile_features(zxing-wrapper PRIVATE cxx_std_17)

# Optional: export symbols for Windows
target_compile_definitions(zxing-wrapper PRIVATE ZXING_WRAPPER_EXPORTS)
