// -----------------------------------------------------------------------------
// Copyright  : (C) 2014-2017 Andreas-C. Bernstein
// License    : MIT (see the file LICENSE)
// Maintainer : Andreas-C. Bernstein <andreas.bernstein@uni-weimar.de>
// Stability  : experimental
//
// Renderer
// -----------------------------------------------------------------------------

#ifndef BUW_RENDERER_HPP
#define BUW_RENDERER_HPP
#define _USE_MATH_DEFINES

#include "color.hpp"
#include "pixel.hpp"
#include "ppmwriter.hpp"
#include "scene.hpp"
#include "shape.hpp"
#include <string>
#include <glm/glm.hpp>

class Renderer
{
public:
  Renderer(unsigned w, unsigned h, std::string const& file);

  void render(Scene const& scene, Camera const& camera);
  void write(Pixel const& p);
  Ray make_cam_ray(Pixel const& p, Camera const& camera, float distance);
  inline std::vector<Color> const& color_buffer() const
  {
    return color_buffer_;
  }

private:
  unsigned width_;
  unsigned height_;
  std::vector<Color> color_buffer_;
  std::string filename_;
  PpmWriter ppm_;
};

Color shade(HitPoint& shadePoint, Scene const& sdfScene);
Color raytrace(Ray const& ray, Scene const& sdfScene);


// Transformations 
Ray transformRay(glm::mat4 const& mat, Ray const& ray);
mat4 translate_vec(vec3 const& translation);
mat4 scale_vec(vec3 const& scale);
mat4 rotate_vec(float angle, vec3 const& direction);

#endif // #ifndef BUW_RENDERER_HPP
