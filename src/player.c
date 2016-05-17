#include "dungeon.h"

struct player_data {
    vec3 pos;
    vec3 v;

    Camera camera;
};

int w, a, s, d;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    (void) scancode;
    (void) mods;

    if (key == GLFW_KEY_W) w = (action != GLFW_RELEASE);
    if (key == GLFW_KEY_A) a = (action != GLFW_RELEASE);
    if (key == GLFW_KEY_S) s = (action != GLFW_RELEASE);
    if (key == GLFW_KEY_D) d = (action != GLFW_RELEASE);
}

void* create() {
    struct player_data *data = (struct player_data*) malloc(sizeof(struct player_data));
    
    data->pos = (vec3) { 0.0, 0.0, 0.0 };
    data->v   = (vec3) { 0.0, 0.0, 0.0 };

    vec3 to = (vec3) { 1.0, 0.0, 0.0 };
    vec3 up = (vec3) { 0.0, 0.0, 1.0 };

    cam_update_view(&data->camera, &data->pos, &to, &up);
    cam_update_perspective(&data->camera, 70.0f, 1.0f, 100.0f);

    set_main_camera(&data->camera);

    w = a = s = d = 0;
    add_key_callback(key_callback);

    return data;
}

void step(void *void_data, double time) {
    struct player_data *data = (struct player_data*) void_data;

    data->pos.x += d - a;
    data->pos.y += w - s;

    double yaw;
    double pitch;

    get_cursor_position(&yaw, &pitch);

    vec3 to = {
        cos(yaw) * cos(pitch),
        sin(yaw) * cos(pitch),
        sin(pitch)
    };

    cam_update_view(&data->camera, NULL, &to, NULL);
}

void destroy(void *data) {
    free(data);
    remove_key_callback(key_callback);
}

Actor* make_player_actor() {
    return make_actor(create, step, NULL, destroy);
}
