#include <stdexcept>
#include <string>
#include <glm/gtc/matrix_transform.hpp> 
#include <SOIL/SOIL.h>
#include <typeinfo>
#include <path_config.h>
#include <iostream>
#include <limits>
#include "particle_system.h"

#include "shader.h"
#include "game.h"


namespace game {

// Some configuration constants
// They are written here as global variables, but ideally they should be loaded from a configuration file

// Globals that define the OpenGL window and viewport
const char *window_title_g = "Liberating Riraellu";
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

    score_ = 0;

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

    // Initialize particle shader
    particle_shader_.Init((resources_directory_g + std::string("/particle_vertex_shader.glsl")).c_str(), (resources_directory_g + std::string("/particle_fragment_shader.glsl")).c_str());
    particle_shader_.CreateParticles();

    //Meant to be the explosion shader but more needs to be done to get explosion looking like an explosion
    jet_shader_.Init((resources_directory_g + std::string("/jet_vertex_shader.glsl")).c_str(), (resources_directory_g + std::string("/jet_fragment_shader.glsl")).c_str());
    jet_shader_.CreateParticles();

    // Set up square geometry

    // Initialize shader
    shader_.Init((resources_directory_g+std::string("/vertex_shader.glsl")).c_str(), (resources_directory_g+std::string("/fragment_shader.glsl")).c_str());
    shader_.CreateSprite();
    //shader_.Enable();
    //shader_.SetSpriteAttributes();

    player_shader_.Init((resources_directory_g + std::string("/player_vertex_shader.glsl")).c_str(), (resources_directory_g + std::string("/player_fragment_shader.glsl")).c_str());
    player_shader_.CreateSprite();

    // Set up z-buffer for rendering
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Enable Alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Enable vector normalizing
    glEnable (GL_NORMALIZE);

    current_time_ = 0.0;
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


    Healthbar* health_foreground = new Healthbar(glm::vec3(1.43f, -0.15f, 0.0f), tex_[16], 1);
    healthbar_ = health_foreground;
    health_foreground->SetScale(2.5f);
    health_foreground->SetScaley(2.5f); 

    //important note the healthbar is scaled between 0 and 2.5. 2.5 being maximum health (only should change scaley)

    ui_objects_.push_back(health_foreground);


    UI_Element* health_background = new UI_Element(glm::vec3(1.5f, 0.0f, 0.0f), tex_[15], 1);
    health_background->SetScale(2.5f);
    ui_objects_.push_back(health_background);

    UI_Element* missile_ready = new UI_Element(glm::vec3(-5.0f, 4.5f, 0.0f), tex_[14], 1);
    missile_ready->SetRotation(-135.0f);
    missile_ready->SetScale(1.5f);
    missile_ready_ = missile_ready;
    ui_objects_.push_back(missile_ready);

    //the label for the score looks like -> score:
    UI_Element* score = new UI_Element(glm::vec3(-4.5f, 5.0f, 0.0f), tex_[17], 1);
    score->SetScale(FONT_SIZE);
    ui_objects_.push_back(score);

    //proof of concept number making:
    Number* score_value = new Number(glm::vec3(-2.0f, 5.0f, 0.0f), tex_[17], 1, 0);
    score_ptr_ = score_value;
    score_value->SetScale(FONT_SIZE);
    ui_objects_.push_back(score_value);

    GameObject* background = new GameObject (glm::vec3 (0.0f, 0.0f, 0.0f), tex_[11], false, 1);
    background->SetScale (100.0);
    background_objects_.push_back (background);

    // Setup the player object (position, texture, vertex count)
    // Note that, in this specific implementation, the player object should always be the first object in the game object vector
    game_objects_.push_back(new PlayerGameObject(glm::vec3(0.0f, 0.0f, 0.0f), tex_[0],  true, 1, 10));

    UI_Element::Setup(game_objects_[0]); // setup ui elements object

    // Set up bases
    Base* base = new Base(glm::vec3(-6.0f, 20.0f, 0.0f), tex_[31], false, 1, 5, 10.0f);
    bases_.push_back(base);
    base = new Base (glm::vec3 (-22.0f, -25.0f, 0.0f), tex_[31], false, 1, 5, 10.0f);
    bases_.push_back (base);
    base = new Base (glm::vec3 (10.0f, -26.0f, 0.0f), tex_[31], false, 1, 5, 10.0f);
    bases_.push_back (base);
    base = new Base (glm::vec3 (27.0f, 16.0f, 0.0f), tex_[31], false, 1, 5, 10.0f);
    bases_.push_back (base);

    // Set up turret and bullets
    Turret* turret = new Turret(glm::vec3(-2.0f, 2.0f, 0.0f), tex_[13], true, 1, 3, bases_[0]); //creates a single turret
    Turret::SetupBullets(&bullet_objects_, &missile_objects_, &particle_objects_, &tex_[7], &tex_[14], &tex_[30], game_objects_[0]); //sets up bullet static variables
    BulletObject::SetDamageBullet(1);
    MissileObject::SetDamageMissile(4);

    // Set up drones
    Drone* drone = new Drone (glm::vec3 (-6.0f, 20.0f, 0.0f), tex_[35], true, 1, 1);
    drone->SetScale (2.0f);
    enemy_objects_.push_back (drone);
    drone = new Drone (glm::vec3 (-22.0f, -25.0f, 0.0f), tex_[35], true, 1, 1);
    drone->SetScale (2.0f);
    enemy_objects_.push_back (drone);
    drone = new Drone (glm::vec3 (10.0f, -26.0f, 0.0f), tex_[35], true, 1, 1);
    drone->SetScale (2.0f);
    enemy_objects_.push_back (drone);

