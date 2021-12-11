//
// Created by adyko on 12/10/2021.
//

#ifndef PPGSO_BUBBLE_H
#define PPGSO_BUBBLE_H


#include <ppgso/ppgso.h>

#include "object.h"


class Bubble final : public Object {
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;


public:
    /*!
     * Create a new player
     */

    Bubble();

    float velocity = 0;

    /*!
     * Update player position considering keyboard inputs
     * @param scene Scene to update
     * @param dt Time delta
     * @return true to delete the object
     */
    bool update(Scene &scene, float dt) override;

    /*!
     * Render player
     * @param scene Scene to render in
     */
    void render(Scene &scene) override;
    void merge(Scene &scene);
    void split(Scene &scene);
    bool collide(Scene &scene);
};


#endif //PPGSO_BUBBLE_H
