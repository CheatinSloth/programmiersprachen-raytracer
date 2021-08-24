// -----------------------------------------------------------------------------
// Copyright  : (C) 2014-2017 Andreas-C. Bernstein
// License    : MIT (see the file LICENSE)
// Maintainer : Andreas-C. Bernstein <andreas.bernstein@uni-weimar.de>
// Stability  : experimental
//
// Renderer
// -----------------------------------------------------------------------------

#include "renderer.hpp"


Renderer::Renderer(unsigned w, unsigned h, std::string const& file)
  : width_(w)
  , height_(h)
  , color_buffer_(w*h, Color{0.0, 0.0, 0.0})
  , filename_(file)
  , ppm_(width_, height_)
{}

void Renderer::render()
{
  std::size_t const checker_pattern_size = 20;

  for (unsigned y = 0; y < height_; ++y) {
    for (unsigned x = 0; x < width_; ++x) {
      Pixel p(x,y);
      if ( ((x/checker_pattern_size)%2) != ((y/checker_pattern_size)%2)) {
        p.color = Color{0.0f, 1.0f, float(x)/height_};
      } else {
        p.color = Color{1.0f, 0.0f, float(y)/width_};
      }

      write(p);
    }
  }
  ppm_.save(filename_);
}

void Renderer::write(Pixel const& p)
{
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

// TODO: Offset ?
// TODO: Trace algo
// TODO: shadowtrace algo

Color shade(HitPoint const& shadePoint, Scene const& sdfScene) {
    Color finalShade{
        shadePoint.mat->ka.r * sdfScene.baseLighting.r,
        shadePoint.mat->ka.g * sdfScene.baseLighting.g,
        shadePoint.mat->ka.b * sdfScene.baseLighting.b
    };
    
    float dist;
    Ray shadowRay{ shadePoint.touchPoint, {0.0f, 0.0f, 0.0f} };

    for (const auto& [lightName, light] : sdfScene.lightSources) {

        shadowRay.direction = light.position - shadowRay.origin;

        for (const auto& [name, shape] : sdfScene.sceneElements) {
            HitPoint intersectPoint = shape->intersect(shadowRay, dist);
            if (intersectPoint.hit == true) {
                break;
            }
            else {
                // Lambert Rule (is this the right place for this?)
                vec3 shadowRayNormal = glm::normalize(shadowRay.direction);
                vec3 norm = shadePoint.normal;

                finalShade.r += light.luminance * shadePoint.mat->kd.r * (glm::dot(shadowRayNormal, norm));
                finalShade.g += light.luminance * shadePoint.mat->kd.g * (glm::dot(shadowRayNormal, norm));
                finalShade.b += light.luminance * shadePoint.mat->kd.b * (glm::dot(shadowRayNormal, norm));
            }
        }
    }
    // HDR Color correcting

    finalShade.r = finalShade.r / (finalShade.r + 1);
    finalShade.g = finalShade.g / (finalShade.g + 1);
    finalShade.b = finalShade.b / (finalShade.b + 1);

    return finalShade;
}

Color raytrace(Ray const& ray, Scene const& sdfScene) {
    HitPoint temp;
    float dist = INFINITY;
    HitPoint minHit;
    Color finalShade;

    for (const auto& [name, shape] : sdfScene.sceneElements) {
        temp = shape->intersect(ray, dist);
        if (temp.dist < minHit.dist) {
            minHit = temp;
        }

        if (minHit.hit == true) {
            finalShade = shade(minHit, sdfScene);
        }
    }
    return finalShade;
}