#include "Display.h"

#include <iostream>

#define DISPLAY_WIDTH  64
#define DISPLAY_HEIGHT 32

static unsigned int pixScale = 1;

Display::Display()
    : window(NULL),
    renderer(NULL)
{
}

Display::~Display()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
}

bool Display::Init(const std::string& title,const unsigned int pixelScale)
{
    pixScale = pixelScale;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Could not initialise SDL!" << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow(title.c_str(), 
                                SDL_WINDOWPOS_UNDEFINED, 
                                SDL_WINDOWPOS_UNDEFINED, 
                                pixScale * DISPLAY_WIDTH, 
                                pixScale * DISPLAY_HEIGHT,
                                SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS
                              );
    if (window == NULL)
    {
        std::cout << "Could not create SDL window!" << SDL_GetError() 
                                                    << std::endl;
        return false;                                                        
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        std::cout << "Could not create SDL renderer!" << SDL_GetError()
                                                      << std::endl;
        return false;
    }

    texture = SDL_CreateTexture(renderer,
                                  SDL_PIXELFORMAT_RGBA8888,
                                  SDL_TEXTUREACCESS_STREAMING,
                                  pixScale,
                                  pixScale
                                );


    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);



    return true;
}

void Display::HandleEvents()
{
    while (SDL_PollEvent(&evt) != 0)
    {
        switch (evt.type)
        {
        case SDL_KEYUP:
            if (evt.key.keysym.sym == SDLK_ESCAPE)
            {
                running = false;
            }
            break;
        }
    }
}

void Display::Update()
{
    
}

void Display::Render(uint32_t* display)
{
    SDL_Rect rect;
    rect.w = pixScale;
    rect.h = pixScale;

    int pitch = sizeof(display[0]) * DISPLAY_WIDTH;
    SDL_UpdateTexture(texture, nullptr, display, pitch);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
 
    //SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    //for (unsigned int row = 0; row < DISPLAY_HEIGHT; row++)
    //{
    //    for (unsigned int col = 0; col < DISPLAY_WIDTH; col++)
    //    {
    //        uint32_t pixel = display[row * DISPLAY_WIDTH + col];
    //        if (pixel == 1)
    //        {
    //            rect.x = col * pixScale;
    //            rect.y = row * pixScale;
    //            SDL_RenderFillRect(renderer, &rect);
    //        }
    //    }
    //}
    
    SDL_RenderPresent(renderer);
}