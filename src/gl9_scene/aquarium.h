//
// Created by adyko on 12/2/2021.
//

#ifndef PPGSO_AQUARIUM_H
#define PPGSO_AQUARIUM_H


#include <ppgso/ppgso.h>

#include "object.h"


class Aquarium final : public Object {
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;

public:
    /*!
     * Create a new player
     */
    Aquarium();
    
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

};


#endif //PPGSO_WALL_H
