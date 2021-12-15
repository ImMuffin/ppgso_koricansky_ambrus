#pragma once
#include <memory>

#include <glm/glm.hpp>
#include <ppgso/ppgso.h>

class Camera {
public:
  glm::vec3 up{0,1,0};
  glm::vec3 position{8,8,-4.2f};
  glm::vec3 back{0,0,-4.2f};

  glm::mat4 viewMatrix;
  glm::mat4 projectionMatrix;

  /*!
   * Create new Camera that will generate viewMatrix and projectionMatrix based on its position, up and back vectors
   * @param fow - Field of view in degrees
   * @param ratio - Viewport screen ratio (usually width/height of the render window)
   * @param near - Distance to the near frustum plane
   * @param far - Distance to the far frustum plane
   */
  Camera(float fow = 45.0f, float ratio = 1.0f, float near = 0.1f, float far = 10.0f);

  /*!
   * Update Camera viewMatrix based on up, position and back vectors
   */
  void update();

  /*!
   * Get direction vector in world coordinates through camera projection plane
   * @param u - camera projection plane horizontal coordinate [-1,1]
   * @param v - camera projection plane vertical coordinate [-1,1]
   * @return Normalized vector from camera position to position on the camera projection plane
   */
  glm::vec3 cast(double u, double v);
};

