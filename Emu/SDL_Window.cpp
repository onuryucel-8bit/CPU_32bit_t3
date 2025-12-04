#include "SDL_Window.h"

Window::Window()
{
	f_running = init();
}

Window::~Window()
{
}

void Window::run()
{
	SDL_Event event;	
	
	while (f_running)
	{
		while (SDL_PollEvent(&event))
		{			
			inputs(event);
			update();
			draw();
		}
	}

	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

void Window::draw()
{
	SDL_SetRenderDrawColor(m_renderer, 100, 100, 100, 255);
	SDL_RenderClear(m_renderer);


	// Present the rendered frame
	SDL_RenderPresent(m_renderer);
}

void Window::update()
{

}

bool Window::init()
{
	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		SDL_Log("SDL could not initialize! SDL error: %s\n", SDL_GetError());
		return false;
	}

	int WINDOW_WIDTH = 800;
	int WINDOW_HEIGHT = 600;
	if (m_window = SDL_CreateWindow("SDL3 Tutorial: Hello SDL3", WINDOW_WIDTH, WINDOW_HEIGHT, 0); m_window == nullptr)
	{
		SDL_Log("Window could not be created! SDL error: %s\n", SDL_GetError());
		return false;
	}

	// Create a renderer
	m_renderer = SDL_CreateRenderer(m_window, nullptr);
	if (m_renderer == nullptr)
	{
		SDL_Log("Renderer could not be created! SDL error: %s\n", SDL_GetError());
		SDL_DestroyWindow(m_window);
		SDL_Quit();
		return false;
	}

	return true;
}

void Window::inputs(SDL_Event& event)
{
	if (event.type == SDL_EVENT_QUIT)
	{
		f_running = false;
	}

	if (event.type == SDL_EVENT_KEY_DOWN)
	{
		if (event.key.key == SDLK_ESCAPE)
		{
			f_running = false;
		}
	}
}
