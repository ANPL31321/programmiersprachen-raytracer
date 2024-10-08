// -----------------------------------------------------------------------------
// Copyright  : (C) 2014-2017 Andreas-C. Bernstein
// License    : MIT (see the file LICENSE)
// Maintainer : Andreas-C. Bernstein <andreas.bernstein@uni-weimar.de>
// Stability  : experimental
//
// Renderer
// -----------------------------------------------------------------------------

#include "renderer.hpp"

Renderer::Renderer(unsigned w, unsigned h, std::string const& file, Scene const& scene)
    : width_(w)
    , height_(h)
    , color_buffer_(w*h, Color{ 0.0, 0.0, 0.0 })
    , filename_(file)
    , ppm_(width_, height_),
    scene_{ scene }
{}

void Renderer::render()
{
    std::size_t const checker_pattern_size = 20;

    for (unsigned y = 0; y < height_; ++y) {
        for (unsigned x = 0; x < width_; ++x) {
            /*
          Pixel p(x,y);
          if ( ((x/checker_pattern_size)%2) != ((y/checker_pattern_size)%2)) {
            p.color = Color{0.0f, 1.0f, float(x)/height_};
          } else {
            p.color = Color{1.0f, 0.0f, float(y)/width_};
          }*/
            Pixel p = scene_.render_pixel(x, y);
            p.c_ldr();
            write(p);
        }
    }
    ppm_.save(filename_);
}

void Renderer::write(Pixel const& p)
{
    //make sense to use exception throw
    // flip pixels, because of opengl glDrawPixels
    size_t buf_pos = (width_*p.y + p.x);
    if (buf_pos >= color_buffer_.size() || (int)buf_pos < 0) {
        std::cerr << "Fatal Error Renderer::write(Pixel p) : "
            << "pixel out of ppm_ : "
            << (int)p.x << "," << (int)p.y
            << std::endl;

    } else {
        color_buffer_[buf_pos] = p.color;

    }
    ppm_.write(p);

}
