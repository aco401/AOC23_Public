
# Make Library target
set(LIB_TARGET LIB_LOGGING)
set(SOURCES 
  FileLogger.cpp
  )


ADD_LIBRARY(${LIB_TARGET}
              ${SOURCES}
  )


target_include_directories(${LIB_TARGET} PUBLIC ${CMAKE_CURRENT_LIST_DIR})