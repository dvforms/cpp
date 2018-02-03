add_executable ( testforms testmain.cpp DVFormGeneratorTest.cpp )
target_link_libraries ( testforms GTest GMock dvforms )

add_test ( NAME testforms COMMAND ${CMAKE_SOURCE_DIR}/scripts/testdriver.sh $<TARGET_FILE:testforms> WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} )
set_property ( TEST testforms PROPERTY ENVIRONMENT "NAME=testforms" )
set_property ( TEST testforms PROPERTY SKIP_RETURN_CODE 10 )
set_property ( TEST testforms PROPERTY TIMEOUT 600 )
