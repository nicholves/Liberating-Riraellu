#ifndef GAME_H_
#define GAME_H_

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "shader.h"
#include "game_object.h"

#include "player_game_object.h"
#include "enemy_game_object.h"
#include "helicopter_blades.h"
#include "shield.h"
#include "bullet_object.h"
#include "collectible_object.h"
#include "buoy_object.h"
#include "turret.h"

namespace game {

    // A class for holding the main game objects
    class Game {

        public:
            // Constructor and destructor
            Game(void);
            ~Game();

            // Call Init() before calling any other method
            // Initialize graphics libraries and main window
            void Init(void); 

            // Set up the game (scene, game objects, etc.)
            void Setup(void);

            // Run the game (keep the game active)
            void MainLoop(void); 

        private:
            // Main window: pointer to the GLFW window structure
            GLFWwindow *window_;

            // Shader for rendering the scene
            Shader shader_;

            // Size of geometry to be rendered
            int size_;

            // References to textures
#define NUM_TEXTURES 14
            GLuint tex_[NUM_TEXTURES];

            // List for background game objects
            // This is so we can create a tilemap without making the game_objects_ array overloaded with stuff
            std::vector<GameObject*> background_objects_;

            // List for generic game objects
            std::vector<GameObject*> game_objects_;

            // List for specific operations on enemy objects as they are added and removed
            std::vector<EnemyGameObject*> enemy_objects_;

            // List for specific operations on bullet objects as they are added and removed
            std::vector<BulletObject*> bullet_objects_;
            
            // List for specific operations on collectible objects as they are added and removed
            std::vector<CollectibleObject*> collectible_objects_;

            std::vector<BuoyObject*> buoy_objects_;

            // Gameover boolean
            bool gameOver = false;

            // Callback for when the window is resized
            static void ResizeCallback(GLFWwindow* window, int width, int height);

            // Create a square for drawing textures
            int CreateSprite(void);

            // Set a specific texture
            void SetTexture(GLuint w, const char *fname, bool tiling);

            // Load all textures
            void SetAllTextures();

            // Handle user input
            void Controls(double, double*);

            // Update the game based on user input and simulation
            void Update(double delta_time, double* time_hold, double* bullet_cooldown, int* num_shield);

            // Iterate over every object for the purposes of collision
            void IterateCollision (int*);

            // Set of functions including all possible collision scenarios
            bool DetectCollision (PlayerGameObject*, EnemyGameObject*);
            bool DetectCollision (PlayerGameObject*, CollectibleObject*);
            bool DetectCollision (PlayerGameObject*, BuoyObject*);

            // Similar to IterateCollision but makes use of object functions for EnemyGameObject and is hard coded
            void EnemyDetect (void);

            // Uses Ray-Circle collision to determine if a bullet is hitting an object
            bool BulletCastCollision (BulletObject*);

            // Applies damage to the player. If the player's # of shields is -1, then we'll apply game over
            void DamagePlayer (int*, int);

            // Applies the effect of the collectible
            void ApplyEffect (int*, int, CollectibleObject*);

            // Collision handling between player and buoys
            void BuoyBounce (PlayerGameObject*, BuoyObject*);

    }; // class Game

} // namespace game

#endif // GAME_H_
