# bomberman
A rough attempt to create the bases for a game using the SDL library in C.

While lacking any of the visual traits of a bomberman game, this projects has the basic gameplay mechanics that will lead to its completition, while also sporting some networking capabilities, such as the ability to move the character using a socket message.

The main character (in this instance, a placeholder sprite) can be moved locally, in which the movement is continous and frame indipendent, or by sending coordinates trough a socket, in which the character will simply teleport to the designated spot.

