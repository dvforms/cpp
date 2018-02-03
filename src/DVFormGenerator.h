#pragma once
#ifndef DVMON_DVFORMGENERATOR_H
#define DVMON_DVFORMGENERATOR_H

#include "FormComponentContainer.h"
#include <string>

namespace dv {
  namespace forms {
    class DVFormGenerator : public FormComponentContainer {
    public:
      DVFormGenerator();
      virtual ~DVFormGenerator();

      virtual std::string generateSchema() const;
    };
  }
}

#endif //DVMON_DVFORMGENERATOR_H
