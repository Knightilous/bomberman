# bomberman
A rough attempt to create the bases for a game using the SDL library in C.

While lacking any of the visual traits of a bomberman game, this projects has the basic gameplay mechanics that will lead to its completition, while also sporting some networking capabilities, such as the ability to move the character using a socket message.

The main character (in this instance, a placeholder sprite) can be moved locally, in which the movement is continous and frame indipendent, or by sending coordinates trough a message, in which the character will simply teleport to the designated spot.

The src folder contains the bomberman.c file and the BM_Client.c

The bomberman.c file is the main program of the applicationn, it contains the gameplay cycle and opens the server socket, while also displaying the game window using the SDL library.
The BM_Client.c file is an example of an external program that connects to the game to send information.

Note that bomberman.c only accepts coordinates messages and that it will ignore them if they are incorrect
