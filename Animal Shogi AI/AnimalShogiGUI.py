import sys
import pygame
from pygame.locals import *
clock = pygame.time.Clock()

pygame.init()
pygame.display.set_caption('12장기')


WW = 1536
WH = 864
WINDOW_SIZE = (WW, WH)
WIDTH = int(WW / 6)
HEIGHT = int(WH / 5)

GRIDW = 5

fps = 60
screen = pygame.display.set_mode(WINDOW_SIZE, 0, 32)
display = pygame.Surface((300, 200))

WHITE = (255, 255, 255)
RED = (255, 0, 0)
BLUE = (0, 0, 255)

font = pygame.font.SysFont('arialunicode', 100)
fontsmall = pygame.font.SysFont('arialunicode', 30)



lion_img_red = pygame.transform.scale(pygame.image.load('img/lion_red.png'), (144, 144))
chick_img_red = pygame.transform.scale(pygame.image.load('img/chick_red.png'), (144, 144))
elephant_img_red = pygame.transform.scale(pygame.image.load('img/elephant_red.png'), (144, 144))
giraffe_img_red = pygame.transform.scale(pygame.image.load('img/giraffe_red.png'), (144, 144))
hen_img_red = pygame.transform.scale(pygame.image.load('img/hen_red.png'), (144, 144))

lion_img_blue = pygame.transform.scale(pygame.image.load('img/lion_blue.png'), (144, 144))
chick_img_blue = pygame.transform.scale(pygame.image.load('img/chick_blue.png'), (144, 144))
elephant_img_blue = pygame.transform.scale(pygame.image.load('img/elephant_blue.png'), (144, 144))
giraffe_img_blue = pygame.transform.scale(pygame.image.load('img/giraffe_blue.png'), (144, 144))
hen_img_blue = pygame.transform.scale(pygame.image.load('img/hen_blue.png'), (144, 144))

CTOPY_FILE = "C:/Users/yoori/Documents/12ChessMedian/ctopy.txt"
PYTOC_FILE = "C:/Users/yoori/Documents/12ChessMedian/pytoc.txt"

class Square:
    def __init__(self, name, cord):
        self.name = name
        self.cord = cord
        self.pos = (WIDTH * cord[0], HEIGHT * cord[1])
        self.piece = None
        if cord[0] <= 1:
            self.color = (255, 71, 74)
        elif 2 <= cord[0] <= 3:
            self.color = (128, 128, 128)
        else:
            self.color = (71, 175, 255)
        self.rect = pygame.Rect(int(self.pos[0]), int(self.pos[1]), WIDTH, HEIGHT)
        pygame.draw.rect(screen, self.color, self.rect)



class Piece:
    def __init__(self, color_piece):
        self.select = False
        self.color = color_piece

    def set_pos(self, square_piece):
        self.square = square_piece

    def draw(self):
        screen.blit(self.img, (int(self.square.pos[0] + 50), int(self.square.pos[1] + 15)))


class Lion(Piece):
    def __init__(self, color, square):
        super().__init__(color)
        self.square = square
        self.square.piece = self
        if self.color == RED:
            self.i = lion_img_red
        else:
            self.i = lion_img_blue
        self.img = pygame.transform.rotate(self.i, -90 if self.color == RED else +90)
        self.atk = [[-1, -1], [-1, 0], [-1, 1], [0, -1], [0, 1], [1, -1], [1, 0], [1, 1]]


class Giraffe(Piece):
    def __init__(self, color, square):
        super().__init__(color)
        self.square = square
        self.square.piece = self
        if self.color == RED:
            self.i = giraffe_img_red
        else:
            self.i = giraffe_img_blue
        self.img = pygame.transform.rotate(self.i, -90 if self.color == RED else +90)
        self.atk = [[-1, 0], [0, -1], [0, 1], [1, 0]]


class Elephant(Piece):
    def __init__(self, color, square):
        super().__init__(color)
        self.square=square
        self.square.piece = self
        if self.color == RED:
            self.i = elephant_img_red
        else:
            self.i = elephant_img_blue
        self.img = pygame.transform.rotate(self.i, -90 if self.color == RED else +90)
        self.atk = [[-1, -1], [-1, 1], [1, -1], [1, 1]]


