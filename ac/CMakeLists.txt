cmake_minimum_required(VERSION 2.8)

SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS}  -g")

SET(CMAKE_BUILD_TYPE Debug)

SET(AC_ROOT_DIR ./)
SET(AC_INCLUDE_DIR ./include)
SET(AC_SRC_DIR ./src)

project(AccessControl CXX C)

 SET_SOURCE_FILES_PROPERTIES(${AC_SRC_DIR}/acyy.cpp GENERATED)
 SET_SOURCE_FILES_PROPERTIES(${AC_SRC_DIR}/acll.cpp GENERATED)
# Create target for the parser
 ADD_CUSTOM_TARGET(AcParser echo "Creating acyy.cpp")

ADD_CUSTOM_COMMAND(
   SOURCE ${AC_ROOT_DIR}/lexer/acyy.y
   COMMAND ${BISON_EXECUTABLE} 
   ARGS -d ${AC_INCLUDE_DIR}/acyy.hpp
        -o ${AC_SRC_DIR}/acyy.cpp
        ${AC_ROOT_DIR}/lexer/acyy.y
   TARGET AcParser
   OUTPUTS ${AC_SRC_DIR}/src/acyy.cpp)

# Create custom command for flex/lex (note the outputs)
 ADD_CUSTOM_COMMAND(
   SOURCE ${AC_ROOT_DIR}/lexer/acll.l
   COMMAND flex 
   ARGS -i
        -o${AC_SRC_DIR}/acll.cpp
        ${AC_ROOT_DIR}/lexer/acll.l
   TARGET AcParser
   OUTPUTS ${AC_SRC_DIR}/src/acll.cpp)

# Create custom command for bison/yacc (note the DEPENDS)
 
# Add parser.c to the list of sources
# SET(AC_SRCS ${Foo_SRCS} ${Foo_BINARY_DIR}/src/parser.c)

# Since parser.c does not exists yet when cmake is run, mark
# it as generated

# Include binary directory to include lexer.c in parser.c
# INCLUDE_DIRECTORIES(${Foo_BINARY_DIR}/src)


# Look for flags to enable C++11 support, if any.
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_HAS_STD_CXX11)
if (COMPILER_HAS_STD_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
else()
    CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_HAS_STD_CXX0X)
    if (COMPILER_HAS_STD_CXX0X)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
    endif()
endif()

include_directories(./include/)
# python header
# include_directories(/usr/include/python2.6)

# link_directories(/usr/lib)

set(EXECUTABLE_OUTPUT_PATH bin)


#add_executable(
#    lemma
#    main.cc
#    include/source/RdrLemmatizer.cpp
#    )

add_executable(
  test
  ${AC_SRC_DIR}/acll.cpp
  ${AC_SRC_DIR}/acyy.cpp
  ${AC_SRC_DIR}/db_worker.cpp
  ${AC_SRC_DIR}/postgres_worker.cpp
  ${AC_SRC_DIR}/access_control.cpp
)

#target_link_libraries(
#   merge_tfidf_aggreagation
#    python2.6
#    )