#ifndef GAME_H
#define GAME_H
#include"stdio.h" 
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>
typedef struct point {
  int x[100];
  int y[100];
} point;

typedef struct point1 {
  int x;
  int y;
} point1;

#define GAME_ROW 15
#define GAME_COL 32

typedef struct gamePane {
  // the game pane, 0 for blank, 1 for plane and 2 for obstacle
  char pane[GAME_ROW][GAME_COL];
  point cursor;
  int gameOver;
} gamePane;

typedef struct plane {
  point1 position;
} plane;

typedef struct obstacle {
  point position;
} obstacle;

typedef struct bullet{
  point position;
}bullet;

// function list
void init(gamePane * thePane, plane * thePlane, obstacle * theObstacle);
void setCursor(gamePane * thePane, const int x, const int y);
void setplane(gamePane * thePane, plane * plane);
void setobstacle(gamePane * thePane, plane *thePlane, obstacle *obstacle);
void removeOBJ(gamePane * thePane, const point1 p);
void gameOver(gamePane *thePane);
void printPane(const gamePane thePane);
void movePlane(gamePane * thePane, plane * thePlane, int op, int *play_Bullet, int *cold_Bullet);
void moveObstacle(gamePane * thePane, plane *thePlane, obstacle * theObstacle);
void productBullet(gamePane *thePane, plane *thePlane, bullet *theBullet, int op, int *play_Bullet);
void moveBullet(gamePane *thePane, obstacle *theObstacle, bullet *theBullet);
#endif // GAME_H 

main() {
  while (1) {
    int i;
    gamePane *thePane = (gamePane *)malloc(sizeof(gamePane));
    plane *thePlane = (plane *)malloc(sizeof(plane)); 
    obstacle *theObstacle = (obstacle *)malloc(sizeof(obstacle));
    bullet *theBullet = (bullet*)malloc(sizeof(bullet));
    int *play_Bullet = (int *)malloc(sizeof(int));
    *play_Bullet = 10;
    int *cold_Bullet = (int *)malloc(sizeof(int));
    *cold_Bullet = 0;
    init(thePane, thePlane, theObstacle);
    for (i = 0; i <= 80; i++) {
        theBullet->position.x[i] = -2;
        theBullet->position.y[i] = -2;
        thePane->pane[theBullet->position.x[i]][theBullet->position.y[i]] = ' ';
    }
    printPane(*thePane);
    while (1) {
        system("cls");
        int op = 0;
        while (kbhit()) {
            op = getch();
        }
        moveObstacle(thePane, thePlane, theObstacle);
        if (thePane->gameOver == 1) {
            break;
        } 
        movePlane(thePane, thePlane, op - '0', play_Bullet, cold_Bullet);
        productBullet(thePane, thePlane, theBullet, op, play_Bullet);
        moveBullet(thePane, theObstacle, theBullet);
        printPane(*thePane);
        Sleep(50);
    }
  }
}

//initialization for the game 
void init(gamePane * thePane, plane * thePlane, obstacle * theObstacle) {
  int i, j;
  srand((unsigned)time(NULL));
  thePlane->position.x = 14;
  thePlane->position.y = 16;
  for (i = 0; i < 21; i++) {
    theObstacle->position.x[i] = -1;
    theObstacle->position.y[i] = rand()%31;
    
  }
  for(i = 0; i < GAME_ROW; i++) {
    for(j = 0; j < GAME_COL; j++) {
      thePane->pane[i][j] = ' ';
    }
  }
  setplane(thePane, thePlane);
  setobstacle(thePane, thePlane, theObstacle);
  thePane->gameOver = 0;
}

// basic operation for game pane 
void setplane(gamePane * thePane, plane * plane) {
  if(thePane->pane[plane->position.x][plane->position.y] == 15) {
    thePane->gameOver = 1;
    gameOver(thePane);
    return;
  }
  thePane->pane[plane->position.x][plane->position.y] = 6;
}

// Let obstacles appear at their next positions and judge whether they are crashing the plane;  
void setobstacle(gamePane *thePane, plane *thePlane, obstacle *obstacle) {
  int i;
  for (i = 0; i < 21; i++) {
    thePane->pane[obstacle->position.x[i]][obstacle->position.y[i]] = 15;
    if(thePlane->position.x == obstacle->position.x[i] && thePlane->position.y == obstacle->position.y[i] ) {
      thePane->gameOver = 1; 
      gameOver(thePane);
      return;
    }
  }
}

// Let bullets appear at their next position and judge whether they are crashing the obstacle;  
void setBullet(gamePane *thePane, obstacle *theObstacle, bullet *theBullet) {
  int i, j;
  for (i = 0; i <= 80; i++) {
    for (j = 0; j < 21; j++) {
      if(theBullet->position.x[i] == theObstacle->position.x[j] && theBullet->position.y[i] == theObstacle->position.y[j] ) {
        thePane->pane[theBullet->position.x[i]][theBullet->position.y[i]] = ' ';
        theBullet->position.x[i] = -6;
        theBullet->position.y[i] = -6;
        thePane->pane[theBullet->position.x[i]][theBullet->position.y[i]] = ' ';
        theObstacle->position.x[j] = -2;
        theObstacle->position.y[j] = 5;
      }
    }
  }
}

