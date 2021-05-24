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

class VertexArray final {
  public:
    VertexArray();

    static VertexArray createTerrain();

    void bufferVertexData(const std::vector<Vertex>& verts);
    void bufferIndicesData(const std::vector<GLuint> indices);

    GLsizei indicesCount();

    void bind() const;

  private:
    GLuint m_vao = 0;
    GLuint m_vbo = 0;
    GLuint m_ibo = 0;
    GLuint m_indexCount = 0;

  public:
    VertexArray& operator=(VertexArray&& other);
    VertexArray(VertexArray&& other);
    ~VertexArray();

    VertexArray& operator=(VertexArray& other) = delete;
    VertexArray(VertexArray& other) = delete;
};