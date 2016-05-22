#include "dungeon.h"

static void make_color_shader() {
    if (get_from_store(COLOR_SHADER)) {
        return;
    }

    // shader creation and variable mapping
    Shader *shader = make_shader("assets/color_vertex.glsl", "assets/color_fragment.glsl");

    map_shader_attrib(shader, VERT, "position");
    map_shader_attrib(shader, NORM, "normal");

    map_shader_uniform(shader, MATRIX_4FV, "mvp", 1);
    map_shader_uniform(shader, MATRIX_4FV, "model", 1);

    add_to_store(COLOR_SHADER, shader);
}

void make_shaders() {
    make_color_shader();
}
