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
    VertexArray& operator=(VertexArray&& other);
    VertexArray(VertexArray&& other);
    ~VertexArray();

    VertexArray& operator=(VertexArray& other) = delete;
    VertexArray(VertexArray& other) = delete;

    static VertexArray create(const std::vector<Vertex>& verts, const std::vector<GLuint> indices);
    static VertexArray createTerrain();
    static VertexArray createEmpty();

    void bufferVertexData(const std::vector<Vertex>& verts);
    void bufferIndicesData(const std::vector<GLuint> indices);

    GLsizei indicesCount();

    void bind();

  private:
    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_ibo;
    GLuint m_indexCount;
};