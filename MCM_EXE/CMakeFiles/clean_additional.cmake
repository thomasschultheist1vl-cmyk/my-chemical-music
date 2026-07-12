# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "CMakeFiles\\mcm_qt_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\mcm_qt_autogen.dir\\ParseCache.txt"
  "mcm_qt_autogen"
  )
endif()
