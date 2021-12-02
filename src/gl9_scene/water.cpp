//
// Created by adyko on 11/30/2021.
//

#include "water.h"
#include "scene.h"


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
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("aquarium.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("ball.obj");
}

bool Water::update(Scene &scene, float dt) {
    collide(scene, {0,-1,0});
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
    mesh->render();
}

void Water::collide(Scene &scene, glm::vec3 direction) {
    std::vector<Object*> intersected = scene.intersect(position, direction);
    if (position.y > 0)
        position.y -= 0.1f;
    if (position.y < 0)
        position.y = 0;
    /*for (auto object : intersected){
        if (dynamic_cast<Water*>(object) != NULL)
            direct(this, object);
    };*/

}

void Water::direct(Object* obj1, Object* obj2) {
    int distance = sqrt((obj1->position.x - obj2->position.x)*(obj1->position.x - obj2->position.x) + (obj1->position.y - obj2->position.y)*(obj1->position.y - obj2->position.y)+(obj1->position.z - obj2->position.z)*(obj1->position.z - obj2->position.z));
    distance -= obj1->scale.x;
    distance = abs(distance);
    glm::vec3 F1 = obj1->position - obj2->position;
    F1 *= distance;

    obj1->position += F1;
    obj2->position -= F1;

}