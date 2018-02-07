#pragma once
#ifndef DVFORMSCPP_JSONVALUE_H
#define DVFORMSCPP_JSONVALUE_H

#include <stddef.h>               // for size_t
#include <boost/variant.hpp>      // for static_visitor, type_info
#include <boost/variant/get.hpp>  // for get
#include <iosfwd>                 // for ostream, nullptr_t
#include <memory>                 // for enable_shared_from_this
#include <stdexcept>              // for runtime_error
#include <type_traits>            // for enable_if, is_same, is_convertible, is_fundamental
#include <unordered_map>          // for operator!=, unordered_map, _Node_const_iterator, _Node_iterator_base, unordered_map<>::const_iterator
#include <utility>                // for forward, pair
#include <vector>                 // for vector
#include "JSONSerialiser.h"       // for JSONConstructor, JSONSerialiser, PriorityTag
#include "jsonfwd.h"              // for JSONTypes::objectType, JSONTypes::arrayType, JSONPtr, Type, JSONTypes::keyType, JSONTypes::stringType, JSONTypes::v...

namespace dv {
  namespace json {
    class JSONDiffListener;
    class JSONPath;
    class JSON : public std::enable_shared_from_this<JSON>, public JSONTypes {
     public:
      JSON() = default;
      JSON( const JSON &other ) = default;
      JSON( JSON &&other ) = default;
      ~JSON() = default;

      JSON &operator=( nullType );
      JSON &operator=( boolType v );
      JSON &operator=( const stringType &v );
      JSON &operator=( const char *v );
      JSON &operator=( intType );
      JSON &operator=( doubleType );
      JSON &operator=( const arrayType & );
      JSON &operator=( const objectType & );
      JSON &operator=( Type );
      JSON &operator=( const JSON &other ) = default;
      JSON &operator=( JSON &&other ) = default;
      template<typename T> inline JSON &operator=( const T &v );
      JSON &operator[]( const keyType &key ) __attribute__((pure));
      JSON &operator[]( indexType index ) __attribute__((pure));
      bool operator==( const JSON &other ) const;
      bool operator==( const char *string ) const;
      bool operator==( const stringType &string ) const;
      bool operator==( std::nullptr_t ) const;
      bool operator!=( std::nullptr_t ) const;
      bool operator==( bool ) const;
      bool operator==( Type ) const;
      bool operator!=( Type ) const;
      template<typename T> inline bool operator!=( const T &v ) const;
      template<typename T> inline bool operator==( const T &v ) const;
      template<typename T> inline explicit operator T() const;
      template<typename T> inline typename JSONConstructor<T>::constructType as() const;
      Type type() const;
      inline JSONPtr sub( const keyType &key ) const;

      bool compare( const JSON &other, JSONDiffListener &listener ) const;

      void dump( std::ostream &os, unsigned int indent = 0 ) const;

      inline JSONPtr emplaceBack( const JSONPtr &json );
      inline size_t size() const;
      class ArrayIterator {
       public:
        explicit ArrayIterator( const arrayType *nArray ) : array( nArray ) {}

        typedef arrayType::iterator iterator;
        typedef arrayType::const_iterator const_iterator;

        inline const_iterator begin() const { return array->begin(); }

        inline const_iterator end() const { return array->end(); }

       private:
        const arrayType *array;
      };
      inline const ArrayIterator arrayIterator() const;

      class ObjectIterator {
       public:
        explicit ObjectIterator( const objectType *nObject ) : object( nObject ) {}

        typedef objectType::iterator iterator;
        typedef objectType::const_iterator const_iterator;

        inline const_iterator begin() const { return object->begin(); }

        inline const_iterator end() const { return object->end(); }

       private:
        const objectType *object;
      };
      inline const ObjectIterator objectIterator() const;
     protected:
      valueType value;
      void dump( std::ostream &os, unsigned int indent, unsigned int level ) const;
      void writeEscaped( std::ostream &os, const stringType &v ) const;
      bool compare( const JSON &other, JSONDiffListener &listener, const JSONPath &path ) const;
    };

    namespace detail {

      template<typename Ret, typename Wanted=Ret> struct get_json_value_visitor : public boost::static_visitor<Ret> {
        explicit get_json_value_visitor( const JSON *pj ) : j( pj ) {}

        template<typename Actual, typename LRet=Ret>
        LRet call( Actual &value, const PriorityTag<3> &, typename std::enable_if<std::is_same<LRet, Actual>::value, int>::type = 0 ) {
          return value;
        }

        template<typename Actual, typename LRet=Ret>
        LRet call( Actual &value, const PriorityTag<2> &, typename std::enable_if<std::is_convertible<LRet, Actual>::value, int>::type = 0 ) {
          return static_cast<LRet>(value);
        }

        template<typename Actual, typename LRet=Ret>
        LRet call( Actual &/*value*/, const PriorityTag<1> &, typename std::enable_if<std::is_fundamental<LRet>::value, int>::type = 0 ) {
          throw std::runtime_error( "Can't convert" );
        }

        template<typename Actual, typename LRet=Ret, typename W=Wanted, typename std::enable_if<!std::is_same<float, W>::value, int>::type = 0>
        LRet call( Actual &/*value*/, const PriorityTag<0> & ) {
          typename JSONConstructor<Wanted>::constructType v = JSONConstructor<Wanted>::construct( static_cast<Wanted *>(nullptr) );
          JSONSerialiser<Wanted>::from_json( *j, v );
          return v;
        }

        template<typename X=Wanted> Ret operator()( X &value ) {
          return call( value, PriorityTag<3>() );
        }

       private:
        const JSON *j;
      };
    }

    template<typename T>
    typename JSONConstructor<T>::constructType JSON::as() const {
      detail::get_json_value_visitor<typename JSONConstructor<T>::constructType, T> visitor( this );
      return value.apply_visitor( visitor );
    }

    template<typename T> JSON::operator T() const {
      detail::get_json_value_visitor<T> visitor( this );
      return value.apply_visitor( visitor );
    }

    inline JSONPtr JSON::emplaceBack( const JSONPtr &json ) {
      auto &array = boost::get<arrayType>( value );
      array.emplace_back( std::forward<const JSONPtr &>( json ) );
      return *array.rbegin();
    }

    inline size_t JSON::size() const {
      if ( type() == Type::ARRAY ) {
        return boost::get<arrayType>( value ).size();
      } else {
        return boost::get<objectType>( value ).size();
      }
    }

    inline const JSON::ArrayIterator JSON::arrayIterator() const {
      return ArrayIterator( &( boost::get<arrayType>( value ) ) );
    }

    inline const JSON::ObjectIterator JSON::objectIterator() const {
      return ObjectIterator( &( boost::get<objectType>( value ) ) );
    }

    JSONPtr JSON::sub( const keyType &key ) const {
      JSONPtr rt;
      if ( value.type() == typeid( objectType ) ) {
        auto &obj = boost::get<objectType>( value );
        auto item = obj.find( key );
        if ( item != obj.end() ) {
          rt = item->second;
        }
      }
      return rt;
    }

    template<typename T> JSON &JSON::operator=( const T &v ) {
      JSONSerialiser<T>::to_json( *this, v );
      return *this;
    }

    template<typename T> bool JSON::operator==( const T &v ) const {
      return JSONSerialiser<T>::compare( *this, v );
    }

    template<typename T> bool JSON::operator!=( const T &v ) const {
      return !JSONSerialiser<T>::compare( *this, v );
    }
  }
}

#endif //DVFORMSCPP_JSONVALUE_H