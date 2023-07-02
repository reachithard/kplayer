#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "SoundTouch::SoundTouch" for configuration "Release"
set_property(TARGET SoundTouch::SoundTouch APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(SoundTouch::SoundTouch PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib64/libSoundTouch.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS SoundTouch::SoundTouch )
list(APPEND _IMPORT_CHECK_FILES_FOR_SoundTouch::SoundTouch "${_IMPORT_PREFIX}/lib64/libSoundTouch.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
