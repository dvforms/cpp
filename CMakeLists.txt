list ( APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake" )
cmake_minimum_required ( VERSION 3.6 )

find_program ( CLANG clang HINTS /opt/local/bin )
find_program ( CLANGPP clang++ HINTS /opt/local/bin )

if ( CLANG AND CLANGPP )
  if ( NOT CMAKE_C_COMPILER )
    set ( CMAKE_C_COMPILER ${CLANG} )
  endif ()
  if ( NOT CMAKE_CXX_COMPILER )
    set ( CMAKE_CXX_COMPILER ${CLANGPP} )
  endif ()
endif ()

project ( dvformscpp VERSION 1.0.0 )
set ( CMAKE_CXX_STANDARD 11 )
set ( CMAKE_CXX_STANDARD_REQUIRED ON )
if ( NOT ${CMAKE_VERSION} VERSION_LESS 3.9 )
  cmake_policy ( SET CMP0068 NEW )
endif ()

include ( FeatureSummary )
include ( GNUInstallDirs )
include ( CheckCXXCompilerFlag )
include ( GenerateExportHeader )
include ( GTest )
include ( CTest )
find_package ( valgrind )

add_subdirectory ( src )
if ( BUILD_TESTING )
  add_subdirectory ( test )

  add_custom_target ( installtest
                      COMMAND ${CMAKE_COMMAND} -E remove_directory installtest
                      COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/install_manifest*
                      COMMAND ${CMAKE_COMMAND} -E env DESTDIR=installtest ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target install
                      )
endif ()

include ( CPack )

include ( CMakePackageConfigHelpers )
write_basic_package_version_file ( "${CMAKE_CURRENT_BINARY_DIR}/dvformscppVersion.cmake"
                                   VERSION ${PROJECT_VERSION} COMPATIBILITY AnyNewerVersion )
export ( EXPORT dvformscpp FILE "${CMAKE_CURRENT_BINARY_DIR}/dvformscppConfig.cmake" )
install ( EXPORT dvformscpp
          FILE dvformscppConfig.cmake
          DESTINATION lib/cmake/dvforms
          )
install ( FILES "${CMAKE_CURRENT_BINARY_DIR}/dvformscppVersion.cmake"
          DESTINATION lib/cmake/dvforms
          COMPONENT Devel
          )
feature_summary ( WHAT ALL )
export ( PACKAGE dvformscpp )
