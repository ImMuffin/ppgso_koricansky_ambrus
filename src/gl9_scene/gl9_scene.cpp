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
  void initScene1()
  {
    scene.objects.clear();

    auto camera = std::make_unique<Camera>(100.0f, (float)SIZEX/(float)SIZEY, 0.1f, 100.0f);
    scene.camera = move(camera);

    auto aqwall1 = std::make_unique<Aquarium>(); //bottom
    auto aqwall2 = std::make_unique<Aquarium>(); //right wall
    auto aqwall3 = std::make_unique<Aquarium>(); //left wall
    auto aqwall4 = std::make_unique<Aquarium>(); //back wall
    auto aqwall5 = std::make_unique<Aquarium>(); //front wall

    aqwall1->master = true;
    aqwall2->slave = true;
    aqwall3->slave = true;
    aqwall4->slave = true;
    aqwall5->slave = true;

    aqwall1->size={3.0f, 0.2f, 10.0f};
    aqwall1->normal={0,1,0};
    aqwall2->size={0.2f, 3.0f, 10.0f};
    aqwall2->normal={-1,0,0};
    aqwall3->size={0.2f, 3.0f, 10.0f};
    aqwall3->normal={1,0,0};
    aqwall4->size={3.0f, 3.0f, 0.2f};
    aqwall4->normal={0,0,1};
    aqwall5->size={3.0f, 3.0f, 0.2f};
    aqwall5->normal={0,0,-1};

    aqwall1->position.y = 0;
    aqwall1->position.z = -5;
    //aqwall1->rotation.x = -ppgso::PI / 2;

    aqwall2->position.x = 6.17f / 2;
    aqwall2->rotation.y = ppgso::PI/2;
    aqwall2->position.y = 6.17f / 2;

    aqwall3->position.x = -6.17f / 2;
    aqwall3->rotation.y = ppgso::PI/2;
    aqwall3->position.y = 6.17f / 2;

    aqwall4->position.z = -5.5f;
    aqwall4->scale.z = 0.55;
    aqwall4->rotation.x = ppgso::PI/2;
    aqwall4->position.y = 3.11f;

    aqwall5->position.z = 5.5f;
    aqwall5->scale.z = 0.55;
    aqwall5->rotation.x = ppgso::PI/2;
    aqwall5->position.y = 3.11f;


    for (int i = 0; i < 100; i++){
        auto water = std::make_unique<Water>();
        water->slave = true;
        water->canCollide = true;
        water->position.y = 2 * i + 2;
        water->position.x = (float)rand()/(float)RAND_MAX;
        water->position.z = (float)rand()/(float)RAND_MAX;
        //water->size = {5.4,5.4,5.4};
        water->size = {0.54f,0.54f,0.54f};
        water->scale *= 0.2;
        scene.objects.push_back(move(water));
    }

    scene.objects.push_back(move(aqwall1));
    scene.objects.push_back(move(aqwall2));
    scene.objects.push_back(move(aqwall3));
    scene.objects.push_back(move(aqwall4));
    scene.objects.push_back(move(aqwall5));
  }

  void initScene2() 
  {
    scene.objects.clear();

    // Create a camera
    auto camera = std::make_unique<Camera>(60.0f, (float)SIZEX/(float)SIZEY, 0.1f, 100.0f);
    scene.camera = move(camera);

    // Add player to the scene
    auto player = std::make_unique<Player>();
    player->position.y = 5;
    player->position.z = 3;
    player->rotation.x = ppgso::PI/2.0f;
    player->rotation.y = ppgso::PI/2.0f;
    player->cameraFocus = true;
    player->slave = true;
    player->canCollide = true;

    // Add chest
    auto chest = std::make_unique<Chest>();
    chest->rotation.x = -ppgso::PI/2;
    chest->rotation.y = ppgso::PI/2.0f;
    chest->position.x = 1;
    chest->position.y = .7f;
    chest->scale *= 0.05f;
    chest->slave = true;
    chest->size = {2,2,2};

    // add castle
    auto castle = std::make_unique<Castle>();
    castle->rotation.x = -ppgso::PI/2.0f;
    castle->scale *= 0.2;
    castle->position.z = -4.2;
    castle->position.x = -2.1;
    castle->slave = true;
    castle->size = {2,2,2};

    // add plant
    for(int i = 0; i < 5; i++)
    {
      auto plant = std::make_unique<Plant>();
      plant->scale *= 10.0f;
      plant->position.y = 0.2f;
      plant->slave = true;
      plant->redist = true;
      plant->size = {1,1,1};
      scene.objects.push_back(move(plant));
    }
    

    // add sand
    auto sand = std::make_unique<Sand>();
    sand->slave = true;
    

    //generate aquarium

    auto aqwall1 = std::make_unique<Aquarium>(); //bottom
    auto aqwall2 = std::make_unique<Aquarium>(); //right wall
    auto aqwall3 = std::make_unique<Aquarium>(); //left wall
    auto aqwall4 = std::make_unique<Aquarium>(); //back wall
    auto aqwall5 = std::make_unique<Aquarium>(); //front wall

    aqwall1->master = true;
    aqwall2->slave = true;
    aqwall3->slave = true;
    aqwall4->slave = true;
    aqwall5->slave = true;

    aqwall1->size={3.0f, 0.2f, 10.0f};
    aqwall1->normal={0,1,0};
    aqwall2->size={0.2f, 3.0f, 10.0f};
    aqwall2->normal={-1,0,0};
    aqwall3->size={0.2f, 3.0f, 10.0f};
    aqwall3->normal={1,0,0};
    aqwall4->size={3.0f, 3.0f, 0.2f};
    aqwall4->normal={0,0,1};
    aqwall5->size={3.0f, 3.0f, 0.2f};
    aqwall5->normal={0,0,-1};

    //aqwall1->position.y = 5;
    //aqwall1->position.z = -5;
    //aqwall1->rotation.x = -ppgso::PI / 2;

    aqwall2->position.x = 6.17f / 2;
    aqwall2->rotation.y = ppgso::PI/2;
    aqwall2->position.y = 6.17f / 2;

    aqwall3->position.x = -6.17f / 2;
    aqwall3->rotation.y = ppgso::PI/2;
    aqwall3->position.y = 6.17f / 2;

    aqwall4->position.z = -5.5f;
    aqwall4->scale.z = 0.55;
    aqwall4->rotation.x = ppgso::PI/2;
    aqwall4->position.y = 3.11f;

    aqwall5->position.z = 5.5f;
    aqwall5->scale.z = 0.55;
    aqwall5->rotation.x = ppgso::PI/2;
    aqwall5->position.y = 3.11f;

    for (int i = 0; i < 0; i++){
        auto bubble = std::make_unique<Bubble>();
        bubble->velocity = (float)rand()/(float)(RAND_MAX/0.3-0.02) + 0.02;
        bubble->position.y = (float)rand()/(float)RAND_MAX;
        bubble->position.x = (float)rand()/(float)RAND_MAX;
        bubble->position.z = (float)rand()/(float)RAND_MAX;
        bubble->scale *= 0.01f;
        scene.objects.push_back(move(bubble));
    }

    scene.objects.push_back(move(player));
    scene.objects.push_back(move(chest));
    scene.objects.push_back(move(sand));
    scene.objects.push_back(move(castle));
    scene.objects.push_back(move(aqwall1));
    scene.objects.push_back(move(aqwall2));
    scene.objects.push_back(move(aqwall3));
    scene.objects.push_back(move(aqwall4));
    scene.objects.push_back(move(aqwall5));
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

    initScene1();
    //scene.redistributeObjects();
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
      scene.update(0, true);
      initScene1();
    }
    // Load scene 2
    if (key == GLFW_KEY_T && action == GLFW_PRESS) {
      scene.update(0,true);
      initScene2();
      //scene.redistributeObjects();
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
    glClearColor(.0f, .0f, .4f, 0);
    // Clear depth and color buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Update and render all objects
    scene.update(dt,false);
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
