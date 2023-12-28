#include <iostream>
#include <fstream>
#include "../src/tensor/vec3.h"
#include "../src/texture/texture.h"
using namespace std;
const int width = 4;
const int height = 4;
const int img_width = 512;
const int img_height = 256;
float data_t[width * height * 4];
float img[img_width * img_height * 4];
Texture texture;
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
    float dx = 1.0f / width;
    float dy = 1.0f / height;
    for(int x = 0; x < width; ++x) {
        for(int y = 0; y < height; ++y) {
            int bias = (y * width + x) * 4;
            data_t[bias + 0] = dx * x;
            data_t[bias + 1] = dy * y;
            data_t[bias + 2] = 0.7f;
            data_t[bias + 3] = 1.0f;
        }
    }
    texture.Init(width, height, data_t, "texture");
}
int main() {
    Init();
    for(int x = 0; x < img_width; ++x) {
        for(int y = 0; y < img_height; ++y) {
            Vec4 c = texture.Get(Vec2(float(x) / img_width, float(y) / img_height));
            int bias = (y * img_width + x) * 4;
            img[bias + 0] = c.x;
            img[bias + 1] = c.y;
            img[bias + 2] = c.z;
        }
    }
    WriteToPPM("texture_simple.ppm");
    for(int x = 0; x < img_width; ++x) {
        for(int y = 0; y < img_height; ++y) {
            Vec4 c = texture.GetBilinear(Vec2(float(x) / img_width, float(y) / img_height));
            int bias = (y * img_width + x) * 4;
            img[bias + 0] = c.x;
            img[bias + 1] = c.y;
            img[bias + 2] = c.z;
        }
    }
    WriteToPPM("texture_bilinear.ppm");
    for(int x = 0; x < img_width; ++x) {
        for(int y = 0; y < img_height; ++y) {
            Vec4 c = texture.GetBilinearRec(Vec2(float(x) / img_width * 4, float(y) / img_height * 4));
            int bias = (y * img_width + x) * 4;
            img[bias + 0] = c.x;
            img[bias + 1] = c.y;
            img[bias + 2] = c.z;
        }
    }
    WriteToPPM("texture_bilinear_recursive.ppm");
    return 0;
}
