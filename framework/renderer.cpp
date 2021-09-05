// -----------------------------------------------------------------------------
// Copyright  : (C) 2014-2017 Andreas-C. Bernstein
// License    : MIT (see the file LICENSE)
// Maintainer : Andreas-C. Bernstein <andreas.bernstein@uni-weimar.de>
// Stability  : experimental
//
// Renderer
// -----------------------------------------------------------------------------

#include "renderer.hpp"

////////////////////////////////////////////////
//                    TODO                    
//  1. Box Dimensionen überprüfen!!!!!
//  2. Reflektion fertigstellen
//  3. Transformationen integrieren

Renderer::Renderer(unsigned w, unsigned h, std::string const& file)
  : width_(w)
  , height_(h)
  , color_buffer_(w*h, Color{0.0, 0.0, 0.0})
  , filename_(file)
  , ppm_(width_, height_)
{}

void Renderer::render(Scene const& scene, Camera const& camera)
{
  std::size_t const checker_pattern_size = 20;

  for (unsigned y = 0; y < height_; ++y) {
    for (unsigned x = 0; x < width_; ++x) {
      Pixel p(x,y);
      /*if ( ((x/checker_pattern_size)%2) != ((y/checker_pattern_size)%2)) {
        p.color = color{0.0f, 1.0f, float(x)/height_};
      }
      else {
          p.color = color{ 1.0f, 0.0f, float(y) / width_ };

      }*/
      p.color = raytrace(make_cam_ray(p, camera, camera.dist()), scene, camera, 1);
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

Color shade(HitPoint& shadePoint, Scene const& sdfScene, Camera const& cam, int recursion) {
    Color finalShade{
        shadePoint.mat->ka.r * sdfScene.baseLighting.r,
        shadePoint.mat->ka.g * sdfScene.baseLighting.g,
        shadePoint.mat->ka.b * sdfScene.baseLighting.b
    };

    int totalLuminance = 1;
    Color totalHue = { sdfScene.baseLighting };
    shadePoint.touchPoint += 0.01f * shadePoint.normal;

    Ray shadowRay{ shadePoint.touchPoint, {0.0f, 0.0f, 0.0f} };

    for (const auto& [lightName, light] : sdfScene.lightSources) {
        // Direction to light
        bool obstructed = false;
        shadowRay.direction = glm::normalize(light.position - shadePoint.touchPoint);

        for (const auto& [name, shape] : sdfScene.sceneElements) {

            // Checking if any light is obscured by other shape
            HitPoint intersectPoint = shape->intersect(shadowRay);

            // Ignoring luminance of obscured light
            if (intersectPoint.hit) {
                obstructed = true;
                break;
            }
        }
        if (obstructed) {
            
        }
        else {
            totalLuminance += light.luminance;
            totalHue.r *= light.hue.r;
            totalHue.g *= light.hue.g;
            totalHue.b *= light.hue.b;

        }
    }

    vec3 norm = glm::normalize(shadePoint.normal);
    float angle = glm::dot(shadowRay.direction, norm);
    angle = std::max(angle, 0.f);

    glm::vec4 vec4Normal = { shadowRay.direction.x, shadowRay.direction.y, shadowRay.direction.z, 0.f };
    mat4 rotateMat = glm::rotate((float)M_PI, shadePoint.normal);
    vec4Normal = rotateMat * vec4Normal;
    vec3 reflectVec = (glm::vec3{vec4Normal});

    vec3 rnormed = glm::normalize(reflectVec);
    vec3 vnormed = glm::normalize(cam.position - shadePoint.touchPoint);
    float angle1 = abs(glm::dot(rnormed, vnormed));

    // Phong illumination
    finalShade.r += (totalLuminance * totalHue.r) * ((shadePoint.mat->kd.r * angle)) + shadePoint.mat->ks.r * pow(angle1, shadePoint.mat->reflectionExponent);
    finalShade.g += (totalLuminance * totalHue.g) * ((shadePoint.mat->kd.g * angle)) + shadePoint.mat->ks.g * pow(angle1, shadePoint.mat->reflectionExponent);
    finalShade.b += (totalLuminance * totalHue.b) * ((shadePoint.mat->kd.b * angle)) + shadePoint.mat->ks.b * pow(angle1, shadePoint.mat->reflectionExponent);

    // HDR Color correcting
    finalShade.r = finalShade.r / (finalShade.r + 1);
    finalShade.g = finalShade.g / (finalShade.g + 1);
    finalShade.b = finalShade.b / (finalShade.b + 1);

    return finalShade;
}


Color raytrace(Ray const& ray, Scene const& sdfScene, Camera const& cam, int recursion) {
    bool reflect = false;
    float smollDis = INFINITY;
    HitPoint temp;
    HitPoint minHit;
    Color finalShade{ sdfScene.baseLighting };

    for (const auto& [name, shape] : sdfScene.sceneElements) {
        temp = shape->intersect(ray);
        if (temp.dist < minHit.dist && temp.hit) {
            smollDis = temp.dist;
            minHit = temp;
        }
    }

    minHit.touchPoint += 0.0001f;

    if (minHit.hit) {
        finalShade = shade(minHit, sdfScene, cam, recursion);

        /*if (minHit.mat->reflectivity > 0.f && recursion <= 10) {
            vec3 toCam = glm::normalize(-minHit.touchPoint);
            float cosine = glm::dot(toCam, minHit.normal);
            vec3 direction = toCam - (minHit.normal * cosine * 2.f);
            Color reflectedColor = raytrace({ minHit.touchPoint, direction }, sdfScene, recursion + 1);

            finalShade.r += minHit.mat->reflectivity * reflectedColor.r;
            finalShade.g += minHit.mat->reflectivity * reflectedColor.g;
            finalShade.b += minHit.mat->reflectivity * reflectedColor.b;
        }*/
        return finalShade;

    }
    else {
        return finalShade;
    }
}

Ray Renderer::make_cam_ray(Pixel const& p, Camera const& camera, float distance) {
    // Create pixel vector
    float x = (((float)p.x / (float)width_ - 0.5f) * 2.f * (float)std::tan(camera.angle / 180.0f * M_PI));
    float y = (((float)p.y  / (float)height_ - 0.5f) * 2.f * (float)std::tan(camera.angle / 180.0f * M_PI));

    vec3 rightVec = glm::cross(camera.direction, camera.up);
    vec3 directionVec = glm::normalize(x * rightVec + y * camera.up + camera.direction);

    // Create Camera movement
    mat4 camTrans{ camera.transform() };
    //return Ray{ camera.position, directionVec };
    return Ray{ transformRay(camTrans, {camera.position, {directionVec}}) };
}   

mat4 translate_vec(vec3 const& translation)
{
    glm::vec4 translation4{ translation, 1.f };
    mat4 result;

    result[0] = { 1.f, 0.f, 0.f, 0.f };
    result[1] = { 0.f, 1.f, 0.f, 0.f };
    result[2] = { 0.f, 0.f, 1.f, 0.f };
    result[3] = { translation4 };

    return result;
}

mat4 scale_vec(vec3 const& scale)
{
    mat4 result;
    result[0] = { scale.x, 0.f, 0.f, 0.f };
    result[1] = { 0.f, scale.y, 0.f, 0.f };
    result[2] = { 0.f, 0.f, scale.z, 0.f };
    result[3] = { 0.f, 0.f, 0.f, 1.f };

    return result;
}

mat4 rotate_vec(float angle, vec3 const& axis)
{
    mat4 result;

    // Checking along which axis to rotate

    // x-axis
    if (axis.x == 1.f && axis.y == 0.f && axis.z == 0.f) {
        result[0] = { 1.f, 0.f, 0.f, 0.f };
        result[1] = { 0.f, cos(angle / 2.f * M_PI / 180.f), sin(angle / 2.f * M_PI / 180.f), 0.f };
        result[2] = { 0.f, -sin(angle / 2.f * M_PI / 180.f), cos(angle / 2.f * M_PI / 180.f), 0.f };
        result[3] = { 0.f, 0.f, 0.f, 1.f };
    }
    // y-Axis
    if (axis.x == 0.f && axis.y == 1.f && axis.z == 0.f) {
        result[0] = { 0.f, cos(angle / 2.f * M_PI / 180.f), -sin(angle / 2.f * M_PI / 180.f), 0.f };
        result[1] = { 0.f, 1.f, 0.f, 0.f };
        result[2] = { 0.f, sin(angle / 2.f * M_PI / 180.f), cos(angle / 2.f * M_PI / 180.f), 0.f };
        result[3] = { 0.f, 0.f, 0.f, 1.f };
    }
    // z-Axis
    if (axis.x == 0.f && axis.y == 0.f && axis.z == 1.f) {
        result[0] = { 0.f, cos(angle / 2.f * M_PI / 180.f), sin(angle / 2.f * M_PI / 180.f), 0.f };
        result[1] = { 0.f, -sin(angle / 2.f * M_PI / 180.f), cos(angle / 2.f * M_PI / 180.f), 0.f };
        result[2] = { 0.f, 0.f, 1.f, 0.f };
        result[3] = { 0.f, 0.f, 0.f, 1.f };
    }
    return result;
}
