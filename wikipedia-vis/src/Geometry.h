#ifndef VTA_GEOMETRY
#define VTA_GEOMETRY

#include <nanovg.h>
#include <nanovg_gl.h>

namespace vta
{

struct Circle
{
    float x_pos;
    float y_pos;
    float radius;

};

struct Line
{
    float x_start;
    float y_start;

    float x_end;
    float y_end;

};

} // namespace vta
#endif // VTA_VIEW