    // Set up helicopters
    Helicopter* helicopter = new Helicopter (glm::vec3 (-4.0f, 18.0f, 0.0f), tex_[38], true, 1, 5);
    Helicopter::SetupBullets (&bullet_objects_, &tex_[7], game_objects_[0]);
    HelicopterBlades* blades = new HelicopterBlades (glm::vec3 (0.0f, 0.13f, 0.0f), tex_[6], false, 1);
    blades->setParent (helicopter);
    helicopter->setBlades (blades);
    enemy_objects_.push_back (helicopter);
    helicopter = new Helicopter (glm::vec3 (-20.0f, 0.0f, 0.0f), tex_[38], true, 1, 5);
    blades = new HelicopterBlades (glm::vec3 (0.0f, 0.13f, 0.0f), tex_[6], false, 1);
    blades->setParent (helicopter);
    helicopter->setBlades (blades);
    enemy_objects_.push_back (helicopter);
    helicopter = new Helicopter (glm::vec3 (8.0f, -24.0f, 0.0f), tex_[38], true, 1, 5);
    blades = new HelicopterBlades (glm::vec3 (0.0f, 0.13f, 0.0f), tex_[6], false, 1);
    blades->setParent (helicopter);
    helicopter->setBlades (blades);
    enemy_objects_.push_back (helicopter);

    // Set up shield packs
    collectible_objects_.push_back (new CollectibleObject (glm::vec3 (33.75f, 29.75f, 0.0f), tex_[9],  true, 1, 0));
    collectible_objects_.push_back (new CollectibleObject (glm::vec3 (-37.0f, -20.0f, 0.0f), tex_[9],  true, 1, 0));
    collectible_objects_.push_back (new CollectibleObject (glm::vec3 (20.0f, -10.0f, 0.0f), tex_[9],  true, 1, 0));
    collectible_objects_.push_back (new CollectibleObject (glm::vec3 (7.0f, -34.0f, 0.0f), tex_[9],  true, 1, 0));

    // Set up health packs
    collectible_objects_.push_back (new CollectibleObject (glm::vec3 (4.0f, 35.0f, 0.0f), tex_[28], true, 1, 1));
    collectible_objects_.push_back (new CollectibleObject (glm::vec3 (-30.0f, -5.0f, 0.0f), tex_[28], true, 1, 1));

    // Set up cloakers
    collectible_objects_.push_back (new CollectibleObject (glm::vec3 (-15.0f, -35.0f, 0.0f), tex_[29], true, 1, 2));
    collectible_objects_.push_back (new CollectibleObject (glm::vec3 (5.0f, -25.0f, 0.0f), tex_[29], true, 1, 2));
    collectible_objects_.push_back (new CollectibleObject (glm::vec3 (5.0f, 15.0f, 0.0f), tex_[29], true, 1, 2));

    for (int i = 0; i < collectible_objects_.size (); ++i) {
        collectible_objects_[i]->SetScale (0.7f);
    }


    // Setup particle system
    /*
    */
    GameObject* particles = new ParticleSystem(glm::vec3(0.0f, -0.5f, 0.0f), tex_[30], game_objects_[0]);
    particles->SetScale(0.2f);
    player_particles_.push_back(particles);


    //setup minimap
    Minimap* minimap = new Minimap(glm::vec3(3.8f, 4.2f, 0.0f), tex_[49], 1);
    minimap->SetScale(8.0f);
    ui_objects_.push_back(minimap);

    

}


