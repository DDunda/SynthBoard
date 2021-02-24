#pragma once

#ifndef TEXT_FIELD
#define TEXT_FIELD

#include "Input.h"
#include "InteractiveElement.h"
#include "RenderableElement.h"
#include "Module.h"

class TextField : public Module, public Interactive, public Renderable
{
	MakeModuleOutput(TextField)
private:
	int visibleCharacters = 5;
	int dstCharacterSize = 8;
	int characterGap = 2;
	padding pad = {0,0,0,0};
	float anchorX = 0, anchorY = 0;
	SDL_Rect area = {0,0,0,0};
	bool typing = false;
	std::string capturedData = "";

protected:
	void startTyping();
	void stopTyping();
	void PresentState();
	void Update(double);
	void Render(SDL_Renderer*);

public:
	int maxData = -1;
	size_t caret = 0; // The cursor thingy

	SDL_Texture* characters = NULL;
	int srcCharacterSize = 8;

	int flashCycleStart = 0;
	int flashCycle = 500;

	TextFieldOutput<std::string> out_output;

	void recalculateArea();

	void setAnchor(float aX, float aY);
	void setPosition(int x, int y);
	void setPadding(padding pad);
	void setVisibleCharacters(int size);
	void setCharacterSize(int size);
	void setCharacterGap(int size);

	bool InArea(int x, int y) const;

	TextField(ModuleRegistry& mRegistry = ModuleRegistry::globalRegistry, InteractiveRegistry& iRegistry = InteractiveRegistry::globalRegistry, RenderableRegistry& rRegistry = RenderableRegistry::globalRegistry);
};

#endif