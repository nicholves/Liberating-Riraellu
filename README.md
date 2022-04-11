# Liberating-Riraellu

## Controls

	W = Increase speed (Hold to go faster)
	S = Decrease speed (Hold to go slower)
	A = Rotate Left
	D = Rotate Right
	Spacebar = Fire primary weapon (bullets)
	Ctrl = Fire secondary weapon (tracking missiles)

## Gameplay

Your goal is to liberate the island nations of Riraellu. To do this, you must fly around the map to find and destroy the various bases. 
Once a base has all of its defenses destroyed, you can fly up to a base to do 1 of 3 things. Either increase your score, restore your HP or add a shield
to yourself.

Once all bases are destroyed, you win. If you are destroyed in the process, you must start again from the beginning.

## Collectibles
There are a few different collectible objects you can pickup scattered around Riraellu. These include repair kits, shields, as well as cloaking devices.
	
	Repair kits: Restore HP to the player

	Shields    : Adds one shield that can absorb up to 5 damage

	Cloaker    : Makes the player enter 'stealth' mode, and becomes temporarily untargetable by enemies and missiles. You can tell this
	             is in effect because of the different player texture (accomplished using the player_fragment_shader).

## Enemies
There are 3 different types of enemies throughout Riraellu.
	
	1. Helicopters
		These utilize a hierarchical transformation to have the propellers follow the helicopter's body. The Helicopter enemy can fire both missiles and
		bullets, except they fire double the amount of bullets at a time. Once a helicopter spots the player, they attempt to keep their distance from the
		player where they can safely shoot.
	2. Drones
		Drones are not something to take lightly. Their whole goal is to kamikaze the player. If successful, the deal 5 damage, or, half of the player's
		maximum HP. If you're facing the drone they won't go as fast, but if you start to fly away they may catch up to you. Since these drones are 
		meant to kamikaze, the enemy forces felt it was unnecessary to equip them with weapons, so they are weaponless.
	3. Turrets
		These are the primary defense system for bases. The bases you encounter will have multiple turrets. These turrets are stationary, but don't get too
		close as they can unleash a volley of bullets and missiles when working with the other turrets.

## Project Requirements 
### 1. Game Mechanics
We feel our game satisfies these requirements. Not only do you have to stay alive while fighting multiple enemies, but you also have to capture all the bases scattered along the map.
### 2. Enemies
#### 2.1. All our enemies have different textures.
#### 2.2. All our enemies have different behaviours, however they don't all have _different_ weapons. The Helicopter and Turret enemies for example, have different ways of _using_ their weapons -- Turrets shoot bullets and missiles when ready, Helicopters shoot 2x the amount of bullets and missiles when ready -- but the weapons are the same.

### 3. Weapons
The weapons the player can use are the regular machine gun, as well as a missile launcher. The missiles can be fired with the "CTRL" key, and the bullets with the "Spacebar". Bullets deal 1 damage while missiles deal 4 damage.

### 4. Particle Systems
There are a couple different instances of particles in the game. Notably, they are the the particles that emit out of the missiles and the particles
that emit out of the player object. The missile particles stay in a constant wobbly motion, but the player particles change based on how fast the
player is travelling (less particles for lower speeds).

	Note: there are explosions in the game when the missiles despawn but these were implemented
	using a "frame by frame" approach rather than with a particle system.
### 5. Collectible Items
See above @Collectibles

### 6. Movement and Transformations
#### 6.1 All movement in our game is done using transformations.
#### 6.2 Our game has a couple different instances of physically-based and parametric-based equations.
#### 6.3 Hierarchical transformation is used on the helicopter enemies, as well as the shield objects we collect.

### 7. Collision Detection
We detect collision many different times in our game. When the player collides with flying enemies, when they collide with collectibles, colliding with a base to capture it, colliding with bullets and missiles, as well as when enemies collide with bullets and missiles.

### 8. Game World
#### 8.1 The camera follows the player, so this criteria is self-explanatory when you boot up the game.
#### 8.2 The map is made of tile objects to show off different islands. However, they are not tiled by OpenGL. We created the map, then imported the .png file to use as our map.

### 9. UI
Our game shows the current score the player has, as well as if we have a missile ready (top-left corner underneath score), and how much HP the player currently has.

### 10. Code Readability and Organization
This one is all you. 
