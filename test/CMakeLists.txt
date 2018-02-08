add_executable ( testjson
                 testmain.cpp
                 JSONPathTest.cpp
                 JSONTest.cpp
                 JSONParserTest.cpp )
target_link_libraries ( testjson GTest GMock dvjson )

add_test ( NAME testjson COMMAND ${CMAKE_SOURCE_DIR}/scripts/testdriver.sh $<TARGET_FILE:testjson> WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} )
set_property ( TEST testjson PROPERTY ENVIRONMENT "NAME=testjson" )
set_property ( TEST testjson PROPERTY SKIP_RETURN_CODE 10 )
set_property ( TEST testjson PROPERTY TIMEOUT 600 )

if ( VALGRIND_PATH )
  add_test ( NAME testjson_valgrind COMMAND ${CMAKE_SOURCE_DIR}/scripts/testdriver.sh $<TARGET_FILE:testjson> )

  set_property ( TEST testjson_valgrind PROPERTY SKIP_RETURN_CODE 10 )
  set_property ( TEST testjson_valgrind PROPERTY TIMEOUT 600 )
  if ( VALGRIND_XML )
    set ( VALGRIND_XML_OPTS "--xml=yes --xml-file=testjson-valgrind.xml" )
  endif ()
  set_property ( TEST testjson_valgrind PROPERTY ENVIRONMENT "NAME=testjson"
                 "VALGRIND_OPTS=${VALGRIND_ARGS} ${VALGRIND_XML_OPTS}"
                 "VALGRIND=${VALGRIND_PATH}"
                 )
endif ()
iwyu_target ( testjson )

add_executable ( testforms
                 testmain.cpp
                 FormGeneratorTest.cpp
                 example.json
                 )
target_link_libraries ( testforms GTest GMock dvforms )
target_compile_definitions ( testforms PRIVATE -DSOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

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
