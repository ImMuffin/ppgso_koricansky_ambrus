//
// Created by adyko on 12/2/2021.
//

#include "aquarium.h"
#include "scene.h"
#include "water.h"


#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

// shared resources
std::unique_ptr<ppgso::Mesh> Aquarium::mesh;
std::unique_ptr<ppgso::Texture> Aquarium::texture;
std::unique_ptr<ppgso::Shader> Aquarium::shader;

Aquarium::Aquarium() {
<<<<<<< HEAD
  // Scale the default model
  scale *= 0.5;
  position.x = 2;

  // Initialize static resources if needed
  if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
  if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("aquarium.bmp"));
  if (!mesh) mesh = std::make_unique<ppgso::Mesh>("aqwall.obj");
=======
    //Scale the default model
    scale *= 3.0f;
>>>>>>> e4fca13eb864b5981bf231d04503f341dfd623af

    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("aquarium.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("aquarium.obj");
}

bool Aquarium::update(Scene &scene, float dt) {
<<<<<<< HEAD
  rotation.x = (float) glfwGetTime();
  rotation.y = (float) glfwGetTime();
  generateModelMatrix();
  return true;
=======


    generateModelMatrix();
    return true;
>>>>>>> e4fca13eb864b5981bf231d04503f341dfd623af
}

void Aquarium::render(Scene &scene) {
    shader->use();

    // Set up light
    shader->setUniform("LightDirection", scene.lightDirection);

<<<<<<< HEAD
  // use camera
  shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
  shader->setUniform("ViewMatrix", scene.camera->viewMatrix);
  shader->setUniform("ViewMatrix", scene.camera->viewMatrix);
  shader->setUniform("Transparency",0.3f);
=======
    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);
>>>>>>> e4fca13eb864b5981bf231d04503f341dfd623af

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);

    // set transparency
    shader->setUniform("Transparency", 0.0f);
    shader->setUniform("TimeOffset", (glfwGetTime() * 2*3.14159 * .75));
    mesh->render();
}

