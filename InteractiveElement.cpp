#include "InteractiveElement.h"

std::vector<InteractiveElement*> InteractiveElement::interactiveElements = std::vector<InteractiveElement*>();
InteractiveElement* InteractiveElement::focusedElement = NULL;