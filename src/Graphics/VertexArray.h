#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

struct Vertex {
    glm::vec3 position{0.0f};
    glm::vec2 texture{0.0f};
};

struct Renderable {
    GLuint vao = 0;
    GLuint indices = 0;
};

struct VertexArray {
    GLuint vao = 0;
    GLuint vbo = 0;
    GLuint ibo = 0;
    GLuint numIndices = 0;
};

VertexArray createVertexArray(const std::vector<Vertex>& verts, const std::vector<GLuint> indices);
VertexArray createTerrainVertexArray();
VertexArray createEmptyVertexArray();

void destroyVertexArray(VertexArray* vertexArray);
