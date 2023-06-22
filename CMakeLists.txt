# CMakeList.txt: proyecto de CMake para NEW_EDA_Level_5, incluya el origen y defina
# la lógica específica del proyecto aquí.
#
cmake_minimum_required(VERSION 3.1.4)
set(CMAKE_CXX_STANDARD 11)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("NEW_EDA_Level_5")

# Agregue un origen al ejecutable de este proyecto.
add_executable (NEW_EDA_Level_5 "NEW_EDA_Level_5.cpp"  "autoRoute.h" "autoRoute.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET NEW_EDA_Level_5 PROPERTY CXX_STANDARD 20)
endif()

# TODO: Agregue pruebas y destinos de instalación si es necesario.
