#include "scene.h"
#include "water.h"
#include "bubble.h"

void Scene::update(float time) {
  //camera movement
  camera->position = {sin(fmod(glfwGetTime(), ppgso::PI * 2)) * 10,(sin(fmod(glfwGetTime(), ppgso::PI * 4) / 4) * 10),(cos(fmod(glfwGetTime(), ppgso::PI * 2)) * 10)};
  //camera->position = {sin(fmod(glfwGetTime(), ppgso::PI * 2)) * 10,8.0f,(cos(fmod(glfwGetTime(), ppgso::PI * 2)) * 10)};
  camera->update();

  // Use iterator to update all objects so we can remove while iterating
  auto i = std::begin(objects);

  while (i != std::end(objects)) {
    // Update and remove from list if needed
    auto obj = i->get();
    if (!obj->update(*this, time))
      i = objects.erase(i); // NOTE: no need to call destructors as we store shared pointers in the scene
    else
      ++i;

    if (obj->master) //set object for connected motion
    {
      masterObject = obj;
    }

    if ((masterObject != nullptr) && (obj->slave == true)) //connected motion
    {
      glm::mat4 originalModel = obj->modelMatrix;
      obj->modelMatrix = masterObject->modelMatrix;
      obj->modelMatrix *= originalModel;
    }

    if (obj->cameraFocus == true) //check if the camera should follow the object
    {
      camera->back = obj->position;
      camera->position = obj->position + obj->forward * 2.0f;
      if (fmod(obj->rotation.x, ppgso::PI*2) - ppgso::PI < 0)
      {
        camera->up.y = 1;
      }
      else
      {
        camera->up.y = -1;
      }
      camera->update();
    }
    else
    {
      camera->back = {0,0,0};;
    }

    if(obj->canCollide)
    {
      auto otherObjects = std::begin(objects);
      int collisionCounter = 0;
      while (otherObjects != std::end(objects)) 
      {
        auto oObj = otherObjects->get();
        collisionCounter = 0;
        if ((oObj != obj) && (oObj->canCollide == false))
        {
          if(fabs(obj->position.x - oObj->position.x) < ((oObj->size.x) + (obj->size.x))) //over koliziu
          {
            collisionCounter++;
          }
          if(fabs(obj->position.y - oObj->position.y) < ((oObj->size.y) + (obj->size.y)))
          {
            collisionCounter++;
          }
          if(fabs(obj->position.z - oObj->position.z) < ((oObj->size.z) + (obj->size.z)))
          {
            collisionCounter++;
          }
          if (collisionCounter == 3)
          {
            obj->fallSpeed *= 0.2; //1 pre aktivnejsiu sim, 0.2 pre splash
            if (oObj->normal.x != 0)
            {
              obj->position.x -= obj->speed.x * oObj->normal.x;
            }
            if (oObj->normal.y != 0)
            {
              obj->position.y -= obj->speed.y * oObj->normal.y;
            }
            if (oObj->normal.z != 0)
            {
              obj->position.z -= obj->speed.z * oObj->normal.z;
            }
          }
        }
        otherObjects++;
      }
    }
  }

  //generate bubbles
  if (generateBubbles)
  {
    auto bubble = std::make_unique<Bubble>();
    bubble->velocity = (float)rand()/(float)(RAND_MAX/0.3-0.02) + 0.02;
    bubble->position.y = (float)rand()/(float)RAND_MAX;
    bubble->position.x = (float)rand()/(float)RAND_MAX + .5f;
    bubble->position.z = (float)rand()/(float)RAND_MAX;
    bubble->scale *= 0.04f;
    bubble->slave = true;
    bubble->canCollide = true;
    objects.push_back(move(bubble));
  }
}

void Scene::render() {
  // Simply render all objects
  for ( auto& obj : objects )
      obj->render(*this);

}

void Scene::redistributeObjects()
{
  auto i = std::begin(objects);
  while (i != std::end(objects)) {
    auto obj = i->get();
    if(obj->redist == true)
    {
      bool distribute = true;
      while (distribute)
      {
        distribute = false;
        obj->position.x = fmod(rand(), 4.2) - 2.1;
        obj->position.z = fmod(rand(), 8.4) - 4.2;
        printf("%f, %f\n",obj->position.x, obj->position.y);
        auto otherObjects = std::begin(objects);
        while (otherObjects != std::end(objects))
        {
          auto oObj = otherObjects->get();
          if ((fabs(obj->position.x - oObj->position.x) < (obj->size.x + oObj->size.x)) && (fabs(obj->position.y - oObj->position.y) < (obj->size.y + oObj->size.y)) && (fabs(obj->position.z - oObj->position.z) < (obj->size.z + oObj->size.z)) && oObj != obj)
          {
            distribute = true;
            break;
          }
          otherObjects++;
        }
      }
    }
    i++;
  }
} 

std::vector<Object*> Scene::intersect(const glm::vec3 &position, const glm::vec3 &direction) {
  std::vector<Object*> intersected = {};
  for(auto& object : objects) {
    // Collision with sphere of size object->scale.x
    auto oc = position - object->position;
    auto radius = object->scale.x;
    auto a = glm::dot(direction, direction);
    auto b = glm::dot(oc, direction);
    auto c = glm::dot(oc, oc) - radius * radius;
    auto dis = b * b - a * c;

    if (dis > 0) {
      auto e = sqrt(dis);
      auto t = (-b - e) / a;

      if ( t > 0 ) {
        intersected.push_back(object.get());
        continue;
      }

      t = (-b + e) / a;

      if ( t > 0 ) {
        intersected.push_back(object.get());
        continue;
      }
    }
  }

  return intersected;
}
