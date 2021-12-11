#include "scene.h"

void Scene::update(float time) {
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
    if (obj->cameraFocus == true)
    {
      camera->back = obj->position;
      //camera->position = obj->position + obj->forward * 2.0f;
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
  }
}

void Scene::render() {
  // Object cleanup
  for ( auto& obj : objects )
        if (obj->scale.x <= 0)
            objects.remove(obj);
  // Simply render all objects
  for ( auto& obj : objects )
      obj->render(*this);

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
