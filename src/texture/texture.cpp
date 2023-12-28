#include "texture.h"
#include <string>

Texture::Texture() {
}

Texture::~Texture() {
    if (data != nullptr){
        delete[] data;
    }
}

Texture::Texture(Vec4 c) {
    simple = true;
    simple_c = c;
    width = height = 0;
}

Texture::Texture(unsigned int width_t, unsigned int height_t, float* data_t, std::string name_t) {
    simple = false;
    width = width_t;
    height = height_t;
    dx = 1.0f / width;
    dy = 1.0f / height;
    name = name_t;
    data = new float[width * height * 4];
    for(int i = 0; i < width * height * 4; ++i) {
        data[i] = data_t[i];
    }
}

void Texture::Init(unsigned int width_t, unsigned int height_t, float* data_t, std::string name_t) {
    simple = false;
    width = width_t;
    height = height_t;
    dx = 1.0f / width;
    dy = 1.0f / height;
    name = name_t;
    data = new float[width * height * 4];
    for(int i = 0; i < width * height * 4; ++i) {
        data[i] = data_t[i];
    }
}

void Texture::Flip(bool horizontal, bool vertical) {
    using namespace std;
    if(horizontal) {
        for(int y = 0; y < height; ++y) {
            for(int x = 0; x < width / 2; ++x) {
                int nx = width - x - 1;
                swap(data[(width * y + x) * 4 + 0], data[(width * y + nx) * 4 + 0]);
                swap(data[(width * y + x) * 4 + 1], data[(width * y + nx) * 4 + 1]);
                swap(data[(width * y + x) * 4 + 2], data[(width * y + nx) * 4 + 2]);
                swap(data[(width * y + x) * 4 + 3], data[(width * y + nx) * 4 + 3]);
            }
        }
    }
    if(vertical) {
        for(int x = 0; x < width; ++x) {
            for(int y = 0; y < height / 2; ++y) {
                int ny = height - y - 1;
                swap(data[(width * y + x) * 4 + 0], data[(width * ny + x) * 4 + 0]);
                swap(data[(width * y + x) * 4 + 1], data[(width * ny + x) * 4 + 1]);
                swap(data[(width * y + x) * 4 + 2], data[(width * ny + x) * 4 + 2]);
                swap(data[(width * y + x) * 4 + 3], data[(width * ny + x) * 4 + 3]);
            }
        }
    }
}

// unsafe
Vec4 Texture::GetFast(unsigned int x, unsigned int y) {
    if(simple) {
        return simple_c;
    }
    unsigned int bias = (y * width + x) * 4;
    return Vec4(data[bias + 0], data[bias + 1], data[bias + 2], data[bias + 3]);
}

Vec4 Texture::Get(unsigned int x, unsigned int y) {
    if(simple) {
        return simple_c;
    }
    using namespace std;
    x = min(width - 1, x);
    y = min(height - 1, y);
    int bias = (y * width + x) * 4;
    return Vec4(data[bias + 0], data[bias + 1], data[bias + 2], data[bias + 3]);
}

Vec4 Texture::Get(Vec2 uv) {
    if(simple) {
        return simple_c;
    }
    using namespace std;
    unsigned int x = min(width - 1, (unsigned int)(uv.x * width));
    unsigned int y = min(height - 1, (unsigned int)(uv.y * height));
    int bias = (y * width + x) * 4;
    return Vec4(data[bias + 0], data[bias + 1], data[bias + 2], data[bias + 3]);
}

Vec4 Texture::GetRec(unsigned int x, unsigned int y) {
    if(simple) {
        return simple_c;
    }
    x %= width;
    y %= height;
    int bias = (y * width + x) * 4;
    return Vec4(data[bias + 0], data[bias + 1], data[bias + 2], data[bias + 3]);
}

Vec4 Texture::GetRec(Vec2 uv) {
    if(simple) {
        return simple_c;
    }
    unsigned int x = (unsigned int)(uv.x * width) % width; // slow here.......
    unsigned int y = (unsigned int)(uv.y * height) % height;
    int bias = (y * width + x) * 4;
    return Vec4(data[bias + 0], data[bias + 1], data[bias + 2], data[bias + 3]);
}

Vec4 Texture::GetBilinear(Vec2 uv) {
    if(simple) {
        return simple_c;
    }
    unsigned int x = (unsigned int)(uv.x * width);
    unsigned int y = (unsigned int)(uv.y * height);
    float alpha = uv.x / dx - (unsigned int)(uv.x / dx);
    float beta = uv.y / dy - (unsigned int)(uv.y / dy);
    unsigned int nx = x + 1, ny = y + 1;
    if(nx >= width) {
        nx = width - 1;
    }
    if(ny >= height) {
        ny = height - 1;
    }
    int bias = (y * width + x) * 4;
    Vec4 A = Vec4(data[bias + 0], data[bias + 1], data[bias + 2], data[bias + 3]);
    bias = (y * width + nx) * 4;
    Vec4 B = Vec4(data[bias + 0], data[bias + 1], data[bias + 2], data[bias + 3]);
    bias = (ny * width + x) * 4;
    Vec4 C = Vec4(data[bias + 0], data[bias + 1], data[bias + 2], data[bias + 3]);
    bias = (ny * width + nx) * 4;
    Vec4 D = Vec4(data[bias + 0], data[bias + 1], data[bias + 2], data[bias + 3]);
    return (A * (1.0f - alpha) + B * alpha) * (1.0f - beta) + (C * (1.0f - alpha) + D * alpha) * beta;
}

Vec4 Texture::GetBilinearRec(Vec2 uv) {
    if(simple) {
        return simple_c;
    }
    unsigned int x = (unsigned int)(uv.x * width) % width;
    unsigned int y = (unsigned int)(uv.y * height) % height;
    float alpha = uv.x / dx - (unsigned int)(uv.x / dx);
    float beta = uv.y / dy - (unsigned int)(uv.y / dy);
    unsigned int nx = x + 1, ny = y + 1;
    if(nx >= width) {
        nx -= width;
    }
    if(ny >= height) {
        ny -= height;
    }
    int bias = (y * width + x) * 4;
    Vec4 A = Vec4(data[bias + 0], data[bias + 1], data[bias + 2], data[bias + 3]);
    bias = (y * width + nx) * 4;
    Vec4 B = Vec4(data[bias + 0], data[bias + 1], data[bias + 2], data[bias + 3]);
    bias = (ny * width + x) * 4;
    Vec4 C = Vec4(data[bias + 0], data[bias + 1], data[bias + 2], data[bias + 3]);
    bias = (ny * width + nx) * 4;
    Vec4 D = Vec4(data[bias + 0], data[bias + 1], data[bias + 2], data[bias + 3]);
    return (A * (1.0f - alpha) + B * alpha) * (1.0f - beta) + (C * (1.0f - alpha) + D * alpha) * beta;
}

unsigned int Texture::GetW() {
    return width;
}

unsigned int Texture::GetH() {
    return height;
}

std::string Texture::GetName() {
    return name;
}
