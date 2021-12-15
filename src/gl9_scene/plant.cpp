#include "plant.h"
#include "scene.h"


#include <shaders/water_vert_glsl.h>
#include <shaders/water_frag_glsl.h>
#include <iostream>
#include <fstream>



// shared resources
std::unique_ptr<ppgso::Mesh> Plant::mesh;
std::unique_ptr<ppgso::Texture> Plant::texture;
std::unique_ptr<ppgso::Shader> Plant::shader;

Plant::Plant() {

  // Initialize static resources if needed
  if (!shader) shader = std::make_unique<ppgso::Shader>(water_vert_glsl, water_frag_glsl);
  if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("green.bmp"));
  if (!mesh) mesh = std::make_unique<ppgso::Mesh>("plant1.obj");

}


bool Plant::update(Scene &scene, float dt) {
  if (plant2)
  {
  shader = std::make_unique<ppgso::Shader>(water_vert_glsl, water_frag_glsl);
  texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("green.bmp"));
  mesh = std::make_unique<ppgso::Mesh>("plant2.obj");
  this->scale = {.2,.2,.2};
  }
  generateModelMatrix();
  return true;
}

void Plant::render(Scene &scene) {
  shader->use();

  // Set up light
  shader->setUniform("LightDirection", scene.lightDirection);
  shader->setUniform("LightColor", scene.lightColor);

  // use camera
  shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
  shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

  // render mesh
  shader->setUniform("ModelMatrix", modelMatrix);
  shader->setUniform("Transparency",1);
  shader->setUniform("TimeOffset", (glfwGetTime() * 2*3.14159 * .75)); //vodna animacia
  shader->setUniform("Texture", *texture);
  mesh->render();
}
