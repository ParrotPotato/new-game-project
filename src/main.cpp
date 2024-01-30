#include <cstdio>
#include <cstdlib>

#include <string>

#include <SDL2/SDL.h>
#include <GL/glew.h>

using std::string;

SDL_Window * initialise_window(string window_name) {
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window * window_handle = SDL_CreateWindow(window_name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 100, 100, SDL_WINDOW_OPENGL);
    if (window_handle == nullptr){
        printf("Error occurred while creating window handle");
        exit(-1);
    }

    SDL_GLContext window_context = SDL_GL_CreateContext(window_handle);
    if (window_context == nullptr){
        printf("Error occurred while generating window context");
        exit(-1);
    }

    if (glewInit() != GLEW_OK) {
        printf("Error occurred while initializing glew");
        exit(-1);
    }

    glClearColor(0.3f, 0.5f, 0.8f, 1.0f);
    glClearDepth(1.0f);

    return window_handle;
}


int main(){
    printf("Starting .... ");

    SDL_Window * window = initialise_window("WindowHandle");

    bool window_quit = false;

    while(window_quit == false){
        // clearing window
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // processing input
        SDL_Event event_buffer = {};
        while(SDL_PollEvent(&event_buffer)){
            switch(event_buffer.type){
                case SDL_QUIT: 
                printf("Window closing");
                window_quit = true;
                break;
                default:
                break;
            }
        }

        // updating game state


        // rendering
        SDL_GL_SwapWindow(window);
    }

    SDL_Quit();

    printf("Exiting ... ");

    return 0;
}