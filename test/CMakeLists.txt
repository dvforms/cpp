add_executable ( testforms testmain.cpp DVFormGeneratorTest.cpp )
target_link_libraries ( testforms GTest GMock dvforms )

add_test ( NAME testforms COMMAND testforms WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} )
