#include <stdexcept>
#include <string>
#include <glm/gtc/matrix_transform.hpp> 
#include <SOIL/SOIL.h>
#include <typeinfo>
#include <path_config.h>
#include <iostream>

#include "shader.h"
#include "game.h"


namespace game {

// Some configuration constants
// They are written here as global variables, but ideally they should be loaded from a configuration file

// Globals that define the OpenGL window and viewport
const char *window_title_g = "Game Demo";
const unsigned int window_width_g = 800;
const unsigned int window_height_g = 600;
const glm::vec3 viewport_background_color_g(1.0, 0.0, 0.0);

// Directory with game resources such as textures
const std::string resources_directory_g = RESOURCES_DIRECTORY;


Game::Game(void)
{
    // Don't do work in the constructor, leave it for the Init() function
}


void Game::Init(void)
{

    // Initialize the window management library (GLFW)
    if (!glfwInit()) {
        throw(std::runtime_error(std::string("Could not initialize the GLFW library")));
    }

    // Set window to not resizable
    // Required or else the calculation to get cursor pos to screenspace will be incorrect
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); 

    // Create a window and its OpenGL context
    window_ = glfwCreateWindow(window_width_g, window_height_g, window_title_g, NULL, NULL);
    if (!window_) {
        glfwTerminate();
        throw(std::runtime_error(std::string("Could not create window")));
    }

    // Make the window's OpenGL context the current one
    glfwMakeContextCurrent(window_);

    // Initialize the GLEW library to access OpenGL extensions
    // Need to do it after initializing an OpenGL context
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        throw(std::runtime_error(std::string("Could not initialize the GLEW library: ") + std::string((const char *)glewGetErrorString(err))));
    }

    // Set event callbacks
    glfwSetFramebufferSizeCallback(window_, ResizeCallback);

    // Set up square geometry
    size_ = CreateSprite();

    // Initialize shader
    shader_.Init((resources_directory_g+std::string("/vertex_shader.glsl")).c_str(), (resources_directory_g+std::string("/fragment_shader.glsl")).c_str());
    shader_.Enable();

    // Set up z-buffer for rendering
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Enable Alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Enable vector normalizing
    glEnable (GL_NORMALIZE);
}


Game::~Game()
{

    glfwDestroyWindow(window_);
    glfwTerminate();
}


void Game::Setup(void)
{

    // Load textures
    SetAllTextures();

    // Setup background
    //GameObject* background = new GameObject (glm::vec3 (0.0f, 0.0f, 0.0f), tex_[3], size_, false, 10);
    //background->SetScale (50.0);
    //game_objects_.push_back (background);
    //GameObject* background = new GameObject(glm::vec3((float)0, (float)0, 0.0f), tex_[11], size_, false, 1);
    //background->SetScale(10.0f);
    //background_objects_.push_back(background);
    //Islands
    GameObject* island = new GameObject(glm::vec3(0.0f, 4.0f, 0.0f), tex_[12], size_, false, 1);
    //island->SetScale(10.0f);
    background_objects_.push_back(island);
    
    for (int x = -10; x < 10; x++) {
        for (int y = -10; y < 10; y++) {
            GameObject* background = new GameObject(glm::vec3((float)x * 10, (float)y * 10, 0.0f), tex_[11], size_, false, 1);
            background->SetScale(10.0f);
            background_objects_.push_back(background);
        }
    }

    


    // Setup the player object (position, texture, vertex count)
    // Note that, in this specific implementation, the player object should always be the first object in the game object vector (but not this one :) )
    game_objects_.push_back(new PlayerGameObject(glm::vec3(0.0f, 0.0f, 0.0f), tex_[0], size_, true, 1, 10));

    // Setup blades object
    game_objects_.push_back (new HelicopterBlades (glm::vec3 (0.0f, 0.13f, 0.0f), tex_[6], size_, false, 1));

    HelicopterBlades* blades = (HelicopterBlades*)(game_objects_[1]);
    blades->setParent (game_objects_[0]);

    // Set up shields (applied to the player)
    game_objects_.push_back (new Shield (glm::vec3 (0.0f, 0.8f, 0.0f), tex_[8], size_, false, 1, 0));
    game_objects_.push_back (new Shield (glm::vec3 (0.0f, 0.8f, 0.0f), tex_[8], size_, false, 1, 90));

    Shield* shield = (Shield*)(game_objects_[2]);
    shield->SetScale (0.2);
    shield->setParent (game_objects_[0]);
    shield = (Shield*)(game_objects_[3]);
    shield->SetScale (0.2);
    shield->setParent (game_objects_[0]);

    // Set up enemy objects
    Turret* turret = new Turret(glm::vec3(-2.0f, 2.0f, 0.0f), tex_[12], size_, true, 1); //creates a single turret
    Turret::SetupBullets(&bullet_objects_, &tex_[7], &tex_[13], &size_, game_objects_[0]); //sets up bullet static variables
    enemy_objects_.push_back (turret);
    
    // Set up collectibles
    collectible_objects_.push_back (new CollectibleObject (glm::vec3 (3.0f, 3.0f, 0.0f), tex_[9], size_, true, 1, 0));
    collectible_objects_.push_back (new CollectibleObject (glm::vec3 (3.0f, -3.0f, 0.0f), tex_[9], size_, true, 1, 0));
    collectible_objects_.push_back (new CollectibleObject (glm::vec3 (-3.0f, -3.0f, 0.0f), tex_[9], size_, true, 1, 0));
    collectible_objects_.push_back (new CollectibleObject (glm::vec3 (-3.0f, 3.0f, 0.0f), tex_[9], size_, true, 1, 0));

    for (int i = 0; i < collectible_objects_.size (); ++i) {
        collectible_objects_[i]->SetScale (0.5);
    }

    // Set up buoys
    buoy_objects_.push_back (new BuoyObject (glm::vec3 (-6.0f, 6.0f, 0.0f), tex_[10], size_, true, 1, 5));
    buoy_objects_.push_back (new BuoyObject (glm::vec3 (-6.0f, -6.0f, 0.0f), tex_[10], size_, true, 1, 20));
    buoy_objects_.push_back (new BuoyObject (glm::vec3 (6.0f, -6.0f, 0.0f), tex_[10], size_, true, 1, 5));
    buoy_objects_.push_back (new BuoyObject (glm::vec3 (6.0f, 6.0f, 0.0f), tex_[10], size_, true, 1, 20));

    buoy_objects_[1]->SetScale (2);
    buoy_objects_[3]->SetScale (2);

}


