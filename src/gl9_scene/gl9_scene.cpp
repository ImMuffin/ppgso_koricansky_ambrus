// Example gl_scene
// - Introduces the concept of a dynamic scene of objects
// - Uses abstract object interface for Update and Render steps
// - Creates a simple game scene with Player, Asteroid and Space objects
// - Contains a generator object that does not render but adds Asteroids to the scene
// - Some objects use shared resources and all object deallocations are handled automatically
// - Controls: LEFT, RIGHT, "R" to reset, SPACE to fire

#include <iostream>
#include <map>
#include <list>

#include <ppgso/ppgso.h>
#include "chest.h"
#include "camera.h"
#include "scene.h"
#include "player.h"
#include "aquarium.h"
#include "castle.h"
#include "plant.h"
#include "sand.h"
#include "water.h"
#include "wall.h"
#include "bubble.h"

const unsigned int SIZEX = 1360;
const unsigned int SIZEY = 720;

/*!
 * Custom windows for our simple game
 */
class SceneWindow : public ppgso::Window {
private:
  Scene scene;
  bool animate = true;

  /*!
   * Reset and initialize the game scene
   * Creating unique smart pointers to objects that are stored in the scene object list
   */
  void initScene() {
    scene.objects.clear();

    // Create a camera
    auto camera = std::make_unique<Camera>(60.0f, (float)SIZEX/(float)SIZEY, 0.1f, 100.0f);
    scene.camera = move(camera);

    // Add chest
    /*auto chest = std::make_unique<Chest>();
    chest->rotation.x = -ppgso::PI/2.0f;
    chest->position.y = 0.5f;
    chest->position.z = 1;
    chest->scale *= 0.04f;
    scene.objects.push_back(move(chest));*/

    // add aquarium 
    //auto aquarium = std::make_unique<Aquarium>();
    //scene.objects.push_back(move(aquarium));

    // add castle 
    /*auto castle = std::make_unique<Castle>();
    castle->rotation.x = -ppgso::PI/2.0f;
    castle->scale *= 0.2;
    castle->position.x = 4.5f;
    castle->position.z = -2;
    scene.objects.push_back(move(castle));*/

    // add plant
    /*auto plant = std::make_unique<Plant>();
    plant->scale *= 10.0f;
    plant->position.x = -4;
    scene.objects.push_back(move(plant));*/

    // add sand
    /*auto sand = std::make_unique<Sand>();
    sand->rotation.z = -ppgso::PI/2.0f;
    scene.objects.push_back(move(sand));*/
    

    // Add player to the scene
    auto player = std::make_unique<Player>();
    player->position.y = 3;
    player->rotation.x = ppgso::PI/2.0f;
    player->rotation.y = ppgso::PI/2.0f;
    player->cameraFocus = true;
    scene.objects.push_back(move(player));

    for (int i = 0; i < 200; i++){
        auto water = std::make_unique<Water>();
        water->position.y = i * 2;
        water->position.x = (float)rand()/(float)RAND_MAX;
        water->position.z = (float)rand()/(float)RAND_MAX;
        water->scale *= 0.05f;
        scene.objects.push_back(move(water));
    }

    for (int i = 0; i < 100; i++){
        auto bubble = std::make_unique<Bubble>();
        bubble->velocity = (float)rand()/(float)(RAND_MAX/0.3-0.02) + 0.02;
        bubble->position.y = 0 - i*2;
        bubble->position.x = (float)rand()/(float)RAND_MAX;
        bubble->position.z = (float)rand()/(float)RAND_MAX;
        bubble->scale *= 0.05f;
        scene.objects.push_back(move(bubble));
    }

    auto cube = std::make_unique<Wall>();
    cube->scale = glm::vec3 {0.5,0.1,0.5};
    cube->position.y = 0.1;
    scene.objects.push_back(move(cube));


  }

public:
  /*!
   * Construct custom game window
   */
  SceneWindow() : Window{"gl9_scene", SIZEX, SIZEY} {
    //hideCursor();
    glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

    // Initialize OpenGL state
    // Enable Z-buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // Enable polygon culling
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    glEnable(GL_BLEND); //enable for transparency
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    initScene();
  }

  /*!
   * Handles pressed key when the window is focused
   * @param key Key code of the key being pressed/released
   * @param scanCode Scan code of the key being pressed/released
   * @param action Action indicating the key state change
   * @param mods Additional modifiers to consider
   */
  void onKey(int key, int scanCode, int action, int mods) override {
    scene.keyboard[key] = action;

    // Reset
    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
      initScene();
    }

    // Pause
    if (key == GLFW_KEY_P && action == GLFW_PRESS) {
      animate = !animate;
    }
  }

  /*!
   * Handle cursor position changes
   * @param cursorX Mouse horizontal position in window coordinates
   * @param cursorY Mouse vertical position in window coordinates
   */
  void onCursorPos(double cursorX, double cursorY) override {
    scene.cursor.x = cursorX;
    scene.cursor.y = cursorY;
  }

  /*!
   * Handle cursor buttons
   * @param button Mouse button being manipulated
   * @param action Mouse bu
   * @param mods
   */
  void onMouseButton(int button, int action, int mods) override {
    if(button == GLFW_MOUSE_BUTTON_LEFT) {
      scene.cursor.left = action == GLFW_PRESS;

      if (scene.cursor.left) {
        // Convert pixel coordinates to Screen coordinates
        double u = (scene.cursor.x / width - 0.5f) * 2.0f;
        double v = - (scene.cursor.y / height - 0.5f) * 2.0f;

        // Get mouse pick vector in world coordinates
        auto direction = scene.camera->cast(u, v);
        auto position = scene.camera->position;

        // Get all objects in scene intersected by ray
        auto picked = scene.intersect(position, direction);

        // Go through all objects that have been picked
        for (auto &obj: picked) {
          // Pass on the click event
          obj->onClick(scene);
        }
      }
    }
    if(button == GLFW_MOUSE_BUTTON_RIGHT) {
      scene.cursor.right = action == GLFW_PRESS;
    }
  }

  /*!
   * Window update implementation that will be called automatically from pollEvents
   */
  void onIdle() override {
    // Track time
    static auto time = (float) glfwGetTime();

    // Compute time delta
    float dt = animate ? (float) glfwGetTime() - time : 0;

    time = (float) glfwGetTime();

    // Set gray background
    glClearColor(.5f, .5f, .5f, 0);
    // Clear depth and color buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Update and render all objects
    scene.update(dt);
    scene.render();
  }
};

int main() {
  // Initialize our window
  SceneWindow window;

  // Main execution loop
  while (window.pollEvents()) {}

  return EXIT_SUCCESS;
}