void Game::MainLoop(void)
{
    PlayerGameObject* player = (PlayerGameObject*)game_objects_[0];
    double lastTime = glfwGetTime();
    double timeHold = 0; // A counter for displaying an explosion for a certain length
    double bulletCooldown = 0; // Keeps track of the cooldown between shots
    int numShield = 0; // Keeps track of the number of shields the player has

    // Loop while the user did not close the window
    while (!glfwWindowShouldClose(window_)){
        if (paused_) {
            lastTime = glfwGetTime();
        }
        paused_ = false;
        selection_made_ = false;

        numShield = player->getNumShield();
        score_ptr_->SetNumber(score_);

        // Clear background
        glClearColor(viewport_background_color_g.r,
                     viewport_background_color_g.g,
                     viewport_background_color_g.b, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set view to zoom out, centered by default at 0,0
        float cameraZoom = 0.175f;
        
        // Get player game object
        //GameObject* player = game_objects_[0];
        //glm::vec3 curpos = player->GetPosition();

        //glm::mat4 view_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(cameraZoom, cameraZoom, cameraZoom));
        //view_matrix = view_matrix * glm::translate(glm::mat4(0.5f), -curpos);
        //shader_.SetUniformMat4("view_matrix", view_matrix);
        //particle_shader_.SetUniformMat4("view_matrix", view_matrix);
        
        


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


        if (CheckBasesFinished()) {
            VictoryLoop();
        }
    }
}


void Game::ResizeCallback(GLFWwindow* window, int width, int height)
{

    // Set OpenGL viewport based on framebuffer width and height
    glViewport(0, 0, width, height);
}


// Create the geometry for a sprite (a squared composed of two triangles)
// Return the number of array elements that form the square


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
    SetTexture(tex_[0], (resources_directory_g+std::string("/textures/jetfighter.png")).c_str(), false);
    SetTexture(tex_[1], (resources_directory_g+std::string("/textures/plane_red.png")).c_str(), false);
    SetTexture(tex_[2], (resources_directory_g+std::string("/textures/plane_green.png")).c_str(), false);
    SetTexture(tex_[3], (resources_directory_g+std::string("/textures/dune.png")).c_str(), true);
    SetTexture(tex_[4], (resources_directory_g+std::string("/textures/explosions/bubble_explo1.png")).c_str(), false);
    SetTexture(tex_[5], (resources_directory_g+std::string("/textures/blank.png")).c_str (), false);
    SetTexture(tex_[6], (resources_directory_g+std::string("/textures/blade.png")).c_str (), false);
    SetTexture(tex_[7], (resources_directory_g+std::string("/textures/bullet.png")).c_str (), false);
    SetTexture(tex_[8], (resources_directory_g+std::string ("/textures/shield.png")).c_str (), false);
    SetTexture (tex_[9], (resources_directory_g + std::string ("/textures/shieldpack.png")).c_str (), false);
    SetTexture (tex_[10], (resources_directory_g + std::string ("/textures/buoy.png")).c_str (), false);
    
    //Tile map textures
    SetTexture(tex_[11], (resources_directory_g + std::string("/textures/tilemap/background.png")).c_str(), true);
    SetTexture(tex_[12], (resources_directory_g + std::string("/textures/isle8.png")).c_str(), false);

    //weaponry related textures
    SetTexture(tex_[13], (resources_directory_g + std::string("/textures/CIWS.png")).c_str(), true);
    SetTexture(tex_[14], (resources_directory_g + std::string("/textures/missile.png")).c_str(), true);

    //ui related textures
    SetTexture(tex_[15], (resources_directory_g + std::string("/textures/red_bar.png")).c_str(), false);
    SetTexture(tex_[16], (resources_directory_g + std::string("/textures/green_bar.png")).c_str(), false);
    SetTexture(tex_[17], (resources_directory_g + std::string("/textures/score.png")).c_str(), false);

    //all the numbers from 0-9
    SetTexture(tex_[18], (resources_directory_g + std::string("/textures/0.png")).c_str(), false);
    SetTexture(tex_[19], (resources_directory_g + std::string("/textures/1.png")).c_str(), false);
    SetTexture(tex_[20], (resources_directory_g + std::string("/textures/2.png")).c_str(), false);
    SetTexture(tex_[21], (resources_directory_g + std::string("/textures/3.png")).c_str(), false);
    SetTexture(tex_[22], (resources_directory_g + std::string("/textures/4.png")).c_str(), false);
    SetTexture(tex_[23], (resources_directory_g + std::string("/textures/5.png")).c_str(), false);
    SetTexture(tex_[24], (resources_directory_g + std::string("/textures/6.png")).c_str(), false);
    SetTexture(tex_[25], (resources_directory_g + std::string("/textures/7.png")).c_str(), false);
    SetTexture(tex_[26], (resources_directory_g + std::string("/textures/8.png")).c_str(), false);
    SetTexture(tex_[27], (resources_directory_g + std::string("/textures/9.png")).c_str(), false);
    //SetTexture(tex_[14], (resources_directory_g + std::string("/textures/isle8.png")).c_str(), false);

    // Health pack texture
    SetTexture (tex_[28], (resources_directory_g + std::string ("/textures/healthpack.png")).c_str (), false);
    // Cloaker texture
    SetTexture (tex_[29], (resources_directory_g + std::string ("/textures/cloaker.png")).c_str (), false);
    SetTexture(tex_[30], (resources_directory_g + std::string("/textures/particleOrb.png")).c_str(), false);
    glBindTexture(GL_TEXTURE_2D, tex_[0]);

    SetTexture(tex_[31], (resources_directory_g + std::string("/textures/base.png")).c_str(), false);

    //big squre
    SetTexture(tex_[32], (resources_directory_g + std::string("/textures/navy.png")).c_str(), false);

    //landing symbol
    SetTexture(tex_[33], (resources_directory_g + std::string("/textures/landing.png")).c_str(), false);

    //Options when base captured
    SetTexture(tex_[34], (resources_directory_g + std::string("/textures/selection.png")).c_str(), false);

    //drone texture
    SetTexture(tex_[35], (resources_directory_g + std::string("/textures/drone.png")).c_str(), false);
    //Particle texture
    SetTexture(tex_[36], (resources_directory_g + std::string("/textures/blank.png")).c_str(), false);

    //game over screen
    SetTexture(tex_[37], (resources_directory_g + std::string("/textures/game_over.png")).c_str(), false);
    // Helicopter texture
    SetTexture (tex_[38], (resources_directory_g + std::string ("/textures/chopper.png")).c_str (), false);
    //Explosion Textures

    SetTexture(tex_[39], (resources_directory_g + std::string("/textures/explosions/bubble_explo1.png")).c_str(), false);
    SetTexture(tex_[40], (resources_directory_g + std::string("/textures/explosions/bubble_explo2.png")).c_str(), false);
    SetTexture(tex_[41], (resources_directory_g + std::string("/textures/explosions/bubble_explo3.png")).c_str(), false);
    SetTexture(tex_[42], (resources_directory_g + std::string("/textures/explosions/bubble_explo4.png")).c_str(), false);
    SetTexture(tex_[43], (resources_directory_g + std::string("/textures/explosions/bubble_explo5.png")).c_str(), false);
    SetTexture(tex_[44], (resources_directory_g + std::string("/textures/explosions/bubble_explo6.png")).c_str(), false);
    SetTexture(tex_[45], (resources_directory_g + std::string("/textures/explosions/bubble_explo7.png")).c_str(), false);
    SetTexture(tex_[46], (resources_directory_g + std::string("/textures/explosions/bubble_explo8.png")).c_str(), false);
    SetTexture(tex_[47], (resources_directory_g + std::string("/textures/explosions/bubble_explo9.png")).c_str(), false);
    SetTexture(tex_[48], (resources_directory_g + std::string("/textures/explosions/bubble_explo10.png")).c_str(), false);


    //minimap textures
    SetTexture(tex_[49], (resources_directory_g + std::string("/textures/radar_display.png")).c_str(), false);
    SetTexture(tex_[50], (resources_directory_g + std::string("/textures/red_chevron.png")).c_str(), false);
    SetTexture(tex_[51], (resources_directory_g + std::string("/textures/green_circle.png")).c_str(), false);

    //misc textures
    SetTexture(tex_[52], (resources_directory_g + std::string("/textures/victory.png")).c_str(), false);


    //setup number textures:
    for (int i = 18; i < 28; ++i) {
        text_arr_.push_back(tex_[i]);
    }

    //Setup explosion textures
    for (int i = 39; i < 49; i++) {
        explo_arr_.push_back(tex_[i]);
    }

    //setup static variables number
    Number::SetupTextures(text_arr_, &size_);

    //setup static variables for base
    Base::SetupBases(&enemy_objects_, &tex_[13]);

    //setup statics for minimap
    Minimap::Setup(&enemy_objects_, &bases_, &tex_[51], &tex_[50]);
}

