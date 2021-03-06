#--------------------------------------------------------------------------------
# Source file specification
#--------------------------------------------------------------------------------

INCLUDE(InstallRequiredSystemLibraries)

INSTALL( TARGETS ${PROJECT_NAME} 
  #DESTINATION bin  
  #BUNDLE DESTINATION .
  RUNTIME DESTINATION bin COMPONENT applications
  #LIBRARY DESTINATION lib
  CONFIGURATIONS "${CMAKE_CONFIGURATION_TYPES}"
  PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)

# INSTALL( FILES MpipCore 
#   #RUNTIME DESTINATION bin COMPONENT RuntimeLibraries
#   LIBRARY DESTINATION bin COMPONENT RuntimeLibraries
#   #ARCHIVE DESTINATION lib COMPONENT Development
# )

# This is for packaging only

SET( EXE_NAME "${PROJECT_NAME}.exe" )
SET( PROJECT_VENDOR "CBICA - UPenn" )
SET( README_FILE  "${PROJECT_SOURCE_DIR}/README.md" )
SET( PROJECT_DOMAIN "http://www.med.upenn.edu/sbia/software/" )
SET( PROJECT_CONTACT "software@cbica.upenn.edu" )

ADD_DEFINITIONS(-DPROJECT_NAME="${PROJECT_NAME}" )
ADD_DEFINITIONS(-DPROJECT_VERSION="${PROJECT_VERSION}" )
ADD_DEFINITIONS(-DEXE_NAME="${EXE_NAME}")
#ADD_DEFINITIONS(-DPROJECT_NAME_EXTENDED="${PROJECT_NAME_EXTENDED}")
#ADD_DEFINITIONS(-DCONTACT="${PROJECT_CONTACT}")
ADD_DEFINITIONS(-DPROJECT_CONTACT="${PROJECT_CONTACT}")
ADD_DEFINITIONS(-DPROJECT_DOMAIN="${PROJECT_DOMAIN}")
#ADD_DEFINITIONS(-DPROJECT_LICENSE="${LICENSE_FILE}")
#ADD_DEFINITIONS(-DLICENSE="${LICENSE_FILE}")
ADD_DEFINITIONS(-DPROJECT_README="${README_FILE}")
ADD_DEFINITIONS(-DREADME="${README_FILE}")
ADD_DEFINITIONS(-DPROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}/")
#ADD_DEFINITIONS(-DPROJECT_VARIANT="${${PROJECT_NAME}_Variant}")

# CPack settings
IF(WIN32)
  #SET(CMAKE_INSTALL_PREFIX "C:\\\\")
  SET(CPACK_GENERATOR "NSIS")
  SET(CPACK_SOURCE_GENERATOR "ZIP")
  SET(CPACK_NSIS_DISPLAY_NAME "${PROJECT_NAME}") 
  SET(CPACK_NSIS_PACKAGE_NAME "${PROJECT_NAME}") 
  #SET(CPACK_PACKAGE_INSTALL_DIRECTORY "${PROJECT_NAME}_${${PROJECT_NAME}_Variant}\\\\${PROJECT_VERSION}")
  SET(CPACK_PACKAGE_INSTALL_DIRECTORY "${PROJECT_NAME}\\\\${PROJECT_VERSION}")
  SET(CPACK_NSIS_INSTALL_ROOT "C:\\\\")
  SET(CPACK_NSIS_DEFINES "RequestExecutionLevel user")
  SET(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL "ON")
  #SET(CPACK_NSIS_MUI_ICON "${DATA_DIR}/icons/application/windows/captk.ico")
  SET(CPACK_NSIS_MUI_FINISHPAGE_RUN "${EXE_NAME}.exe")
  #SET(CPACK_NSIS_INSTALLED_ICON_NAME "${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX}")
  #SET(CPACK_NSIS_MUI_FINISHPAGE_RUN "${CPACK_NSIS_INSTALLED_ICON_NAME}")
  SET(CPACK_NSIS_HELP_LINK "https:\\\\\\\\www.med.upenn.edu\\\\sbia\\\\software\\\\")
  SET(CPACK_NSIS_CONTACT "${PROJECT_CONTACT}")
  SET(CPACK_NSIS_URL_INFO_ABOUT "https:\\\\\\\\www.med.upenn.edu\\\\sbia\\\\software\\\\")
  #SET(CPACK_NSIS_MODIFY_PATH "ON") # desktop shortcut doesn't work, for some reason
  # create extra shortcuts and respective uninstall targets 
  #SET(CPACK_NSIS_CREATE_ICONS_EXTRA "CreateShortCut '\$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\${EXE_NAME}.lnk' '\$INSTDIR\\\\bin\\\\${EXE_NAME}.exe'")
  #SET(CPACK_NSIS_CREATE_ICONS "CreateShortCut '$DESKTOP\\\\${EXE_NAME}.lnk' '$INSTDIR\\\\bin\\\\${EXE_NAME}.exe'")
  #SET(CPACK_NSIS_CREATE_ICONS_EXTRA "  CreateShortCut '$INSTDIR\\\\${EXE_NAME}.lnk' '$INSTDIR\\\\bin\\\\${EXE_NAME}.exe'")
  #SET(CPACK_NSIS_CREATE_ICONS "CreateShortCut '\$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\README.lnk' '\$INSTDIR\\\\share\\\\doc\\\\index.html'")
  #SET(CPACK_NSIS_DELETE_ICONS_EXTRA "  Delete '$SMPROGRAMS\\\\$START_MENU\\\\${EXE_NAME}.lnk'")
  SET(CPACK_NSIS_DELETE_ICONS "Delete '$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\${EXE_NAME}.lnk'")
  #SET(CPACK_NSIS_DELETE_ICONS "Delete '$DESKTOP\\\\${EXE_NAME}.lnk'")
  #SET(CPACK_NSIS_DELETE_ICONS_EXTRA "Delete '$INSTDIR\\\\${EXE_NAME}.lnk'")
  SET(CPACK_NSIS_DELETE_ICONS "Delete '$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\README.lnk'")
  SET(CPACK_NSIS_INSTALLED_ICON_NAME "${EXE_NAME}") 
  SET(CPACK_SYSTEM_NAME "Win64")
