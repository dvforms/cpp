add_library ( dvforms SHARED DVFormGenerator.cpp DVFormGenerator.h )
install ( TARGETS dvforms EXPORT dvforms-cpp DESTINATION ${CMAKE_INSTALL_SBINDIR} )

