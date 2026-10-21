# The emulators the engine needs, from the engine repo (libs/machinemodule = sd88me/mpc-vst-machinedrum): dsp56300 (its static libraries
# only) and Musashi (inside gearmulator-md-mm). As that repo's own cmake/dsp56300.cmake, with the paths of this repo.
set(MD_DSP56300_DIR "${MD_ROOT}/libs/dsp56300")
if(NOT EXISTS "${MD_DSP56300_DIR}/source/dsp56kEmu/dsp.h")
  message(FATAL_ERROR "dsp56300 not found at ${MD_DSP56300_DIR} -- run 'git submodule update --init --recursive' first")
endif()
set(ASMJIT_STATIC TRUE)
set(ASMJIT_NO_INSTALL TRUE)
add_subdirectory("${MD_DSP56300_DIR}/source/asmjit"     "${CMAKE_BINARY_DIR}/dsp56300/asmjit"     EXCLUDE_FROM_ALL)
add_subdirectory("${MD_DSP56300_DIR}/source/dsp56kBase" "${CMAKE_BINARY_DIR}/dsp56300/dsp56kBase" EXCLUDE_FROM_ALL)
add_subdirectory("${MD_DSP56300_DIR}/source/dsp56kEmu"  "${CMAKE_BINARY_DIR}/dsp56300/dsp56kEmu"  EXCLUDE_FROM_ALL)
add_subdirectory("${MD_DSP56300_DIR}/source/vtuneSdk"   "${CMAKE_BINARY_DIR}/dsp56300/vtuneSdk"   EXCLUDE_FROM_ALL)
add_subdirectory("${MD_ROOT}/libs/gearmulator-md-mm/source/mc68k" "${CMAKE_BINARY_DIR}/mc68k" EXCLUDE_FROM_ALL)
