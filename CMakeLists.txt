cmake_minimum_required(VERSION 2.6)
project(Gamework)

# Set the SFML_ROOT folder relative to the location of this file
# for where you put SFML
# You can also set this as an environment variable using a full path
# For Linux I used this to install SFML: sudo apt-get install libsfml-dev
# which installed it in a standard place so FindSFML locates it automatically
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	# This is for OS X
	set(SFML_ROOT "../../../Mac_Stuff/SFML-2.3.2-osx-clang-universal")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	# This is for Windows
	set(SFML_ROOT "../SFML-2.3.2")
endif()
	

# FindSFML.cmake needs to be in the source tree at cmake/Modules
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules" ${CMAKE_MODULE_PATH})
find_package(SFML 2 REQUIRED system window graphics network audio) #  
include_directories(
	${SFML_INCLUDE_DIR} 
	${CMAKE_SOURCE_DIR}/src/WC
	${CMAKE_SOURCE_DIR}/src/plist
	${CMAKE_SOURCE_DIR}/src/Gamework
	${CMAKE_SOURCE_DIR}/src/Example
)

# Add the sources and resources
file(GLOB src
	src/WC/*
	src/plist/* 
	src/Gamework/*
	src/Example/*
	src/main.cpp
)
#file(GLOB art res/* )

add_executable(Gamework ${src} ${art})
target_link_libraries(Gamework ${SFML_LIBRARIES})

# This makes OS X use C++-11 features (auto keyword)
target_compile_features(Gamework PRIVATE cxx_range_for)

# This copies the "resources" from the res folder to be with the executable
add_custom_command(TARGET Gamework POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:Gamework>)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	file(GLOB dll ${SFML_INCLUDE_DIR}/../bin/*.dll)
	# This, on Windows, copyes the SFML dll's to the exe's folder
	add_custom_command(TARGET Gamework POST_BUILD
	                   COMMAND ${CMAKE_COMMAND} -E copy_if_different
	                       ${dll} $<TARGET_FILE_DIR:Gamework>)
endif()
