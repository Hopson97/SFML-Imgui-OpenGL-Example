#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#define ARR_LENGTH(a) (sizeof(a) / sizeof(a[0]))
#define CREATE_VERTEX_ARRAY(vertices, indices)                                                                         \
    createVertexArray(vertices, indices, ARR_LENGTH(vertices), ARR_LENGTH(indices));

struct Vertex {
    glm::vec3 position;
    glm::vec2 texture;
};

struct Renderable {
    GLuint vao;
    GLuint indices;
};

struct VertexArray {
    GLuint vao;
    GLuint vbo;
    GLuint ibo;
    GLuint numIndices;
};

struct VertexArray createVertexArray(const struct Vertex* vertices, const GLuint* indices, GLsizei numVerticies,
                                     GLsizei numIndices);
struct VertexArray createTerrainVertexArray();
struct VertexArray createEmptyVertexArray();

void destroyVertexArray(struct VertexArray* vertexArray);
