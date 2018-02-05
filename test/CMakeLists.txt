add_executable ( testforms testmain.cpp DVFormGeneratorTest.cpp JSONPathTest.cpp JSONTest.cpp )
target_link_libraries ( testforms GTest GMock dvforms )

add_test ( NAME testforms COMMAND ${CMAKE_SOURCE_DIR}/scripts/testdriver.sh $<TARGET_FILE:testforms> WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} )
set_property ( TEST testforms PROPERTY ENVIRONMENT "NAME=testforms" )
set_property ( TEST testforms PROPERTY SKIP_RETURN_CODE 10 )
set_property ( TEST testforms PROPERTY TIMEOUT 600 )

if ( VALGRIND_PATH )
  add_test ( NAME testforms_valgrind COMMAND ${CMAKE_SOURCE_DIR}/scripts/testdriver.sh $<TARGET_FILE:testforms> )

  set_property ( TEST testforms_valgrind PROPERTY SKIP_RETURN_CODE 10 )
  set_property ( TEST testforms_valgrind PROPERTY TIMEOUT 600 )
  if ( VALGRIND_XML )
    set ( VALGRIND_XML_OPTS "--xml=yes --xml-file=testforms-valgrind.xml" )
  endif ()
  set_property ( TEST testforms_valgrind PROPERTY ENVIRONMENT "NAME=testforms"
                 "VALGRIND_OPTS=${VALGRIND_ARGS} ${VALGRIND_XML_OPTS}"
                 "VALGRIND=${VALGRIND_PATH}"
                 )
endif ()
iwyu_target ( testforms )
