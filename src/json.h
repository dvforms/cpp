#pragma once
#ifndef DVFORMSCPP_JSON_H
#define DVFORMSCPP_JSON_H

#include <boost/variant.hpp>
#include <boost/variant/get.hpp>
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
  namespace forms {
    class FormGenerator;
  }
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
    typedef std::shared_ptr<JSON> JSONPtr;

    struct JSONTypes {
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
    };

    template<unsigned N> struct PriorityTag : PriorityTag<N - 1> {};
    template<> struct PriorityTag<0> {};

    namespace detail {

      template<typename T, typename V>
        struct variant_has_type;

      template<typename T, typename... Ts>
        struct variant_has_type<T, boost::variant<T, Ts...> > {
          static const bool value = true;
        };

      template<typename T, typename U, typename... Ts>
        struct variant_has_type<T, boost::variant<U, Ts...> > : variant_has_type<T, boost::variant<Ts..., void> > {};

      template<typename T, typename... Ts>
        struct variant_has_type<T, boost::variant<void, Ts...> > {
          static const bool value = false;
        };

      struct to_json_function {
       private:
        template<typename JsonType, typename T>
        auto call( JsonType &j, std::shared_ptr<T> &&val, PriorityTag<2> ) const noexcept( noexcept( to_json( j, std::forward<std::shared_ptr<T>>( val ) ) ) )
        -> decltype( to_json( j, std::forward<std::shared_ptr<T>>( val ) ), void() ) {
          return to_json( j, std::forward<std::shared_ptr<T>>( val ) );
        }

        template<typename JsonType, typename T>
        auto call( JsonType &j, T &&val, PriorityTag<1> ) const noexcept( noexcept( to_json( j, std::forward<T>( val ) ) ) )
        -> decltype( to_json( j, std::forward<T>( val ) ), void() ) {
          return to_json( j, std::forward<T>( val ) );
        }

        template<typename JsonType, typename T>
        void call( JsonType &, T &&, PriorityTag<0> ) const noexcept {
          static_assert( sizeof( JsonType ) == 0, "could not find to_json() method in T's namespace" );
        }

       public:
        template<typename JsonType, typename T>
        void operator()( JsonType &j, T &&val ) const noexcept( noexcept( std::declval<to_json_function>().call( j, std::forward<T>( val ),
                                                                                                                 PriorityTag<1> {} ) ) ) {
          return call( j, std::forward<T>( val ), PriorityTag<2> {} );
        }
      };

      struct from_json_function {
       private:
        template<typename JsonType, typename T>
        auto call( const JsonType &j, std::shared_ptr<T> &val, PriorityTag<3> ) const noexcept( noexcept( from_json( j, *val ) ) )
        -> decltype( from_json( j, *val ), void() ) {
          assert( val );
          return from_json( j, *val );
        }

        template<typename JsonType, typename T>
        auto call( const JsonType &j, T &val, PriorityTag<2> ) const noexcept( noexcept( from_json( j, val ) ) )
        -> decltype( from_json( j, val ), void() ) {
          return from_json( j, val );
        }

        template<typename JsonType, typename T>
        T &call( const JsonType &/*j*/, T &/*val*/, PriorityTag<1>,
                 typename std::enable_if<variant_has_type<typename std::remove_cv<T>::type, JSONTypes::valueType>::value, int>::type = 0 ) const {
          throw std::runtime_error( "Invalid conversion" );
        }

        template<typename JsonType, typename T>
        void call( const JsonType &, T &, PriorityTag<0> ) const noexcept {
          static_assert( sizeof( JsonType ) == 0, "could not find from_json() method in T's namespace" );
        }

       public:
        template<typename JsonType, typename T>
        auto operator()( const JsonType &j, T &val ) const noexcept( noexcept( std::declval<from_json_function>().call( j, val, PriorityTag<3> {} ) ) )
        -> decltype( std::declval<from_json_function>().call( j, val, PriorityTag<3> {} ) ) {
          return call( j, val, PriorityTag<3> {} );
        }
      };

      struct json_compare_function {
       private:
        template<typename JsonType, typename T>
        bool call( const JsonType &j, T &val, PriorityTag<1> ) const noexcept( noexcept( json_compare( j, val ) ) ) {
          return json_compare( j, val );
        }

        template<typename JsonType, typename T>
        bool call( const JsonType &, T &, PriorityTag<0> ) const noexcept {
          static_assert( sizeof( JsonType ) == 0, "could not find json_compare() method in T's namespace" );
          return false;
        }

       public:
        template<typename JsonType, typename T>
        bool
        operator()( const JsonType &j, T &val ) const noexcept( noexcept( std::declval<json_compare_function>().call( j, val, PriorityTag<1> {} ) ) ) {
          return call( j, val, PriorityTag<1> {} );
        }
      };

      struct json_construct_function {
       private:
        template<typename JsonType, typename T>
        auto call( JsonType */*j*/, T &val, PriorityTag<1> ) const noexcept( noexcept( json_construct( val ) ) )
        -> decltype( json_construct( val ) ) {
          return json_construct( val );
        }

        template<typename JsonType, typename T>
        T call( const JsonType *, T &, PriorityTag<0>,
                typename std::enable_if<!std::is_same<std::shared_ptr<dv::forms::FormGenerator>, T>::value, int>::type = 0 ) const noexcept {
          return T();
        }

       public:
        template<typename JsonType, typename T>
        auto
        operator()( JsonType *j, T &val ) const noexcept( noexcept( std::declval<json_construct_function>().call( j, val, PriorityTag<1> {} ) ) )
        -> decltype( std::declval<json_construct_function>().call( j, val, PriorityTag<1> {} ) ) {
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
      constexpr const auto &json_construct = detail::static_const<detail::json_construct_function>::value;
    }

    template<typename T = void>
      struct JSONSerialiser {
        template<typename JsonType, typename ValueType>
        static void from_json( JsonType &&j, ValueType &val ) noexcept( noexcept( ::dv::json::from_json( std::forward<JsonType>( j ), val ) ) ) {
          ::dv::json::from_json( std::forward<JsonType>( j ), val );
        }

        template<typename JsonType, typename ValueType>
        static void to_json( JsonType &j, ValueType &&val ) noexcept( noexcept( ::dv::json::to_json( j, std::forward<ValueType>( val ) ) ) ) {
          ::dv::json::to_json( j, std::forward<ValueType>( val ) );
        }

        template<typename JsonType, typename ValueType>
        static bool compare( JsonType &j, ValueType &&val ) noexcept( noexcept( ::dv::json::json_compare( j, std::forward<ValueType>( val ) ) ) ) {
          return ::dv::json::json_compare( j, std::forward<ValueType>( val ) );
        }

        typedef decltype( ::dv::json::json_construct( std::declval<JSON *>(), std::declval<T &>() ) ) constructType;

        static constructType construct( T *val ) {
          return ::dv::json::json_construct( static_cast<JSON *>(nullptr), *val );
        }
      };

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
      template<typename T> inline typename JSONSerialiser<T>::constructType as() const;
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

    namespace detail {

      template<typename Ret, typename Wanted=Ret> struct get_json_value_visitor : public boost::static_visitor<Ret> {
        explicit get_json_value_visitor( const JSON *pj ) : j( pj ) {}

        template<typename Actual, typename LRet=Ret>
        LRet call( Actual &value, const PriorityTag<3> &, typename std::enable_if<std::is_same<LRet, Actual>::value, int>::type = 0 ) {
          return value;
        }

        template<typename Actual, typename LRet=Ret>
        LRet call( Actual &value, const PriorityTag<2> &, typename std::enable_if<std::is_convertible<Actual, LRet>::value, int>::type = 0 ) {
          return static_cast<Ret>(value);
        }

        template<typename Actual, typename LRet=Ret> LRet call( Actual &/*value*/, const PriorityTag<0> & ) {
          typename JSONSerialiser<Wanted>::constructType v = JSONSerialiser<Wanted>::construct( static_cast<Wanted *>(nullptr) );
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
    typename JSONSerialiser<T>::constructType JSON::as() const {
      detail::get_json_value_visitor<typename JSONSerialiser<T>::constructType, T> visitor( this );
      return value.apply_visitor( visitor );
    }

    template<typename T> JSON::operator T() const {
      detail::get_json_value_visitor<T> visitor( this );
      return value.apply_visitor( visitor );
    }

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

    template<typename T, typename std::enable_if<std::is_base_of<std::enable_shared_from_this<T>, T>::value, int>::type = 0>
    std::shared_ptr<T> json_construct( T & ) {
      return std::make_shared<T>();
    };
  }
}

#endif //DVFORMSCPP_JSON_H