class Chick(Piece):
    def __init__(self, color, square):
        super().__init__(color)
        self.square = square
        self.square.piece = self
        if self.color == RED:
            self.i = chick_img_red
        else:
            self.i = chick_img_blue
        self.img = pygame.transform.rotate(self.i, -90 if self.color == RED else +90)
        if self.color == RED:
            self.atk = [[1, 0]]
        else:
            self.atk = [[-1, 0]]


class Hen(Piece):
    def __init__(self, color, square):
        super().__init__(color)
        self.square = square
        self.square.piece = self
        if self.color == RED:
            self.i = hen_img_red
        else:
            self.i = hen_img_blue
        self.img = pygame.transform.rotate(self.i, -90 if self.color == RED else +90)
        if self.color == RED:
            self.atk = [[-1, 0], [0, -1], [0, 0], [0, 1], [1, -1], [1, 0], [1, 1]]
        else:
            self.atk = [[-1, -1], [-1, 0], [-1, 1], [0, -1], [0, 1], [1, 0]]


def draw_text(text, font, color, surface, x, y):
    text_obj = font.render(text, 0, color)
    text_rect = text_obj.get_rect()
    text_rect.topleft = (int(x - text_obj.get_width() / 2), int(y - text_obj.get_height() / 2))
    surface.blit(text_obj, text_rect)


turn_player = RED

PR1 = Square('PR1', [0, 0])
PR2 = Square('PR2', [1, 0])
PR3 = Square('PR3', [2, 0])
PR4 = Square('PR4', [3, 0])
PR5 = Square('PR5', [4, 0])
PR6 = Square('PR6', [5, 0])

PB1 = Square('PB1', [0, 4])
PB2 = Square('PB2', [1, 4])
PB3 = Square('PB3', [2, 4])
PB4 = Square('PB4', [3, 4])
PB5 = Square('PB5', [4, 4])
PB6 = Square('PB6', [5, 4])

A1 = Square('A1', [1, 1])
A2 = Square('A2', [1, 2])
A3 = Square('A3', [1, 3])
B1 = Square('B1', [2, 1])
B2 = Square('B2', [2, 2])
B3 = Square('B3', [2, 3])
C1 = Square('C1', [3, 1])
C2 = Square('C2', [3, 2])
C3 = Square('C3', [3, 3])
D1 = Square('D1', [4, 1])
D2 = Square('D2', [4, 2])
D3 = Square('D3', [4, 3])

li = [A1, A2, A3, B1, B2, B3, C1, C2, C3, D1, D2, D3]
Board = {}

lion_RED = Lion(RED, A2)
giraffe_RED = Giraffe(RED, A3)
elephant_RED = Elephant(RED, A1)
chick_RED = Chick(RED, B2)

lion_BLUE = Lion(BLUE, D2)
giraffe_BLUE = Giraffe(BLUE, D1)
elephant_BLUE = Elephant(BLUE, D3)
chick_BLUE = Chick(BLUE, C2)

for i in li:
    Board[i] = {'name': i.name, 'cord': i.cord, 'pos': i.pos, 'piece': i.piece, 'color': i.color, 'rect': i.rect}

reverse_Board = {(0, 0): PR1, (1, 0): PR2, (2, 0): PR3, (3, 0): PR4, (4, 0): PR5, (5, 0): PR6,  (1, 1): A1, (1, 2): A2, (1, 3): A3, (2, 1): B1, (2, 2): B2, (2, 3): B3, (3, 1): C1, (3, 2): C2, (3, 3): C3, (4, 1): D1, (4, 2): D2, (4, 3): D3,  (0, 4): PB1, (1, 4): PB2, (2, 4): PB3, (3, 4): PB4, (4, 4): PB5, (5, 4): PB6}

pieces = [lion_RED, giraffe_RED, elephant_RED, chick_RED, lion_BLUE, giraffe_BLUE, elephant_BLUE, chick_BLUE]
prisoners = {RED: [], BLUE: []}

selected_area = None
selected_piece = None


def turn_switch(turn):
    global turn_player
    global selected_piece
    global selected_area
    if turn == RED:
        turn_player = BLUE
    else:
        turn_player = RED

    selected_piece = None
    selected_area = None

    return turn_player


def area_select(pos):
    if 0 < int(pos[0] / WIDTH) < 5 and 0 < int(pos[1] / HEIGHT) < 4:
        return [int(pos[0] / WIDTH), int(pos[1] / HEIGHT)]
    elif int(pos[1] / HEIGHT) == 0 or int(pos[1] / HEIGHT) == 4:
        return [int(pos[0] / WIDTH), int(pos[1] / HEIGHT)]
    else:
        return None


