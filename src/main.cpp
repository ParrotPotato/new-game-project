#include <cstdio>
#include <cstdlib>

#include <string>

#include <SDL2/SDL.h>
#include <GL/glew.h>

#include "core.h"


int main(){

    printf("Starting ....\n");
    
    core::create_window_instance("main_window", 400, 300);

    while(core::window_close_requested() == false){
        core::clear_window();
        core::process_input();

        
        core::update_window();
    }

    printf("Exiting  ....\n");

    return 0;
}