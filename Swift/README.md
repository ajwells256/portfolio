Early in my senior year – to procrastinate my college applications – I began coding a game I had played in my childhood - Atari's asteroids. 
This application was built from the ground up. I used the SceneKit to do much of the graphical and physical work, and wrote my code on my own. 

One of the parts I had the most fun writing was the alien's targeting system. The evolution of the alien can be seen by playing the various archive applications and comparing them.
Furthermore, I added "cheat codes" into the pause page (reached by typing 'p' during the game). Simply click the "developer code" text and type the code, then press enter. It will
display a message if you were successful.
I suggest the following (in individual games, not all together):
    "Allies" (go back to the game for 30 seconds or so to see the difference) followed by "The Beacon is Lit" (yes, a Lord of the Rings reference)
    "Star Wars"
    "Flying Solo" - Asteroids disappear and you duel a procession of aliens. Their targeting systems have improved since the '80s
    
I would not suggest the following (a comprehensive list can be found in the PauseScene.swift file):
    "Coward" combined with "Allies" and "The Beacon is Lit" - your space ship will never reappear and you just watch the aliens get you points
    "Manhattan Project" - the nuclear bomb (activated by using Enter instead of Space) will probably not work and instead just crash 
                        - it works by iteratively destroying every asteroid object in the game; the bug is likely in trying to destroy the same object twice
                        - unfortunately, I have since moved on to college and never got around to finishing this feature