def long_live_the_king():
    global pieces
    return lion_RED in pieces and lion_BLUE in pieces


def can_go(piece1):
    available = {'none': [], 'equal': [], 'differ': []}
    for move in piece1.atk:
        go_x = piece1.square.cord[0]+move[0]
        go_y = piece1.square.cord[1]+move[1]
        if 0 < go_x < 5 and 0 < go_y < 4:
            if reverse_Board[(go_x, go_y)].piece == None:
                available['none'].append((go_x, go_y))
            elif reverse_Board[(go_x, go_y)].piece.color == piece1.color:
                available['equal'].append((go_x, go_y))
            elif reverse_Board[(go_x, go_y)].piece.color != piece1.color:
                available['differ'].append((go_x, go_y))

    return available


def fly_high_jujak():
    for i in range(len(pieces)):
        if isinstance(pieces[i], Chick):
            if pieces[i].color == RED:
                if pieces[i].square.cord[0] == 4 and 1 <= pieces[i].square.cord[1] <= 3:
                    return RED
            elif pieces[i].color == BLUE:
                if pieces[i].square.cord[0] == 1 and 1 <= pieces[i].square.cord[1] <= 3:
                    return BLUE
    return None


def goodbye(animal_type):
    global turn_player
    global prisoners
    if animal_type == Hen:
        if turn_player == RED:
            if reverse_Board[(prison_num[animal_type], prison_y[turn_player])].piece != None:
                prisoners[turn_player].append(Chick(turn_player, reverse_Board[(prison_num[animal_type] + 3, prison_y[turn_player])]))
            else:
                prisoners[turn_player].append(Chick(turn_player, reverse_Board[(prison_num[animal_type], prison_y[turn_player])]))
        elif turn_player == BLUE:
            if reverse_Board[(5 - prison_num[animal_type], prison_y[turn_player])].piece != None:
                prisoners[turn_player].append(Chick(turn_player, reverse_Board[(2 - prison_num[animal_type], prison_y[turn_player])]))
            else:
                prisoners[turn_player].append(Chick(turn_player, reverse_Board[(5 - prison_num[animal_type], prison_y[turn_player])]))
    else:
        if turn_player == RED:
            if reverse_Board[(prison_num[animal_type], prison_y[turn_player])].piece != None:
                prisoners[turn_player].append(animal_type(turn_player, reverse_Board[(prison_num[animal_type] + 3, prison_y[turn_player])]))
            else:
                prisoners[turn_player].append(animal_type(turn_player, reverse_Board[(prison_num[animal_type], prison_y[turn_player])]))
        elif turn_player == BLUE:
            if reverse_Board[(5 - prison_num[animal_type], prison_y[turn_player])].piece != None:
                prisoners[turn_player].append(animal_type(turn_player, reverse_Board[(2 - prison_num[animal_type], prison_y[turn_player])]))
            else:
                prisoners[turn_player].append(animal_type(turn_player, reverse_Board[(5 - prison_num[animal_type], prison_y[turn_player])]))


def king2times():
    if turn_player == RED:
        for i in range(len(pieces)):
            if isinstance(pieces[i], Lion):
                if pieces[i].color == RED:
                    if pieces[i].square.cord[0] == 4:
                        return RED
    elif turn_player == BLUE:
        for i in range(len(pieces)):
            if isinstance(pieces[i], Lion):
                if pieces[i].color == BLUE:
                    if pieces[i].square.cord[0] == 1:
                        return BLUE
    return None


def cordtoc(l):
    newl = ""
    if l[1] == 1:
        newl += "a"
    if l[1] == 2:
        newl += "b"
    if l[1] == 3:
        newl += "c"
    newl += str(5 - l[0])
    return newl


