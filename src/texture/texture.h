#ifndef TEXTURE_H
#define TEXTURE_H
#include <string>
#include "../tensor/vec4.h"
#include "../tensor/vec3.h"
#include "../tensor/vec2.h"
class Texture {
private:
    /// @brief name of the texture
    std::string name;

    /// @brief width the the texture
    unsigned int width;

    /// @brief height the the texture
    unsigned int height;

    /// @brief data of the texure, arranged in RGBA by row
    /// 0 -> 0.0f, 255 -> 1.0f
    float* data;

    /// @brief if this texture is composed of 1 color
    bool simple;

    /// @brief the single color
    Vec4 simple_c;

public:
    Texture();
    ~Texture();

    /// @brief simple texture
    Texture(Vec4 c);

    /// @brief read data
    Texture(unsigned int width_t, unsigned int height_t, float* data_t, std::string name_t);

    /// @brief flip the texture
    void Flip(bool horizontal, bool vertical);

    /// @brief return data[x][y] dirrectly
    Vec4 GetFast(unsigned int x,unsigned  int y);

    /// @brief return data[x][y], x and y is clamped
    Vec4 Get(unsigned int x,unsigned  int y);

    /// @brief return data[x % width][y % height]
    Vec4 GetRec(unsigned int x,unsigned  int y);
};
#endif
