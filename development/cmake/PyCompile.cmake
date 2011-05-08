# This file is part of LyX, the document processor.
# Licence details can be found in the file COPYING.
#
# Copyright (c) 2009, Peter K�mmel, <syntheticpp@gmx.net>
#		    , Kornel Benko, <Kornel.Benko@berlin.de>
#

# _project has to be set before adding
#		#include(../PyCompile)
project(${_project})

FIND_PROGRAM(LYX_PYTHON_EXECUTABLE python)

file(GLOB _py_files ${TOP_SRC_DIR}/lib/${_project}/*.py)

set(py_compile sh ${TOP_SRC_DIR}/config/py-compile)

set(_generated)
set(ENV{PYTHON} ${LYX_PYTHON_EXECUTABLE})

foreach(_orig_py ${_py_files})
  get_filename_component(_base_we_py ${_orig_py} NAME_WE)
  set(_compiled_py_ "${CMAKE_CURRENT_BINARY_DIR}/${_base_we_py}.pyc" "${CMAKE_CURRENT_BINARY_DIR}/${_base_we_py}.pyo")
  set(_copied_py "${CMAKE_CURRENT_BINARY_DIR}/${_base_we_py}.py")
# we need that file really local first, due to the "py_compile"-script
# because the creation will be in the directory of .py-file
  add_custom_command(
    OUTPUT ${_copied_py}
    PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${_orig_py}" ${_copied_py}
    DEPENDS ${_orig_py}
    )
  add_custom_command(
    OUTPUT ${_compiled_py_}
    PRE_BUILD
    COMMAND ${py_compile} ${_copied_py}
    DEPENDS ${_copied_py}
    )
  SET_SOURCE_FILES_PROPERTIES(${_copied_py} GENERATED)
  install(PROGRAMS ${_compiled_py_} ${_copied_py} DESTINATION ${_project})
  LIST(APPEND _generated ${_compiled_py_})
endforeach(_orig_py)

ADD_CUSTOM_TARGET(${_project} ALL DEPENDS ${_generated})
