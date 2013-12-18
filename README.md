Current Features:

Ghost AI and targeting is based on orgininal rules and is almost the same.
Cool win png if all coins are collected.
Life system that functions by having pacman and the ghosts reset each time they catch him (2 lives only)
Power up will cause ghosts to choose random tiles using Mersenne Twister random gen.
Movement is frame independent
Collision between ghosts and player is done by seperate collision interface.
Tilemap is rendered seperatly in order to improve performce

Current Issues:

Due to a bug, Coin class (used to iherit from ICollider) has been removed and coins are no longer separate objects. 
They have been returned to the tilemap which in turn will be re rendered each time we eat a coin. Fix is being worked on.

For some reason, when ghosts are frightened and pacman collides with them, nothing happes, despite having a handler for that case. This is a known issue and is, also, being worked on.

Notes:

Timer class appeared after some research about timers. Not written specifically for the project, but taken from http://stackoverflow.com/posts/1739265/revisions.