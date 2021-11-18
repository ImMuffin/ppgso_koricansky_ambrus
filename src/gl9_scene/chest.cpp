//
// Created by adyko on 11/18/2021.
//

#include "chest.h"
#include "scene.h"


#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

// shared resources
std::unique_ptr<ppgso::Mesh> Chest::mesh;
std::unique_ptr<ppgso::Texture> Chest::texture;
std::unique_ptr<ppgso::Shader> Chest::shader;

Chest::Chest() {
    // Scale the default model
    scale *= 3.0f;

    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("asteroid.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("chest.obj");
}

bool Chest::update(Scene &scene, float dt) {
    // Fire delay increment
    scale.x = 0.2f;
    scale.y = 0.2f;
    scale.z = 0.2f;

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
    shader->setUniform("Texture", *texture);
    mesh->render();
}