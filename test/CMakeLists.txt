add_executable ( testforms testmain.cpp )
target_link_libraries ( testforms GTest GMock )

add_test ( testforms testforms )
