#include "InteractiveElement.h"

std::vector<InteractiveElement*> InteractiveElement::interactiveElements = std::vector<InteractiveElement*>();
InteractiveElement* InteractiveElement::focusedElement = NULL;

InteractiveElement::InteractiveElement() {
	interactiveElements.push_back(this);
}
InteractiveElement::~InteractiveElement() {
	interactiveElements.erase(std::find(interactiveElements.begin(), interactiveElements.end(), this));
}

void InteractiveElement::UpdateElementFocus() {
	if (buttonPressed(SDL_BUTTON_LEFT)) {
		InteractiveElement* lastFocus = focusedElement;
		focusedElement = NULL;

		for (auto e : interactiveElements) if (e->inArea(mouseX, mouseY) && e->active) focusedElement = e;

		if (lastFocus != focusedElement) {
			if (lastFocus != NULL)	lastFocus->unfocus();
			if (focusedElement != NULL)	focusedElement->focus();
		}
	}
}

bool InteractiveElement::inFocus() {
	return this == focusedElement;
}