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

void Renderer::render(Scene const& scene)
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

////////////////////////
/*
 TODO: 
 - Eine Test SDF zu rendern w�r vllt sinnvoll | added test sdf to repository, probelms with parsing?
 - Ich hab das Gef�hl dass shade() und raytrace() grunds�tzlich stimmen, aber mag sein dass bei
 den Normalisierungen bzw Richtungen der Vektoren Schusselfehler drin sein k�nnten | ich hab nichts gefunden was falsch aussieht, weiß aber nicht genau wo wir die rekursion reinpacken

 - Distance vergleich zwischen Lichtquelle und Objekt fehlt, k�nnten noch einbauen, dass Lichtquelle n�her als Objekt trotzdem beleuchtet | idk what you mean

 - Erweiterungen f�r Reflektion/Refraktion k�nnten wir vielleicht einbauen, falls Testrender erfolgreich ist
 - Transform Methoden w�ren auch sinnvoll

*/

Color shade(HitPoint& shadePoint, Scene const& sdfScene) {
    Color finalShade{
        shadePoint.mat->ka.r * sdfScene.baseLighting.r,
        shadePoint.mat->ka.g * sdfScene.baseLighting.g,
        shadePoint.mat->ka.b * sdfScene.baseLighting.b
    };
    
    float dist;
    Ray shadowRay{ shadePoint.touchPoint, {0.0f, 0.0f, 0.0f} };

    for (const auto& [lightName, light] : sdfScene.lightSources) {
        // Direction to light
        shadowRay.direction = light.position - shadowRay.origin;
        for (const auto& [name, shape] : sdfScene.sceneElements) {

            // Checking if any light is obscured by other shape
            HitPoint intersectPoint = shape->intersect(shadowRay, dist);

            // Ignoring luminance of obscured light
            if (intersectPoint.hit == true) {
                break;
            }
            else {

                // Lambert Rule (is this the right place for this?)
                //seems right...
                vec3 shadowRayNormal = glm::normalize(shadowRay.direction);
                vec3 norm = glm::normalize(shadePoint.normal);

                // Summation of all visible light intensities using normalized simple lighting (I = I_p * k_d * (n * I))
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
    Color finalShade{sdfScene.baseLighting};

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