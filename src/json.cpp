#include "json.h"
#include "JSONPath.h"

using namespace dv::json;

static const std::map<const std::type_info *, Type> typeMap( []() {
    std::map<const std::type_info *, Type> rt;
    rt.emplace( &typeid( JSON::intType ), Type::INT );
    rt.emplace( &typeid( JSON::boolType ), Type::BOOL );
    rt.emplace( &typeid( JSON::doubleType ), Type::DOUBLE );
    rt.emplace( &typeid( JSON::stringType ), Type::STRING );
    rt.emplace( &typeid( JSON::objectType ), Type::OBJECT );
    rt.emplace( &typeid( JSON::arrayType ), Type::ARRAY );
    rt.emplace( &typeid( JSON::nullType ), Type::NULLVALUE );
    return rt;
}() );

std::ostream &dv::json::operator<<( std::ostream &os, const JSON &json ) {
  json.dump( os, 2 );
  return os;
}

std::ostream &dv::json::operator<<( std::ostream &os, Type type ) {
  switch ( type ) {
    case Type::INT:
      os << "int";
      break;
    case Type::BOOL:
      os << "bool";
      break;
    case Type::DOUBLE:
      os << "double";
      break;
    case Type::STRING:
      os << "string";
      break;
    case Type::OBJECT:
      os << "object";
      break;
    case Type::ARRAY:
      os << "array";
      break;
    case Type::NULLVALUE:
      os << "null";
      break;
  }
  return os;
}

std::ostream &dv::json::operator<<( std::ostream &os, const JSONDiffListenerImpl &listener ) {
  for ( const auto &item : listener.differences ) {
    os << item.first << ": " << item.second << std::endl;
  }
  return os;
}

JSON &JSON::operator[]( const JSON::keyType &key ) {
  if ( value.type() != typeid( objectType ) ) {
    value = objectType();
  }
  return boost::get<objectType>( value )[key];
}

JSON &JSON::operator[]( JSON::indexType index ) {
  if ( value.type() != typeid( arrayType ) ) {
    value = arrayType();
  }
  return boost::get<arrayType>( value )[index];
}

bool JSON::compare( const JSON &other, JSONDiffListener &listener ) const {
  JSONPath path;
  return compare( other, listener, path );
}

bool JSON::compare( const JSON &other, JSONDiffListener &listener, const JSONPath &path ) const {
  bool rt = false;
  auto t1 = type();
  auto t2 = other.type();
  if ( t1 == t2 ) {
    switch ( t1 ) {
      case Type::STRING:
      case Type::DOUBLE:
      case Type::BOOL:
      case Type::INT:
        rt = value == other.value;
        if ( !rt && listener.isInterested() ) {
          std::ostringstream msg;
          msg << *this << " != " << other;
          listener.recordDifference( path, msg.str() );
        }
        break;
      case Type::NULLVALUE:
        rt = true;
        break;
      case Type::OBJECT: {
        std::set<stringType> keys1, keys2;
        auto obj1 = boost::get<objectType>( value );
        auto obj2 = boost::get<objectType>( other.value );

        for ( const auto &item : obj1 ) {
          keys1.insert( item.first );
        }
        for ( const auto &item : obj2 ) {
          keys2.insert( item.first );
        }

        if ( !listener.isInterested() ) {
          if ( keys1 != keys2 ) {
            break;
          }
        }
        rt = true;
        auto it1 = keys1.begin(), it2 = keys2.begin();
        while ( it1 != keys1.end() && it2 != keys2.end() ) {
          if ( *it1 == *it2 ) {
            if ( !obj1.find( *it1 )->second.compare( obj2.find( *it2 )->second, listener, path / *it1 ) ) {
              rt = false;
              if ( !listener.isInterested() ) {
                it1 = keys1.end();
                it2 = keys2.end();
                break;
              }
            }
            ++it1;
            ++it2;
          } else if ( *it1 < *it2 ) {
            rt = false;
            auto x = it1;
            while ( x != keys1.end() && *x < *it2 ) { ++x; }
            if ( x == keys1.end() ) {
              listener.recordDifference( path / *it2, "Missing from left" );
              ++it2;
            } else {
              while ( it1 != x ) {
                listener.recordDifference( path / *it1, "Missing from right" );
                ++it1;
              }
            }
          } else {
            rt = false;
            auto x = it2;
            while ( x != keys2.end() && *x < *it1 ) { ++x; }
            if ( x == keys2.end() ) {
              listener.recordDifference( path / *it1, "Missing from right" );
              ++it1;
            } else {
              while ( it2 != x ) {
                listener.recordDifference( path / *it2, "Missing from left" );
                ++it2;
              }
            }
          }
        }
        while ( it1 != keys1.end() ) {
          rt = false;
          listener.recordDifference( path / *it1, "Missing from right" );
          ++it1;
        }
        while ( it2 != keys2.end() ) {
          rt = false;
          listener.recordDifference( path / *it2, "Missing from left" );
          ++it2;
        }
        break;
      }
      case Type::ARRAY:
        break;
    }
  } else if ( listener.isInterested() ) {
    std::ostringstream msg;
    msg << "Type mismatch " << t1 << " != " << t2;
    listener.recordDifference( path, msg.str() );
  }
  return rt;
}

