#include "InteractiveElement.h"

InteractiveRegistry InteractiveRegistry::globalRegistry;

void InteractiveRegistry::UpdateElementFocus(int x, int y) {
	Interactive* newFocus = NULL;

	for (auto e : entries)
		if (e->InArea(x, y) && e->interactive) {
			focusedElement = e;
			break;
		}

	if (newFocus != focusedElement) {
		if (focusedElement != NULL) {
			focusedElement->Unfocus();
		}
		focusedElement = newFocus;
		if (focusedElement != NULL) {
			focusedElement->Focus();
		}
	}
}
bool InteractiveRegistry::IsFocus(const Interactive* element) const {
	return element == focusedElement;
}

Interactive::Interactive(InteractiveRegistry& interactiveRegistry) : parent(interactiveRegistry) {
	parent.Register(this);
}
Interactive::~Interactive() {
	parent.Unregister(this);
}