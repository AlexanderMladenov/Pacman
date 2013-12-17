Current Issues:

Due to a bug, Coin class (used to iherit from ICollider) has been removed and coins are no longer separate objects. 
They have been returned to the tilemap which in turn will be re rendered each time we eat a coin. Fix is being worked on.

For some reason, when ghosts are frightened and pacman collides with them, nothing happes, despite having a handler for that case. This is a known issue and is, also, being worked on.