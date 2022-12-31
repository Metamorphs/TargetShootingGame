# TargetShootingGame
TSG is a shooting game in arduino for airsoft player. do you have a replica that is gathering dust? no problem, you can use it and train your aim with this game !


# Required libraries
IRremote
LiquidCrystal
(includes in this repo)

# Other requirements
Here i use ELEGOO MEGA 2560 R3
X leds (X/2 red and X/2 green) (X is a pair number)
X resistors (i use 1K Ohm)
X/2 push button
LCD led but it will change in futur by a mobile application in bluetooth
IR receiver and remote but it will change in futur by a mobile application in bluetooth
USB cable for connected arduino to your computer
too many male/male cable
And Some talent in airsoft :) (that i don't have)

# Code variables
int ledPin[10][2] = {{22, 23},{24, 25},{26, 27},{28, 29},{30, 31},{32, 33},{34, 35},{36, 37},{38, 39},{40, 41}}; 
its all my possibly pin that i can use for leds. as you can see on the image, i have only 4 target. so on the menu, i wil choose the 4 first target ({22, 23},{24, 25},{26, 27},{28, 29}) the other pins will not be used
you have only 2 target ? no problem, select with the remote 2 for number of target and you will play with these pin: ({22, 23},{24, 25})

int buttonPin[10] = {53,52,51,50,49,48,47,46,45,44}; 
its all my possibly pin that i can use for button. with 4 target, these 4 first buttonpin will be used 53,52,51,50
you have only 2 target ? no problem, select with the remote 2 for number of target and you will play with these pin: 53,52

!! you don't have to change the code !! (only if you are in another arduino card with other pins)