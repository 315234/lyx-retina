#
#  from kdelibs
#

if(WIN32)

	# check if GNUWIN32_DIR is already set 
	# (e.g. by command line argument or the calling script)
	if(NOT GNUWIN32_DIR)
		# check for enviroment variable
		file(TO_CMAKE_PATH "$ENV{GNUWIN32_DIR}" GNUWIN32_DIR)
		if(NOT GNUWIN32_DIR)
			# search in the default program install folder
			file(TO_CMAKE_PATH "$ENV{PROGRAMFILES}" _progFiles)
			find_file(GNUWIN32_DIR gnuwin32 win32libs 
					PATHS
					"${_progFiles}"
					"C:/" "D:/" "E:/" "F:/" "G:/"
			)
		endif()
	endif()

	if(GNUWIN32_DIR)
		 set(GNUWIN32_INCLUDE_DIR ${GNUWIN32_DIR}/include)
		 set(GNUWIN32_LIBRARY_DIR ${GNUWIN32_DIR}/lib)
		 set(GNUWIN32_BINARY_DIR  ${GNUWIN32_DIR}/bin)
		 set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} ${GNUWIN32_INCLUDE_DIR})
		 set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} ${GNUWIN32_LIBRARY_DIR})
		 set(GNUWIN32_FOUND TRUE)
	else()
		 set(GNUWIN32_FOUND)
	endif()

	if(GNUWIN32_FOUND)
		if (NOT GNUWIN32_FIND_QUIETLY)
			message(STATUS "Found GNUWIN32: ${GNUWIN32_DIR}")
		endif()
	else()
		if (GNUWIN32_FIND_REQUIRED)
			set(GNUWIN32_DIR CACHE PATH FORCE)
			message(STATUS "ERROR: Could NOT find GNUWIN32, please set GNUWIN32_DIR")
			message(STATUS "ERROR: or let cmake download all required files by using -DLYX_3RDPARTY_DOWNLOAD=1")
			message(FATAL_ERROR)
		endif()
	endif()

endif()

