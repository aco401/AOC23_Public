
# Make Library target
set(LIB_TARGET LIB_FILEHANDLING)
set(SOURCES 
  FileLineHandler.cpp
  )

set(HEADERS
  FileLineHandler.hpp)


ADD_LIBRARY(${LIB_TARGET}
              ${SOURCES}
              ${HEADERS}
  )


target_include_directories(${LIB_TARGET} PUBLIC ${CMAKE_CURRENT_LIST_DIR})

#message(${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(${LIB_TARGET} LIB_LOGGING )