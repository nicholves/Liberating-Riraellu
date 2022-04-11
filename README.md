# Liberating-Riraellu

CONTROLS
	W = Increase speed (Hold to go faster)
	S = Decrease speed (Hold to go slower)
	A = Rotate Left
	D = Rotate Right

	Spacebar = Fire primary weapon (bullets)
	Ctrl = Fire secondary weapon (tracking missiles)

GAMEPLAY
	Your goal is to liberate the island nations of Riraellu. To do this, you must fly around the map to find and destroy the various bases. 
	Once a base has all of its defenses destroyed, you can fly up to a base to do 1 of 3 things. Either increase your score, restore your HP or add a shield
        to yourself.

	Once all bases are destroyed, you win. If you are destroyed in the process, you must start again from the beginning.

COLLECTIBLES
	There are a few different collectible objects you can pickup scattered around Riraellu. These include repair kits, shields, as well as cloaking devices.
	
	Repair kits: Restore HP to the player

	Shields    : Adds one shield that can absorb up to 5 damage

	Cloaker    : Makes the player enter 'stealth' mode, and becomes untargetable by enemies and missiles.

PARTICLES
	There are a couple different instances of particles in the game. Notably, they are the the particles that emit out of the missiles and the particles
	that emit out of the player object. The missile particles stay in a constant wobbly motion, but the player particles change based on how fast the
	player is travelling (less particles for lower speeds). Note: there are explosions in the game when the missiles despawn but these were implemented
	using a "frame by frame" approach rather than with a particle system.

ENEMIES
