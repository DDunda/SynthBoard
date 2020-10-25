#include "RenderableElement.h"
std::vector<RenderableElement*> RenderableElement::elements = std::vector<RenderableElement*>();

RenderableElement::RenderableElement() {
	elements.push_back(this);
}

RenderableElement::~RenderableElement() {
	auto iter = std::find(elements.begin(), elements.end(), this);
	if (iter != elements.end())
		elements.erase(iter);
}

void RenderableElement::UpdateAllElements() {
	for (RenderableElement* e : elements)
		if (e->active)
			e->update();
}

void RenderableElement::RenderAllElements(SDL_Renderer* r) {
	for (RenderableElement* e : elements)
		if (e->visible)
			e->render(r);
}