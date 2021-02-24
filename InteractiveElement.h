#pragma once

#ifndef INTERACTIVE_ELEMENTS
#define INTERACTIVE_ELEMENTS
#include "Batcher.h"

class Interactive;

class InteractiveRegistry : public BatchRegistry<Interactive> {
private:
	Interactive* focusedElement = NULL;
public:
	// Recalculates what element the mouse is on - to be called each frame
	void UpdateElementFocus(int x, int y);
	bool IsFocus(const Interactive*) const;

	static InteractiveRegistry globalRegistry;
};

class Interactive
{
	friend class InteractiveRegistry;
protected:
	// Called when the element gains focus
	virtual void Focus() {};
	// Called when the element loses focus
	virtual void Unfocus() {};

	InteractiveRegistry& parent;

public:
	bool interactive = true;

	Interactive(InteractiveRegistry& registry);
	~Interactive();

	// Checks if point is on the element
	virtual bool InArea(int x, int y) const = 0;
};
#endif