#include <iostream>
#include <fstream>
#include <chrono>
#include <random>
#include "../src/tensor/vec3.h"
#include "../src/geometry/triangle.h"
#include "../src/texture/texture.h"
#include "../src/accel/accel_naive.h"
#include "../src/accel/accel_kdtree.h"
using namespace std;
const int width = 32;
const int height = 32;
const int img_width = 1024;
const int img_height = 1024;
float data_t[width * height * 4];
float img[img_width * img_height * 4];
Texture texture;
AccelNaive accel_naive;
AccelKDtree accel_kdtree;
vector<Triangle> triangles;
vector<Geometry*> shapes;
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
    triangles.push_back(Triangle(A, B, C, uv1, uv2, uv3));
    triangles.push_back(Triangle(A, C, D, uv1, uv3, uv4));
    for(int i = 0; i < triangles.size(); ++i) {
        shapes.push_back(&triangles[i]);
    }
    accel_naive.Init(shapes);
    accel_kdtree.Init(shapes);
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
    float f = 1.0f;
    Begin();
    for(int x = 0; x < img_width; ++x) {
        for(int y = 0; y < img_height; ++y) {
            Vec3 d(dx * (x - img_width / 2), f, dy * (y - img_height / 2));
            d.Normalize();
            auto res = accel_kdtree.Inter(p, d);
//            auto res2 = accel_naive.Inter(p, d);
            int id = res.first;
            if(id != -1) {
                Vec2 uv = shapes[id]->GetUVInter(res.second.first);
                Vec4 c = texture.GetRec(uv);
                int bias = (y * img_width + x) * 4;
                img[bias + 0] = c.x;
                img[bias + 1] = c.y;
                img[bias + 2] = c.z;
            }
        }
    }
    End("1spp");
    WriteToPPM("recursive_texture.ppm");
    Begin();
    for(int x = 0; x < img_width; ++x) {
        for(int y = 0; y < img_height; ++y) {
            Vec4 sum;
            for(int ddx = 0; ddx < 4; ++ddx) {
                for(int ddy = 0; ddy < 4; ++ddy) {
                    Vec3 d(dx * (-0.375f + 0.25f * ddx + (x - img_width / 2)), f, dy * (-0.375f + 0.25f * ddy + (y - img_height / 2)));
                    d.Normalize();
                    auto res = accel_kdtree.Inter(p, d);
//                    auto res2 = accel_naive.Inter(p, d);
                    int id = res.first;
                    if(id != -1) {
                        Vec2 uv = shapes[id]->GetUVInter(res.second.first);
                        Vec4 c = texture.GetRec(uv);
                        sum.x += c.x;
                        sum.y += c.y;
                        sum.z += c.z;
                    }
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
            for(int ddx = 0; ddx < 4; ++ddx) {
                for(int ddy = 0; ddy < 4; ++ddy) {
                    Vec3 d(dx * (-0.375f + 0.25f * ddx + (x - img_width / 2)), f, dy * (-0.375f + 0.25f * ddy + (y - img_height / 2)));
                    d.Normalize();
                    auto res = accel_kdtree.Inter(p, d);
//                    auto res2 = accel_naive.Inter(p, d);
                    int id = res.first;
                    if(id != -1) {
                        Vec2 uv = shapes[id]->GetUVInter(res.second.first);
                        Vec4 c = texture.GetBilinearRec(uv);
                        sum.x += c.x;
                        sum.y += c.y;
                        sum.z += c.z;
                    }
                }
            }
            int bias = (y * img_width + x) * 4;
            img[bias + 0] = sum.x / 16;
            img[bias + 1] = sum.y / 16;
            img[bias + 2] = sum.z / 16;
        }
    }
    End("16spp with bilinear interpolation");
    WriteToPPM("recursive_texture_16spp_bi.ppm");
    Begin();
    for(int x = 0; x < img_width; ++x) {
        for(int y = 0; y < img_height; ++y) {
            Vec4 sum;
            for(int dd = 0; dd < 64; ++dd) {
                Vec3 d(dx * (R() / 2 + (x - img_width / 2)), f, dy * (R() / 2 + (y - img_height / 2)));
                d.Normalize();
                auto res = accel_kdtree.Inter(p, d);
//                auto res2 = accel_naive.Inter(p, d);
                int id = res.first;
                if(id != -1) {
                    Vec2 uv = shapes[id]->GetUVInter(res.second.first);
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
    End("64spp with bilinear interpolation");
    WriteToPPM("recursive_texture_64spp_bilinear.ppm");
    return 0;
}
