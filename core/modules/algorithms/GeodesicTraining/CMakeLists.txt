# Note: Project name is set by options.cmake
project( ${MODULE_NAME} )

find_package( MITK REQUIRED )

find_package(Qt5 COMPONENTS Core)
set(CMAKE_AUTOMOC ON)

add_subdirectory( 
  GeodesicTraining_external
)

set( CPP_FILES
  GeodesicTrainingModule.cpp
  GeodesicTrainingQt.cpp
  GeodesicTrainingQtOnly.cpp
  Warnings/GeodesicTrainingWarningImageSize.cpp
) 
set( H_FILES
  GeodesicTrainingModule.h
  GeodesicTrainingQt.h
  GeodesicTrainingQtOnly.h
  Warnings/GeodesicTrainingWarningImageSize.h
) 
set( UI_FILES

) 
set( MOC_H_FILES
  GeodesicTrainingModule.h
  GeodesicTrainingQt.h
  GeodesicTrainingQtOnly.h
  Warnings/GeodesicTrainingWarningImageSize.h
) 


# Create the library
add_library( ${MODULE_NAME} STATIC
  ${CPP_FILES} ${H_FILES} ${UI_FILES} ${MOC_H_FILES}
)

target_link_libraries( ${MODULE_NAME} 
  MpipBase
  ${NEEDS_MODULES}
  GeodesicTrainingSegmentation
  
  Qt5::Core

  MitkCore 
  MitkMultilabel
  MitkSegmentation
)

target_include_directories( ${MODULE_NAME} PUBLIC
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/Warnings
)
