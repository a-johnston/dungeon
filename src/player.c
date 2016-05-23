#include "dungeon.h"

struct player_data {
    vec3 pos;
    vec3 v;
    double yaw, pitch;

    Camera camera;
};

int w, a, s, d, jump;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    (void) window;
    (void) scancode;
    (void) mods;

    if (key == GLFW_KEY_W) w = (action != GLFW_RELEASE);
    if (key == GLFW_KEY_A) a = (action != GLFW_RELEASE);
    if (key == GLFW_KEY_S) s = (action != GLFW_RELEASE);
    if (key == GLFW_KEY_D) d = (action != GLFW_RELEASE);
    if (key == GLFW_KEY_SPACE) jump = (action != GLFW_RELEASE);
}

static void* create() {
    struct player_data *data = (struct player_data*) malloc(sizeof(struct player_data));
    
    data->pos = (vec3) { -5.0, 0.0, 0.0 };
    data->v   = (vec3) {  0.0, 0.0, 0.0 };

    data->yaw = data->pitch = 0.0;

    vec3 to = (vec3) { 1.0, 0.0, 0.0 };
    vec3 up = (vec3) { 0.0, 0.0, 1.0 };

    cam_update_view(&data->camera, &data->pos, &to, &up);
    cam_update_perspective(&data->camera, 70.0f, 1.0f, 100.0f);

    set_main_camera(&data->camera);

    w = a = s = d = jump = 0;
    add_key_callback(key_callback);

    return data;
}

static void step(void *void_data, double time) {
    (void) time;
    struct player_data *data = (struct player_data*) void_data;

    double ws = w - s;
    double da = d - a;

    double dx;
    double dy;

    get_cursor_delta(&dx, &dy);

    dx /= -100;
    dy /= -100;

    data->yaw   += dx;
    data->pitch += dy;

    double limit = M_PI / 2.0 - 0.01;
    data->pitch = clamp(-limit, data->pitch, limit);

    data->v.x /= 2.0;
    data->v.y /= 2.0;

    if (ws != 0 || da != 0) {
        data->v.x += (-da * sin(data->yaw) - ws * cos(data->yaw));
        data->v.y += ( da * cos(data->yaw) - ws * sin(data->yaw));
    }

    if (data->pos.z < 0) {
        data->pos.z = 0;
        data->v.z = 0;
    } else if (data->pos.z > 0) {
        data->v.z += 0.2;
    } else if (jump) {
        printf("JUMP\n");
        data->v.z = -4;
    }

    data->pos = add_vec3(data->pos, mult_vec3(data->v, time * 2.0));

    vec3 to = {
        cos(data->yaw) * cos(data->pitch),
        sin(data->yaw) * cos(data->pitch),
        sin(data->pitch)
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
