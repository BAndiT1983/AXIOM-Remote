#include <iostream>
#include <stdint.h>
#include <stdlib.h>
#include <thread>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <GL/gl3w.h>
#include <GL/gl.h>

#include "VirtualUI.h"

#include "UI/Painter.h"
#include "UI/MainMenu.h"

// Periphery
#include "USBCDCTerminalDevice.h"

// extern "C"
// {
// #include "AXIOM_Remote_Prototype_V01.X/globals.h"
// #include "AXIOM_Remote_Prototype_V01.X/menu.h"
// #include "AXIOM_Remote_Prototype_V01.X/page.h"
// #include "AXIOM_Remote_Prototype_V01.X/page_wb.h"
// #include "AXIOM_Remote_Prototype_V01.X/page_wb_help.h"
// #include "AXIOM_Remote_Prototype_V01.X/gfxfont.h"

// //#include "AXIOM_Remote_Prototype_V01.X/fonts/FreeSans9pt7b.h"
// //#include "AXIOM_Remote_Prototype_V01.X/fonts/FreeSans12pt7b.h"
// //#include "AXIOM_Remote_Prototype_V01.X/fonts/FreeSans18pt7b.h"
// //#include "AXIOM_Remote_Prototype_V01.X/fonts/FreeSans24pt7b.h"

// #include "AXIOM_Remote_Prototype_V01.X/oop.h"
// }

void Shutdown(SDL_Window *win, SDL_Renderer *ren)
{
    if (ren != nullptr)
    {
        SDL_DestroyRenderer(ren);
    }

    if (win != nullptr)
    {
        SDL_DestroyWindow(win);
    }

    SDL_Quit();
}

void Initialization(SDL_Window **win, SDL_Renderer **renderer)
{
    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
    }

    *win = SDL_CreateWindow("AXIOM Remote Visualizer", 100, 100, 800, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);
    if (*win == nullptr)
    {
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        Shutdown(nullptr, nullptr);
        exit(1);
    }

    *renderer = SDL_CreateRenderer(*win, -1, 0);
    if (*renderer == nullptr)
    {
        Shutdown(*win, nullptr);
        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        exit(2);
    }

    /*SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
       SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
       SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
       SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);*/

    //    SDL_GLContext glContext = SDL_GL_CreateContext(*win);
}

#define FRAMEBUFFER_WIDTH 320
#define FRAMEBUFFER_HEIGHT 240
uint8_t* framebuffer = new uint8_t[FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT * 3];
uint16_t* frameBuffer = new uint16_t[FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT];

void RenderDisplay(uint8_t *fb, int width, int height)
{
    //     if (current_menu != MENU_NONE) {
    //         draw_menu();
    //     } else if (current_page == PAGE_HOME) {
    //         draw_page();
    //     } else if (current_page == PAGE_WB) {
    //         draw_wb_page();
    //     } else if (current_page == PAGE_WB_HELP) {
    //         draw_wb_help_page();
    //     }

    unsigned int j = 0;
    for (int yIndex = 0; yIndex < height; yIndex++)
    {
        for (int xIndex = 0; xIndex < width; xIndex++)
        {
            uint16_t val = frameBuffer[yIndex * width + xIndex];
            /*if(val > 0)
            {
                std::cout << "PIXEL" << std::endl;
            }*/
            framebuffer[j] = (val >> 11) << 3;
            framebuffer[j + 1] = ((val >> 5) & 0x3F) << 2;
            framebuffer[j + 2] = (val & 0x1F) << 3;
            j += 3; //(xIndex + 2) * yIndex + yIndex;
        }
    }
}

int main()
{
    std::cout << "AXIOM Remote Visualizer" << std::endl;

    // menu1 m1;
    // menu1_init(&m1);
    // menu_base* mb = (menu_base*)&m1;
    // mb->vtable->draw(mb);

    //_FreeSans9pt7b = FreeSans9pt7b;
    //_FreeSans12pt7b = FreeSans12pt7b;
    //_FreeSans18pt7b = FreeSans18pt7b;
    // _FreeSans24pt7b = FreeSans24pt7b;

    SDL_Window *window;
    SDL_Renderer *renderer;
    Initialization(&window, &renderer);

    gl3wInit();

    ImGui::CreateContext();
    const ImGuiIO &io = ImGui::GetIO();
    (void)io;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    SDL_GLContext gl_context = SDL_GL_CreateContext(window);

    int majorVersionGL = 0;
    int minorVersionGL = 0;
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &majorVersionGL);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &minorVersionGL);
    std::cout << "GL Version: " << majorVersionGL << "." << minorVersionGL << std::endl;

    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init("#version 130");

    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);
    void *textureData;
    int pitch = 0;

    SDL_Rect texture_rect;
    texture_rect.x = 400;                //the x coordinate
    texture_rect.y = 120;                // the y coordinate
    texture_rect.w = FRAMEBUFFER_WIDTH;  //the width of the texture
    texture_rect.h = FRAMEBUFFER_HEIGHT; //the height of the texture

    unsigned int knobTextureID = 0;
    SDL_Surface *surface = IMG_Load("images/knob_clean.png");
    glGenTextures(1, &knobTextureID);
    glBindTexture(GL_TEXTURE_2D, knobTextureID);

    int Mode = GL_RGB;
    if (surface->format->BytesPerPixel == 4)
    {
        Mode = GL_RGBA;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, Mode, surface->w, surface->h, 0, Mode, GL_UNSIGNED_BYTE, surface->pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    SDL_SetRenderDrawColor(renderer, 64, 64, 64, 255);

    SDL_GL_MakeCurrent(window, gl_context);

    Painter painter(frameBuffer, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);
    USBCDCTerminalDevice cdcDevice;
    MainMenu mainMenu(&cdcDevice);

    IMenu *currentMenu = &mainMenu;

    bool saved = false;

    bool appIsRunning = true;
    const int frames = 30;
    SDL_Event events;
    while (appIsRunning)
    {
        while (SDL_PollEvent(&events))
        {
            if ((events.type == SDL_WINDOWEVENT && events.window.event == SDL_WINDOWEVENT_CLOSE) || (events.type == SDL_KEYDOWN && events.key.keysym.sym == SDLK_ESCAPE))
            {
                appIsRunning = false;
            }
        }

        SDL_RenderClear(renderer);

        painter.DrawFillRectangle(0, 0, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT, currentMenu->GetBackgroundColor());
        currentMenu->Draw(&painter);
        RenderDisplay(framebuffer, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);

        SDL_LockTexture(texture, nullptr, &textureData, &pitch);
        memcpy(textureData, framebuffer, FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT * 3);
        SDL_UnlockTexture(texture);

        // if(!saved)
        // {
        //     SDL_SaveBMP(surface, "test.bmp");
        //     saved = true;
        // }

        RenderUI(window, reinterpret_cast<ImTextureID>(knobTextureID));
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        SDL_RenderCopy(renderer, texture, nullptr, &texture_rect);
        SDL_RenderPresent(renderer); //updates the renderer

        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / frames));
    }

    Shutdown(window, renderer);
    return 0;
}