bool Game::CheckBasesFinished() {
    for (int i = 0; i < bases_.size(); ++i) {
        if (!bases_[i]->GetAllegiance()) {
            return true;
        }
    }

    return false;
}


void Game::Controls (double delta_time, double* bullet_cooldown)
{
    // Get player game object
    PlayerGameObject *player = (PlayerGameObject*)game_objects_[0];
    glm::vec3 curpos = player->GetPosition();
    glm::vec3 curvelocity = player->GetVelocity ();
    float currot = player->GetRotation();
    float currotRadians = currot * 3.14159f / 180.0f;

    // Check for player input and make changes accordingly
    if (glfwGetKey (window_, GLFW_KEY_W) == GLFW_PRESS) { // When moving forward, apply velocity in the bearing direction
        player->SetVelocity (glm::vec3 (
            (-(MAX_SPEED * sin (currotRadians))),
            ((MAX_SPEED * cos (currotRadians))),
            0));

    }
    else if (glfwGetKey (window_, GLFW_KEY_S) == GLFW_PRESS) {
        player->SetVelocity (glm::vec3 (
            (-(MIN_SPEED * sin (currotRadians))),
            ((MIN_SPEED * cos (currotRadians))),
            0));
    }
    else {
        player->SetVelocity (glm::vec3 (
            (-(STANDARD_SPEED * sin (currotRadians))),
            ((STANDARD_SPEED * cos (currotRadians))),
            0));
    }
    if (glfwGetKey (window_, GLFW_KEY_D) == GLFW_PRESS) {
        player->SetRotation (currot - 2.0f);
    }
    if (glfwGetKey (window_, GLFW_KEY_A) == GLFW_PRESS) {
        player->SetRotation (currot + 2.0f);
    }
    if (glfwGetKey(window_, GLFW_KEY_Q) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window_, true);
    }
    if (glfwGetKey (window_, GLFW_KEY_SPACE) == GLFW_PRESS) {
        if (*bullet_cooldown <= 0) {
            BulletObject* bullet = new BulletObject
            (curpos + glm::vec3 ((-(0.25 * sin (currotRadians))), ((0.25 * cos (currotRadians))), 0),
                tex_[7],  false, 1, currot, 30.0f, "player");
            bullet->SetScale (0.5f);
            bullet_objects_.push_back (bullet);
            *bullet_cooldown = 0.10f;
        }
    }
    if (glfwGetKey(window_, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS && player->getMissileCooldown() >= MISSILE_COOLDOWN) {
        GameObject* target = FindClosest();
        if (target != NULL) {
            MissileObject* missile = new MissileObject(curpos + glm::vec3((-(0.25 * sin(currotRadians))), ((0.25 * cos(currotRadians))), 0),
                tex_[14], false, 1, currot, 10.0f, target, "player");
            missile->SetScale(0.5f);
            missile->SetDuration(2.0f);
            missile_objects_.push_back(missile);
            player->SetMissileCooldown(0);
            //Attach particle system to missile
            ParticleSystem* particles = new ParticleSystem(glm::vec3(0.3f, 0.0f, 0.0f), tex_[30], missile_objects_[missile_objects_.size() - 1]);
            particles->SetScale(0.2f);
            particles->SetRotation(90);
            
            particle_objects_.push_back(particles);
        }
    }


    if (glfwGetKey(window_, GLFW_KEY_UP) == GLFW_PRESS && !gameOver) {
        gameOver = true;
        GameOverLoop();
    }

    if (glfwGetKey(window_, GLFW_KEY_DOWN) == GLFW_PRESS && !gameOver) {
        score_ += 5;
    }

    if (glfwGetKey(window_, GLFW_KEY_1) == GLFW_PRESS) {
        selection_made_ = true;
        choice_ = 1;
    }
    if (glfwGetKey(window_, GLFW_KEY_2) == GLFW_PRESS) {
        selection_made_ = true;
        choice_ = 2;
    }
    if (glfwGetKey(window_, GLFW_KEY_3) == GLFW_PRESS) {
        selection_made_ = true;
        choice_ = 3;
    }
}


