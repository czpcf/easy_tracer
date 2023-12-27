#include "texture.h"

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
}

Texture::Texture(unsigned int width_t, unsigned int height_t, float* data_t, std::string name_t) {
    simple = false;
    width = width_t;
    height = height_t;
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

Vec4 Texture::GetRec(unsigned int x, unsigned int y) {
    if(simple) {
        return simple_c;
    }
    x %= width;
    y %= height;
    int bias = (y * width + x) * 4;
    return Vec4(data[bias + 0], data[bias + 1], data[bias + 2], data[bias + 3]);
}
