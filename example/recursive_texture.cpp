#include <iostream>
#include <fstream>
#include <chrono>
#include <random>
#include "../src/tensor/vec3.h"
#include "../src/geometry/triangle.h"
#include "../src/geometry/sphere.h"
#include "../src/texture/texture.h"
#include "../src/accel/accel_naive.h"
#include "../src/accel/accel_kdtree.h"
#include "../src/camera/camera_pinhole.h"
#include "../src/const.h"
using namespace std;
const int width = 32;
const int height = 32;
const int img_width = 1024;
const int img_height = 1024;
float data_t[width * height * 4];
float img[img_width * img_height * 4];
Texture texture;
AccelNaive accel;
AccelKDtree accel_kdtree;
CameraPinhole camera;
vector<Triangle> triangles;
vector<Sphere> spheres;
vector<Geometry*> shapes;
vector<Vec2> uvs;
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
            p << int(img[bias + 0] * 255) << ' '
              << int(img[bias + 1] * 255) << ' '
              << int(img[bias + 2] * 255) << ' ';
        }
    }
    p.close();
}
void Init() {
    for(int x = 0; x < width; ++x) {
        for(int y = 0; y < height; ++y) {
            int bias = (y * width + x) * 4;
            data_t[bias + 0] = 1.0f;
            data_t[bias + 1] = 1.0f;
            data_t[bias + 2] = 1.0f;
            data_t[bias + 3] = 1.0f;
        }
    }
    int x = 0;
    for(int y = 0; y < height; ++y) {
        int bias = (y * width + x) * 4;
        x = (x + 1) % width;
        data_t[bias + 0] = 0.0f;
        data_t[bias + 1] = 0.0f;
        data_t[bias + 2] = 0.0f;
        data_t[bias + 3] = 1.0f;
        bias = (y * width + x) * 4;
        x = (x + 1) % width;
        data_t[bias + 0] = 0.0f;
        data_t[bias + 1] = 0.0f;
        data_t[bias + 2] = 0.0f;
        data_t[bias + 3] = 1.0f;
    }
    texture.Init(width, height, data_t, "texture");
    Vec3 A(-256, -256, 0), B(256, -256, 0), C(256, 256, 0), D(-256, 256, 0);
    Vec2 uv1(0, 0), uv2(512, 0), uv3(512, 512), uv4(0, 512);

    triangles.push_back(Triangle(A, B, C));
    uvs.push_back(uv1);
    uvs.push_back(uv2);
    uvs.push_back(uv3);

    triangles.push_back(Triangle(A, C, D));
    uvs.push_back(uv1);
    uvs.push_back(uv2);
    uvs.push_back(uv3);

    spheres.push_back(Sphere(Vec3(0.0f, 0.0f, 1.0f), Vec3(cos(PI / 6), sin(PI / 6), 0.0f), Vec3(-50.0f, 0.0f, 100.0f), 50.0f));
    spheres.push_back(Sphere(Vec3(0.0f, 0.0f, 1.0f), Vec3(1.0f, 0.0f, 0.0f), Vec3(100.0f, 200.0f, 120.0f), 50.0f));
    for(int i = 0; i < triangles.size(); ++i) {
        shapes.push_back(&triangles[i]);
    }
    for(int i = 0; i < spheres.size(); ++i) {
        shapes.push_back(&spheres[i]);
    }
    accel.Init(shapes);
    accel_kdtree.Init(shapes);
    float dep_t = 1.0f;
    float width_t = 1.0f;
    float height_t = 1.0f;
    Vec3 z_axis(0.0f, 0.0f, 1.0f);
    Vec3 x_axis(1.0f, 0.0f, 0.0f);
    Vec3 origin(0.0f, -256.0f, 3.0f);
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
    cout << msg << " time used: " << (double)(duration.count()) / 1000 << " ms" << endl;
}