while True:
    turn_player = RED

    PR1 = Square('PR1', [0, 0])
    PR2 = Square('PR2', [1, 0])
    PR3 = Square('PR3', [2, 0])
    PR4 = Square('PR4', [3, 0])
    PR5 = Square('PR5', [4, 0])
    PR6 = Square('PR6', [5, 0])

    PB1 = Square('PB1', [0, 4])
    PB2 = Square('PB2', [1, 4])
    PB3 = Square('PB3', [2, 4])
    PB4 = Square('PB4', [3, 4])
    PB5 = Square('PB5', [4, 4])
    PB6 = Square('PB6', [5, 4])

    A1 = Square('A1', [1, 1])
    A2 = Square('A2', [1, 2])
    A3 = Square('A3', [1, 3])
    B1 = Square('B1', [2, 1])
    B2 = Square('B2', [2, 2])
    B3 = Square('B3', [2, 3])
    C1 = Square('C1', [3, 1])
    C2 = Square('C2', [3, 2])
    C3 = Square('C3', [3, 3])
    D1 = Square('D1', [4, 1])
    D2 = Square('D2', [4, 2])
    D3 = Square('D3', [4, 3])

    li = [A1, A2, A3, B1, B2, B3, C1, C2, C3, D1, D2, D3]
    Board = {}

    lion_RED = Lion(RED, A2)
    giraffe_RED = Giraffe(RED, A3)
    elephant_RED = Elephant(RED, A1)
    chick_RED = Chick(RED, B2)

    lion_BLUE = Lion(BLUE, D2)
    giraffe_BLUE = Giraffe(BLUE, D1)
    elephant_BLUE = Elephant(BLUE, D3)
    chick_BLUE = Chick(BLUE, C2)

    for i in li:
        Board[i] = {'name': i.name, 'cord': i.cord, 'pos': i.pos, 'piece': i.piece, 'color': i.color, 'rect': i.rect}

    reverse_Board = {(0, 0): PR1, (1, 0): PR2, (2, 0): PR3, (3, 0): PR4, (4, 0): PR5, (5, 0): PR6, (1, 1): A1,
                     (1, 2): A2, (1, 3): A3, (2, 1): B1, (2, 2): B2, (2, 3): B3, (3, 1): C1, (3, 2): C2, (3, 3): C3,
                     (4, 1): D1, (4, 2): D2, (4, 3): D3, (0, 4): PB1, (1, 4): PB2, (2, 4): PB3, (3, 4): PB4,
                     (4, 4): PB5, (5, 4): PB6}

    pieces = [lion_RED, giraffe_RED, elephant_RED, chick_RED, lion_BLUE, giraffe_BLUE, elephant_BLUE, chick_BLUE]
    prisoners = {RED: [], BLUE: []}

    selected_area = None
    selected_piece = None

    color = {(255, 0, 0): 'red', (0, 0, 255): 'blue'}
    prison_num = {Chick: 0, Elephant: 1, Giraffe: 2, Hen: 0}
    prison_y = {RED: 0, BLUE: 4}

    isking2 = False
    text = ""

    before_length = 0
    player_color = ""
    ai_moved = False
    ai_move = ""
    ctopy = open(PYTOC_FILE, "w")
    ctopy.close()
    while True:
        event = pygame.event.poll()
        if event.type == QUIT:
            ctopy = open(PYTOC_FILE, "w")
            ctopy.close()
            pygame.quit()
            sys.exit()
        ctopy = open(CTOPY_FILE, "r")
        ctopylines = ctopy.readlines()
        if len(ctopylines) > before_length:
            before_length += 1
            player_color = ctopylines[0].rstrip("\n")
            if len(ctopylines) != 1:
                ai_moved = True
                ai_move = ctopylines[len(ctopylines) - 1].rstrip("\n")
        else:
            ai_moved = False
        ctopy.close()
        if long_live_the_king() and not isking2:
            if ai_moved and len(ai_move) != 0:
                print(ai_move)
                if ai_move[0] != "0":
                    ai_piece = ""
                    ai_area = ""
                    ai_piece += str(5 - int(ai_move[1]))
                    if ai_move[0] == "a":
                        ai_piece += "1"
                    if ai_move[0] == "b":
                        ai_piece += "2"
                    if ai_move[0] == "c":
                        ai_piece += "3"

                    ai_area += str(5 - int(ai_move[3]))
                    if ai_move[2] == "a":
                        ai_area += "1"
                    if ai_move[2] == "b":
                        ai_area += "2"
                    if ai_move[2] == "c":
                        ai_area += "3"

                    selected_piece = reverse_Board[(int(ai_piece[0]), int(ai_piece[1]))].piece
                    l = []
                    l.append(int(ai_area[0]))
                    l.append(int(ai_area[1]))
                    selected_area = l
                else:
                    if player_color == "BLUE":
                        for i in prisoners[RED]:
                            print(i)
                            if ai_move[1] == "p" and isinstance(i, Chick):
                                print("RP")
                                selected_piece = i
                                break
                            if ai_move[1] == "j" and isinstance(i, Giraffe):
                                print("RJ")
                                selected_piece = i
                                break
                            if ai_move[1] == "s" and isinstance(i, Elephant):
                                print("RS")
                                selected_piece = i
                                break
                        print(tuple(selected_piece.square.cord))
                        selected_piece = reverse_Board[tuple(selected_piece.square.cord)].piece
                        print(selected_piece)
                        ai_area = ""
                        ai_area += str(5 - int(ai_move[3]))
                        if ai_move[2] == "a":
                            ai_area += "1"
                        if ai_move[2] == "b":
                            ai_area += "2"
                        if ai_move[2] == "c":
                            ai_area += "3"
                        l = []
                        l.append(int(ai_area[0]))
                        l.append(int(ai_area[1]))
                        selected_area = l
                        print(selected_area)
                    elif player_color == "RED":
                        for i in prisoners[BLUE]:
                            print(i)
                            if ai_move[1] == "p" and isinstance(i, Chick):
                                print("BP")
                                selected_piece = i
                                break
                            if ai_move[1] == "j" and isinstance(i, Giraffe):
                                print("BJ")
                                selected_piece = i
                                break
                            if ai_move[1] == "s" and isinstance(i, Elephant):
                                print("BS")
                                selected_piece = i
                                break
                        print(tuple(selected_piece.square.cord))
                        selected_piece = reverse_Board[tuple(selected_piece.square.cord)].piece
                        print(selected_piece)
                        ai_area = ""
                        ai_area += str(5 - int(ai_move[3]))
                        if ai_move[2] == "a":
                            ai_area += "1"
                        if ai_move[2] == "b":
                            ai_area += "2"
                        if ai_move[2] == "c":
                            ai_area += "3"
                        l = []
                        l.append(int(ai_area[0]))
                        l.append(int(ai_area[1]))
                        selected_area = l

            screen.fill((0, 0, 0))
            PR1 = Square('PR1', [0, 0])
            PR2 = Square('PR2', [1, 0])
            PR3 = Square('PR3', [2, 0])
            PR4 = Square('PR4', [3, 0])
            PR5 = Square('PR5', [4, 0])
            PR6 = Square('PR6', [5, 0])

            PB1 = Square('PB1', [0, 4])
            PB2 = Square('PB2', [1, 4])
            PB3 = Square('PB3', [2, 4])
            PB4 = Square('PB4', [3, 4])
            PB5 = Square('PB5', [4, 4])
            PB6 = Square('PB6', [5, 4])

            A1 = Square('A1', [1, 1])
            A2 = Square('A2', [1, 2])
            A3 = Square('A3', [1, 3])
            B1 = Square('B1', [2, 1])
            B2 = Square('B2', [2, 2])
            B3 = Square('B3', [2, 3])
            C1 = Square('C1', [3, 1])
            C2 = Square('C2', [3, 2])
            C3 = Square('C3', [3, 3])
            D1 = Square('D1', [4, 1])
            D2 = Square('D2', [4, 2])
            D3 = Square('D3', [4, 3])

            pygame.draw.rect(screen, (100, 100, 100), [0, 0, WW, HEIGHT])
            pygame.draw.rect(screen, (100, 100, 100), [0, HEIGHT * 4, WW, HEIGHT])

            if turn_player == RED:
                pygame.draw.rect(screen, (175, 71, 74), [0, HEIGHT, WIDTH, HEIGHT * 3])
                pygame.draw.rect(screen, (175, 71, 74), [WIDTH * 5, HEIGHT, WIDTH, HEIGHT * 3])
            if turn_player == BLUE:
                pygame.draw.rect(screen, (51, 155, 175), [0, HEIGHT, WIDTH, HEIGHT * 3])
                pygame.draw.rect(screen, (51, 155, 175), [WIDTH * 5, HEIGHT, WIDTH, HEIGHT * 3])

            if event.type == MOUSEBUTTONDOWN or ai_moved:
                if ai_moved or event.button == 1:
                    if ai_moved:
                        pass
                    else:
                        pos = pygame.mouse.get_pos()

                        selected_area = area_select(pos)
                    if (selected_area != None) or ai_moved:

                        if selected_piece != None and selected_piece.color == turn_player:
                            if selected_area[1] not in [0, 4]:
                                if selected_piece.square.cord[1] != 0 and selected_piece.square.cord[1] != 4:
                                    if tuple(selected_area) in can_go(selected_piece)['none']:

                                        text = "RED: " + cordtoc(selected_piece.square.cord) + cordtoc(selected_area) if turn_player == RED else "BLUE: " + cordtoc(selected_piece.square.cord) + cordtoc(selected_area)
                                        if (player_color == "RED" and RED == turn_player) or (player_color == "BLUE" and BLUE == turn_player):
                                            pytoc = open(PYTOC_FILE, "a")
                                            pytoc.write(cordtoc(selected_piece.square.cord) + cordtoc(selected_area) + "\n")
                                            pytoc.close()

                                        if isinstance(selected_piece, Chick) and selected_piece.square.cord[0] == (3 if turn_player == RED else 2):
                                            if turn_player == RED:
                                                for i in range(len(pieces)):
                                                    if pieces[i] == selected_piece:
                                                        del pieces[i]
                                                        break

                                                hen_RED = Hen(RED, selected_piece.square)
                                                pieces.append(hen_RED)
                                                selected_piece = hen_RED
                                            else:
                                                for i in range(len(pieces)):
                                                    if pieces[i] == selected_piece:
                                                        del pieces[i]
                                                        break

                                                hen_BLUE = Hen(BLUE, selected_piece.square)
                                                pieces.append(hen_BLUE)
                                                selected_piece = hen_BLUE
                                        selected_piece.square.piece = None
                                        reverse_Board[tuple(selected_piece.square.cord)].piece = None
                                        selected_piece.set_pos(reverse_Board[tuple(selected_area)])
                                        reverse_Board[tuple(selected_area)].piece = selected_piece

                                        turn_switch(turn_player)

                                        if king2times() != None:
                                            isking2 = True
                                    elif tuple(selected_area) in can_go(selected_piece)['equal']:
                                        selected_piece = reverse_Board[tuple(selected_area)].piece

                                    elif tuple(selected_area) in can_go(selected_piece)['differ']:
                                        text = "RED: " + cordtoc(selected_piece.square.cord) + cordtoc(selected_area) if turn_player == RED else "BLUE: " + cordtoc(selected_piece.square.cord) + cordtoc(selected_area)
                                        if (player_color == "RED" and RED == turn_player) or (player_color == "BLUE" and BLUE == turn_player):
                                            pytoc = open(PYTOC_FILE, "a")
                                            pytoc.write(cordtoc(selected_piece.square.cord) + cordtoc(selected_area) + "\n")
                                            pytoc.close()
                                        if type(reverse_Board[tuple(selected_area)].piece) == Lion:
                                            pieces.remove(reverse_Board[tuple(selected_area)].piece)
                                            reverse_Board[tuple(selected_area)].piece = None
                                            selected_piece.square.piece = None
                                            reverse_Board[tuple(selected_piece.square.cord)].piece = None
                                            selected_piece.set_pos(reverse_Board[tuple(selected_area)])
                                            reverse_Board[tuple(selected_area)].piece = selected_piece
                                            continue
                                        else:
                                            goodbye(type(reverse_Board[tuple(selected_area)].piece))

                                        if isinstance(selected_piece, Chick) and selected_piece.square.cord[0] == (3 if turn_player == RED else 2):
                                            if turn_player == RED:
                                                for i in range(len(pieces)):
                                                    if pieces[i] == selected_piece:
                                                        del pieces[i]
                                                        break

                                                hen_RED = Hen(RED, selected_piece.square)
                                                pieces.append(hen_RED)
                                                selected_piece = hen_RED
                                            else:
                                                for i in range(len(pieces)):
                                                    if pieces[i] == selected_piece:
                                                        del pieces[i]
                                                        break

                                                hen_BLUE = Hen(BLUE, selected_piece.square)
                                                pieces.append(hen_BLUE)
                                                selected_piece = hen_BLUE
                                        pieces.remove(reverse_Board[tuple(selected_area)].piece)
                                        reverse_Board[tuple(selected_area)].piece = None
                                        selected_piece.square.piece = None
                                        reverse_Board[tuple(selected_piece.square.cord)].piece = None
                                        selected_piece.set_pos(reverse_Board[tuple(selected_area)])
                                        reverse_Board[tuple(selected_area)].piece = selected_piece
                                        turn_switch(turn_player)

                                        if king2times() != None:
                                            isking2 = True
                                elif selected_piece.square.cord[1] == 0 or selected_piece.square.cord[1] == 4:
                                    if reverse_Board[tuple(selected_area)].piece == None:
                                        char = ""

                                        if isinstance(selected_piece, Chick):
                                            char = "p"
                                        elif isinstance(selected_piece, Elephant):
                                            char = "s"
                                        elif isinstance(selected_piece, Giraffe):
                                            char = "j"
                                        text = "RED: " + "0" + char + cordtoc(selected_area) if turn_player == RED else "BLUE: " + "0" + char + cordtoc(selected_area)
                                        if (player_color == "RED" and RED == turn_player) or (player_color == "BLUE" and BLUE == turn_player):
                                            pytoc = open(PYTOC_FILE, "a")
                                            pytoc.write("0" + char + cordtoc(selected_area) + "\n")
                                            pytoc.close()
                                        selected_piece.square.piece = None
                                        reverse_Board[tuple(selected_area)].piece = selected_piece
                                        selected_piece.set_pos(reverse_Board[tuple(selected_area)])
                                        prisoners[selected_piece.color].remove(selected_piece)
                                        pieces.append(selected_piece)
                                        turn_switch(turn_player)
                                        if king2times() != None:
                                            isking2 = True
                                elif turn_player == reverse_Board[tuple(selected_area)].piece.color:
                                    selected_piece = reverse_Board[tuple(selected_area)].piece
                                    reverse_Board[tuple(selected_area)].piece = selected_piece

                            elif selected_area[1] in [0, 4]:
                                for prisoner in prisoners[RED]:
                                    if prisoner.square.cord == selected_area:
                                        selected_piece = prisoner
                                for prisoner in prisoners[BLUE]:
                                    if prisoner.square.cord == selected_area:
                                        selected_piece = prisoner

                        else:
                            for piece in pieces:
                                if piece.square.cord == selected_area:
                                    selected_piece = piece
                            for prisoner in prisoners[RED]:
                                if prisoner.square.cord == selected_area:
                                    selected_piece = prisoner
                            for prisoner in prisoners[BLUE]:
                                if prisoner.square.cord == selected_area:
                                    selected_piece = prisoner

            draw_text(text, fontsmall, WHITE, screen, WIDTH * 3, 10)

            draw_text("a", fontsmall, WHITE, screen, WIDTH * 5 + 20, int(HEIGHT * 1.5))
            draw_text("b", fontsmall, WHITE, screen, WIDTH * 5 + 20, int(HEIGHT * 2.5))
            draw_text("c", fontsmall, WHITE, screen, WIDTH * 5 + 20, int(HEIGHT * 3.5))

            draw_text("4", fontsmall, WHITE, screen, int(WIDTH * 1.5), HEIGHT * 4 + 20)
            draw_text("3", fontsmall, WHITE, screen, int(WIDTH * 2.5), HEIGHT * 4 + 20)
            draw_text("2", fontsmall, WHITE, screen, int(WIDTH * 3.5), HEIGHT * 4 + 20)
            draw_text("1", fontsmall, WHITE, screen, int(WIDTH * 4.5), HEIGHT * 4 + 20)

            if selected_piece != None:
                pygame.draw.rect(screen, (90, 90, 90), [selected_piece.square.cord[0] * WIDTH, selected_piece.square.cord[1] * HEIGHT, WIDTH, HEIGHT])

            pygame.draw.line(screen, WHITE, [WIDTH, HEIGHT], [WIDTH, WH - HEIGHT - GRIDW], GRIDW)

            for i in range(2, 6):
                pygame.draw.line(screen, WHITE, [WIDTH * i, HEIGHT], [WIDTH * i, WH - HEIGHT - GRIDW], GRIDW)

            pygame.draw.line(screen, WHITE, [WIDTH, HEIGHT + int(GRIDW / 2)], [WIDTH * 5, HEIGHT + int(GRIDW / 2)], GRIDW)

            pygame.draw.line(screen, WHITE, [WIDTH, HEIGHT * 2], [WIDTH * 5, HEIGHT * 2], GRIDW)
            pygame.draw.line(screen, WHITE, [WIDTH, HEIGHT * 3], [WIDTH * 5, HEIGHT * 3], GRIDW)

            pygame.draw.line(screen, WHITE, [WIDTH, HEIGHT * 4 - int(GRIDW / 2) - 1], [WIDTH * 5, HEIGHT * 4 - int(GRIDW / 2) - 1], GRIDW)

            pygame.draw.line(screen, WHITE, [0, HEIGHT], [WW, HEIGHT], GRIDW)
            pygame.draw.line(screen, WHITE, [0, HEIGHT * 4], [WW, HEIGHT * 4], GRIDW)

            for piece in pieces:
                piece.draw()
                if piece.square.cord == selected_area:
                    selected_piece = piece

            for prisoner in prisoners[RED]:
                prisoner.draw()
            for prisoner in prisoners[BLUE]:
                prisoner.draw()

            if selected_piece != None and (selected_piece.square.cord[1] != 0 and selected_piece.square.cord[1] != 4):
                for destination in can_go(selected_piece)['none'] + can_go(selected_piece)['differ']:
                    if selected_piece.color == turn_player:
                        circle_x = int((destination[0] + 0.5) * WIDTH)
                        circle_y = int((destination[1] + 0.5) * HEIGHT)
                        if WIDTH < circle_x < WIDTH * 5 and HEIGHT < circle_y < HEIGHT * 4:
                            pygame.draw.circle(screen, (50, 50, 50), (circle_x, circle_y), 10)

        else:
            if event.type == MOUSEBUTTONDOWN:
                if event.button == 1:
                    ctopy = open(PYTOC_FILE, "w")
                    ctopy.close()
                    break

            draw_text(text, fontsmall, WHITE, screen, WIDTH * 3, 10)

            draw_text("a", fontsmall, WHITE, screen, WIDTH * 5 + 20, int(HEIGHT * 1.5))
            draw_text("b", fontsmall, WHITE, screen, WIDTH * 5 + 20, int(HEIGHT * 2.5))
            draw_text("c", fontsmall, WHITE, screen, WIDTH * 5 + 20, int(HEIGHT * 3.5))

            draw_text("4", fontsmall, WHITE, screen, int(WIDTH * 1.5), HEIGHT * 4 + 20)
            draw_text("3", fontsmall, WHITE, screen, int(WIDTH * 2.5), HEIGHT * 4 + 20)
            draw_text("2", fontsmall, WHITE, screen, int(WIDTH * 3.5), HEIGHT * 4 + 20)
            draw_text("1", fontsmall, WHITE, screen, int(WIDTH * 4.5), HEIGHT * 4 + 20)

            pygame.draw.line(screen, WHITE, [WIDTH, HEIGHT], [WIDTH, WH - HEIGHT - GRIDW], GRIDW)
            pygame.draw.line(screen, WHITE, [WIDTH * 2, HEIGHT], [WIDTH * 2, WH - HEIGHT - GRIDW], GRIDW)
            pygame.draw.line(screen, WHITE, [WIDTH * 3, HEIGHT], [WIDTH * 3, WH - HEIGHT - GRIDW], GRIDW)
            pygame.draw.line(screen, WHITE, [WIDTH * 4, HEIGHT], [WIDTH * 4, WH - HEIGHT - GRIDW], GRIDW)
            pygame.draw.line(screen, WHITE, [WIDTH * 5, HEIGHT], [WIDTH * 5, WH - HEIGHT - GRIDW], GRIDW)
            pygame.draw.line(screen, WHITE, [WIDTH, HEIGHT + int(GRIDW / 2)], [WIDTH * 5, HEIGHT + int(GRIDW / 2)], GRIDW)
            pygame.draw.line(screen, WHITE, [WIDTH, HEIGHT * 2], [WIDTH * 5, HEIGHT * 2], GRIDW)
            pygame.draw.line(screen, WHITE, [WIDTH, HEIGHT * 3], [WIDTH * 5, HEIGHT * 3], GRIDW)
            pygame.draw.line(screen, WHITE, [WIDTH, HEIGHT * 4 - int(GRIDW / 2) - 1], [WIDTH * 5, HEIGHT * 4 - int(GRIDW / 2) - 1], GRIDW)

            pygame.draw.line(screen, WHITE, [0, HEIGHT], [WW, HEIGHT], GRIDW)
            pygame.draw.line(screen, WHITE, [0, HEIGHT * 4], [WW, HEIGHT * 4], GRIDW)

            for piece in pieces:
                piece.draw()
                if piece.square.cord == selected_area:
                    selected_piece = piece

            if turn_player == BLUE:
                draw_text('Blue Wins!', font, (50, 80, 200), screen, WW / 2, WH / 2)
            elif turn_player == RED:
                draw_text('Red Wins!', font, (240, 50, 50), screen, WW / 2, WH / 2)

        pygame.display.update()
        clock.tick(fps)
