add_library ( dvforms SHARED
              FormGenerator.cpp FormGenerator.h
              FormComponent.cpp FormComponent.h
              FormInput.cpp FormInput.h
              FormInputText.cpp FormInputText.h
              FormSection.cpp FormSection.h
              FieldType.cpp
              FormInputSimple.cpp FormInputSimple.h
              FormExpression.cpp FormExpression.h
              FormFieldTypeDeterminer.cpp FormFieldTypeDeterminer.h
              FormInputStatic.cpp FormInputStatic.h
              FormInputEnum.cpp FormInputEnum.h
              FormInputCurrency.cpp FormInputCurrency.h
              FormInputNumber.cpp FormInputNumber.h
              FormInputMultiple.cpp FormInputMultiple.h
              FormInputRepeating.cpp FormInputRepeating.h FormFwd.h Forms.h )
target_link_libraries ( dvforms PUBLIC Boost::boost dvjson )
set_target_properties ( dvforms PROPERTIES VERSION ${PROJECT_VERSION} )
get_target_property ( HEADERS dvforms SOURCES )
list ( FILTER HEADERS INCLUDE REGEX ".h$" )
set_target_properties ( dvforms PROPERTIES PUBLIC_HEADER "${HEADERS}" )
install ( TARGETS dvforms EXPORT dvformscpp
          LIBRARY COMPONENT lib DESTINATION ${CMAKE_INSTALL_LIBDIR} NAMELINK_SKIP
          PUBLIC_HEADER COMPONENT Devel DESTINATION include/dvformscpp/ )
install ( TARGETS dvforms EXPORT dvformscpp LIBRARY NAMELINK_ONLY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Devel )
target_include_directories ( dvforms PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                             $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
                             $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include/dvformscpp/> )
iwyu_target ( dvforms )
clang_tidy_target ( dvforms )
