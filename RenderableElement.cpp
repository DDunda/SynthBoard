#include "RenderableElement.h"

RenderableRegistry RenderableRegistry::globalRegistry;

void RenderableRegistry::UpdateAllElements(double dT) {
	for (Renderable* e : entries)
		if (e->active)
			e->Update(dT);
}
void RenderableRegistry::RenderAllElements(SDL_Renderer* r) {
	for (auto it = entries.rbegin(); it != entries.rend(); it++) { // Things at the front have to be drawn LAST
		Renderable* e = *it;
		if (e->visible)
			e->Render(r);
	}
}

Renderable::Renderable(RenderableRegistry& registry) : parent(registry) {
	parent.Register(this);
}
Renderable::~Renderable() {
	parent.Unregister(this);
}