#pragma once

#ifndef INTERACTIVE_ELEMENTS
#define INTERACTIVE_ELEMENTS

#include "Input.h"
#include "RenderableElement.h"

class InteractiveElement : public RenderableElement
{
private:
	// All the elements that can be interacted with the mouse, at the very least
	// The order of elements in this list determines the drawing order
	static std::vector<InteractiveElement*> interactiveElements;
	// The element the mouse is on
	static InteractiveElement* focusedElement;
	
public:
	// Visible
	bool active = true;

	InteractiveElement();
	~InteractiveElement();

	// Called when the element gains focus
	virtual void focus() {};
	// Called when the element loses focus
	virtual void unfocus() {};

	// Checks if point is on the element
	virtual bool inArea(int x, int y) = 0;
	// Recalculates what element the mouse is on - to be called each frame
	static void UpdateElementFocus();
	// An element comes into "focus" when the mouse is in this area
	bool inFocus();
};

#endif