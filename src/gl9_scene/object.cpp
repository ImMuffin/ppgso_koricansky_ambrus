#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include <fstream>
#include <bits/stdc++.h>

#include "object.h"

void Object::generateModelMatrix() {
  modelMatrix =
          glm::translate(glm::mat4(1.0f), position)
          * glm::orientate4(rotation)
          * glm::scale(glm::mat4(1.0f), scale);
}

void Object::recordMovement(std::ofstream& f) {

    std::string matrix;
    for (int i = 0; i < 4; i++)
    {
        for (int a = 0; a < 4; a++)
        {
            matrix += std::to_string(modelMatrix[i][a]) + " ";
        };
    };
    f << matrix << std::endl;
}

bool Object::playbackMovement(std::ifstream& p) {


    std::string text;
    std::string fl;
    std::getline(p, text);
    std::stringstream stream(text);

    if (p.eof()){
        return false;
    };

    for (int i = 0; i < 4; i++)
    {
        for (int a = 0; a < 4; a++)
        {
            stream >> fl;
            modelMatrix[i][a] = std::stof(fl);
        };
    };

    return true;

}