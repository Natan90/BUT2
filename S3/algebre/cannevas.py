##Modules
import sys
import pygame
import numpy as np

##Initialisation des couleurs
RED = (255, 0, 0)
GREEN = (0, 255, 0)
BLUE = (0, 0, 255)
YELLOW = (255, 255, 0)
WHITE = (255, 255, 255)
BLACK = (0, 0, 0)
PURPLE = (128, 0, 128)
ORANGE = (255, 165 ,0)
GREY = (128, 128, 128)
TURQUOISE = (64, 224, 208)

#-----------------Conditions initiales (au tout début du jeu)---------------
#------------------------------À COMPLÉTER----------------------------------

global t
t=0

def rotx(P, angle):
    c = np.cos(np.radians(angle))
    s = np.sin(np.radians(angle))
    R = np.array([[1, 0, 0],
                  [0, c, -s],
                  [0, s, c]])
    return np.dot(R, P)

def roty(P, angle):
    c = np.cos(np.radians(angle))
    s = np.sin(np.radians(angle))
    R = np.array([[c, 0, s],
                  [0, 1, 0],
                  [-s, 0, c]])
    return np.dot(R, P)

def rotz(P, angle):
    c = np.cos(np.radians(angle))
    s = np.sin(np.radians(angle))
    R = np.array([[c, -s, 0],
                  [s, c, 0],
                  [0, 0, 1]])
    return np.dot(R, P)

def etirement(P,x, y, z):
   E = np.array([[x, 0, 0],
                 [0, y, 0],
                 [0, 0, z]])
   return np.dot(E, P)

def symetrieOz(P):
   S = np.array([[-1, 0, 0],
                 [0, 1, 0],
                 [0, 0, 1]])
   return np.dot(S, P)

#==============================distance=======================================

def distance(x1, y1, z1, x2, y2, z2, ):
    return np.sqrt((x2 - x1)**2 + (y2 - y1)**2 + (z2 - z1)**2)

#====================================fonction de translation=========================================
def translation(P, x, y, z):
   P[0] += x
   P[1] += y
   P[2] += z
   return P

#====================================fonction de zoom=========================================
def zoom(P, rapport):
   return P * rapport

#====================================TORE (TORUS)=========================================

# Création d'un tore avec paramètres (R=grand rayon, r=petit rayon)
R = 2  # rayon majeur
r = 0.8  # rayon mineur
n_points = 40  # nombre de points

Ptore_points = []
for i in range(n_points):
   u = 2 * np.pi * i / n_points
   for j in range(n_points):
      v = 2 * np.pi * j / n_points
      x = (R + r * np.cos(v)) * np.cos(u)
      y = (R + r * np.cos(v)) * np.sin(u)
      z = r * np.sin(v)
      Ptore_points.append([x, y, z])

Ptore = np.array(Ptore_points).T.astype(float)

# Matrice d'adjacence pour le tore
Atore = np.array([[False]*n_points*n_points]*n_points*n_points)
for pt1 in range(len(Ptore[0])):
   for pt2 in range(pt1+1, len(Ptore[0])):
      dist = distance(Ptore[0][pt1],Ptore[1][pt1],Ptore[2][pt1], 
                      Ptore[0][pt2],Ptore[1][pt2],Ptore[2][pt2])
      if 0.5 < dist <= 1.2:  # Les arêtes du tore
         Atore[pt1][pt2]=True
         Atore[pt2][pt1]=True

Ptore = zoom(Ptore, 50)
Ptore = translation(Ptore, 800, 500, 0)

print("Tore créé avec", len(Ptore[0]), "points")


#=========================================fonction de dessin==========================================

def dessiner(P, A):
   for pt1 in range(len(P[0])):
      for pt2 in range(pt1+1, len(P[0])):
         if A[pt1][pt2]:
            pygame.draw.line(screen, YELLOW, (P[0][pt1], P[1][pt1]), (P[0][pt2], P[1][pt2]))


    
#booleens de clavier
global keyup,keydown,keyright,keyleft,keyspace 
keyup,keydown,keyright,keyleft,keyspace=False,False,False,False,False


# booleens gestion du jeu 
start = False

##Fenêtre
pygame.init()
screen = pygame.display.set_mode((1600,1000))

#Temps
clock = pygame.time.Clock()

##Boucle principale
while True:
    #Gestion du temps
    clock.tick(60)
    t+=1/60

    #Gestion des Ã©vÃ©nements
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            sys.exit()
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_SPACE:
              keyspace=True
            if event.key == pygame.K_UP:
              keyup=True
            if event.key == pygame.K_DOWN:
              keydown=True
            if event.key == pygame.K_RIGHT:
              keyright=True
            if event.key == pygame.K_LEFT:
              keyleft=True
        if event.type == pygame.KEYUP:
            if event.key == pygame.K_SPACE:
              keyspace=False
            if event.key == pygame.K_UP:
              keyup=False
            if event.key == pygame.K_DOWN:
              keydown=False
            if event.key == pygame.K_RIGHT:
              keyright=False
            if event.key == pygame.K_LEFT:
              keyleft=False

              
    #Clearscreen
    screen.fill(BLACK)

    #Create text and text rectangle
    font = pygame.font.Font('freesansbold.ttf', 32)
    text = font.render('To start press space', True, WHITE, BLACK)
    textRect = text.get_rect()
    textRect.center = (800, 500)


#----------------------- Jeu -----------------------------



    #Démarrage
    if not start:
        t = 0
        #Affichage du texte
        screen.blit(text, textRect)      
    
    if keyspace and not start:
        start = True

    if start:
        
        dessiner(Ptore, Atore)
      

        Ptore = translation(Ptore, -800, -500, 0)
        Ptore=zoom(Ptore, 1+0.01*np.cos(t))
        Ptore = rotx(Ptore, 0.5)
        Ptore = roty(Ptore, 0.5)
        

        Ptore = translation(Ptore, 800, 500, 0)



        





    
    


        

    #Mise Ã  jour de l'Ã©cran
    pygame.display.update()