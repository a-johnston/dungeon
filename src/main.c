#include <5kgl.h>
#include "dungeon.h"

#define TITLE "Dungeon"

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    (void) window;
    (void) scancode;
    (void) mods;

    if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

int main() {
    make_window(-1, -1, TITLE);
    add_key_callback(key_callback);

    add_actor(make_player_actor());
    add_actor(make_cube_actor());

    start_main_loop();

    return EXIT_SUCCESS;
}
