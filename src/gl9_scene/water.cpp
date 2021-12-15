//
// Created by adyko on 11/30/2021.
//

#include "water.h"
#include "scene.h"
#include "player.h"
#include "sand.h"


#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

// shared resources
std::unique_ptr<ppgso::Mesh> Water::mesh;
std::unique_ptr<ppgso::Texture> Water::texture;
std::unique_ptr<ppgso::Shader> Water::shader;

Water::Water() {
    //Scale the default model
    scale *= 3.0f;

    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("water.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("ball.obj");
}

bool Water::update(Scene &scene, float dt) {

    speed.x = -(3 * dt);
    speed.y = -(3 * dt);
    speed.z = -(3 * dt);
    position.y += speed.y;
    collide(scene);


    generateModelMatrix();
    return true;
}

void Water::render(Scene &scene) {
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

void Water::collide(Scene &scene) {

    for (auto& object : scene.objects){

        if (dynamic_cast<Water*>(object.get()) == nullptr)
            continue;

        if (abs(position.x - object->position.x) > scale.x*2 || abs(position.y - object->position.y) > scale.x*2 || abs(position.z - object->position.z) > scale.x*2 )
            continue;

        glm::vec3 pdif = position - object->position;
        float dis = glm::length(pdif);
        float col = dis - scale.x - object->scale.x;

        if (col < 0 && dis != 0) {
            auto ratio = -col / dis;
            auto vec = pdif * ratio;
            position += vec;
            //position = {roundf(position.x * 1000) / 1000,roundf(position.y * 1000) / 1000,roundf(position.z * 1000) / 1000};
        }

    }
}

glm::vec3 Water::multipleCollisions(Scene &scene) {

    glm::vec3 finalVec = {0,0,0};

        for (auto& object : scene.objects){
            if (position == object->position)
                continue;

            glm::vec3 pdif = position - object->position;
            float dis = glm::length(pdif);
            float col = dis - scale.x - object->scale.x;

            if (col < 0.1f && dis != 0) {
                auto ratio = -col / dis;
                auto vec = pdif * ratio;
                finalVec += vec;
            }



    }

    position += finalVec;

    return finalVec;

}

glm::vec3 Water::newCollide(Scene &scene) {

    for (auto& object : scene.objects){
        if (dynamic_cast<Sand*>(object.get()) != nullptr)
        {
            if (object->position.y+object->scale.y > position.y-scale.y){
                position.y = object->position.y + object->scale.y + scale.y;
            }
            continue;
        }
    }

}

bool Water::collides(Scene &scene){
    for (auto& object : scene.objects){
        glm::vec3 pdif = position - object->position;
        float dis = glm::length(pdif);
        float col = dis - scale.x - object->scale.x;

        if (col < 0 && dis != 0) {
            return true;
        }
    }
    return false;
}