void Game::MainLoop(void)
{
    double lastTime = glfwGetTime();
    double timeHold = 0; // A counter for displaying an explosion for a certain length
    double bulletCooldown = 0; // Keeps track of the cooldown between shots
    int numShield = 2; // Keeps track of the number of shields the player has

    // Loop while the user did not close the window
    while (!glfwWindowShouldClose(window_)){

        // Clear background
        glClearColor(viewport_background_color_g.r,
                     viewport_background_color_g.g,
                     viewport_background_color_g.b, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set view to zoom out, centered by default at 0,0
        float cameraZoom = 0.25f;
        
        // Get player game object
        GameObject* player = game_objects_[0];
        glm::vec3 curpos = player->GetPosition();

        glm::mat4 view_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(cameraZoom, cameraZoom, cameraZoom));
        view_matrix = view_matrix * glm::translate(glm::mat4(0.5f), -curpos);
        shader_.SetUniformMat4("view_matrix", view_matrix);

        // Calculate delta time
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // Update the game
        Update(deltaTime, &timeHold, &bulletCooldown, &numShield);

        // Push buffer drawn in the background onto the display
        glfwSwapBuffers(window_);

        // Update other events like input handling
        glfwPollEvents();
    }
}


void Game::ResizeCallback(GLFWwindow* window, int width, int height)
{

    // Set OpenGL viewport based on framebuffer width and height
    glViewport(0, 0, width, height);
}


// Create the geometry for a sprite (a squared composed of two triangles)
// Return the number of array elements that form the square
int Game::CreateSprite(void)
{
    // The face of the square is defined by four vertices and two triangles

    // Number of attributes for vertices and faces
    // const int vertex_att = 7;  // 7 attributes per vertex: 2D (or 3D) position (2), RGB color (3), 2D texture coordinates (2)
    // const int face_att = 3; // Vertex indices (3)

    GLfloat vertex[]  = {
        // Four vertices of a square
        // Position      Color                Texture coordinates
        -0.5f,  0.5f,    1.0f, 0.0f, 0.0f,    0.0f, 0.0f, // Top-left
         0.5f,  0.5f,    0.0f, 1.0f, 0.0f,    1.0f, 0.0f, // Top-right
         0.5f, -0.5f,    0.0f, 0.0f, 1.0f,    1.0f, 1.0f, // Bottom-right
        -0.5f, -0.5f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f  // Bottom-left
    };

    // Two triangles referencing the vertices
    GLuint face[] = {
        0, 1, 2, // t1
        2, 3, 0  //t2
    };

    // OpenGL buffers
    GLuint vbo, ebo;

    // Create buffer for vertices
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

    // Create buffer for faces (index buffer)
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(face), face, GL_STATIC_DRAW);

    // Return number of elements in array buffer (6 in this case)
    return sizeof(face) / sizeof(GLuint);
}


