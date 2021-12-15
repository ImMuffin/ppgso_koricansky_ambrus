#include "water.h"
#include "scene.h"
#include "player.h"
#include "sand.h"


#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>

// shared resources
std::unique_ptr<ppgso::Mesh> Water::mesh;
std::unique_ptr<ppgso::Texture> Water::texture;
std::unique_ptr<ppgso::Shader> Water::shader;

Water::Water() {
    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("aquarium.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("ball.obj");
}

bool Water::update(Scene &scene, float dt) {

    //speed.x = -(fallSpeed * 1.1 * dt);
    //speed.y = -(fallSpeed * 1.5 * dt);
    //speed.z = -(fallSpeed * 1.1 * dt);

    speed.x = -(10 * dt);
    speed.y = -(10 * dt);
    speed.z = -(10 * dt);
    position.y += -(fallSpeed * dt);
    collide(scene);


    generateModelMatrix();
    return true;
}

void Water::render(Scene &scene) {
    shader->use();

    // Set up light
    shader->setUniform("LightDirection", scene.lightDirection);
    shader->setUniform("LightColor", scene.lightColor);

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
