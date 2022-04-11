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
#include "missile.h"
#include "collectible_object.h"
#include "turret.h"
#include "ui_element.h"
#include "healthbar.h"
#include "number.h"
#include "base.h"
#include "drone.h"
#include "helicopter.h"
#include "minimap.h"

#define FONT_SIZE 20.0f


#define MISSILE_COOLDOWN 5.0f
#define MAX_MISSILE_RANGE 6.0f

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

            //Pause to allow player to do something
            void ChooseLoop(void);

            //Game over loop
            void GameOverLoop(void);

            //Increase score for player
            void UpdateScore(int score);

            //victory loop
            void VictoryLoop();

        private:
            // Main window: pointer to the GLFW window structure
            GLFWwindow *window_;

            // Sprite Shader for rendering the scene
            Shader shader_;

            Shader particle_shader_;

            Shader player_shader_;

            Shader jet_shader_;

            // Size of geometry to be rendered
            int size_;

            // References to textures
#define NUM_TEXTURES 53
            GLuint tex_[NUM_TEXTURES];

            std::vector<GLuint> text_arr_;

            std::vector<GLuint> explo_arr_;

            // List for background game objects
            // This is so we can create a tilemap without making the game_objects_ array overloaded with stuff
            std::vector<GameObject*> background_objects_;

            // List for generic game objects
            std::vector<GameObject*> game_objects_;

            // List for specific operations on enemy objects as they are added and removed
            std::vector<EnemyGameObject*> enemy_objects_;

            // List for specific operations on bullet objects as they are added and removed
            std::vector<BulletObject*> bullet_objects_;

            // // List for specific operations on missile objects as they are added and removed
            std::vector<MissileObject*> missile_objects_;

            // List for specific operations on collectible objects as they are added and removed
            std::vector<CollectibleObject*> collectible_objects_;
            
            //Bases
            std::vector<Base*> bases_;

            //ui element objects
            std::vector<UI_Element*> ui_objects_;

            //Missile particles
            std::vector<GameObject*> particle_objects_;

            std::vector<GameObject*> player_particles_;

            //Explosion particles
            std::vector<GameObject*> explosion_objects_;

            //healthbar
            UI_Element* healthbar_;

            //player score
            int score_;
            Number* score_ptr_;

            bool paused_; //the game is paused
            bool selection_made_; //a choice has been made on the pause menu
            int choice_;

            // Keep track of time for particles
            double current_time_;

            // Gameover boolean
            bool gameOver = false;

            //a pointer to the missile ready ui element (this tells the user whether or not a missile is ready)
            UI_Element* missile_ready_;

            // Callback for when the window is resized
            static void ResizeCallback(GLFWwindow* window, int width, int height);

            // Set a specific texture
            void SetTexture(GLuint w, const char *fname, bool tiling);

            // Load all textures
            void SetAllTextures();

            // Handle user input
            void Controls(double, double*);

            // Update the game based on user input and simulation
            void Update(double delta_time, double* time_hold, double* bullet_cooldown, int* num_shield);

            // Iterate over every object for the purposes of collision
            void IterateCollision ();

            // Set of functions including all possible collision scenarios
            bool DetectCollision (PlayerGameObject*, EnemyGameObject*);
            bool DetectCollision (PlayerGameObject*, CollectibleObject*);          
            bool Game::DetectCollision(PlayerGameObject* player, Base* base);

            // Similar to IterateCollision but makes use of object functions for EnemyGameObject and is hard coded
            void EnemyDetect (void);

            // Uses Ray-Circle collision to determine if a bullet is hitting an object
            bool BulletCastCollision (BulletObject*);

            // Applies damage to the player. If the player's # of shields is -1, then we'll apply game over
            void DamagePlayer (int);

            //
            void addShieldToPlayer ();

            // Applies the effect of the collectible
            void ApplyEffect (int, CollectibleObject*);           

            //Detects the closest enemy in front of the player
            GameObject* FindClosest(void);

            //only renders objects without updating
            void Render(std::vector<UI_Element*> extras);

            //checks for base completion
            bool CheckBasesFinished();

    }; // class Game

} // namespace game

#endif // GAME_H_
