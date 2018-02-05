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
      JSON &operator=( arrayType );
      JSON &operator=( objectType );
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
      bool operator==( bool ) const;
      bool operator==( Type ) const;
      bool operator!=( Type ) const;
      template<typename T> inline bool operator!=( const T &v ) const;
      template<typename T> inline bool operator==( const T &v ) const;
      template<typename T> inline explicit operator T() const;
      template<typename T> inline T as() const;
      Type type() const;

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

      struct json_compare_function {
       private:
        template<typename BasicJsonType, typename T>
        bool call( const BasicJsonType &j, T &val, PriorityTag<1> ) const noexcept( noexcept( json_compare( j, val ) ) ) {
          return json_compare( j, val );
        }

        template<typename BasicJsonType, typename T>
        bool call( const BasicJsonType &, T &, PriorityTag<0> ) const noexcept {
          static_assert( sizeof( BasicJsonType ) == 0, "could not find json_compare() method in T's namespace" );
          return false;
        }

       public:
        template<typename BasicJsonType, typename T>
        bool
        operator()( const BasicJsonType &j, T &val ) const noexcept( noexcept( std::declval<json_compare_function>().call( j, val, PriorityTag<1> {} ) ) ) {
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
      constexpr const auto &json_compare = detail::static_const<detail::json_compare_function>::value;
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

        template<typename BasicJsonType, typename ValueType>
        static bool compare( BasicJsonType &j, ValueType &&val ) noexcept( noexcept( ::dv::json::json_compare( j, std::forward<ValueType>( val ) ) ) ) {
          return ::dv::json::json_compare( j, std::forward<ValueType>( val ) );
        }
      };

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

    template<typename T> T JSON::as() const {
      return static_cast<T>(*this);
    }

    template<typename T> JSON::operator T() const {
      T v;
      JSONSerialiser<T>::from_json( *this, v );
      return v;
    }

    template<> JSON::operator JSON::intType() const;
    template<> JSON::operator const JSON::stringType &() const;
    template<> JSON::operator JSON::doubleType() const;
    template<> JSON::operator JSON::boolType() const;

    std::ostream &operator<<( std::ostream &os, const JSON &json );

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

    template<typename T>
    inline bool operator==( T &&v, const JSON &j ) {
      return j == v;
    }

    template<typename T>
    inline bool operator!=( T &&v, const JSON &j ) {
      return j != v;
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
      json = static_cast<JSON::intType>(v);
    }

    template<typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
    inline void from_json( const JSON &json, T &v ) {
      v = static_cast<T>(json.as<JSON::intType>());
    }

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

    template<typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
    bool json_compare( const JSON &j, T value ) {
      bool rt = false;

      if ( j.type() == Type::INT ) {
        rt = static_cast<JSON::intType>(j) == value;
      }

      return rt;
    }

    template<typename T, typename std::enable_if<std::is_floating_point<T>::value, int>::type = 0>
    bool json_compare( const JSON &j, T value ) {
      bool rt = false;

      if ( j.type() == Type::DOUBLE ) {
        rt = static_cast<JSON::doubleType>(j) == value;
      }

      return rt;
    }

    template<typename T>
    bool json_compare( const JSON &j, const std::vector<T> &value ) {
      bool rt = true;

      if ( value.size() != j.size() ) {
        rt = false;
      } else {
        auto values = value.begin();
        for ( const auto &item : j.arrayIterator() ) {
          if ( *item != *values ) {
            rt = false;
            break;
          }
          ++values;
        }
      }

      return rt;
    }
  }
}

#endif //DVFORMSCPP_JSON_H
