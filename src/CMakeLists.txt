add_library ( dvforms SHARED FormGenerator.cpp FormGenerator.h FormComponent.cpp FormComponent.h FormInput.cpp FormInput.h FormInputText.cpp FormInputText.h FormSection.cpp FormSection.h FieldType.h FormInputSimple.cpp FormInputSimple.h FieldType.cpp variantjson.h FormExpression.cpp FormExpression.h json.h json.cpp JSONPath.cpp JSONPath.h )
target_link_libraries ( dvforms PUBLIC Boost::boost )
install ( TARGETS dvforms EXPORT dvformscpp DESTINATION ${CMAKE_INSTALL_LIBDIR} )
target_include_directories ( dvforms PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                             $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
                             $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include/dvformscpp/> )
iwyu_target ( dvforms )
clang_tidy_target ( dvforms )
