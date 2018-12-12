CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

FIND_PACKAGE(OpenCV REQUIRED)

PROJECT(exposurefusion)

ADD_EXECUTABLE(exposurefusion exposurefusion.cpp image.cpp image.h pyramids.cpp pyramids.h)
TARGET_LINK_LIBRARIES(exposurefusion ${OpenCV_LIBS})
