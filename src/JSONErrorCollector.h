#pragma once
#ifndef DVFORMSCPP_JSONERRORCOLLECTOR_H
#define DVFORMSCPP_JSONERRORCOLLECTOR_H

#include "jsonfwd.h"
#include <string>
#include <list>

namespace dv {
  namespace json {
    class JSONErrorCollector {
     public:
      virtual ~JSONErrorCollector();
      virtual void error( const JSONPath &path, const std::string &message ) = 0;
    };

    class JSONErrorCollectorThrow : public JSONErrorCollector {
     public:
      ~JSONErrorCollectorThrow() override;
      void error( const JSONPath &path, const std::string &message ) override;
    };

    class JSONErrorCollectorImpl : public JSONErrorCollector {
     public:
      ~JSONErrorCollectorImpl() override;
      void error( const JSONPath &path, const std::string &message ) override;

     protected:
      std::list<std::string> messages;
    };
  }
}

#endif //DVFORMSCPP_JSONERRORCOLLECTOR_H
