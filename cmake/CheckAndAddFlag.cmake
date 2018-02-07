include ( CMakePushCheckState )
function ( checkandaddflag flag addto )
  set ( NAME "accepts_flag_${flag}" )
  string ( MAKE_C_IDENTIFIER ${NAME} NAME )
  string ( REPLACE "__" "_" NAME ${NAME} )
  string ( REPLACE "__" "_" NAME ${NAME} )
  cmake_push_check_state ( RESET )
  set ( CMAKE_REQUIRED_FLAGS "${${addto}} ${flag}" )
  set ( CMAKE_REQUIRED_QUIET true )
  message ( STATUS "Checking if compiler supports ${flag}" )
  check_cxx_source_compiles ( "
  #ifdef __GNUC__
  #warning to make gcc report unknown options
  #endif
                              int main() { return 0; }
                              " "${NAME}" FAIL_REGEX "unknown warning option|is not supported|unrecognized command line option" )
  if ( ${NAME} )
    message ( STATUS "Checking if compiler supports ${flag} - success" )
  else ()
    message ( STATUS "Checking if compiler supports ${flag} - failed" )
  endif ()
  cmake_pop_check_state ()
  if ( ${NAME} )
    set ( ${addto} "${${addto}} ${flag}" PARENT_SCOPE )
    list ( APPEND "${addto}_LIST" ${flag} )
    set ( "${addto}_LIST" ${${addto}_LIST} PARENT_SCOPE )
  endif ()
endfunction ()