void Game::SetTexture(GLuint w, const char *fname, bool tiling)
{
    // Bind texture buffer
    glBindTexture(GL_TEXTURE_2D, w);

    // Load texture from a file to the buffer
    int width, height;
    unsigned char* image = SOIL_load_image(fname, &width, &height, 0, SOIL_LOAD_RGBA);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);

    if (!tiling) {
        // Texture Wrapping
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    } else {
        // Texture Wrapping
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    // Texture Filtering
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

}


void Game::SetAllTextures(void)
{
    // Load all textures that we will need
    glGenTextures(NUM_TEXTURES, tex_);
    SetTexture(tex_[0], (resources_directory_g+std::string("/textures/chopper.png")).c_str(), false);
    SetTexture(tex_[1], (resources_directory_g+std::string("/textures/plane_red.png")).c_str(), false);
    SetTexture(tex_[2], (resources_directory_g+std::string("/textures/plane_green.png")).c_str(), false);
    SetTexture(tex_[3], (resources_directory_g+std::string("/textures/dune.png")).c_str(), true);
    SetTexture(tex_[4], (resources_directory_g+std::string("/textures/explosions/bubble_explo1.png")).c_str(), false);
    SetTexture(tex_[5], (resources_directory_g+std::string("/textures/blank.png")).c_str (), false);
    SetTexture(tex_[6], (resources_directory_g+std::string("/textures/blade.png")).c_str (), false);
    SetTexture(tex_[7], (resources_directory_g+std::string("/textures/bullet.png")).c_str (), false);
    SetTexture(tex_[8], (resources_directory_g+std::string ("/textures/orb.png")).c_str (), false);
    SetTexture (tex_[9], (resources_directory_g + std::string ("/textures/shieldpack.png")).c_str (), false);
    SetTexture (tex_[10], (resources_directory_g + std::string ("/textures/buoy.png")).c_str (), false);
    
    //Tile map textures
    SetTexture(tex_[11], (resources_directory_g + std::string("/textures/tilemap/water.jpg")).c_str(), true);
    SetTexture(tex_[12], (resources_directory_g + std::string("/textures/CIWS.png")).c_str(), true);
    SetTexture(tex_[13], (resources_directory_g + std::string("/textures/missile.png")).c_str(), true);
    SetTexture(tex_[14], (resources_directory_g + std::string("/textures/isle8.png")).c_str(), false);
    glBindTexture(GL_TEXTURE_2D, tex_[0]);
}


void Game::Controls (double delta_time, double* bullet_cooldown)
{
    // Get player game object
    GameObject *player = game_objects_[0];
    glm::vec3 curpos = player->GetPosition();
    glm::vec3 curvelocity = player->GetVelocity ();
    float currot = player->GetRotation();
    float currotRadians = currot * 3.14159 / 180;

    // Check for player input and make changes accordingly
    if (glfwGetKey (window_, GLFW_KEY_W) == GLFW_PRESS) { // When moving forward, apply velocity in the bearing direction
        player->SetVelocity (glm::vec3 (
            (-(7 * sin (currotRadians))),
            ((7 * cos (currotRadians))),
            0));

    }
    else if (glfwGetKey (window_, GLFW_KEY_S) == GLFW_PRESS) {
        player->SetVelocity (glm::vec3 (
            (-(3 * sin (currotRadians))),
            ((3 * cos (currotRadians))),
            0));
    }
    else {
        player->SetVelocity (glm::vec3 (
            (-(5 * sin (currotRadians))),
            ((5 * cos (currotRadians))),
            0));
    }
    if (glfwGetKey (window_, GLFW_KEY_D) == GLFW_PRESS) {
        player->SetRotation (currot - 2.0);
    }
    if (glfwGetKey (window_, GLFW_KEY_A) == GLFW_PRESS) {
        player->SetRotation (currot + 2.0);
    }
    if (glfwGetKey(window_, GLFW_KEY_Q) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window_, true);
    }
    if (glfwGetKey (window_, GLFW_KEY_SPACE) == GLFW_PRESS) {
        if (*bullet_cooldown <= 0) {
            BulletObject* bullet = new BulletObject
            (curpos + glm::vec3 ((-(0.25 * sin (currotRadians))), ((0.25 * cos (currotRadians))), 0),
                tex_[7], size_, false, 1, currot, 30.0f);
            bullet->SetScale (0.3);
            bullet_objects_.push_back (bullet);
            *bullet_cooldown = 0.20;
        }
        else {
            *bullet_cooldown -= delta_time;
        }
    }
}


