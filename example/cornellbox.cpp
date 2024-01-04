#include <iostream>
#include <fstream>
#include <chrono>
#include <random>
#include <string>
#include <cmath>
#include "../src/tensor/vec3.h"
#include "../src/geometry/triangle.h"
#include "../src/texture/texture.h"
#include "../src/accel/accel_kdtree.h"
#include "../src/accel/accel_naive.h"
#include "../src/camera/camera_pinhole.h"
#include "../src/const.h"
using namespace std;
typedef unsigned int uint;
const int img_width = 1024;
const int img_height = 1024;
float img[img_width * img_height * 4];
AccelKDtree accel;
CameraPinhole camera;
vector<Triangle> triangles;
vector<Geometry*> shapes;
vector<Vec3> emittors;
vector<Vec3> albedos;
mt19937 RD(123);
float R() {
    return (float)((RD() % 2147483648) - 1073741824) / 1073741824;
}
void WriteToPPM(string name) {
    ofstream p(name);
    p << "P3" << endl;
    p << img_width << ' ' << img_height << endl;
    p << 255 << endl;
    for(int y = img_height - 1; y >= 0; --y) {
        for(int x = 0; x < img_width; ++x) {
            int bias = (y * img_width + x) * 4;
            p << min(int(img[bias + 0] * 255), 255) << ' '
              << min(int(img[bias + 1] * 255), 255) << ' '
              << min(int(img[bias + 2] * 255), 255) << ' ';
        }
    }
    p.close();
}
void AddFace(Vec3 A, Vec3 B, Vec3 C, Vec3 D, Vec3 emittor, Vec3 albedo) {
    triangles.push_back(Triangle(A, B, C));
    emittors.push_back(emittor);
    albedos.push_back(albedo);

    triangles.push_back(Triangle(A, C, D));
    emittors.push_back(emittor);
    albedos.push_back(albedo);
}
void Init() {
    // floor
    AddFace(Vec3(-1.01f, 0.00f, 0.99f),
            Vec3(1.00f, 0.00f, 0.99f),
            Vec3(1.00f, 0.00f, -1.04f),
            Vec3(-0.99f, 0.00f, -1.04f),
            Vec3(0.00f, 0.00f, 0.00f),
            Vec3(0.725f, 0.710f, 0.680f));

    // ceiling
    AddFace(Vec3(-1.02f, 1.99f, 0.99f),
            Vec3(-1.02f, 1.99f, -1.04f),
            Vec3(1.00f, 1.99f, -1.04f),
            Vec3(1.00f, 1.99f, 0.99f),
            Vec3(0.00f, 0.00f, 0.00f),
            Vec3(0.725f, 0.710f, 0.680f));

    // backWall
    AddFace(Vec3(-0.99f, 0.00f, -1.04f),
            Vec3(1.00f, 0.00f, -1.04f),
            Vec3(1.00f, 1.99f, -1.04f),
            Vec3(-1.02f, 1.99f, -1.04f),
            Vec3(0.00f, 0.00f, 0.00f),
            Vec3(0.725f, 0.710f, 0.680f));

    // rightWall
    AddFace(Vec3(1.00f, 0.00f, -1.04f),
            Vec3(1.00f, 0.00f, 0.99f),
            Vec3(1.00f, 1.99f, 0.99f),
            Vec3(1.00f, 1.99f, -1.04f),
            Vec3(0.00f, 0.00f, 0.00f),
            Vec3(0.140f, 0.450f, 0.091f));

    // leftWall
    AddFace(Vec3(-1.01f, 0.00f, 0.99f),
            Vec3(-0.99f, 0.00f, -1.04f),
            Vec3(-1.02f, 1.99f, -1.04f),
            Vec3(-1.02f, 1.99f, 0.99f),
            Vec3(0.00f, 0.00f, 0.00f),
            Vec3(0.630f, 0.065f, 0.050f));

    // shortBox
    // Top Face
    AddFace(Vec3(0.53f, 0.60f, 0.75f),
            Vec3(0.70f, 0.60f, 0.17f),
            Vec3(0.13f, 0.60f, 0.00f),
            Vec3(-0.05f, 0.60f, 0.57f),
            Vec3(0.00f, 0.00f, 0.00f),
            Vec3(0.725, 0.710, 0.680));

    // Left Face
    AddFace(Vec3(-0.05f, 0.00f, 0.57f),
            Vec3(-0.05f, 0.60f, 0.57f),
            Vec3(0.13f, 0.60f, 0.00f),
            Vec3(0.13f, 0.00f, 0.00f),
            Vec3(0.00f, 0.00f, 0.00f),
            Vec3(0.725, 0.710, 0.680));

    // Front Face
    AddFace(Vec3(0.53f, 0.00f, 0.75f),
            Vec3(0.53f, 0.60f, 0.75f),
            Vec3(-0.05f, 0.60f, 0.57f),
            Vec3(-0.05f, 0.00f, 0.57f),
            Vec3(0.00f, 0.00f, 0.00f),
            Vec3(0.725, 0.710, 0.680));

    // Right Face
    AddFace(Vec3(0.70f, 0.00f, 0.17f),
            Vec3(0.70f, 0.60f, 0.17f),
            Vec3(0.53f, 0.60f, 0.75f),
            Vec3(0.53f, 0.00f, 0.75f),
            Vec3(0.00f, 0.00f, 0.00f),
            Vec3(0.725, 0.710, 0.680));

    // Back Face
    AddFace(Vec3(0.13f, 0.00f, 0.00f),
            Vec3(0.13f, 0.60f, 0.00f),
            Vec3(0.70f, 0.60f, 0.17f),
            Vec3(0.70f, 0.00f, 0.17f),
            Vec3(0.00f, 0.00f, 0.00f),
            Vec3(0.725, 0.710, 0.680));

    // Bottom Face
    AddFace(Vec3(0.53f, 0.00f, 0.75f),
            Vec3(0.70f, 0.00f, 0.17f),
            Vec3(0.13f, 0.00f, 0.00f),
            Vec3(-0.05f, 0.00f, 0.57f),
            Vec3(0.00f, 0.00f, 0.00f),
            Vec3(0.725, 0.710, 0.680));
    // tallBox
    // Top Face
    AddFace(Vec3(-0.53f, 1.20f, 0.09f),
            Vec3(0.04f, 1.20f, -0.09f),
            Vec3(-0.14f, 1.20f, -0.67f),
            Vec3(-0.71f, 1.20f, -0.49f),
            Vec3(0.00f, 0.00f, 0.00f),
            Vec3(0.725f, 0.710f, 0.680f));

    // Left Face
    AddFace(Vec3(-0.53f, 0.00f, 0.09f),
            Vec3(-0.53f, 1.20f, 0.09f),
            Vec3(-0.71f, 1.20f, -0.49f),
            Vec3(-0.71f, 0.00f, -0.49f),
            Vec3(0.00f, 0.00f, 0.00f),
            Vec3(0.725f, 0.710f, 0.680f));

    // Back Face
    AddFace(Vec3(-0.71f, 0.00f, -0.49f),
            Vec3(-0.71f, 1.20f, -0.49f),
            Vec3(-0.14f, 1.20f, -0.67f),
            Vec3(-0.14f, 0.00f, -0.67f),
            Vec3(0.00f, 0.00f, 0.00f),
            Vec3(0.725f, 0.710f, 0.680f));

    // Right Face
    AddFace(Vec3(-0.14f, 0.00f, -0.67f),
            Vec3(-0.14f, 1.20f, -0.67f),
            Vec3(0.04f, 1.20f, -0.09f),
            Vec3(0.04f, 0.00f, -0.09f),
            Vec3(0.00f, 0.00f, 0.00f),
            Vec3(0.725f, 0.710f, 0.680f));

    // Front Face
    AddFace(Vec3(0.04f, 0.00f, -0.09f),
            Vec3(0.04f, 1.20f, -0.09f),
            Vec3(-0.53f, 1.20f, 0.09f),
            Vec3(-0.53f, 0.00f, 0.09f),
            Vec3(0.00f, 0.00f, 0.00f),
            Vec3(0.725f, 0.710f, 0.680f));

    // Bottom Face
    AddFace(Vec3(-0.53f, 0.00f, 0.09f),
            Vec3(0.04f, 0.00f, -0.09f),
            Vec3(-0.14f, 0.00f, -0.67f),
            Vec3(-0.71f, 0.00f, -0.49f),
            Vec3(0.00f, 0.00f, 0.00f),
            Vec3(0.725f, 0.710f, 0.680f));

    // light
    AddFace(Vec3(-0.24f, 1.98f, 0.16f),
            Vec3(-0.24f, 1.98f, -0.22f),
            Vec3(0.23f, 1.98f, -0.22f),
            Vec3(0.23f, 1.98f, 0.16f),
            Vec3(17.0f, 12.0f, 4.0f),
            Vec3(1.0f, 1.0f, 1.0f));
    for(int i = 0; i < triangles.size(); ++i) {
        shapes.push_back(&triangles[i]);
    }
    accel.Init(shapes);
    float dep_t = 2.03f;
    float width_t = 1.0f;
    float height_t = 1.0f;
    Vec3 z_axis(0.0f, 1.0f, 0.0f);
    Vec3 x_axis(1.0f, 0.0f, 0.0f);
    Vec3 origin(-0.01f, 0.995f, 5.0f);
    camera.Init(z_axis, x_axis, origin, width_t, height_t, dep_t);
}

