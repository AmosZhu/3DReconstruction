cmake_minimum_required(VERSION 3.12)
project(3DReconstruction)

set(CMAKE_CXX_STANDARD 17)

# Include the folder we need
include_directories(
        $ENV{OPENCV}/include
        .
)

# Inlude the 3rd-party library folder
link_directories(
        $ENV{OPENCV}/x64/vc15/lib
)

# Target the specific libraries
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    # opencv libraries
    set(OPENCV_MODULE
            opencv_highgui343d.lib
            opencv_core343d.lib
            opencv_imgcodecs343d.lib
            opencv_imgproc343d.lib)
else ()
    # opencv libraries
    set(OPENCV_MODULE
            opencv_highgui343.lib
            opencv_core343.lib
            opencv_imgcodecs343.lib
            opencv_imgproc343.lib)
endif ()

add_subdirectory(
        Polarisation
)

add_executable(ShapeEstimation examples/Polarisation_Decompose.cpp)

target_link_libraries(ShapeEstimation PolarisationInfo ${OPENCV_MODULE})
