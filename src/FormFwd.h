#pragma once
#ifndef DVFORMSCPP_FORMFWD_H
#define DVFORMSCPP_FORMFWD_H

#ifdef __has_attribute
#if __has_attribute( pure )
#define DVFORMS_PURE __attribute__( ( pure ) )
#else
#define DVFORMS_PURE
#endif
#if __has_attribute( unused )
#define DVFORMS_UNUSED __attribute__( ( unused ) )
#else
#define DVFORMS_UNUSED
#endif
  #if __has_attribute( warn_unused_result )
    #define DVFORMS_WARN_UNUSED __attribute__((warn_unused_result))
  #else
    #define DVFORMS_WARN_UNUSED
  #endif
#else
#define DVFORMS_PURE
#define DVFORMS_UNUSED
#define DVFORMS_WARN_UNUSED
#endif

#include <iosfwd> // for ostream
#include <jsonfwd.h>
#include <memory>
#include <string>

namespace dv {
  namespace forms {
    using json = dv::json::JSON;
    extern const std::string currentSchemaVersion;

    enum class FieldType { TEXT, NUMBER, EMAIL, CURRENCY, STATIC, ENUM, MULTIPLE, REPEATING, DECIMAL };

    std::ostream &operator<<( std::ostream &os, FieldType type );
    std::ostream &operator>>( std::ostream &os, FieldType &type );
    void to_json( json &json, FieldType type, const dv::json::JSONPath &path );

    class FormExpression;
    typedef std::shared_ptr<FormExpression> FormExpressionPtr;
    typedef std::weak_ptr<FormExpression> FormExpressionWeakPtr;
    class FormExpressionWrapper;
    typedef std::shared_ptr<FormExpressionWrapper> FormExpressionWrapperPtr;
    typedef std::weak_ptr<FormExpressionWrapper> FormExpressionWrapperWeakPtr;
    class FormExpressionContainer;
    typedef std::shared_ptr<FormExpressionContainer> FormExpressionContainerPtr;
    class FormExpressionContext;
    class FormGenerator;
    typedef std::shared_ptr<FormGenerator> FormGeneratorPtr;
    typedef std::weak_ptr<FormGenerator> FormGeneratorWeakPtr;
    class FormComponent;
    typedef std::shared_ptr<FormComponent> FormComponentPtr;
    class FormInput;
    typedef std::shared_ptr<FormInput> FormInputPtr;
    class FormInputCurrency;
    typedef std::shared_ptr<FormInputCurrency> FormInputCurrencyPtr;
    class FormInputEnum;
    typedef std::shared_ptr<FormInputEnum> FormInputEnumPtr;
    class FormInputMultiple;
    typedef std::shared_ptr<FormInputMultiple> FormInputMultiplePtr;
    class FormInputNumber;
    typedef std::shared_ptr<FormInputNumber> FormInputNumberPtr;
    class FormInputRepeating;
    typedef std::shared_ptr<FormInputRepeating> FormInputRepeatingPtr;
    class FormInputSimple;
    typedef std::shared_ptr<FormInputSimple> FormInputSimplePtr;
    class FormInputStatic;
    typedef std::shared_ptr<FormInputStatic> FormInputStaticPtr;
    class FormInputText;
    typedef std::shared_ptr<FormInputText> FormInputTextPtr;
    class FormSection;
    typedef std::shared_ptr<FormSection> FormSectionPtr;
  }
}

#endif // DVFORMSCPP_FORMFWD_H
