#include "Mesh.h"

#include <iostream>
#include <stb/stb_perlin.h>
namespace {
    struct NoiseOptions {
        float roughness;
        float smoothness;
        float amplitude;

        int octaves;
        float offset;
    };

    float getNoiseAt(const glm::ivec2& position, int seed, NoiseOptions& options)
    {
        float value = 0;
        float acc = 0;
        for (int i = 0; i < options.octaves; i++) {
            float frequency = glm::pow(2.0f, i);
            float amplitude = glm::pow(options.roughness, i);

            float x = position.x * frequency / options.smoothness;
            float z = position.y * frequency / options.smoothness;

            float noiseValue = stb_perlin_noise3_seed(x, z, 0, 0, 0, 0, seed);
            noiseValue = (noiseValue + 1.0f) / 2.0f;
            value += noiseValue * amplitude;
            acc += amplitude;
        }
        return value / acc * options.amplitude + options.offset;
    }
} // namespace

Mesh createTerrainMesh()
{
    Mesh mesh;
    NoiseOptions terrainNoise;
    terrainNoise.roughness = 0.7;
    terrainNoise.smoothness = 350.0f;
    terrainNoise.octaves = 5;
    terrainNoise.amplitude = 80.0f;
    terrainNoise.offset = 0;

    NoiseOptions bumpNoise;
    bumpNoise.roughness = 1.2;
    bumpNoise.smoothness = 50.0f;
    bumpNoise.octaves = 5;
    bumpNoise.amplitude = 5.0f;
    bumpNoise.offset = 0;

    // std::random_device rd;
    // std::mt19937 rng(rd());
    // std::uniform_int_distribution<int> dist(-20000, 20000);
    int seed = 13913; // dist(rng);

    constexpr float SIZE = TERRAIN_SIZE;
    constexpr float VERTS = TERRAIN_SIZE;
    constexpr int HEIGHT_MAPS_WIDTH = VERTS + 2;
    constexpr unsigned HEIGHT_MAP_SIZE = HEIGHT_MAPS_WIDTH * HEIGHT_MAPS_WIDTH;

    std::vector<float> heights(HEIGHT_MAP_SIZE);

    for (int y = 0; y < HEIGHT_MAPS_WIDTH; y++) {
        for (int x = 0; x < HEIGHT_MAPS_WIDTH; x++) {
            int tx = x + (HEIGHT_MAPS_WIDTH - 1);
            int ty = y + (HEIGHT_MAPS_WIDTH - 1);
            float height = getNoiseAt({tx, ty}, seed, terrainNoise);
            float bumps = getNoiseAt({tx, ty}, seed, bumpNoise);
            heights[y * HEIGHT_MAPS_WIDTH + x] = height + bumps;
        }
    }

    auto getHeight = [&](int x, int y) {
        if (x < 0 || x >= HEIGHT_MAPS_WIDTH || y < 0 || y >= HEIGHT_MAPS_WIDTH) {
            return 0.0f;
        }
        else {
            return heights[y * HEIGHT_MAPS_WIDTH + x];
        }
    };

    for (int y = 0; y < VERTS; y++) {
        for (int x = 0; x < VERTS; x++) {
            auto fx = static_cast<float>(x);
            auto fy = static_cast<float>(y);

            float vx = fx / (VERTS - 1) * SIZE;
            float vz = fy / (VERTS - 1) * SIZE;

            int hx = x + 1;
            int hy = y + 1;
            float vy = getHeight(hx, hy);

            Vertex vert;
            vert.position = {vx, vy, vz};

            float h1 = getHeight(hx - 1, hy);
            float h2 = getHeight(hx + 1, hy);
            float h3 = getHeight(hx, hy - 1);
            float h4 = getHeight(hx, hy + 1);
            // glm::vec3 normal{h1 - h2, 2, h3 - h4};
            // glm::vec3 n = glm::normalize(normal);
            // mesh.normals.emplace_back(n.x, n.y, n.z);

            std::cout << vy << std::endl;

            // glm::vec3 colour;

            // int height = static_cast<int>(vy);
            // if (height > 10) {
            //    colour = {255, 255, 255};
            //}
            // else if (height > 6) {
            //    colour = {100, 100, 100};
            //}
            // else if (height > 0) {
            //    colour.g = 255;
            //}
            // else if (height > -4) {
            //    colour.r = 255;
            //    colour.g = 220;
            //    colour.b = 127;
            //}
            // else {
            //    colour.r = 255 / 4;
            //    colour.g = 220 / 4;
            //    colour.b = 127 / 4;
            //}
            // vert.colour = colour;

            // float u = y % (int)VERTS;
            // float v = x % (int)VERTS;
            float u = fx / VERTS - 1;
            float v = fy / VERTS - 1;
            vert.texture = {u, v};
            mesh.vertices.push_back(vert);
        }
    }

    for (int y = 0; y < (VERTS - 1); y += 1) {
        for (int x = 0; x < (VERTS - 1); x += 1) {
            int topLeft = (y * VERTS) + x;
            int topRight = topLeft + 1;
            int bottomLeft = ((y + 1) * VERTS) + x;
            int bottomRight = bottomLeft + 1;

            mesh.indices.push_back(topLeft);
            mesh.indices.push_back(bottomLeft);
            mesh.indices.push_back(topRight);
            mesh.indices.push_back(bottomLeft);
            mesh.indices.push_back(bottomRight);
            mesh.indices.push_back(topRight);

            mesh.indicesCount += 6;
        }
    }

    return mesh;
}

Mesh createQuadMesh()
{
    // clang-format off
    Mesh mesh;
    mesh.vertices = {
        {{-0.5f, -0.5f, 0.0f}, {0.0f, 1.0f}},
        {{ 0.5f, -0.5f, 0.0f}, {1.0f, 1.0f}},
        {{ 0.5f,  0.5f, 0.0f}, {1.0f, 0.0f}},
        {{-0.5f,  0.5f, 0.0f}, {0.0f, 0.0f}},
    };
    // clang-format on
    mesh.indices = {0, 1, 2, 2, 3, 0};
    return mesh;
}

Mesh createCubeMesh()
{
    Mesh mesh;
    return mesh;
}