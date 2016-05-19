#include "dungeon.h"

struct player_data {
    vec3 pos;
    vec3 v;

    Camera camera;
};

int w, a, s, d;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    (void) window;
    (void) scancode;
    (void) mods;

    if (key == GLFW_KEY_W) w = (action != GLFW_RELEASE);
    if (key == GLFW_KEY_A) a = (action != GLFW_RELEASE);
    if (key == GLFW_KEY_S) s = (action != GLFW_RELEASE);
    if (key == GLFW_KEY_D) d = (action != GLFW_RELEASE);
}

static void* create() {
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

static void step(void *void_data, double time) {
    (void) time;
    struct player_data *data = (struct player_data*) void_data;

    double ws = w - s;
    double da = d - a;

    double yaw;
    double pitch;

    get_cursor_position(&yaw, &pitch);

    yaw   /= -100;
    pitch /= -100;

    data->pos.x += (da * sin(yaw) + ws * cos(yaw)) * 0.2;
    data->pos.y += (da * cos(yaw) + ws * sin(yaw)) * 0.2;

    vec3 to = {
        cos(yaw) * cos(pitch),
        sin(yaw) * cos(pitch),
        sin(pitch)
    };

    to = add_vec3(to, data->pos);

    cam_update_view(&data->camera, &data->pos, &to, NULL);
}

static void destroy(void *data) {
    free(data);
    remove_key_callback(key_callback);
}

Actor* make_player_actor() {
    return make_actor(create, step, NULL, destroy);
}
