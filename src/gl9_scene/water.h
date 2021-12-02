//
// Created by adyko on 11/30/2021.
//

#ifndef PPGSO_WATER_H
#define PPGSO_WATER_H

#include <ppgso/ppgso.h>

#include "object.h"


class Water final : public Object {
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;

public:
    /*!
     * Create a new player
     */
    Water();

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

    void collide(Scene &scene);

    void direct(Object* obj1, Object* obj2);

    void multipleCollisions(Scene &scene);
};


#endif //PPGSO_WATER_H
