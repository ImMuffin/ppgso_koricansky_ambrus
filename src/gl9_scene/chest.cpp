//
// Created by adyko on 11/18/2021.
//

#include "chest.h"
#include "scene.h"


#include <shaders/water_vert_glsl.h>
#include <shaders/water_frag_glsl.h>

// shared resources
std::unique_ptr<ppgso::Mesh> Chest::mesh;
std::unique_ptr<ppgso::Texture> Chest::texture;
std::unique_ptr<ppgso::Shader> Chest::shader;

Chest::Chest() {
    //Scale the default model
    scale *= 3.0f;

    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(water_vert_glsl, water_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("chest.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("chest.obj");
}

bool Chest::update(Scene &scene, float dt) {
    generateModelMatrix();
    return true;
}

void Chest::render(Scene &scene) {
    shader->use();

    // Set up light
    shader->setUniform("LightDirection", scene.lightDirection);

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