#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "freetype" for configuration ""
set_property(TARGET freetype APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(freetype PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "C"
  IMPORTED_LINK_INTERFACE_LIBRARIES_NOCONFIG "/usr/lib/x86_64-linux-gnu/libz.so;/usr/lib/x86_64-linux-gnu/libpng.so;/usr/lib/x86_64-linux-gnu/libz.so;/usr/lib/x86_64-linux-gnu/libharfbuzz.so;/usr/lib/x86_64-linux-gnu/libharfbuzz-icu.so"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libfreetype.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS freetype )
list(APPEND _IMPORT_CHECK_FILES_FOR_freetype "${_IMPORT_PREFIX}/lib/libfreetype.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
