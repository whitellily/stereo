PROJECT(sift)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
if(COMMAND cmake_policy)
      cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

FIND_PACKAGE( OpenCV REQUIRED )

# Declare the target (an executable)
ADD_EXECUTABLE(sift  sift.c dspfeat.c imgfeatures.c kdtree.c match.c minpq.c  siftfeat.c utils.c xform.c)

TARGET_LINK_LIBRARIES(sift ${OpenCV_LIBS})

#MESSAGE(STATUS "OpenCV_LIBS: ${OpenCV_LIBS}")



