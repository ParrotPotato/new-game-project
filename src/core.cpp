#include "core.h"

#include <SDL2/SDL.h>
#include <GL/glew.h>

#include <vector>


// NOTE(nitesh): I think there is some sort of signed bit issue happening
// try to find if we can do buffer assignment without the if statement
#define CAPSLOCKOFFSET     SDLK_CAPSLOCK
#define PHYSICALKEYCOUNT   128
#define FUNCTIONALKEYCOUNT 258

#define PHYSICALBUTTONCOUNT   5
#define FUNCTIONALBUTTONCOUNT 0

namespace core{

    struct _WindowState{
        SDL_Window * window_handle;
        int width;
        int height;
        bool quit_requested;
    }WindowState = {};

    struct _KeyboardState{
        bool prev[PHYSICALKEYCOUNT + FUNCTIONALKEYCOUNT];
        bool curr[PHYSICALKEYCOUNT + FUNCTIONALKEYCOUNT];
    }KeyboardState = {};

    struct _MouseState{
        bool prev[PHYSICALBUTTONCOUNT + FUNCTIONALBUTTONCOUNT];
        bool curr[PHYSICALBUTTONCOUNT + FUNCTIONALBUTTONCOUNT];
        float posx;
        float posy;
        float relx;
        float rely;
        bool motion;
    }MouseState = {};

    void create_window_instance(const char * window_name, int window_width, int window_height) {
        SDL_Init(SDL_INIT_EVERYTHING);

        SDL_Window * window_handle = SDL_CreateWindow(window_name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_OPENGL);
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

        WindowState.window_handle = window_handle;
        WindowState.width = window_width;
        WindowState.height = window_height;
    }

    void clear_window(){
        // TOOD(nitesh): We can also later make sure that the current framebuffer
        // is the default framebuffer, ensuring that we are clearing the main window
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    }
    void update_window(){
        SDL_GL_SwapWindow(WindowState.window_handle);
    }

    bool window_close_requested(){
        return WindowState.quit_requested;
    }

    void process_input(){

        // Resetting keyboard state
        for(unsigned int i = 0; i < PHYSICALKEYCOUNT + FUNCTIONALKEYCOUNT ; i++){
            KeyboardState.prev[i] = KeyboardState.curr[i];
            KeyboardState.curr[i] = false;
        }

        // Resetting mouse state
        for(unsigned int i = 0; i < PHYSICALBUTTONCOUNT + FUNCTIONALBUTTONCOUNT ; i++){
            MouseState.prev[i] = KeyboardState.curr[i];
            MouseState.curr[i] = false;
        }

        MouseState.relx = 0.0;
        MouseState.rely = 0.0;

        SDL_Event event_buffer = {};
        while(SDL_PollEvent(&event_buffer)) {
            switch(event_buffer.type) {
                case SDL_KEYDOWN:{
                    unsigned long long target_key = event_buffer.key.keysym.sym;
                    if(event_buffer.key.keysym.sym >= CAPSLOCKOFFSET) {
                        target_key -= CAPSLOCKOFFSET;
                    }
                    KeyboardState.curr[target_key] = true;
                }
                break;
                case SDL_KEYUP:{
                    unsigned long long target_key = event_buffer.key.keysym.sym;
                    if(event_buffer.key.keysym.sym >= CAPSLOCKOFFSET) {
                        target_key -= CAPSLOCKOFFSET;
                    }
                    KeyboardState.curr[target_key] = false;
                }
                break;
                case SDL_MOUSEBUTTONDOWN:{
                    MouseState.curr[event_buffer.button.button] = true;
                }
                break;
                case SDL_MOUSEBUTTONUP:{
                    MouseState.curr[event_buffer.button.button] = false;
                }
                break;
                case SDL_MOUSEMOTION:{
                    MouseState.posx = event_buffer.motion.x;
                    MouseState.posy = event_buffer.motion.y;
                    MouseState.relx = event_buffer.motion.xrel;
                    MouseState.rely = event_buffer.motion.yrel;
                    
                    MouseState.motion = true;
                }
                break;
                case SDL_QUIT:{
                    WindowState.quit_requested= true;
                }
                default:{
                }
                break;
            }
        }

    }

    bool key_down(int keycode){ 
        unsigned long long target_keycode = keycode;
        if (keycode > CAPSLOCKOFFSET) {
            target_keycode -= CAPSLOCKOFFSET;
        }
        return KeyboardState.curr[target_keycode];
    }

    bool key_pressed(int keycode){
        unsigned long long target_keycode = keycode;
        if (keycode > CAPSLOCKOFFSET) {
            target_keycode -= CAPSLOCKOFFSET;
        }
        return (KeyboardState.curr[target_keycode] && !KeyboardState.prev[target_keycode]);
    }

    bool key_up(int keycode){
        unsigned long long target_keycode = keycode;
        if (keycode > CAPSLOCKOFFSET) {
            target_keycode -= CAPSLOCKOFFSET;
        }
        return !KeyboardState.curr[target_keycode];
    }

    bool key_released(int keycode){
        unsigned long long target_keycode = keycode;
        if (keycode > CAPSLOCKOFFSET) {
            target_keycode -= CAPSLOCKOFFSET;
        }
        return (!KeyboardState.curr[target_keycode] && KeyboardState.prev[target_keycode]);
    }

    bool mouse_down(int buttoncode){
        return MouseState.curr[buttoncode];
    }
    bool mouse_pressed(int buttoncode){
        return (MouseState.curr[buttoncode] && !MouseState.prev[buttoncode]);
    }
    bool mouse_up(int buttoncode){
        return !MouseState.curr[buttoncode];
    }
    bool mouse_released(int buttoncode){
        return (!MouseState.curr[buttoncode] && MouseState.prev[buttoncode]);
    }

    // TODO(nitesh): This function is defined preemptively, we don't know if this
    // would be required or not TBH 
    std::vector<RecordedEvent> stream_window_events() {
        return {};
    }
}