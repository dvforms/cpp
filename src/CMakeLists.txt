add_library ( dvjson SHARED
              json.h
              JSONParser.cpp JSONParser.h
              JSONPath.cpp JSONPath.h
              JSONValue.cpp JSONValue.h
              jsonfwd.h
              JSONSerialiser.h
              JSONDiffListener.cpp JSONDiffListener.h
              JSONConverters.h
              JSONException.cpp JSONException.h
              JSONErrorCollector.cpp JSONErrorCollector.h
              JSONContext.h JSONContext.cpp )
target_link_libraries ( dvjson PUBLIC Boost::boost )
install ( TARGETS dvjson EXPORT dvformscpp DESTINATION ${CMAKE_INSTALL_LIBDIR} )
target_include_directories ( dvjson PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                             $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
                             $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include/dvformscpp/> )
iwyu_target ( dvjson )
clang_tidy_target ( dvjson )

add_library ( dvforms SHARED
              FormGenerator.cpp FormGenerator.h
              FormComponent.cpp FormComponent.h
              FormInput.cpp FormInput.h
              FormInputText.cpp FormInputText.h
              FormSection.cpp FormSection.h
              FieldType.h FieldType.cpp
              FormInputSimple.cpp FormInputSimple.h
              FormExpression.cpp FormExpression.h
              FormFieldTypeDeterminer.cpp FormFieldTypeDeterminer.h )
target_link_libraries ( dvforms PUBLIC Boost::boost dvjson )
install ( TARGETS dvforms EXPORT dvformscpp DESTINATION ${CMAKE_INSTALL_LIBDIR} )
target_include_directories ( dvforms PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                             $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
                             $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include/dvformscpp/> )
iwyu_target ( dvforms )
clang_tidy_target ( dvforms )
