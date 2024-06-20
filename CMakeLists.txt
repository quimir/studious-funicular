cmake_minimum_required(VERSION 3.5)

project(student_management_system VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# 填上自己的Qt编译器路径，如果没有则会使用默认的Qt路径
#IF (MSVC)
#  set(CMAKE_PREFIX_PATH "E:\\Qt\\6.2.4\\msvc2019_64")
#  set(QT_DIR "E:\\Qt\\6.2.4\\msvc2019_64\\lib\\cmake\\Qt6")
#ELSEIF (MINGW)
#  set(CMAKE_PREFIX_PATH "E:\\Qt\\6.2.4\\mingw_64")
#  set(QT_DIR "E:\\Qt\\6.2.4\\mingw_64\\lib\\cmake\\Qt6")
#ENDIF ()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools Sql)

set(TS_FILES student_management_system_zh_CN.ts)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set(HEADER_FILES_DIR ${CMAKE_SOURCE_DIR})

set(SOURCE_FILES_DIR ${CMAKE_SOURCE_DIR})

set(UI_FILES_DIR ${CMAKE_SOURCE_DIR})

file(GLOB HEADER_FILES
	"${HEADER_FILES_DIR}/*.h"
	"${HEADER_FILES_DIR}/*.hpp")

file(GLOB SOURCE_FILES
	"${SOURCE_FILES_DIR}/*.cpp"
	"${SOURCE_FILES_DIR}/*.cc")

file(GLOB UI_FILES
	"${UI_FILES_DIR}/*.ui")

set(PROJECT_SOURCES
	main.cpp
	${UI_FILES}
	${HEADER_FILES}
	${SOURCE_FILES}
	${TS_FILES}
)

if (${QT_VERSION_MAJOR} GREATER_EQUAL 6)
  qt_add_executable(student_management_system
	  MANUAL_FINALIZATION
	  ${PROJECT_SOURCES}
	  软件需求.md
  )
  # Define target properties for Android with Qt 6 as:
  #    set_property(TARGET student_management_system APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
  #                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
  # For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

  qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else ()
  if (ANDROID)
	add_library(student_management_system SHARED
		${PROJECT_SOURCES}
	)
	# Define properties for Android with Qt 5 after find_package() calls as:
	#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
  else ()
	add_executable(student_management_system
		${PROJECT_SOURCES}
	)
  endif ()

  qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif ()

target_link_libraries(student_management_system PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
	Qt${QT_VERSION_MAJOR}::Sql)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if (${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.student_management_system)
endif ()
set_target_properties(student_management_system PROPERTIES
	${BUNDLE_ID_OPTION}
	MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
	MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
	MACOSX_BUNDLE TRUE
	WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS student_management_system
	BUNDLE DESTINATION .
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if (QT_VERSION_MAJOR EQUAL 6)
  qt_finalize_executable(student_management_system)
endif ()

include_directories(${CMAKE_SOURCE_DIR}/include)