void Game::EnemyDetect (void) { // Used for enemy objects to detect the player
    GameObject* player = game_objects_[0];
    float distance;    
    
    for (int i = 0; i < enemy_objects_.size(); ++i) {
        float distance = glm::length (player->GetPosition () - enemy_objects_[i]->GetPosition ());
        if (distance <= enemy_objects_[i]->getSightingRange()) {
            if (enemy_objects_[i]->getState () == 0) { // If the object was patrolling, change state.
                enemy_objects_[i]->setState (1);
                
            }
            // Set the object's target to the player's current position
            enemy_objects_[i]->setTarget (player->GetPosition ());
        }
        else {
            if (enemy_objects_[i]->getState () == 1) { // If the object was pursuing, give up and patrol nearby
                enemy_objects_[i]->setState (0);
            }
        }
    }
}

void Game::Update (double delta_time, double* time_hold, double* bullet_cooldown, int* num_shield)
{
    if (!gameOver) {
        Controls (delta_time, bullet_cooldown);
    }

    GameObject* current_game_object;
    EnemyGameObject* current_enemy_object;
    BulletObject* current_bullet_object;
    CollectibleObject* current_collectible_object;
    BuoyObject* current_buoy_object;

    EnemyDetect ();

    // Update and render the enemy objects
    for (int i = 0; i < enemy_objects_.size (); i++) {
        current_enemy_object = enemy_objects_[i];
        if (!gameOver) {
            current_enemy_object->Update (delta_time);
        }
        current_enemy_object->Render (shader_);
    }

    // Update and render the bullet objects
    for (int i = 0; i < bullet_objects_.size (); i++) {
        bool hitTarget = false;
        current_bullet_object = bullet_objects_[i];
        float duration = current_bullet_object->getDuration();
        if (!gameOver) {
            current_bullet_object->Update (delta_time);
        }
        hitTarget = BulletCastCollision (current_bullet_object);
        double bulletTime = current_bullet_object->getTimer ();
        if (bulletTime > duration || hitTarget) {
            bullet_objects_.erase (bullet_objects_.begin () + i);
            --i;
        }
        else {
            current_bullet_object->Render (shader_);
        }

    }

    // Update and render collectibles
    for (int i = 0; i < collectible_objects_.size (); i++) {
        current_collectible_object = collectible_objects_[i];
        if (!gameOver) {
            current_collectible_object->Update (delta_time);
        }
        current_collectible_object->Render (shader_);
    }

    // Update and render buoys
    for (int i = 0; i < buoy_objects_.size (); i++) {
        current_buoy_object = buoy_objects_[i];
        if (!gameOver) {
            current_buoy_object->Update (delta_time);
        }
        current_buoy_object->Render (shader_);
    }

    // Update and render other game objects
    for (int i = game_objects_.size () - 1; i >= 0; --i) {

        if (i == 2) { // Helicopter blades get updated before their list order
            continue;
        }

        // Get the current game object
        current_game_object = game_objects_[i];
        if (!gameOver) {
            current_game_object->Update (delta_time);
        }

        if (i == 1) { // Exception to update the blades after the helicopter is updated, but it needs to be rendered first.
            if (!gameOver) {
                game_objects_[1]->Update (delta_time);
            }
            game_objects_[1]->Render (shader_);
        }

        // Render game object
        current_game_object->Render (shader_);
    }

    // Update and render the background
    for (int i = 0; i < background_objects_.size(); i++) {
        background_objects_[i]->Update(delta_time);
        background_objects_[i]->Render(shader_);
    }

    IterateCollision(num_shield);

}

// Does collision testing between all the objects in the game (except bullets for now)
void Game::IterateCollision (int* num_shield) {
    PlayerGameObject* player = (PlayerGameObject*)(game_objects_[0]);
    bool playerHit = false;
    for (int i = 0; i < enemy_objects_.size (); ++i) {
        if (DetectCollision (player, enemy_objects_[i])) {
            DamagePlayer (num_shield, i);
            break;
        }
    }

    for (int i = 0; i < collectible_objects_.size (); ++i) {
        if (DetectCollision (player, collectible_objects_[i])) {
            ApplyEffect (num_shield, i, collectible_objects_[i]);
        }
    }

    for (int i = 0; i < buoy_objects_.size (); ++i) {
        if (DetectCollision (player, buoy_objects_[i])) {
            BuoyBounce (player, buoy_objects_[i]);
        }
    }
}

//************************* Collision Detection *******************************

