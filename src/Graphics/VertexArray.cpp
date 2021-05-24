#include "VertexArray.h"
#include <glm/glm.hpp>
#include <stb/stb_perlin.h>

static void bufferVertexData(VertexArray* v, const std::vector<Vertex>& verts)
{
    glCreateBuffers(1, &v->vbo);

    // glBufferData
    glNamedBufferStorage(v->vbo, sizeof(struct Vertex) * verts.size(), verts.data(), GL_DYNAMIC_STORAGE_BIT);

    // Attach the vertex array to the vertex buffer and element buffer
    glVertexArrayVertexBuffer(v->vao, 0, v->vbo, 0, sizeof(struct Vertex));

    // glEnableVertexAttribArray
    glEnableVertexArrayAttrib(v->vao, 0);
    glEnableVertexArrayAttrib(v->vao, 1);

    // glVertexAttribPointer
    glVertexArrayAttribFormat(v->vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, position));
    glVertexArrayAttribFormat(v->vao, 1, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, texture));
    glVertexArrayAttribBinding(v->vao, 0, 0);
    glVertexArrayAttribBinding(v->vao, 1, 0);
}

static void bufferIndicesData(VertexArray* v, const std::vector<GLuint> indices)
{
    glCreateBuffers(1, &v->ibo);
    glNamedBufferStorage(v->ibo, sizeof(GLuint) * indices.size(), indices.data(), GL_DYNAMIC_STORAGE_BIT);
    glVertexArrayElementBuffer(v->vao, v->ibo);
    v->numIndices = indices.size();
}

VertexArray createVertexArray(const std::vector<Vertex>& verts, const std::vector<GLuint> indices)
{
    struct VertexArray v;
    glCreateVertexArrays(1, &v.vao);
    bufferVertexData(&v, verts);
    bufferIndicesData(&v, indices);
    return v;
}

VertexArray createEmptyVertexArray()
{
    struct VertexArray v;
    glCreateVertexArrays(1, &v.vao);
    return v;
}

#define VERTS 128
#define SIZE 50

struct VertexArray createTerrainVertexArray()
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

    return createVertexArray(terrainVerts, terrainIndices);
}

void destroyVertexArray(struct VertexArray* v)
{
    glDeleteBuffers(1, &v->ibo);
    glDeleteBuffers(1, &v->vbo);
    glDeleteVertexArrays(1, &v->vao);
    v->numIndices = 0;
}
