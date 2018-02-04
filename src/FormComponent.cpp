#include "FormComponent.h"

using namespace dv::forms;

FormComponent::FormComponent() = default;

FormComponent::~FormComponent() = default;

FormGeneratorPtr FormComponent::getForm() const {
  return form.lock();
}

void FormComponent::setForm( const FormGeneratorPtr &nForm ) {
  form = nForm;
}
