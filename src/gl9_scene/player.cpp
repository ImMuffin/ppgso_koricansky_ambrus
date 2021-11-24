#include "player.h"
#include "scene.h"


#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>
#include <iostream>
#include <fstream>



// shared resources
std::unique_ptr<ppgso::Mesh> Player::mesh;
std::unique_ptr<ppgso::Texture> Player::texture;
std::unique_ptr<ppgso::Shader> Player::shader;

Player::Player() {
  // Scale the default model
  scale *= 3.0f;

  // Initialize static resources if needed
  if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
  if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("diver.bmp"));
  if (!mesh) mesh = std::make_unique<ppgso::Mesh>("diver.obj");

}

bool Player::update(Scene &scene, float dt) {
  // Fire delay increment
  fireDelay += dt;
  scale.x = 0.2f;
  scale.y = 0.2f;
  scale.z = 0.2f;


  // Keyboard controls
  if(scene.keyboard[GLFW_KEY_LEFT]) {
    position.x += 10 * dt;
    rotation.z = -ppgso::PI/4.0f;
  } else if(scene.keyboard[GLFW_KEY_RIGHT]) {
    position.x -= 10 * dt;
    rotation.z = ppgso::PI/4.0f;
  } else {
    rotation.z = 0;
  }

  if (scene.keyboard[GLFW_KEY_R])
  {
        record = true;
  }

  if (scene.keyboard[GLFW_KEY_T])
  {
      if (record) {
          record = false;
          f.close();
      }
  }

  if (scene.keyboard[GLFW_KEY_P])
  {
      playback = true;
  }


  if (playback){
      if (!p.is_open()) p.open("../data/matrix.txt");
      if(!playbackMovement(p)) {
          playback = false;
          p.close();
      }
  } else {
      generateModelMatrix();
  }

  if (record) {
      if (!f.is_open()) f.open("../data/matrix.txt");
      recordMovement(f);
  }


  return true;
}

void Player::render(Scene &scene) {
  shader->use();

  // Set up light
  shader->setUniform("LightDirection", scene.lightDirection);

  // use camera
  shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
  shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

  // render mesh
  shader->setUniform("ModelMatrix", modelMatrix);
  shader->setUniform("Texture", *texture);
  mesh->render();
}

void Player::onClick(Scene &scene) {
  std::cout << "Player has been clicked!" << std::endl;
}
