# equalizer.py - show equalizer based on microphone input
# (c) BotBook.com - Karvinen, Karvinen, Valtokari


import pygame  # sudo apt-get -y install python-pygame
import math
import numpy  # sudo apt-get -y install python-numpy


import microphone    # <1>
from pygame.locals import *

pygame.init()

width = 800
height = 640

size = width, height
background = 0, 0, 0

screen = pygame.display.set_mode(size, pygame.FULLSCREEN)
fullBar = pygame.image.load("equalizer-full-small4.jpg")    # <2>
emptyBar = pygame.image.load("equalizer-empty-small4.jpg")
clock = pygame.time.Clock()
pygame.mouse.set_visible(False)
mainloop = True

barHeight = 36
barWidth = 80
barGraphHeight = 327
barPos = [55, 130]

sampleLength = 16


def fftCalculations(data):    # <3>
    data2 = numpy.array(data) / 4    # <4>
    fourier = numpy.fft.rfft(data2)    # <5>
    ffty = numpy.abs(fourier)    # <6>
    ffty = ffty / 256.0        # <7>
    return ffty

while mainloop:
    buff = microphone.readSound(sampleLength)    # <8>

    barData = fftCalculations(buff)    # <9>

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            mainloop = False
        if (event.type == KEYUP) or (event.type == KEYDOWN):
            if event.key == K_ESCAPE:
                mainloop = False
    screen.fill(background)
    # Draw data to pillars
    for i in range(8):    # <10>
        bars = barData[i+1]    # <11>
        bars = int(math.floor(bars*10))    # <12>
        if bars > 10:
            bars = 10
        bars -= 1
        screen.blit(emptyBar, (barPos[0]+i*(barWidth+10), barPos[1]),
                              (0, 0, barWidth, barHeight*(10-bars)))
        if bars >= 0:
            barStartPos = (barPos[0] + i * (barWidth + 10),
                           barPos[1] + barGraphHeight - barHeight * bars + 6)
            barSourceBlit = (0, barGraphHeight - barHeight * bars+6,
                             barWidth, barHeight*bars)
            screen.blit(fullBar, barStartPos, barSourceBlit)    # <13>
    pygame.display.update()