void Game::EnemyDetect (void) { // Used for enemy objects to detect the player
    PlayerGameObject* player = (PlayerGameObject*)game_objects_[0];
    
    for (int i = 0; i < enemy_objects_.size(); ++i) {
        float distance = glm::length (player->GetPosition () - enemy_objects_[i]->GetPosition ());
        if (distance <= enemy_objects_[i]->getSightingRange() && !player->getCloaked()) {
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

    Controls(delta_time, bullet_cooldown);
    
    current_time_ += delta_time;

    GameObject* current_game_object;
    EnemyGameObject* current_enemy_object;
    BulletObject* current_bullet_object;
    Base* current_base;
    CollectibleObject* current_collectible_object;
    BuoyObject* current_buoy_object;
    MissileObject* current_missile_object;
    PlayerGameObject* player = (PlayerGameObject*)game_objects_[0];

    EnemyDetect ();

    //View_matrix initialization goes here
    float cameraZoom = 0.175f;    
    glm::vec3 curpos = player->GetPosition();

    glm::mat4 view_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(cameraZoom, cameraZoom, cameraZoom));
    view_matrix = view_matrix * glm::translate(glm::mat4(0.5f), -curpos);

    // Update and render the ui elements
    int health = player->getHealth();

    *bullet_cooldown -= delta_time;

    float health_percent = health * (2.5f / 10);

    healthbar_->SetScaley(health_percent);

    player->Update(delta_time);
    player->Render(player_shader_, view_matrix);

    for (int i = 0; i < ui_objects_.size(); i++) {
        ui_objects_[i]->Update(delta_time);

        //don't render missile ready ui element when missile isn't ready
        if (ui_objects_[i] == missile_ready_ && ((PlayerGameObject*)game_objects_[0])->getMissileCooldown() < MISSILE_COOLDOWN) {
            continue;
        }

        ui_objects_[i]->Render(shader_, view_matrix);
    }

    // Update and render the enemy objects
    for (int i = 0; i < enemy_objects_.size (); i++) {
        current_enemy_object = enemy_objects_[i];
        if (!gameOver) {
            current_enemy_object->Update (delta_time);
        }
        current_enemy_object->Render (shader_, view_matrix);
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
            delete bullet_objects_[i];
            bullet_objects_.erase (bullet_objects_.begin () + i);
            --i;
        }
        else {
            current_bullet_object->Render (shader_, view_matrix);
        }

    }

    // Update and render collectibles
    for (int i = 0; i < collectible_objects_.size (); i++) {
        current_collectible_object = collectible_objects_[i];
        if (!gameOver) {
            current_collectible_object->Update (delta_time);
        }
        current_collectible_object->Render (shader_, view_matrix);
    }



    // Update and render buoys
    for (int i = 0; i < buoy_objects_.size (); i++) {
        current_buoy_object = buoy_objects_[i];
        if (!gameOver) {
            current_buoy_object->Update (delta_time);
        }
        current_buoy_object->Render (shader_, view_matrix);
    }

    // Update and render missiles
    for (int i = 0; i < missile_objects_.size(); i++) {
        current_missile_object = missile_objects_[i];
        bool hitTarget = false;        
        float duration = current_missile_object->getDuration();
        if (!gameOver) {
            if (current_missile_object->GetFrom() == "enemy" && player->getCloaked ()) {
                current_missile_object->BulletObject::Update (delta_time);
            }
            else {
                current_missile_object->Update (delta_time);
            }
        }
        
        hitTarget = BulletCastCollision(current_missile_object);
        double missileTime = current_missile_object->getTimer();
        if (missileTime > duration || hitTarget) {         
            //If the missile hit, create an explosion at it's point of impact            
            GameObject* explosion = new GameObject(current_missile_object->GetPosition(), explo_arr_[0], false, 1);
            explosion->SetScale(1.2f);
            explosion->SetTime(0);
            explosion_objects_.push_back(explosion);            
            
            particle_objects_.erase(particle_objects_.begin() + i);           
            missile_objects_.erase(missile_objects_.begin() + i); //If the missile time has exceeded its duration, or if it hit something, get rid of it and it's particles
            --i;
        }
        else {
            current_missile_object->Render(shader_, view_matrix);
        }
    }

    // Update and render other game objects
    for (int i = 0; i < game_objects_.size(); i++) {

        // Get the current game object
        current_game_object = game_objects_[i];
        if (!gameOver) {
            current_game_object->Update (delta_time);
        }
        // Render game object
        if (i == 0) {
            current_game_object->Render(player_shader_, view_matrix);
        }

        else {
            current_game_object->Render (shader_, view_matrix);
        }
    }



    for (int i = 0; i < explosion_objects_.size(); i++) {
        GameObject* current_explosion_object = explosion_objects_[i];
        current_explosion_object->Update(delta_time);
        if (current_explosion_object->GetTexture() < 49 && current_explosion_object->GetTime() > 0.07f) {
            current_explosion_object->SetTexture(current_explosion_object->GetTexture() + 1);
            current_explosion_object->SetTime(0);
        }
        else if (!(current_explosion_object->GetTime() > 0.07f)) {
            current_explosion_object->SetTime(current_explosion_object->GetTime() + (float)delta_time);
        }
        else {
            delete current_explosion_object;
            explosion_objects_.erase(explosion_objects_.begin() + i);
            --i;
            continue;
        }
        current_explosion_object->Render(shader_, view_matrix);
      
    }
    // Update and render bases
    for (int i = 0; i < bases_.size(); i++) {
        current_base = bases_[i];
        if (!gameOver) {
            current_base->Update(delta_time);
        }
        current_base->Render(shader_, view_matrix);
    }
    // Update and render the background
    for (int i = 0; i < background_objects_.size(); i++) {
        background_objects_[i]->Update(delta_time);
        background_objects_[i]->Render(shader_, view_matrix);
    }
    //Render particles (missiles)
    for (int i = 0; i < particle_objects_.size(); i++) {
        ParticleSystem* current_particle_object = dynamic_cast<ParticleSystem*>(particle_objects_[i]);
        current_particle_object->Update(delta_time);

        if (current_particle_object != nullptr) {
            current_particle_object->Render(particle_shader_, view_matrix, current_time_);
        }
        else {
            current_particle_object->Render(shader_, view_matrix, current_time_);
        }
    }

    //Render particles (player)
    for (int i = 0; i < player_particles_.size(); i++) {
        ParticleSystem* player_particle = dynamic_cast<ParticleSystem*>(player_particles_[i]);
        player_particle->Update(delta_time);

        if (player_particle != nullptr) {
            player_particle->Render(jet_shader_, view_matrix, current_time_); 
        }
        else {
            player_particle->Render(shader_, view_matrix, current_time_);
        }
        
    }

    IterateCollision();

}