ELSEIF(APPLE)
  SET(CMAKE_INSTALL_PREFIX "/Applications")
  SET(CPACK_GENERATOR "DragNDrop")
  SET(CPACK_DMG_FORMAT "UDBZ")
  SET(CPACK_DMG_VOLUME_NAME "${EXE_NAME}")
  SET(CPACK_SYSTEM_NAME "OSX")
  #SET(CPACK_PACKAGE_INSTALL_DIRECTORY "${PROJECT_NAME}_${${PROJECT_NAME}_Variant}/${PROJECT_VERSION}")
  SET(CPACK_PACKAGE_INSTALL_DIRECTORY "${PROJECT_NAME}/${PROJECT_VERSION}")
  #SET(CPACK_PACKAGING_INSTALL_PREFIX "/usr")
  #SET(CPACK_DMG_DS_STORE "${ICONS_DIR}/DMGDSStore")
  #SET(CPACK_DMG_BACKGROUND_IMAGE "${ICONS_DIR}/DMGBackground.png")
ELSEIF(UNIX)
  #SET(CPACK_PACKAGE_INSTALL_DIRECTORY "${PROJECT_NAME}_${${PROJECT_NAME}_Variant}/${PROJECT_VERSION}")
  SET(CPACK_PACKAGE_INSTALL_DIRECTORY "${PROJECT_NAME}/${PROJECT_VERSION}")
  #SET(CPACK_PACKAGING_INSTALL_PREFIX "/usr")
  SET(CPACK_CMAKE_GENERATOR "Unix Makefiles")
  SET(CPACK_GENERATOR "STGZ")
  SET(CPACK_SOURCE_GENERATOR "TGZ")
  SET(CPACK_SYSTEM_NAME "${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")
  SET(CPACK_SYSTEM_NAME "Linux-i686")
  SET(CPACK_TOPLEVEL_TAG "Linux-i686")
  SET(CPACK_STRIP_FILES "${EXE_NAME}")
ENDIF()

# Common
SET(CPACK_SOURCE_STRIP_FILES TRUE )
SET(CPACK_PACKAGE_EXECUTABLES "${EXE_NAME}" "${EXE_NAME}")
SET(CPACK_CREATE_DESKTOP_LINKS "${EXE_NAME}" ) 
SET(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}_${PROJECT_VERSION}")
SET(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
SET(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
SET(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
SET(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
SET(CPACK_PACKAGE_VENDOR "${PROJECT_VENDOR}")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "CaPTk is a collection of algorithms from CBICA")
SET(CPACK_SOURCE_IGNORE_FILES "\\\\.svn/;^${PROJECT_SOURCE_DIR}/doc/")
SET(CPACK_SOURCE_PACKAGE_FILE_NAME "captk-${SW_VER}-source" CACHE INTERNAL "tarball basename")
#SET(CPACK_RESOURCE_FILE_README "${README_FILE}")
#SET(CPACK_RESOURCE_FILE_LICENSE "${LICENSE_FILE}") 
SET(CPACK_COMPONENT_LIBRARIES_INSTALL_TYPES Developer Full)
SET(CPACK_COMPONENT_HEADERS_INSTALL_TYPES Developer Full)
SET(CPACK_COMPONENT_APPLICATIONS_INSTALL_TYPES Full)
SET(CPACK_ALL_INSTALL_TYPES Full Developer)
set(CPACK_SOURCE_IGNORE_FILES "${CMAKE_SOURCE_DIR}/bin/;{CMAKE_SOURCE_DIR}/build/")

INCLUDE(InstallRequiredSystemLibraries)
#set(CPACK_GENERATOR WIX)
INCLUDE(CPack)
