#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include <fstream>

#include "object.h"

void Object::generateModelMatrix() {
  modelMatrix =
          glm::translate(glm::mat4(1.0f), position)
          * glm::orientate4(rotation)
          * glm::scale(glm::mat4(1.0f), scale);
}

void Object::recordMovement(std::ofstream file) {

    std::string matrix;
    for (int i = 0; i < 4; i++)
    {
        for (int a = 0; i < 4; a++)
        {
            matrix += modelMatrix[i][a];
        };
    };
    file << matrix << std::flush;;
}