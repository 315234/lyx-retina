# - Try to find ASPELL
# Once done this will define
#
#  ASPELL_FOUND - system has ASPELL
#  ASPELL_INCLUDE_DIR - the ASPELL include directory
#  ASPELL_LIBRARIES - The libraries needed to use ASPELL
#  ASPELL_DEFINITIONS - Compiler switches required for using ASPELL
#

if (ASPELL_INCLUDE_DIR AND ASPELL_LIBRARIES)
  # Already in cache, be silent
  set(ASPELL_FIND_QUIETLY TRUE)
endif (ASPELL_INCLUDE_DIR AND ASPELL_LIBRARIES)

FIND_PATH(ASPELL_INCLUDE_DIR aspell.h
  /usr/include
  /usr/local/include
  /usr/local/include/aspell
)

FIND_LIBRARY(ASPELL_LIBRARIES NAMES aspell aspell-15 libaspell
  PATHS
  /usr/lib
  /usr/local/lib
)


if (ASPELL_INCLUDE_DIR AND ASPELL_LIBRARIES)
   set(ASPELL_FOUND TRUE)
endif (ASPELL_INCLUDE_DIR AND ASPELL_LIBRARIES)

if (ASPELL_FOUND)
  if (NOT ASPELL_FIND_QUIETLY)
    message(STATUS "Found ASPELL: ${ASPELL_LIBRARIES}")
  endif (NOT ASPELL_FIND_QUIETLY)
else (ASPELL_FOUND)
  if (ASPELL_FIND_REQUIRED)
    message("aspell header: ${ASPELL_INCLUDE_DIR}")
    message("aspell lib   : ${ASPELL_LIBRARIES}")
    message(FATAL_ERROR "Could NOT find ASPELL")
  endif (ASPELL_FIND_REQUIRED)
endif (ASPELL_FOUND)

MARK_AS_ADVANCED(ASPELL_INCLUDE_DIR ASPELL_LIBRARIES)