// Does collision testing between all the objects in the game (except bullets for now)
void Game::IterateCollision () {
    PlayerGameObject* player = (PlayerGameObject*)(game_objects_[0]);
    bool playerHit = false;
    for (int i = 0; i < enemy_objects_.size (); ++i) {
        if (!enemy_objects_[i]->GetCollidable()) { continue; }
        if (DetectCollision (player, enemy_objects_[i])) {
            if (player->GetCollidable()) {
                DamagePlayer (5);
            }
            UpdateScore(-50);
            delete enemy_objects_[i];
            enemy_objects_.erase (enemy_objects_.begin () + i);
            break;
        }
    }

    for (int i = 0; i < collectible_objects_.size (); ++i) {
        if (DetectCollision (player, collectible_objects_[i])) {
            ApplyEffect (i, collectible_objects_[i]);
        }
    }

    for (int i = 0; i < buoy_objects_.size (); ++i) {
        if (DetectCollision (player, buoy_objects_[i])) {
            BuoyBounce (player, buoy_objects_[i]);
        }
    }


    for (int i = 0; i < bases_.size(); ++i) {
        if (DetectCollision(player, bases_[i]) && bases_[i]->IsCapturable()) {
            bases_[i]->SetAllegiance(false);
            bases_[i]->SetCapturable(false);
            ChooseLoop();
            bases_[i]->SetAllegiance(false);
            bases_[i]->SetCapturable(false);
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

bool Game::DetectCollision(PlayerGameObject* player, Base* base) {
    float x2 = base->GetPosition().x;
    float y2 = base->GetPosition().y;
    float r2 = 0.4f * base->GetScale();
    float x1 = player->GetPosition().x;
    float y1 = player->GetPosition().y;
    float r1 = 0.4f;
    if ((pow((x2 - x1), 2) + pow((y2 - y1), 2)) <= pow((r1 + r2), 2)) {
        return true;
    }
    return false;
}

//*************************************************************************

// I was going to move this into different functions, but it became 11:00pm really quickly
bool Game::BulletCastCollision (BulletObject* bullet) {
    //If the bullet fired is from the player, check the enemies to see who got hit
    
    if (bullet->GetFrom() == "player") {

        //Checking for enemy collision with bullets
        for (int i = 0; i < enemy_objects_.size(); i++) {
                float distance = glm::length(enemy_objects_[i]->GetPosition() - bullet->GetPosition());
                //Increased hitbox of turret boi because bullets were missing too often.
                if (distance < 0.35f) {
                    enemy_objects_[i]->setHealth(enemy_objects_[i]->getHealth() - bullet->GetDamage());
                    if (enemy_objects_[i]->getHealth() <= 0) {
                        delete enemy_objects_[i];
                        UpdateScore(100);
                        enemy_objects_.erase(enemy_objects_.begin() + i);
                        return true;
                    }
                }
        }
    }
    //If the bullet fired is from an enemy, check to see if it hits the player
    else if (bullet->GetFrom() == "enemy") {
        PlayerGameObject* player = (PlayerGameObject*)game_objects_[0];
        if (player->GetCollidable()) {
            float distance = glm::length(player->GetPosition() - bullet->GetPosition());
            if (distance < 0.2f) {
                if(!player->IsInvincible()) {
                    int damage = bullet->GetDamage();
                    DamagePlayer(damage);
                }
                return true;
            }
        }
    }
    return false;
}

// *********************** Collision Resolution *****************************

void Game::DamagePlayer (int damage) {
    PlayerGameObject* player = (PlayerGameObject*)game_objects_[0];
  ;
    
    if (player->getNumShield() > 0) {
        float shieldOrbsBefore = ceil(player->getNumShield () / float((MAX_SHIELD) / 4)); // Current number of orbs is proportional to shielding
       
        player->minusShield(damage);
        player->resetIFrame(); //Make player invincible for short time
        float shieldOrbsAfter = ceil(player->getNumShield () / float((MAX_SHIELD) /4));
      
        if (shieldOrbsBefore > shieldOrbsAfter) {
            delete game_objects_.at (game_objects_.size () - 1);
            game_objects_.erase (game_objects_.end () - 1);
        }
 
    }
    else if (player->getHealth() - damage > 0){
        player->addHealth (-damage);
        player->resetIFrame (); //Make player invincible for short time
    }
    else {
        if (!gameOver) {
            gameOver = true;
            GameOverLoop(); // TODO: flip back to true once gameOver working
        }
    }

    float scale = (float)(player->getHealth() / 4.0f);
    healthbar_->SetScaley(scale);

   
}

void Game::addShieldToPlayer () {
    PlayerGameObject* player = (PlayerGameObject*)game_objects_[0];
    if (player->getNumShield () < (MAX_SHIELD / 4) * 3 + 1) {
        Shield* shield;
        if (player->getNumShield () == 0) {
            shield = new Shield (glm::vec3 (0.0f, 0.8f, 0.0f), tex_[8], false, 1, 0);
        }
        else {
            Shield* backshield = (Shield*)(game_objects_.back ());
            shield = new Shield (glm::vec3 (0.0f, 0.8f, 0.0f), tex_[8], false, 1, backshield->getOrbit ());
            
        }
        shield->SetScale (0.4f);
        shield->setParent (game_objects_[0]);
        game_objects_.push_back (shield);
    }
    player->addShield (MAX_SHIELD / 4);
}

void Game::ApplyEffect (int collectible_hit, CollectibleObject* collectible) {
    PlayerGameObject* player = (PlayerGameObject*)game_objects_[0];
    if (collectible->getType() == 0) { // Check the collectible type
        if (player->getNumShield() < MAX_SHIELD) { // Player can't have more than MAX_SHIELD shielding
            addShieldToPlayer ();
            delete collectible_objects_[collectible_hit];
            collectible_objects_.erase (collectible_objects_.begin () + collectible_hit);
        }
    }
    else if (collectible->getType () == 1) {
        if (player->getHealth () < MAX_HEALTH) {
            player->addHealth (3);
            delete collectible_objects_[collectible_hit];
            collectible_objects_.erase (collectible_objects_.begin () + collectible_hit);
        }
    }
    else if (collectible->getType () == 2) {
        player->cloak ();
        delete collectible_objects_[collectible_hit];
        collectible_objects_.erase (collectible_objects_.begin () + collectible_hit);
    }
}

void Game::BuoyBounce (PlayerGameObject* player, BuoyObject* buoy) {
    // Turning variables into neater versions for the formula
    float m1 = (float)player->getMass ();
    float m2 = (float)buoy->getMass ();
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

GameObject* Game::FindClosest() {
    PlayerGameObject* player = (PlayerGameObject*)game_objects_[0];
    GameObject* closest = NULL;
    float closestDist = std::numeric_limits<float>::max();

    for (int i = 0; i < enemy_objects_.size(); ++i) {
        EnemyGameObject* target = enemy_objects_[i];

        if (glm::dot(player->GetVelocity(), target->GetPosition() - player->GetPosition()) > 0) { //checks to ensure enemy is in front of player
            float distance = glm::length(player->GetPosition() - target->GetPosition()); //gets distance from player to enemy
            if (distance < closestDist) { //if the distance is less than the current closes target, update the current closest target
                closestDist = distance;
                closest = target;
            }
        }
    }


    return closest;
}
// **************************************************************************

void Game::ChooseLoop() {
    PlayerGameObject* player = (PlayerGameObject*)game_objects_[0];
    glm::vec3 player_pos = player->GetPosition();
    paused_ = true;

    const int arr_size = 3;
    int added_ui_count[arr_size] = { 0, 0, 0 };


    //vector of extra ui elements to pass into render
    std::vector<UI_Element*> extras;

    //big square background
    UI_Element* menu1 = new UI_Element(glm::vec3(player->GetPosition()), tex_[32], 1);
    menu1->SetAbsolute(true);
    menu1->SetScale(10.0f);


    //Landing symbol
    UI_Element* menu2 = new UI_Element(glm::vec3(player_pos.x, player_pos.y + 3.0f, 0), tex_[33], 1);
    menu2->SetAbsolute(true);
    menu2->SetScale(5.0f);



    //Selection Options
    UI_Element* menu3 = new UI_Element(glm::vec3(player_pos.x+5.0f, player_pos.y-5.0f, 0), tex_[34], 1);
    menu3->SetAbsolute(true);
    menu3->SetScale(FONT_SIZE*0.8);


    extras.push_back(menu3);
    extras.push_back(menu2);
    extras.push_back(menu1);


    // Loop while the user did not close the window
    while (!glfwWindowShouldClose(window_) && !selection_made_) {
        // Update the game
        Render(extras);



        // Push buffer drawn in the background onto the display
        glfwSwapBuffers(window_);

        // Update other events like input handling
        glfwPollEvents();
    }
    paused_ = true;
    selection_made_ = false;

    switch (choice_) {
        case 1:
            score_ += 1000;
            break;
        case 2:
            player->addHealth(2);
            break;
        case 3:
            if (player->getNumShield() >= MAX_SHIELD) {
                break;
            }

            addShieldToPlayer ();
            break;
    }

    delete menu1;
    delete menu2;
    delete menu3;
}

void Game::VictoryLoop() {
    PlayerGameObject* player = (PlayerGameObject*)game_objects_[0];
    glm::vec3 player_pos = player->GetPosition();
    paused_ = true;

    const int arr_size = 3;
    int added_ui_count[arr_size] = { 0, 0, 0 };


    //vector of extra ui elements to pass into render
    std::vector<UI_Element*> extras;

    //big square background
    UI_Element* menu1 = new UI_Element(glm::vec3(player->GetPosition()), tex_[52], 1);
    menu1->SetAbsolute(true);
    menu1->SetScale(14.0f);




    //Score value
    Number* menu2 = new Number(glm::vec3(1.0f, -2.0f, 0), tex_[17], 1, score_);
    menu2->SetScale(FONT_SIZE * 0.8);

    UI_Element* score_prompt = new UI_Element(glm::vec3(-1.0f, -2.0f, 0), tex_[17], 1);
    score_prompt->SetScale(FONT_SIZE * 0.8);



    extras.push_back(score_prompt);
    extras.push_back(menu2);
    extras.push_back(menu1);


    // Loop while the user did not close the window
    while (!glfwWindowShouldClose(window_)) {
        // Update the game
        Render(extras);



        // Push buffer drawn in the background onto the display
        glfwSwapBuffers(window_);

        // Update other events like input handling
        glfwPollEvents();
    }
}


void Game::Render(std::vector<UI_Element*> extras) {
    // Clear background
    glClearColor(viewport_background_color_g.r,
        viewport_background_color_g.g,
        viewport_background_color_g.b, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set view to zoom out, centered by default at 0,0
    float cameraZoom = 0.175f; // 0.175f

    // Get player game object
    glm::vec3 curpos = game_objects_[0]->GetPosition();

    glm::mat4 view_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(cameraZoom, cameraZoom, cameraZoom));
    view_matrix = view_matrix * glm::translate(glm::mat4(0.5f), -curpos);
    shader_.SetUniformMat4("view_matrix", view_matrix);
    particle_shader_.SetUniformMat4("view_matrix", view_matrix);



    double delta_time = 0;
    double garbage = 99999;

    Controls(delta_time, &garbage);
    current_time_ += delta_time;

    GameObject* current_game_object;
    EnemyGameObject* current_enemy_object;
    BulletObject* current_bullet_object;
    Base* current_base;
    CollectibleObject* current_collectible_object;
    BuoyObject* current_buoy_object;
    MissileObject* current_missile_object;
    PlayerGameObject* player = (PlayerGameObject*)game_objects_[0];

    //EnemyDetect();


    // Update and render the ui elements
    int health = player->getHealth();


    float health_percent = health * (2.5f / 10);

    healthbar_->SetScaley(health_percent);
    

    //render extra ui elements
    for (int i = 0; i < extras.size(); ++i) {
        extras[i]->Update(delta_time);
        extras[i]->Render(shader_, view_matrix);
    }

    //render normal ui elements
    for (int i = (int)ui_objects_.size() - 1; i >= 0; --i) {

        //don't render missile ready ui element when missile isn't ready
        if (ui_objects_[i] == missile_ready_ && ((PlayerGameObject*)game_objects_[0])->getMissileCooldown() < MISSILE_COOLDOWN) {
            continue;
        }

        ui_objects_[i]->Update(delta_time);
        ui_objects_[i]->Render(shader_, view_matrix);
    }

    // Update and render the enemy objects
    for (int i = 0; i < enemy_objects_.size(); i++) {
        current_enemy_object = enemy_objects_[i];
        if (!gameOver) {
            current_enemy_object->Update(delta_time);
        }
        current_enemy_object->Render(shader_, view_matrix);
    }

    // Update and render the bullet objects
    for (int i = 0; i < bullet_objects_.size(); i++) {
        bool hitTarget = false;
        current_bullet_object = bullet_objects_[i];
        float duration = current_bullet_object->getDuration();
        if (!gameOver) {
            current_bullet_object->Update(delta_time);
        }
        hitTarget = BulletCastCollision(current_bullet_object);
        double bulletTime = current_bullet_object->getTimer();
        if (bulletTime > duration || hitTarget) {
delete bullet_objects_[i];
bullet_objects_.erase(bullet_objects_.begin() + i);
--i;
        }
        else {
        current_bullet_object->Render(shader_, view_matrix);
        }

    }

    // Update and render collectibles
    for (int i = 0; i < collectible_objects_.size(); i++) {
        current_collectible_object = collectible_objects_[i];
        if (!gameOver) {
            current_collectible_object->Update(delta_time);
        }
        current_collectible_object->Render(shader_, view_matrix);
    }



    // Update and render buoys
    for (int i = 0; i < buoy_objects_.size(); i++) {
        current_buoy_object = buoy_objects_[i];
        if (!gameOver) {
            current_buoy_object->Update(delta_time);
        }
        current_buoy_object->Render(shader_, view_matrix);
    }

    // Update and render missiles
    for (int i = 0; i < missile_objects_.size(); i++) {
        current_missile_object = missile_objects_[i];
        bool hitTarget = false;
        float duration = current_missile_object->getDuration();
        if (!gameOver) {
            if (current_missile_object->GetFrom() == "enemy" && player->getCloaked()) {
                current_missile_object->BulletObject::Update(delta_time);
            }
            else {
                current_missile_object->Update(delta_time);
            }
        }

        hitTarget = BulletCastCollision(current_missile_object);
        double missileTime = current_missile_object->getTimer();
        if (missileTime > duration || hitTarget) {

            delete particle_objects_[i];
            particle_objects_.erase(particle_objects_.begin() + i);

            delete missile_objects_[i];
            missile_objects_.erase(missile_objects_.begin() + i);
            --i;
        }
        else {
            current_missile_object->Render(shader_, view_matrix);
        }
    }

    // Update and render other game objects
    for (int i = (int)game_objects_.size() - 1; i >= 0; --i) {

        // Get the current game object
        current_game_object = game_objects_[i];
        if (!gameOver) {
            current_game_object->Update(delta_time);
        }

        // Render game object
        current_game_object->Render(shader_, view_matrix);
    }
    // Update and render bases
    for (int i = 0; i < bases_.size(); i++) {
        current_base = bases_[i];
        if (!gameOver) {
            current_base->Update(delta_time);
        }
        current_base->Render(shader_, view_matrix);
    }

    // Update and render the background
    for (int i = 0; i < background_objects_.size(); i++) {
        background_objects_[i]->Update(delta_time);
        background_objects_[i]->Render(shader_, view_matrix);
    }
    //This isn't properly rendering the particles yet...
    for (int i = 0; i < particle_objects_.size(); i++) {
        ParticleSystem* current_particle_object = dynamic_cast<ParticleSystem*>(particle_objects_[i]);
        current_particle_object->Update(delta_time);

        if (current_particle_object != nullptr) {
            //current_particle_object->Render(particle_shader_, view_matrix, current_time_);           
        }
        else {
            current_particle_object->Render(shader_, view_matrix, current_time_);
        }
    }
}

void Game::UpdateScore(int score){

    if (score_ + score >= 0) {
        score_ += score;
    }
    else {
        score_ = 0;
    }
}

void Game::GameOverLoop() {
    PlayerGameObject* player = (PlayerGameObject*)game_objects_[0];
    glm::vec3 player_pos = player->GetPosition();
    paused_ = true;

    const int arr_size = 3;
    int added_ui_count[arr_size] = { 0, 0, 0 };


    //vector of extra ui elements to pass into render
    std::vector<UI_Element*> extras;

    //big square background
    UI_Element* menu1 = new UI_Element(glm::vec3(player->GetPosition()), tex_[37], 1);
    menu1->SetAbsolute(true);
    menu1->SetScale(15.0f);




    //Score value
    Number* menu2 = new Number(glm::vec3(1.0f, -2.0f, 0), tex_[17], 1, score_);
    menu2->SetScale(FONT_SIZE * 0.8);

    UI_Element* score_prompt = new UI_Element(glm::vec3(-1.0f, -2.0f, 0), tex_[17], 1);
    score_prompt->SetScale(FONT_SIZE * 0.8);



    extras.push_back(score_prompt);
    extras.push_back(menu2);
    extras.push_back(menu1);


    // Loop while the user did not close the window
    while (!glfwWindowShouldClose(window_)) {
        // Update the game
        Render(extras);



        // Push buffer drawn in the background onto the display
        glfwSwapBuffers(window_);

        // Update other events like input handling
        glfwPollEvents();
    }
   }

} // namespace game