//
// Created by adyko on 12/10/2021.
//

#include "bubble.h"
#include "scene.h"


#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

// shared resources
std::unique_ptr<ppgso::Mesh> Bubble::mesh;
std::unique_ptr<ppgso::Texture> Bubble::texture;
std::unique_ptr<ppgso::Shader> Bubble::shader;

Bubble::Bubble() {
    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("aquarium.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("ball.obj");
}

bool Bubble::update(Scene &scene, float dt) {


    position.y += velocity * dt;

    if (position.y > 5){
        position.y = 5;
    }

    collide(scene);

    generateModelMatrix();
    return true;
}

void Bubble::render(Scene &scene) {
    shader->use();

    // Set up light
    shader->setUniform("LightDirection", scene.lightDirection);

    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);

    // set transparency
    shader->setUniform("Transparency", 0.3f);
    shader->setUniform("TimeOffset", (glfwGetTime() * 2*3.14159 * .75));
    mesh->render();
}

void Bubble::split(Scene &scene) {

}

void Bubble::merge(Scene &scene) {

}

bool Bubble::collide(Scene &scene) {
    for (auto& object : scene.objects){
        if (dynamic_cast<Bubble*>(object.get()) == nullptr)
            continue;

        if (abs(position.x - object->position.x) > scale.x*2 || abs(position.y - object->position.y) > scale.x*2 || abs(position.z - object->position.z) > scale.x*2 )
            continue;

        glm::vec3 pdif = position - object->position;
        float dis = glm::length(pdif);
        float col = dis - scale.x - object->scale.x;

        if (col < 0 && dis != 0) {
            auto ratio = -col / dis;
            auto vec = pdif * ratio;

            float volume1 = 4/3 * scale.x * scale.x * scale.x * 3.14;
            float volume2 = 4/3 * object->scale.x * object->scale.x * object->scale.x * 3.14;
            float finalScale = (volume1 + volume2) / (4/3 * 3.14);
            finalScale = cbrt(finalScale);
            float pos_offset = scale.x / object->scale.x;
            //object->position = glm::vec3 {(position.x + object->position.x)};
            if (position.y > object->position.y)
            {
                scale = glm::vec3 {finalScale,finalScale,finalScale};
                object->scale = {0,0,0};
            } else {
                object->scale = glm::vec3 {finalScale,finalScale,finalScale};
                scale = {0,0,0};
            }


            //position = {roundf(position.x * 1000) / 1000,roundf(position.y * 1000) / 1000,roundf(position.z * 1000) / 1000};
        }

    }
}