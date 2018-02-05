#pragma once
#ifndef DVFORMSCPP_JSON_H
#define DVFORMSCPP_JSON_H

#include <boost/variant.hpp>
#include <unordered_map>
#include <vector>
#include <map>
#include <memory>
#include <cstddef>            // for size_t
#include <cstdint>            // for int64_t
#include <iosfwd>             // for ostream, nullptr_t
#include <string>             // for string
#include <type_traits>        // for declval, enable_if, false_type, is_integral, is_same
#include <algorithm>          // for forward

namespace dv {
  namespace json {
    enum class Type {
      OBJECT,
      ARRAY,
      BOOL,
      INT,
      DOUBLE,
      STRING,
      NULLVALUE
    };

    std::ostream &operator<<( std::ostream &os, Type type );

    class JSON;
    class JSONPath;

    class JSONDiffListener {
    public:
      virtual ~JSONDiffListener() = default;
      virtual bool isInterested() = 0;
      virtual void recordDifference( const JSONPath &path, const std::string &diff ) = 0;
    };

    class JSONDiffNullListener : public JSONDiffListener {
    public:
      ~JSONDiffNullListener() override = default;

      bool isInterested() override { return false; }

      void recordDifference( const JSONPath &/*path*/, const std::string &/*compare*/ ) override {}
    };

    class JSONDiffListenerImpl : public JSONDiffListener {
    public:
      ~JSONDiffListenerImpl() override = default;
      bool isInterested() override;
      void recordDifference( const JSONPath &path, const std::string &diff ) override;
      void clear();
      std::string toString() const;
    protected:
      std::map<JSONPath, std::string> differences;
      friend std::ostream &operator<<( std::ostream &os, const JSONDiffListenerImpl &listener );
    };

    std::ostream &operator<<( std::ostream &os, const JSONDiffListenerImpl &listener );

    typedef std::shared_ptr<JSON> JSONPtr;

    class JSON : public std::enable_shared_from_this<JSON> {
    public:
      typedef std::nullptr_t nullType;
      typedef int64_t intType;
      typedef bool boolType;
      typedef double doubleType;
      typedef std::string stringType;
      typedef std::unordered_map<stringType, JSONPtr> objectType;
      typedef std::vector<JSONPtr> arrayType;
      typedef stringType keyType;
      typedef boost::variant<nullType, intType, boolType, doubleType, stringType, objectType, arrayType> valueType;
      typedef size_t indexType;
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
      JSON &operator=( const JSON &other ) = default;
      JSON &operator=( JSON &&other ) = default;
      template<typename T> inline JSON &operator=( const T &v );
      JSON &operator[]( const keyType &key ) __attribute__((pure));
      JSON &operator[]( indexType index ) __attribute__((pure));
      bool operator==( const JSON &other ) const;
      Type type() const;

      bool compare( const JSON &other, JSONDiffListener &listener ) const;

      void dump( std::ostream &os, unsigned int indent = 0 ) const;

    protected:
      valueType value;
      void dump( std::ostream &os, unsigned int indent, unsigned int level ) const;
      void writeEscaped( std::ostream &os, const stringType &v ) const;
      bool compare( const JSON &other, JSONDiffListener &listener, const JSONPath &path ) const;
    };

    template<unsigned N> struct PriorityTag : PriorityTag<N - 1> {};
    template<> struct PriorityTag<0> {};

    namespace detail {
      struct to_json_function {
      private:
        template<typename BasicJsonType, typename T>
        auto call( BasicJsonType &j, T &&val, PriorityTag<1> ) const noexcept( noexcept( to_json( j, std::forward<T>( val ) ) ) )
        -> decltype( to_json( j, std::forward<T>( val ) ), void() ) {
          return to_json( j, std::forward<T>( val ) );
        }

        template<typename BasicJsonType, typename T>
        void call( BasicJsonType &, T &&, PriorityTag<0> ) const noexcept {
          static_assert( sizeof( BasicJsonType ) == 0, "could not find to_json() method in T's namespace" );
        }