bool JSON::operator==( const JSON &other ) const {
  JSONDiffNullListener listener;
  return compare( other, listener );
}

Type JSON::type() const {
  auto it = typeMap.find( &value.type() );
  Type rt = Type::NULLVALUE;
  if ( it != typeMap.end() ) {
    rt = it->second;
  }
  return rt;
}

JSON &JSON::operator=( std::nullptr_t ) {
  value = nullptr;
  return *this;
}

JSON &JSON::operator=( bool v ) {
  value = v;
  return *this;
}

JSON &JSON::operator=( const JSON::stringType &v ) {
  value = v;
  return *this;
}

JSON &JSON::operator=( const char *v ) {
  value = std::string( v );
  return *this;
}

JSON &JSON::operator=( JSON::intType v ) {
  value = v;
  return *this;
}

JSON &JSON::operator=( JSON::doubleType v ) {
  value = v;
  return *this;
}

void JSON::writeEscaped( std::ostream &os, const JSON::stringType &v ) const {
  os << v; /// @todo finish
}

void JSON::dump( std::ostream &os, unsigned int indent ) const {
  dump( os, indent, 1 );
}

static std::string doIndent( unsigned int indent, unsigned int level ) __attribute__((pure));

static std::string doIndent( unsigned int indent, unsigned int level ) {
  std::string rt;

  if ( indent * level > 0 ) {
    rt = "\n";
    for ( unsigned int i = 0; i < ( level - 1 ) * indent; i++ ) {
      rt += " ";
    }
  }

  return rt;
}

void JSON::dump( std::ostream &os, unsigned int indent, unsigned int level ) const {
  level++;
  switch ( type() ) {
    case Type::NULLVALUE:
      os << "null";
      break;
    case Type::OBJECT: {
      const auto &obj = boost::get<objectType>( value );
      os << "{" << doIndent( indent, level );
      bool first = true;
      for ( const auto &it : obj ) {
        if ( !first ) { os << "," << doIndent( indent, level ); }
        os << "\"";
        writeEscaped( os, it.first );
        os << "\":";
        if ( indent > 0 ) { os << " "; }
        it.second.dump( os, indent, level );
        first = false;
      }
      os << doIndent( indent, level - 1 ) << "}";
      break;
    }
    case Type::STRING:
      os << "\"";
      writeEscaped( os, boost::get<stringType>( value ) );
      os << "\"";
      break;
    case Type::BOOL:
      os << ( boost::get<boolType>( value ) ? "true" : "false" );
      break;
    case Type::INT:
      os << boost::get<intType>( value );
      break;
    case Type::DOUBLE:
      os << boost::get<doubleType>( value );
      break;
    case Type::ARRAY: {
      os << "[";
      const auto &list = boost::get<arrayType>( value );
      bool first = true;
      for ( const auto &it : list ) {
        if ( !first ) { os << ","; }
        first = false;
        it.dump( os, indent, level );
      }
      os << "]";
      break;
    }
  }
}

bool JSONDiffListenerImpl::isInterested() {
  return true;
}

void JSONDiffListenerImpl::recordDifference( const JSONPath &path, const std::string &diff ) {
  differences[path] = diff;
}

void JSONDiffListenerImpl::clear() {
  differences.clear();
}

std::string JSONDiffListenerImpl::toString() const {
  std::ostringstream str;
  str << *this;
  return str.str();
}