bool Game::DetectCollision (PlayerGameObject* player, EnemyGameObject* enemy) {

    float x2 = enemy->GetPosition ().x;
    float y2 = enemy->GetPosition ().y;
    float r2 = 0.4f;
    float x1 = player->GetPosition ().x;
    float y1 = player->GetPosition ().y;
    float r1 = 0.4f;
    if ((pow ((x2 - x1), 2) + pow ((y2 - y1), 2)) <= pow ((r1 + r2), 2)) {
        return true;
    }
    return false;
}

bool Game::DetectCollision (PlayerGameObject* player, CollectibleObject* collectible) {
    float x2 = collectible->GetPosition ().x;
    float y2 = collectible->GetPosition ().y;
    float r2 = 0.4f;
    float x1 = player->GetPosition ().x;
    float y1 = player->GetPosition ().y;
    float r1 = 0.4f;
    if ((pow ((x2 - x1), 2) + pow ((y2 - y1), 2)) <= pow ((r1 + r2), 2)) {
        return true;
    }
    return false;
}

bool Game::DetectCollision (PlayerGameObject* player, BuoyObject* buoy) {
    float x2 = buoy->GetPosition ().x;
    float y2 = buoy->GetPosition ().y;
    float r2 = 0.4f * buoy->GetScale();
    float x1 = player->GetPosition ().x;
    float y1 = player->GetPosition ().y;
    float r1 = 0.4f;
    if ((pow ((x2 - x1), 2) + pow ((y2 - y1), 2)) <= pow ((r1 + r2), 2)) {
        return true;
    }
    return false;
}

//*************************************************************************

// I was going to move this into different functions, but it became 11:00pm really quickly
bool Game::BulletCastCollision (BulletObject* bullet) {
    for (int i = 0; i < enemy_objects_.size (); i++) {
        if (enemy_objects_[i]->GetCollidable ()) {

            // Turning variables into neater versions for the formula
            float x0 = bullet->GetPosition ().x;
            float x1 = x0 + bullet->GetVelocity ().x;
            float y0 = bullet->GetPosition ().y;
            float y1 = y0 + bullet->GetVelocity ().y;
            float h = enemy_objects_[i]->GetPosition ().x;
            float k = enemy_objects_[i]->GetPosition ().y;
            float a = pow((x1 - x0),2) + pow((y1-y0), 2);
            float b = 2 * (x1 - x0) * (x0 - h) + 2 * (y1 - y0) * (y0 - k);
            float c = pow ((x0 - h), 2) + pow ((y0 - k), 2) - pow (0.7f, 2);
            float t = (2 * c) / (-b + sqrt (pow (b, 2) - (4 * a * c)));
            if (t > 0) {
                enemy_objects_.erase (enemy_objects_.begin () + i);
                return true;
            }
        }
    }
    return false;
}

// *********************** Collision Resolution *****************************

void Game::DamagePlayer (int* num_shield, int enemy_hit) {
    if (*num_shield > 0) {
        game_objects_.erase (game_objects_.end () - 1);
        enemy_objects_.erase (enemy_objects_.begin () + enemy_hit);
        *num_shield -= 1;
    }
    else {
        gameOver = true;
    }
}

void Game::ApplyEffect (int* num_shield, int collectible_hit, CollectibleObject* collectible) {
    if (collectible->getType() == 0) { // Check the collectible type
        if (*num_shield < 4) { // Player can't have more than 4 shields
            *num_shield += 1;
            Shield* shield = new Shield (glm::vec3 (0.0f, 0.8f, 0.0f), tex_[8], size_, false, 1, game_objects_.back()->GetRotation());
            shield->SetScale (0.2);
            shield->setParent (game_objects_[0]);
            game_objects_.push_back (shield);
            collectible_objects_.erase (collectible_objects_.begin () + collectible_hit);
        }
    }    
}

void Game::BuoyBounce (PlayerGameObject* player, BuoyObject* buoy) {
    // Turning variables into neater versions for the formula
    float m1 = player->getMass ();
    float m2 = buoy->getMass ();
    glm::vec3 c1 = player->GetPosition ();
    glm::vec3 c2 = buoy->GetPosition ();
    glm::vec3 n = glm::normalize(c1 - c2);
    glm::vec3 v1i = player->GetVelocity ();
    glm::vec3 v2i = buoy->GetVelocity ();
    glm::vec3 v1f = v1i - (((2 * m2) / (m1 + m2)) * (n * (v1i - v2i)) * n);
    glm::vec3 v2f = v2i - (((2 * m1) / (m1 + m2)) * (n * (v2i - v1i)) * n);;
    player->SetVelocity (v1f);
    buoy->SetVelocity (v2f);
}

// **************************************************************************

} // namespace game