#pragma once

#ifndef INTERACTIVE_ELEMENTS
#define INTERACTIVE_ELEMENTS

#include "Input.h"
#include "RenderableElement.h"

class InteractiveElement : public RenderableElement
{
protected:
	bool infocus = false;
	static std::vector<InteractiveElement*> interactiveElements;
	static InteractiveElement* focusedElement;
	static std::vector<RenderableElement*> elements;
	
public:
	callback OnLeftPress = NULL;
	callback OnLeftRelease = NULL;
	callback OnRightPress = NULL;
	callback OnRightRelease = NULL;
	callback OnFocus = NULL;
	callback OnUnfocus = NULL;

	bool interactive = true;
	SDL_Rect clickArea = {0,0,0,0};

	InteractiveElement() {
		interactiveElements.push_back(this);
	}
	~InteractiveElement() {
		interactiveElements.erase(std::find(interactiveElements.begin(), interactiveElements.end(), this));
	}

	virtual void focus() {
		TryCall(OnFocus);
		infocus = true;
	};
	virtual void unfocus() {
		TryCall(OnUnfocus);
		infocus = false;
	};

	static void UpdateElementFocus() {
		if (buttonPressed(SDL_BUTTON_LEFT)) {
			InteractiveElement* lastFocus = focusedElement;
			focusedElement = NULL;

			if (lastFocus != NULL && !lastFocus->infocus) lastFocus = NULL;

			for (auto e : interactiveElements)
				if (inBounds(e->clickArea, mouseX, mouseY) && e->interactive)
					focusedElement = e;

			if (lastFocus != focusedElement) {
				if(lastFocus != NULL)
					lastFocus->unfocus();
				if(focusedElement != NULL)
					focusedElement->focus();
			}
		}
	}
};

#endif