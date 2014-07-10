# pong.py - play ball game classic with joystick and big screen
# (c) BotBook.com - Karvinen, Karvinen, Valtokari

import time
import sys
import pygame
import botbook_gpio as gpio
from pygame.locals import *

print "Loading BotBook.com Pong..."
pygame.init()    # <1>

width = pygame.display.Info().current_w    # <2>
height = pygame.display.Info().current_h

size = width, height    # <3>
background = 0, 0, 0    # <4>
screen = pygame.display.set_mode(size,pygame.FULLSCREEN)    # <5>
normalSpeed = 512
ballrect = Rect(width/2, height/2, 16, 16)    # <6>
computerrect = Rect(width-20, 0, 20, 120)    # <7>
playerrect = Rect(0, 0, 20, 120)    # <8>
#movement is diff in x and y. ball can only move in 45 degree angles.
speed = [normalSpeed, normalSpeed]    # <9>
clock = pygame.time.Clock()    # <10>
pygame.mouse.set_visible(False)
mainloop = True

uppin = 2   
downpin = 3   
gpio.mode(uppin, "in")
gpio.mode(downpin, "in")

while mainloop:    # <11>
    seconds = clock.tick(30) / 1000.0 # seconds since last frame    # <12>
        
    # User input
        
    for event in pygame.event.get():    # <13>
        if event.type == pygame.QUIT: mainloop = False    # <14>
        if (event.type == KEYUP) or (event.type == KEYDOWN):
            if event.key == K_ESCAPE: mainloop = False

    # Movement and collisions
    playerspeed = 0
    if gpio.read(uppin) == gpio.LOW:
        playerspeed = -normalSpeed
    if gpio.read(downpin) == gpio.LOW:
        playerspeed = normalSpeed        
    ballrect.x += speed[0] * seconds    # <15>
    ballrect.y += speed[1] * seconds
    if ballrect.left < 0 or ballrect.right > width:    # <16>
        ballrect.x = width/2;
    if ballrect.top < 0 or ballrect.bottom > height:
        speed[1] = -speed[1]

    computerrect.y = round(ballrect.y)    # <17>
    playerrect.y += playerspeed * seconds    # <18>
    if playerrect.top < 0: playerrect.top = 0    # <19>
    if playerrect.bottom > height: playerrect.bottom = height    # <20>
    if computerrect.colliderect(ballrect):    # <21>
        speed[0] = -normalSpeed

    if playerrect.colliderect(ballrect):
        speed[0] = normalSpeed

    # Draw frame      
    screen.fill(background)
    pygame.draw.circle(screen, (255, 255, 255), (int(round(ballrect.x+8)), 
                                                 int(round(ballrect.y+8))), 10)    # <22>
    pygame.draw.rect(screen, (255, 255, 255), computerrect)    # <23>
    pygame.draw.rect(screen, (255, 255, 255), playerrect)
    pygame.display.update() # <24>
