#pragma once

#define SDL_MAIN_HANDLED

#include <string>
#include <SDL.h>


class Display
{
public:
    Display();
    ~Display();

    bool Init(const std::string& title, const unsigned int pixelScale);

    void HandleEvents();
    void Update();
    void Render(uint32_t* display);

    inline bool IsRunning() { return running; }

private:
    SDL_Window*    window;
    SDL_Renderer*  renderer;

    SDL_Texture* texture;

    SDL_Event evt;
    bool running{true};



};