//Product bullets above the plane when Space key is pressed, and count the amount of the bullets. 
void productBullet(gamePane *thePane, plane *thePlane, bullet *theBullet, int op, int *play_Bullet) {
  int i;
  switch(op) {
  case ' ' :
    for (i = 0; i <= 80; i++) {
      if (thePane->pane[theBullet->position.x[i]][theBullet->position.y[i]] == ' ' && *play_Bullet >= 0) {
        theBullet->position.x[i] = thePlane->position.x - 1;
        theBullet->position.y[i] = thePlane->position.y;
        thePane->pane[theBullet->position.x[i]][theBullet->position.y[i]] = 24;
        (*play_Bullet)--;
        break;
      }
      
    }
  }
}

//Let the plane's position become blank 
void removeOBJ(gamePane * thePane, const point1 p) {
  thePane->pane[p.x][p.y] = ' ';
}

//Let the obstacle's position become blank 
void removeOBJ1(gamePane * thePane, const point p) {
  int i;
  for (i = 0; i < 21; i++) {
    thePane->pane[p.x[i]][p.y[i]]= ' ';
  }
}

//Game over 
void gameOver(gamePane *thePane) {
  thePane->gameOver = 1;
  system("cls");
  printf("GameOver\n");
  printf("Please press any key for a long time to restart!");
  getch();
}

//print the game pane 
void printPane(const gamePane thePane) {
  int row, col;
  printf("  ©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤ \n");
  for (row = 0; row < 15; row++) {
    for (col = 0; col < 32; col++) {
      if (col == 0 || col == 31) {
        printf("|");
      }
      printf("%c", thePane.pane[row][col]);
    }
    printf("\n");
  } 
  printf("  ©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤ \n");
}

// operation for plane, and cold time decreases. 
void movePlane(gamePane * thePane, plane * thePlane, int op, int *play_Bullet, int *cold_Bullet) {
  removeOBJ(thePane, thePlane->position);
  switch(op) {
  case 5 :
    if(thePlane->position.x > 0) {
      thePlane->position.x--;
    }
    if (*play_Bullet < 0){
        (*cold_Bullet)++;
        if (*cold_Bullet >= 10) {
          *cold_Bullet = 0;
          *play_Bullet = 10;
      }
  }
    break;
  case 2 :
    if(thePlane->position.x < GAME_ROW - 1) {
      thePlane->position.x++;
    }
    if (*play_Bullet < 0){
        (*cold_Bullet)++;
        if (*cold_Bullet >= 10) {
          *cold_Bullet = 0;
          *play_Bullet = 10;
      }
  }
    break;
  case 1:
    if(thePlane->position.y > 0) {
      thePlane->position.y--;
    }
    if (*play_Bullet < 0){
        (*cold_Bullet)++;
        if (*cold_Bullet >= 10) {
          *cold_Bullet = 0;
          *play_Bullet = 10;
      }
  }
    break;
  case 3:
    if(thePlane->position.y < GAME_COL - 1) {
      thePlane->position.y++;
    }
    if (*play_Bullet < 0){
        (*cold_Bullet)++;
        if (*cold_Bullet >= 10) {
          *cold_Bullet = 0;
          *play_Bullet = 10;
      }
  }
    break;
  }
  setplane(thePane, thePlane);
}

//operations for obstacles and reset their position 
void moveObstacle(gamePane * thePane, plane *thePlane, obstacle * theObstacle) {
  int i, j;
  srand((unsigned)time(NULL));
  for (i = 0; i < 21; i++) {
    removeOBJ1(thePane, theObstacle->position);
   if(theObstacle->position.x[i] >= 15) {
      for (j = 0; j < 21; j++) {
        theObstacle->position.y[j] = rand()%31;
        theObstacle->position.x[j] = - 1;
      }
      
    }
   theObstacle->position.x[i]++;
   setobstacle(thePane, thePlane, theObstacle);
  }
}

//operations for bullets and reset their position 
void moveBullet(gamePane *thePane, obstacle *theObstacle, bullet *theBullet) {
  int i;
  for (i = 0; i <= 80; i++) {
    if (thePane->pane[theBullet->position.x[i]][theBullet->position.y[i]] == 24) {
          thePane->pane[theBullet->position.x[i]][theBullet->position.y[i]] = ' ';
        theBullet->position.x[i]--;
        if (theBullet->position.x[i] >= -2) {
          thePane->pane[theBullet->position.x[i]][theBullet->position.y[i]] = 24;   
        }
      else {
        thePane->pane[theBullet->position.x[i]][theBullet->position.y[i]] = ' ';
      }
    }
  }
  setBullet(thePane, theObstacle, theBullet);
}
