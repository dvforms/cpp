add_library ( dvforms SHARED DVFormGenerator.cpp DVFormGenerator.h FormComponent.cpp FormComponent.h FormInput.cpp FormInput.h FormComponentContainer.cpp FormComponentContainer.h FormInputText.cpp FormInputText.h )
install ( TARGETS dvforms EXPORT dvformscpp DESTINATION ${CMAKE_INSTALL_LIBDIR} )
target_include_directories ( dvforms PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                             $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
                             $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include/dvformscpp/> )
iwyu_target ( dvforms )
