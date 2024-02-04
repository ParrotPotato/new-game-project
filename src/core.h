#ifndef CORE_H
#define CORE_H

#include <vector>

using std::vector;

namespace core{

    struct RecordedEvent{
        // this is for when we have to stream out event to some structure
    };

    void create_window_instance(const char *window_name, int window_width, int window_height);
    void clear_window();
    void update_window();
    bool window_close_requested();

    void process_input();

    bool key_down(int keycode);
    bool key_pressed(int keycode);
    bool key_up(int keycode);
    bool key_released(int keycode);

    bool mouse_down(int buttoncode);
    bool mouse_pressed(int buttoncode);
    bool mouse_up(int buttoncode);
    bool mouse_released(int buttoncode);

    std::vector<RecordedEvent> stream_window_events();
}

#endif