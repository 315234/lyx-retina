
# CPack packaging
#
# http://cmake.org/cmake/help/cmake-2-8-docs.html#module:CPack

if(LYX_BUNDLE)
	if(APPLE)
		set(CPACK_GENERATOR DragNDrop)
	elseif(UNIX)
		set(CPACK_GENERATOR STGZ)
	elseif(WIN32)
		set(CPACK_GENERATOR ZIP)
	endif()
endif()


set(CPACK_PACKAGE_VERSION_MAJOR "${LYX_MAJOR_VERSION}")
set(CPACK_PACKAGE_VERSION_MINOR "${LYX_MINOR_VERSION}")
FILE(STRINGS "${TOP_SRC_DIR}/${LYX_CMAKE_DIR}/LyX_summary.txt" CPACK_PACKAGE_DESCRIPTION_SUMMARY)


set(CPACK_PACKAGE_INSTALL_DIRECTORY "CMake ${LYX_INSTALL_SUFFIX}")

#
# needed by rpm
set(CPACK_SET_DESTDIR "ON")
FILE(READ "${TOP_SRC_DIR}/${LYX_CMAKE_DIR}/LyX_license.txt" CPACK_RPM_PACKAGE_LICENSE)
set(CPACK_RPM_PACKAGE_GROUP "Applications/Publishing")
set(CPACK_RPM_PACKAGE_VENDOR "The LyX team")
#
# the next ones are needed by deb
set(CPACK_PACKAGE_CONTACT "${PACKAGE_BUGREPORT}")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${LYX_PROJECT}-${LYX_INSTALL_SUFFIX}")

# This is experimental, valid on _my_ system (Kornel)
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libqtgui4 (>= 4.4.3-0)")
if(ASPELL_FOUND)
	set(CPACK_DEBIAN_PACKAGE_DEPENDS "libaspell15 (>= 0.60.6-1),${CPACK_DEBIAN_PACKAGE_DEPENDS}")
endif()
if(ENCHANT_FOUND)
	set(CPACK_DEBIAN_PACKAGE_DEPENDS "libenchant1c2a (>= 1.6.0-0),${CPACK_DEBIAN_PACKAGE_DEPENDS}")
endif()
if(LYX_EXTERNAL_BOOST)
	if(Boost_FOUND)
		set(CPACK_DEBIAN_PACKAGE_DEPENDS "libboost-regex1.40.0 (>= 1.40.0-4),libboost-signals1.40.0 (>= 1.40.0-4),${CPACK_DEBIAN_PACKAGE_DEPENDS}")
	endif()
endif()
#
# for the next ones, cpack insists on data with values in some file
set(CPACK_PACKAGE_DESCRIPTION_FILE "${TOP_SRC_DIR}/${LYX_CMAKE_DIR}/LyX_description.txt")
set(CPACK_RESOURCE_FILE_LICENSE "${TOP_SRC_DIR}/${LYX_CMAKE_DIR}/LyX_license.txt")

# Use the revision number saved in ${LYX_PACKAGE_RELEASE}
# as the release in rpm-package-build.
# This way we maycan omit the otherwise needed "--force" parameter when
# installing from that rpm package.
set(CPACK_RPM_PACKAGE_RELEASE ${LYX_PACKAGE_RELEASE})
set(CPACK_PACKAGE_VERSION_PATCH ${LYX_REVISION_VERSION})

# we do not have a readme or welcome data,
# so we do not provide infofiles for
# CPACK_RESOURCE_FILE_README and CPACK_RESOURCE_FILE_WELCOME

# sources
set(CPACK_SOURCE_INSTALLED_DIRECTORIES "${TOP_SRC_DIR};/") # http://www.mail-archive.com/cmake@cmake.org/msg33720.html


set(CPACK_STRIP_FILES 1)

# Finaly give some hints about files not to be included in souse package
SET(CPACK_SOURCE_IGNORE_FILES
"/CVS/"
"/\\\\.svn/"
"/\\\\.bzr/"
"/\\\\.hg/"
"/\\\\.git/"
"\\\\.swp$"
"~$"
"\\\\.history$"
"\\\\.#"
"/#"
"/_CPack_Packages/"
"/CMakeCache.txt$"
"CPackSourceConfig\\\\.cmake"
"\\\\.tar\\\\.gz$"
"\\\\.tar\\\\.bz2$"
"\\\\.tar\\\\.Z$"
"\\\\.deb$"
"\\\\.rpm$"
"\\\\.rej$"
"\\\\.orig$"
"/CMakeFiles/"
"Makefile\\\\.in"
"/Makefile$"
"/autom4te\\\\.cache/"
"/\\\\.deps/"
"/build/"
"/lyx-2\\\\."
)

if(LYX_CPACK)
	include(CPack)
endif()


#Now it is possible to create some packages
# cd <BuildDir>
# make package
#############################################################################################
# So, e.g. for variables
#    CMAKE_PROJECT_NAME                     : lyx
#    CPACK_PACKAGE_VERSION_MAJOR            : 2
#    CPACK_PACKAGE_VERSION_MINOR            : 0
#    CPACK_PACKAGE_VERSION_PATCH            : 1
#    CMAKE_SYSTEM_NAME                      : Linux
#    CPACK_BINARY_DEB:BOOL                  : ON
#
# the package name builds as "lyx-2.0.1-Linux.deb"
#
############################## rpm ################################### deb ##################
# create    # cpack -G RPM --config CPackConfig.cmake   # cpack -G DEB --config CPackConfig.cmake
# creates =># lyx-2.0.1-Linux.rpm                       # lyx-2.0.1-Linux.deb
# list      # rpm -qlp lyx-2.0.1-Linux.rpm              # dpkg-deb -c lyx-2.0.1-Linux.deb
# install   # rpm -U lyx-2.0.1-Linux.rpm                # dpkg -i lyx-2.0.1-Linux.deb
#

