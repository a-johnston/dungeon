#include "dungeon.h"

struct cube_data {
    Mesh *mesh;
    Shader *shader;
    list *uniforms;

    mat4 m, mvp;
    quat rot, q;
};

static void* create() {
    struct cube_data *data = (struct cube_data*) malloc(sizeof(struct cube_data));

    // shader creation and variable mapping
    data->shader = make_shader("assets/color_vertex.glsl", "assets/color_fragment.glsl");

    map_shader_attrib(data->shader, VERT, "position");
    map_shader_attrib(data->shader, NORM, "normal");

    map_shader_uniform(data->shader, MATRIX_4FV, "mvp", 1);
    map_shader_uniform(data->shader, MATRIX_4FV, "model", 1);

    // shader uniforms
    data->uniforms = create_list();

    list_add(data->uniforms, &data->mvp);
    list_add(data->uniforms, &data->m);

    // mesh and animation stuff
    data->mesh = mesh_build_cube();
    mesh_make_vbo(data->mesh);

    data->q = (quat) { 0.0, 0.0, 0.0, 1.0 };
    data->rot = quat_from_euler_angles(0.1, 0.5, 1.0);

    return data;
}

static void step(void *void_data, double time) {
    (void) time;
    struct cube_data *data = (struct cube_data*) void_data;

    data->q = quat_mult(data->rot, data->q);
    quat_to_matrix(data->q, data->m);
}

static void draw(void *void_data) {
    struct cube_data *data = (struct cube_data*) void_data;

    cam_get_mvp(data->mvp, get_main_camera(), data->m);
    draw_mesh(data->shader, data->mesh, data->uniforms);
}

static void destroy(void *void_data) {
    struct cube_data *data = (struct cube_data*) void_data;

    free_mesh(data->mesh);
    free_shader(data->shader);
    list_free_keep_elements(data->uniforms);

    free(data);
}

Actor* make_cube_actor() {
    return make_actor(create, step, draw, destroy);
}
