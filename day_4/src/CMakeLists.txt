# GSL Usage
FetchContent_Declare(GSL
    GIT_REPOSITORY "https://github.com/microsoft/GSL"
    GIT_TAG "v4.0.0"
    GIT_SHALLOW ON
)

FetchContent_MakeAvailable(GSL)


# Make Library target
set(LIB_TARGET LIB_DAY4)
set(SOURCES 
  CoutLogger.cpp
  FileLogger.cpp
  ScratchCard.cpp
  ScratchCardParser.cpp
  Day4Solver.cpp
  Utility.cpp)


ADD_LIBRARY(${LIB_TARGET}
              ${SOURCES}
  )

target_include_directories(${LIB_TARGET} PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(${LIB_TARGET} PRIVATE GSL)

# Make Executable
set(EXEC_TARGET EXEC_DAY4)

ADD_EXECUTABLE(${EXEC_TARGET} main.cpp)

target_link_libraries(${EXEC_TARGET} ${LIB_TARGET})


add_compile_options(${EXEC_TARGET} PRIVATE 
-Werror
-Wall
-Wextra # reasonable and standard
-Wshadow # warn the user if a variable declaration shadows one from a parent context
-Wnon-virtual-dtor # warn the user if a class with virtual functions has a non-virtual destructor. This helps
# catch hard to track down memory errors
-Wold-style-cast # warn for c-style casts
-Wcast-align # warn for potential performance problem casts
-Wunused # warn on anything being unused
-Woverloaded-virtual # warn if you overload (not override) a virtual function
-Wpedantic # warn if non-standard C++ is used
-Wconversion # warn on type conversions that may lose data
-Wsign-conversion # warn on sign conversions
-Wnull-dereference # warn if a null dereference is detected
-Wdouble-promotion # warn if float is implicit promoted to double
-Wformat=2 # warn on security issues around functions that format output (ie printf)
-Wimplicit-fallthrough # warn on statements that fallthrough without an explicit annotation
)