int main() {
    Init();
    Vec3 p(0, -256, 3);
    float dx = 1.0f / img_width;
    float dy = 1.0f / img_height;
    Begin();
    for(int x = 0; x < img_width; ++x) {
        for(int y = 0; y < img_height; ++y) {
            auto ray = camera.CastRay(Vec2(-dx * (x - img_width / 2) * 2, -dy * (y - img_height / 2) * 2));
            Vec3 p = ray.first, d = ray.second;
//            auto res = accel_kdtree.Inter(p, d);
            auto res = accel.Inter(p, d);
            int id = res.first;
            if(id != -1) {
                Vec2 uv;
                if(id < triangles.size()) {
                    uv = triangles[id].Interpolate(res.second.first, uvs[id * 3 + 0], uvs[id * 3 + 1], uvs[id * 3 + 2]);
                }
                else {
                    uv = spheres[id - triangles.size()].GetUV(res.second.first) * 16.0f;
                }
                Vec4 c = texture.GetRec(uv);
                int bias = (y * img_width + x) * 4;
                img[bias + 0] = c.x;
                img[bias + 1] = c.y;
                img[bias + 2] = c.z;
            }
        }
    }
    End("1spp");
    WriteToPPM("recursive_texture_1spp.ppm");
    Begin();
    for(int x = 0; x < img_width; ++x) {
        for(int y = 0; y < img_height; ++y) {
            Vec4 sum;
            for(int i = 0; i < 16; ++i) {
                auto ray = camera.CastRay(Vec2(-dx * (R() / 2 + x - img_width / 2) * 2, -dy * (R() / 2 + y - img_height / 2) * 2));
                Vec3 p = ray.first, d = ray.second;
//                auto res = accel_kdtree.Inter(p, d);
                auto res = accel.Inter(p, d);
                int id = res.first;
                if(id != -1) {
                    Vec2 uv;
                    if(id < triangles.size()) {
                        uv = triangles[id].Interpolate(res.second.first, uvs[id * 3 + 0], uvs[id * 3 + 1], uvs[id * 3 + 2]);
                    }
                    else {
                        uv = spheres[id - triangles.size()].GetUV(res.second.first) * 16.0f;
                    }
                    Vec4 c = texture.GetRec(uv);
                    sum.x += c.x;
                    sum.y += c.y;
                    sum.z += c.z;
                }
            }
            int bias = (y * img_width + x) * 4;
            img[bias + 0] = sum.x / 16;
            img[bias + 1] = sum.y / 16;
            img[bias + 2] = sum.z / 16;
        }
    }
    End("16spp");
    WriteToPPM("recursive_texture_16spp.ppm");
    Begin();
    for(int x = 0; x < img_width; ++x) {
        for(int y = 0; y < img_height; ++y) {
            Vec4 sum;
            for(int i = 0; i < 16; ++i) {
                auto ray = camera.CastRay(Vec2(-dx * (R() / 2 + x - img_width / 2) * 2, -dy * (R() / 2 + y - img_height / 2) * 2));
                Vec3 p = ray.first, d = ray.second;
//                auto res = accel_kdtree.Inter(p, d);
                auto res = accel.Inter(p, d);
                int id = res.first;
                if(id != -1) {
                    Vec2 uv;
                    if(id < triangles.size()) {
                        uv = triangles[id].Interpolate(res.second.first, uvs[id * 3 + 0], uvs[id * 3 + 1], uvs[id * 3 + 2]);
                    }
                    else {
                        uv = spheres[id - triangles.size()].GetUV(res.second.first) * 16.0f;
                    }
                    Vec4 c = texture.GetBilinearRec(uv);
                    sum.x += c.x;
                    sum.y += c.y;
                    sum.z += c.z;
                }
            }
            int bias = (y * img_width + x) * 4;
            img[bias + 0] = sum.x / 16;
            img[bias + 1] = sum.y / 16;
            img[bias + 2] = sum.z / 16;
        }
    }
    End("16spp bilinear");
    WriteToPPM("recursive_texture_16spp_bilinear.ppm");
    Begin();
    for(int x = 0; x < img_width; ++x) {
        for(int y = 0; y < img_height; ++y) {
            Vec4 sum;
            for(int i = 0; i < 64; ++i) {
                auto ray = camera.CastRay(Vec2(-dx * (R() / 2 + x - img_width / 2) * 2, -dy * (R() / 2 + y - img_height / 2) * 2));
                Vec3 p = ray.first, d = ray.second;
//                auto res = accel_kdtree.Inter(p, d);
                auto res = accel.Inter(p, d);
                int id = res.first;
                if(id != -1) {
                    Vec2 uv;
                    if(id < triangles.size()) {
                        uv = triangles[id].Interpolate(res.second.first, uvs[id * 3 + 0], uvs[id * 3 + 1], uvs[id * 3 + 2]);
                    }
                    else {
                        uv = spheres[id - triangles.size()].GetUV(res.second.first) * 16.0f;
                    }
                    Vec4 c = texture.GetBilinearRec(uv);
                    sum.x += c.x;
                    sum.y += c.y;
                    sum.z += c.z;
                }
            }
            int bias = (y * img_width + x) * 4;
            img[bias + 0] = sum.x / 64;
            img[bias + 1] = sum.y / 64;
            img[bias + 2] = sum.z / 64;
        }
    }
    End("64spp");
    WriteToPPM("recursive_texture_64spp_bilinear.ppm");
    return 0;
}
