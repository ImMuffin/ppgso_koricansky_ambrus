#include "wall.h"
#include "scene.h"
#include "water.h"
#include "player.h"
#include "sand.h"


#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>

// shared resources
std::unique_ptr<ppgso::Mesh> Wall::mesh;
std::unique_ptr<ppgso::Texture> Wall::texture;
std::unique_ptr<ppgso::Shader> Wall::shader;

Wall::Wall() {
    //Scale the default model
    scale.y *= 0.05;

    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("aquarium.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("ball.obj");
}

bool Wall::update(Scene &scene, float dt) {

    float sandPos = 0;
    float sandY = 0;

    for (auto& object : scene.objects){

        if (dynamic_cast<Sand*>(object.get()) == nullptr)
            continue;
        sandPos = object->position.y;
        sandY = object->scale.y;
        

    }

    for (auto& object : scene.objects){

        if (dynamic_cast<Player*>(object.get()) == nullptr)
            continue;

        glm::vec3 pdif = scene.lightSource - object->position;

        float newY = sandPos + sandY;

        position = object->position;
        
        while (position.y > newY){
            position -= pdif / (float)1000;
        }
        position.y = 0.8;

        glm::vec3 pdif2 = object->position - position;

        float ratio2 = glm::length(pdif2)/glm::length(pdif);

        scale.x = 0.1 + ratio2/5;
        scale.z = 0.1 + ratio2/5;
    }

    generateModelMatrix();
    return true;
}

void Wall::render(Scene &scene) {
    shader->use();

    // Set up light
    shader->setUniform("LightDirection", scene.lightDirection);
    shader->setUniform("LightColor", glm::vec3{0,0,0});

    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);

    // set transparency
    shader->setUniform("Transparency", 0.8f);
    shader->setUniform("TimeOffset", (glfwGetTime() * 2*3.14159 * .75));
    mesh->render();
}

void Wall::merge(Scene &scene){

    for (auto& object : scene.objects){

        if (dynamic_cast<Water*>(object.get()) == nullptr)
            continue;

        if (object->position.y+object->scale.y < position.y+scale.y){
            float objVol = 4/3*3.14*object->scale.x*object->scale.x*object->scale.x;
            float waterVol = scale.x*scale.y*scale.z*8;
            object->scale =glm::vec3 {0,0,0};
            float scaleOffset = objVol / (scale.x*scale.z*8);
            scale.y += scaleOffset;
            position.y += scaleOffset;
        }

    }
}

void Wall::fluid(Scene &scene){

    int xp,xm,zp,zm = 0;

    if (position.x-scale.x > 0)
        xm = 1;
    if (position.x+scale.x < 1)
        xp = 1;
    if (position.z-scale.z > 0)
        zm = 1;
    if (position.z+scale.z < 1)
        zp = 1;

    float volume = scale.x * scale.y * scale.z * 8;

    scale.x += 0.02 * (xm + xp);
    scale.z += 0.02 * (zm + zp);
    position.x += 0.02*xp;
    position.x -= 0.02*xm;
    position.z += 0.02*zp;
    position.z -= 0.02*zm;

    float newY = volume / (scale.x*scale.z*8);

    float scaleOffset = abs(scale.y-newY);

    position.y -= scaleOffset;
    scale.y = newY;

}