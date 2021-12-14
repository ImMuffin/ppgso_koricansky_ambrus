#include "aquarium.h"
#include "scene.h"


#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>
#include <iostream>
#include <fstream>



// shared resources
std::unique_ptr<ppgso::Mesh> Aquarium::mesh;
std::unique_ptr<ppgso::Texture> Aquarium::texture;
std::unique_ptr<ppgso::Shader> Aquarium::shader;

Aquarium::Aquarium() {
  // Scale the default model
  scale *= 0.5;
  position.x = 2;

  // Initialize static resources if needed
  if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
  if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("aquarium.bmp"));
  if (!mesh) mesh = std::make_unique<ppgso::Mesh>("aqwall.obj");

}

bool Aquarium::update(Scene &scene, float dt) {
  rotation.x = (float) glfwGetTime();
  rotation.y = (float) glfwGetTime();
  generateModelMatrix();
  return true;
}

void Aquarium::render(Scene &scene) {
  shader->use();

  // Set up light
  shader->setUniform("LightDirection", scene.lightDirection);

  // use camera
  shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
  shader->setUniform("ViewMatrix", scene.camera->viewMatrix);
  shader->setUniform("ViewMatrix", scene.camera->viewMatrix);
  shader->setUniform("Transparency",0.3f);

  // render mesh
  shader->setUniform("ModelMatrix", modelMatrix);
  shader->setUniform("Texture", *texture);
  mesh->render();
}
