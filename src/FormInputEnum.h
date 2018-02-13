#pragma once
#ifndef DVFORMSCPP_FORMINPUTENUM_H
#define DVFORMSCPP_FORMINPUTENUM_H

#include "FormFwd.h"             // IWYU pragma: keep
#include "FormInputSimple.h"     // for FormInputSimple
#include <UnorderedIndexedMap.h> // for UnorderedIndexedMap
#include <boost/variant.hpp>     // for variant
#include <cstddef>               // for size_t
#include <functional>            // for less
#include <string>                // for string, allocator

namespace dv {
  namespace forms {
    class FormInputEnum : public FormInputSimple {
    public:
      FormInputEnum();
      ~FormInputEnum() override;
      FieldType getType() const override;
      json generateSchema() const override;
      void fromJSON( const json &j, const dv::json::JSONPath &path ) override;

    protected:
      typedef boost::variant<std::string, std::size_t> keyType;
      dv::json::UnorderedIndexedMap<keyType, std::string> options;
      std::string toKey( const keyType &key ) const;
      std::string defaultValue;
    };
  }
}
#endif // DVFORMSCPP_FORMINPUTENUM_H
