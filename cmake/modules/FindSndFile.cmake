# Try to find sndfile
# Once done this will define
#
# SndFile_FOUND - system has sndfile
# SndFile::SndFile - the sndfile library

find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(SndFile sndfile QUIET)
else()
    find_path(
        SndFile_INCLUDEDIR
        NAMES sndfile.h
        PATH_SUFFIXES sndfile
        DOC "SndFile include directory"
    )

    find_library(
        SndFile_LINK_LIBRARIES
        NAMES sndfile sndfile-1
        DOC "SndFile library"
    )
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
    SndFile
    DEFAULT_MSG
    SndFile_LINK_LIBRARIES
    SndFile_INCLUDEDIR
)

if(EXISTS "${SndFile_INCLUDEDIR}/sndfile.h")
    file(STRINGS "${SndFile_INCLUDEDIR}/sndfile.h"
         SndFile_SUPPORTS_SET_COMPRESSION_LEVEL
         REGEX ".*SFC_SET_COMPRESSION_LEVEL.*")
endif()

if(SndFile_SUPPORTS_SET_COMPRESSION_LEVEL)
    set(SndFile_SUPPORTS_SET_COMPRESSION_LEVEL ON)
else()
    set(SndFile_SUPPORTS_SET_COMPRESSION_LEVEL OFF)
endif()
mark_as_advanced(SndFile_SUPPORTS_SET_COMPRESSION_LEVEL)

if(SndFile_FOUND)
    if(NOT TARGET SndFile::sndfile)
        add_library(SndFile::sndfile INTERFACE IMPORTED)
        target_link_libraries(
            SndFile::sndfile INTERFACE "${SndFile_LINK_LIBRARIES}"
        )
        target_include_directories(
            SndFile::sndfile INTERFACE "${SndFile_INCLUDEDIR}"
        )
    endif()
endif()
