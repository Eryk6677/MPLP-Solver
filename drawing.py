import os
import sys
import pygame
from pygame.locals import KEYDOWN, K_q

# CONSTANTS:
SCREENSIZE = WIDTH, HEIGHT = 800, 600
BLACK = (0, 0, 0)
GREY = (145, 145, 145)
WHITE = (255, 255, 255)
_VARS = {'surf': pygame.Surface, 'gridOrigin': (20, 20), 'lineWidth': 3}


def main():
    if len(sys.argv) != 2:
        sys.exit("Incorrect amount of arguments! Expected ONE file name")

    try:
        with open(sys.argv[1], "r") as f:
            data = f.readlines()
            f.close()
    except FileNotFoundError:
        sys.exit("Given file does NOT exist!")

    algo, p_l, p_w, b_l, b_w = data[0].split()

    pygame.init()
    _VARS['surf'] = pygame.display.set_mode(SCREENSIZE)

    check_events()
    _VARS['surf'].fill(WHITE)

    # Get possible up/down scaling to fit the grid onto the window
    scale = min((HEIGHT - 2*(_VARS['gridOrigin'][0])) / int(p_w),
                (WIDTH - 2 * (_VARS['gridOrigin'][1])) / int(p_l))

    # Draw the pallet outline
    draw_pallet(_VARS['gridOrigin'], int(p_l) * scale, int(p_w) * scale)

    # Draw the boxes on the pallet
    for i in range(1, len(data)):
        line = data[i]
        draw_box(_VARS['gridOrigin'], int(b_l) * scale, int(b_w) * scale, line, scale, int(p_w))

    pygame.image.save(_VARS['surf'], algo + "_" + p_l + "_" + p_w + "_" + b_l + "_" + b_w + ".jpeg")


def draw_pallet(origin, length, width):
    cont_x, cont_y = origin

    pygame.draw.rect(_VARS['surf'], GREY, pygame.Rect(cont_x, cont_y, length, width))

    # TOP lEFT TO RIGHT
    pygame.draw.line(
        _VARS['surf'], BLACK,
        (cont_x, cont_y),
        (length + cont_x, cont_y), _VARS['lineWidth'])
    # BOTTOM lEFT TO RIGHT
    pygame.draw.line(
        _VARS['surf'], BLACK,
        (cont_x, width + cont_y),
        (length + cont_x,
         width + cont_y), _VARS['lineWidth'])
    # LEFT TOP TO BOTTOM
    pygame.draw.line(
        _VARS['surf'], BLACK,
        (cont_x, cont_y),
        (cont_x, cont_y + width), _VARS['lineWidth'])
    # RIGHT TOP TO BOTTOM
    pygame.draw.line(
        _VARS['surf'], BLACK,
        (length + cont_x, cont_y),
        (length + cont_x,
         width + cont_y), _VARS['lineWidth'])


def draw_box(origin, box_length, box_width, point, scale, pallet_w):
    cont_x, cont_y = origin
    cont_y = cont_y - 1
    box_x, box_y, orient = point.split()

    box_x = int(box_x) * scale

    if orient == "H":
        length = box_length
        width = box_width
        box_y = (pallet_w - int(box_y)) * scale - width
    else:
        length = box_width
        width = box_length
        box_y = (pallet_w - int(box_y)) * scale - width

    # White background
    pygame.draw.rect(_VARS['surf'], WHITE, pygame.Rect(cont_x + box_x + 1, cont_y + box_y + 1, length, width))

    # TOP lEFT TO RIGHT
    pygame.draw.line(
        _VARS['surf'], BLACK,
        (cont_x + box_x, cont_y + box_y),
        (length + cont_x + box_x, cont_y + box_y), 2)

    # BOTTOM lEFT TO RIGHT
    pygame.draw.line(
        _VARS['surf'], BLACK,
        (cont_x + box_x, width + cont_y + box_y),
        (length + cont_x + box_x,
         width + cont_y + box_y), 2)

    # LEFT TOP TO BOTTOM
    pygame.draw.line(
        _VARS['surf'], BLACK,
        (cont_x + box_x, cont_y + box_y),
        (cont_x + box_x, cont_y + box_y + width), 2)

    # RIGHT TOP TO BOTTOM
    pygame.draw.line(
        _VARS['surf'], BLACK,
        (length + cont_x + box_x, cont_y + box_y),
        (length + cont_x + box_x,
         width + cont_y + box_y), 2)


def check_events():
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            sys.exit()
        elif event.type == KEYDOWN and event.key == K_q:
            pygame.quit()
            sys.exit()


if __name__ == '__main__':
    main()
