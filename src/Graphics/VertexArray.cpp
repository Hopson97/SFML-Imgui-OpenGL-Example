#include "VertexArray.h"
#include <glm/glm.hpp>
#include <stb/stb_perlin.h>

VertexArray::VertexArray()
{
    glGenVertexArrays(1, &m_vao);
}

VertexArray& VertexArray::operator=(VertexArray&& other)
{
    m_vao = other.m_vao;
    m_vbo = other.m_vbo;
    m_ibo = other.m_ibo;
    m_indexCount = other.m_indexCount;

    other.m_vao = 0;
    other.m_vbo = 0;
    other.m_ibo = 0;
    other.m_indexCount = 0;

    return *this;
}

VertexArray::VertexArray(VertexArray&& other)
    : m_vao{other.m_vao}
    , m_vbo{other.m_vbo}
    , m_ibo{other.m_ibo}
    , m_indexCount{other.m_indexCount}
{
    other.m_vao = 0;
    other.m_vbo = 0;
    other.m_ibo = 0;
    other.m_indexCount = 0;
}

VertexArray::~VertexArray()
{
    if (m_vao) {
        glDeleteVertexArrays(1, &m_vao);
        glDeleteBuffers(1, &m_vbo);
        glDeleteBuffers(1, &m_ibo);
    }
}

VertexArray VertexArray::create(const std::vector<Vertex>& verts, const std::vector<GLuint> indices)
{
    VertexArray v;
    glCreateVertexArrays(1, &v.m_vao);
    v.bufferVertexData(verts);
    v.bufferIndicesData(indices);
    return v;
}

VertexArray VertexArray::createEmpty()
{
    VertexArray v;
    glCreateVertexArrays(1, &v.m_vao);
    return v;
}

#define VERTS 128
#define SIZE 50

struct VertexArray VertexArray::createTerrain()
{
    std::vector<Vertex> terrainVerts(VERTS * VERTS);
    int ptr = 0;
    for (int z = 0; z < VERTS; z++) {
        for (int x = 0; x < VERTS; x++) {
            float fx = (float)x;
            float fy = (float)z;

            float vx = fx / (VERTS - 1) * SIZE;
            float vz = fy / (VERTS - 1) * SIZE;

            // Begin iterating through the octaves

            int octaves = 5;
            float smooth = 200;
            float roughness = 0.58f;

            float value = 0;
            float accumulatedAmps = 0;
            for (int i = 0; i < octaves; i++) {
                float frequency = glm::pow(2.0f, i);
                float amplitude = powf(roughness, i);

                float nx = x * frequency / smooth;
                float nz = z * frequency / smooth;

                float noise = stb_perlin_noise3_seed(nx, 4221, nz, 0, 0, 0, 21421);
                noise = (noise + 1.0f) / 2.0f;
                value += noise * amplitude;
                accumulatedAmps += amplitude;
            }
            float height = (value / accumulatedAmps) * 100;

            Vertex vertex;
            vertex.position.x = vx;
            vertex.position.y = height - 100;
            vertex.position.z = vz;

            float u = fx / VERTS - 1;
            float v = fy / VERTS - 1;
            vertex.texture.x = u;
            vertex.texture.y = v;

            terrainVerts[ptr++] = vertex;
        }
    }

    int indicesCount = 0;
    ptr = 0;
    std::vector<GLuint> terrainIndices;
    terrainIndices.reserve(VERTS * VERTS);
    for (int y = 0; y < (VERTS - 1); y += 1) {
        for (int x = 0; x < (VERTS - 1); x += 1) {
            int topLeft = (y * VERTS) + x;
            int topRight = topLeft + 1;
            int bottomLeft = ((y + 1) * VERTS) + x;
            int bottomRight = bottomLeft + 1;

            terrainIndices.push_back(topLeft);
            terrainIndices.push_back(bottomLeft);
            terrainIndices.push_back(topRight);
            terrainIndices.push_back(bottomLeft);
            terrainIndices.push_back(bottomRight);
            terrainIndices.push_back(topRight);

            indicesCount += 6;
        }
    }

    return create(terrainVerts, terrainIndices);
}

void VertexArray::bufferVertexData(const std::vector<Vertex>& verts)
{
    glCreateBuffers(1, &m_vbo);

    // glBufferData
    glNamedBufferStorage(m_vbo, sizeof(struct Vertex) * verts.size(), verts.data(), GL_DYNAMIC_STORAGE_BIT);

    // Attach the vertex array to the vertex buffer and element buffer
    glVertexArrayVertexBuffer(m_vao, 0, m_vbo, 0, sizeof(struct Vertex));

    // glEnableVertexAttribArray
    glEnableVertexArrayAttrib(m_vao, 0);
    glEnableVertexArrayAttrib(m_vao, 1);

    // glVertexAttribPointer
    glVertexArrayAttribFormat(m_vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, position));
    glVertexArrayAttribFormat(m_vao, 1, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, texture));
    glVertexArrayAttribBinding(m_vao, 0, 0);
    glVertexArrayAttribBinding(m_vao, 1, 0);
}

void VertexArray::bufferIndicesData(const std::vector<GLuint> indices)
{
    glCreateBuffers(1, &m_ibo);
    glNamedBufferStorage(m_ibo, sizeof(GLuint) * indices.size(), indices.data(), GL_DYNAMIC_STORAGE_BIT);
    glVertexArrayElementBuffer(m_vao, m_ibo);
    m_indexCount = indices.size();
}

GLsizei VertexArray::indicesCount()
{
    return m_indexCount;
}

void VertexArray::bind()
{
    glBindVertexArray(m_vao);
}