auto start_t = chrono::high_resolution_clock::now();
auto end_t = chrono::high_resolution_clock::now();
auto duration = chrono::duration_cast<chrono::microseconds>(end_t - start_t);
void Begin() {
    start_t = chrono::high_resolution_clock::now();
}
void End(string msg) {
    end_t = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::microseconds>(end_t - start_t);
    cout << msg << " time used: " << (double)(duration.count()) / 1000000 << " s" << endl;
}

float max(float a, float b) {
    return a > b ? a : b;
}
uint tea(uint v0, uint v1) {
    uint s0 = 0;
    for(uint n = 0u; n < 4u; n++) {
        s0 += 0x9e3779b9u;
        v0 += ((v1 << 4) + 0xa341316cu) ^ (v1 + s0) ^ ((v1 >> 5u) + 0xc8013ea4u);
        v1 += ((v0 << 4) + 0xad90777du) ^ (v0 + s0) ^ ((v0 >> 5u) + 0x7e95761eu);
    }
    return v0;
}
float lcg(uint& state) noexcept {
    constexpr uint lcg_a = 1664525u;
    constexpr uint lcg_c = 1013904223u;
    state = lcg_a * state + lcg_c;
    return (state & 0x00ffffffu) *
            (1.0f / float(0x01000000u));
};
float balanced_heuristic(float pdf_a, float pdf_b) {
    return pdf_a / max(pdf_a + pdf_b, 1e-4);
}
Vec3 offset(Vec3 p, Vec3 n) {
    return p + n * (0.00390625f);
}
Vec3 cosine_sample_hemisphere(float x, float y) {
    float r = sqrt(x);
    float phi = 2.0f * PI * y;
    return Vec3(r * cos(phi), r * sin(phi), sqrt(1.0f - x));
}
Vec3 mc(Vec3 origin, Vec3 d, uint& state) {
    Vec3 color;
    Vec3 radiance(0.0f, 0.0f, 0.0f);
    Vec3 beta(1.0f, 1.0f, 1.0f);
    float pdf_bsdf = 0.0f;
    Vec3 light_position(-0.24f, 1.98f, 0.16f);
    Vec3 light_u = Vec3(-0.24f, 1.98f, -0.22f) - light_position;
    Vec3 light_v = Vec3(0.23f, 1.98f, 0.16f) - light_position;
    Vec3 light_emission(17.0f, 12.0f, 4.0f);
    float light_area = (light_u.Cross(light_v)).Len();
    Vec3 light_normal = light_u.Cross(light_v).Norm();
    for(int depth = 0; depth < 10; ++depth) {
        auto hit = accel.Inter(offset(origin, d), d);
        if(hit.first == -1) {
            break;
        }
        auto inter = hit.second.second;
        Vec3 norm = triangles[hit.first].GetNorm();
        float cos_wo = -norm.Dot(d);
        if(hit.first < triangles.size() - 2 && cos_wo < 0) { // in case the norm is reversed
            cos_wo = -cos_wo;
            norm = Vec3(0.0f, 0.0f, 0.0f) - norm;
        }
        if(cos_wo < 1e-4) {
            break;
        }
        // hit light
        if(hit.first >= triangles.size() - 2) {
            if(depth == 0) {
                radiance = radiance + light_emission;
            } else {
                float pdf_light = (inter - origin).Square() / (light_area * cos_wo);
                float mis_weight = balanced_heuristic(pdf_bsdf, pdf_light);
                radiance = radiance + beta * mis_weight * light_emission;
            }
            break;
        }
        // sample light
        float ux_light = lcg(state);
        float uy_light = lcg(state);
        Vec3 p_light = light_position + light_u * ux_light + light_v * uy_light;
        Vec3 pp = offset(inter, norm); // in case the ray is too close to the surface
        Vec3 wi_light = (p_light - pp).Norm();
        auto hit2 = accel.Inter(pp, wi_light);
        float cos_wi_light = wi_light.Dot(norm);
        float cos_light = -light_normal.Dot(wi_light);
        Vec3 albedo = albedos[hit.first];
        if(hit2.first >= triangles.size() - 2 && cos_wi_light > 1e-4 && cos_light > 1e-4) { // hit light
            float pdf_light = (pp - p_light).Square() / (light_area * cos_light);
            float pdf_bsdf = cos_wi_light / PI;
            float mis_weight = balanced_heuristic(pdf_light, pdf_bsdf);
            Vec3 bsdf = albedo / PI * cos_wi_light;
            radiance = radiance + beta * bsdf * mis_weight * light_emission / max(pdf_light, 1e-4f);
        }

        // sample BSDF
        float ux = lcg(state);
        float uy = lcg(state);
        Vec3 wi_local = cosine_sample_hemisphere(ux, uy);
        float cos_wi = abs(wi_local.z);
        Vec3 nx = CrossSafe(norm, Vec3(0.0f, 0.0f, 0.0f) - norm); // to get a tangential axis
        Vec3 nd = TransZXP(norm, nx, Vec3(0.0f, 0.0f, 0.0f)).MapA(wi_local);
        d = nd;
        origin = inter;
        pdf_bsdf = cos_wi / PI;
        beta = beta * albedo;

        // russian roulette
        float l = Vec3(0.212671f, 0.715160f, 0.072169f).Dot(beta); // magical number ???
        if(abs(l) <= EPS) {
            break;
        }
        float q = max(l, 0.05f);
        float r = lcg(state);
        if(r >= q) {
            break;
        }
        beta = beta * (1.0f / q);
    }
    if(std::isnan(radiance.x) || std::isnan(radiance.y) || std::isnan(radiance.z)) {
        radiance = Vec3(0.0f, 0.0f, 0.0f);
    }
    return radiance;
}
float linear_to_srgb(float x) {
    if(x <= 0.0031308f) {
        return 12.92f * x;
    }
    return 1.055f * pow(x, 1.0f / 2.4f) - 0.055f;
};
uint state[img_width][img_height];
int main() {
    Init();
    float dx = 1.0f / img_width;
    float dy = 1.0f / img_height;
    Begin();
    for(int x = 0; x < img_width; ++x) {
        for(int y = 0; y < img_height; ++y) {
            state[x][y] = tea(x, y);
        }
    }
    int spp = 256;
    for(int T = 0; T < spp; ++T) {
        cout << spp - T << " samples left" << endl;
        for(int x = 0; x < img_width; ++x) {
            for(int y = 0; y < img_height; ++y) {
                float rx = lcg(state[x][y]) / 2;
                float ry = lcg(state[x][y]) / 2;
                auto ray = camera.CastRay(Vec2(-dx * (x - img_width / 2 + rx) * 2, -dy * (y - img_height / 2 + ry) * 2));
                Vec3 p = ray.first, d = ray.second;
                Vec3 c = mc(p, d, state[x][y]);
                int bias = (y * img_width + x) * 4;
                img[bias + 0] += c.x;
                img[bias + 1] += c.y;
                img[bias + 2] += c.z;
            }
        }
    }
    for(int i = 0; i < img_height * img_width * 4; ++i) {
        img[i] = linear_to_srgb(img[i] / spp);
    }
    End(to_string(spp) + "spp");
    WriteToPPM("cornellbox.ppm");
    return 0;
}