      public:
        template<typename BasicJsonType, typename T>
        void operator()( BasicJsonType &j, T &&val ) const noexcept( noexcept( std::declval<to_json_function>().call( j, std::forward<T>( val ),
                                                                                                                      PriorityTag<1> {} ) ) ) {
          return call( j, std::forward<T>( val ), PriorityTag<1> {} );
        }
      };

      struct from_json_function {
      private:
        template<typename BasicJsonType, typename T>
        auto call( const BasicJsonType &j, T &val, PriorityTag<1> ) const noexcept( noexcept( from_json( j, val ) ) )
        -> decltype( from_json( j, val ), void() ) {
          return from_json( j, val );
        }

        template<typename BasicJsonType, typename T>
        void call( const BasicJsonType &, T &, PriorityTag<0> ) const noexcept {
          static_assert( sizeof( BasicJsonType ) == 0, "could not find from_json() method in T's namespace" );
        }

      public:
        template<typename BasicJsonType, typename T>
        void operator()( const BasicJsonType &j, T &val ) const noexcept( noexcept( std::declval<from_json_function>().call( j, val, PriorityTag<1> {} ) ) ) {
          return call( j, val, PriorityTag<1> {} );
        }
      };

      template<typename T>
        struct static_const {
          static constexpr T value{};
        };

      template<typename T>
        constexpr T static_const<T>::value;
    }

    namespace {
      constexpr const auto &to_json = detail::static_const<detail::to_json_function>::value;
      constexpr const auto &from_json = detail::static_const<detail::from_json_function>::value;
    }

    template<typename = void>
      struct JSONSerialiser {
        template<typename BasicJsonType, typename ValueType>
        static void from_json( BasicJsonType &&j, ValueType &val ) noexcept( noexcept( ::dv::json::from_json( std::forward<BasicJsonType>( j ), val ) ) ) {
          ::dv::json::from_json( std::forward<BasicJsonType>( j ), val );
        }

        template<typename BasicJsonType, typename ValueType>
        static void to_json( BasicJsonType &j, ValueType &&val ) noexcept( noexcept( ::dv::json::to_json( j, std::forward<ValueType>( val ) ) ) ) {
          ::dv::json::to_json( j, std::forward<ValueType>( val ) );
        }
      };

    template<typename T> JSON &JSON::operator=( const T &v ) {
      JSONSerialiser<T>::to_json( *this, v );
      return *this;
    }

    template<typename T>
    inline void to_json( JSON &json, const std::map<JSON::stringType, T> &v ) {
      json = nullptr;
      for ( const auto &it : v ) {
        json[it.first] = it.second;
      }
    }

    template<typename T>
    inline void to_json( JSON &json, const std::unordered_map<JSON::stringType, T> &v ) {
      json = nullptr;
      for ( const auto &it : v ) {
        json[it.first] = it.second;
      }
    }

    template<typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
    inline void to_json( JSON &json, T v ) {
      json = static_cast<int64_t>(v);
    }

    std::ostream &operator<<( std::ostream &os, const JSON &json );

    namespace detail {
      class to_json_visitor : public boost::static_visitor<> {
      public:
        explicit to_json_visitor( JSON &njson ) : json( &njson ) {}

        template<class T> void operator()( const std::shared_ptr<T> &value ) {
          if ( value ) {
            *json = *value;
          }
        }

        template<class T> void operator()( const std::weak_ptr<T> &value ) {
          ( *this )( value.lock() );
        }

        template<class T> void operator()( T &value ) {
          *json = value;
        }

      protected:
        JSON *json;
      };
    }

    template<typename... Args> inline void to_json( JSON &json, const boost::variant<Args...> &value ) {
      detail::to_json_visitor visitor( json );
      value.apply_visitor( visitor );
    }
  }
}

#endif //DVFORMSCPP_JSON_H
