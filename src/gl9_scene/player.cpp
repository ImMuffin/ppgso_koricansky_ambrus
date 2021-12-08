#include "player.h"
#include "scene.h"


#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>
#include <shaders/convolution_frag_glsl.h>
#include <shaders/convolution_vert_glsl.h>
#include <shaders/water_vert_glsl.h>
#include <shaders/water_frag_glsl.h>
#include <iostream>
#include <fstream>



// shared resources
std::unique_ptr<ppgso::Mesh> Player::mesh;
std::unique_ptr<ppgso::Texture> Player::texture;
std::unique_ptr<ppgso::Shader> Player::shader;

Player::Player() {
  // Scale the default model
  //scale *= 1;

  // Initialize static resources if needed
  if (!shader) shader = std::make_unique<ppgso::Shader>(water_vert_glsl, water_frag_glsl);
  if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("fish.bmp"));
  if (!mesh) mesh = std::make_unique<ppgso::Mesh>("fish.obj");

}

bool Player::update(Scene &scene, float dt) {
  // calculate forward
    forward.x = sin(rotation.z)*sin(rotation.x);
    forward.y = cos(rotation.x);
    forward.z = sin(rotation.x)*cos(rotation.z);
  // Keyboard controls
  if(scene.keyboard[GLFW_KEY_LEFT]) {
    rotation.z += ppgso::PI/2.0f * dt;
  } else if(scene.keyboard[GLFW_KEY_RIGHT]) {
    rotation.z -= ppgso::PI/2.0f * dt;
  }
  
  if(scene.keyboard[GLFW_KEY_UP])
  {
    position -= forward * dt;
  } else if(scene.keyboard[GLFW_KEY_DOWN])
  {
    position += forward * dt;
  }

  if(scene.keyboard[GLFW_KEY_PAGE_UP])
  {
    rotation.x += ppgso::PI/2.0f * dt;
  }
  else if (scene.keyboard[GLFW_KEY_PAGE_DOWN])
  {
    rotation.x -= ppgso::PI/2.0f * dt;
  }

  if (scene.keyboard[GLFW_KEY_B] && record == false)
  {
      if (!f.is_open()) f.open("../data/matrix.txt");
      keyframeTime = 0;
      record = true;
  }

  if (scene.keyboard[GLFW_KEY_N] && record == true)
  {
      if (record) {
          record = false;
          f.close();
      }
  }

  if (scene.keyboard[GLFW_KEY_M] && playback == false)
  {
      if (!p.is_open()) p.open("../data/matrix.txt");
      keyframeTime = (((int) glfwGetTime()) * 2) + (( (int) (glfwGetTime() * 10)) % 10) / 5;
      playbackMovement(p);
      firstModel = modelMatrix;
      playbackMovement(p);
      secondModel = modelMatrix;
      modelMatrix = firstModel;
      playback = true;
  }

  if (playback){
      int halfSeconds = (((int) glfwGetTime()) * 2) + (( (int) (glfwGetTime() * 10)) % 10) / 5;
      if(halfSeconds > keyframeTime)
      {
        keyframeTime = halfSeconds;
        firstModel = secondModel;
        if (!playbackMovement(p))
        {
          playback = false;
          p.close();
        }
        secondModel = modelMatrix;
        modelMatrix = firstModel;
      }
      else
      {
        modelMatrix = firstModel + (secondModel - firstModel) * (( (float) ( (int) (glfwGetTime() * 100) % 50) * 2) / 100); 
      }
  } else {
      generateModelMatrix();
  }

  if (record) {
      int halfSeconds = (((int) glfwGetTime()) * 2) + (( (int) (glfwGetTime() * 10)) % 10) / 5;
      if(halfSeconds > keyframeTime)
      {
        keyframeTime = halfSeconds;
        recordMovement(f);
      }
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
  //
  shader->setUniform("Transparency",1.0f);
  shader->setUniform("TimeOffset", (glfwGetTime() * 2*3.14159 * .75)); //vodna animacia
  glDisable(GL_CULL_FACE); //ryba nemala plutvy
  mesh->render();
  glEnable(GL_CULL_FACE);
}

void Player::onClick(Scene &scene) {
  std::cout << "Player has been clicked!" << std::endl;
}
