#pragma once

#include "SDL3/SDL.h"

class Window
{
public:
	Window();
	~Window();

	void run();
private:

	void draw();
	void update();
	bool init();
	void inputs(SDL_Event& event);

	SDL_Window* m_window;
	SDL_Renderer* m_renderer;
	bool f_running;
};