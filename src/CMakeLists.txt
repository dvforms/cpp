add_library ( dvforms SHARED DVFormGenerator.cpp DVFormGenerator.h )
install ( TARGETS dvforms EXPORT dvformscpp DESTINATION ${CMAKE_INSTALL_LIBDIR} )

