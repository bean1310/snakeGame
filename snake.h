/* ヘッダファイル */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include <stdbool.h>

#define GAME_NAME "Snake Game"

#define UP      0
#define DOWN    1
#define LEFT    2
#define RIGHT   3

#define WIDTH 80
#define HEIGHT 40

typedef struct blockData{
    int x;
    int y;
    struct blockData *next;
} block_t;

extern block_t *snake;

extern int snakelen;

extern int food_X;
extern int food_Y;

extern int windowMin_X;
extern int windowMax_X;
extern int windowMin_Y;
extern int windowMax_Y;

extern int key;
extern int oldKey;

/* 関数一覧 */
void initGameScreen();
void initGameConfig();

bool crawl(int udlr);

void addBlock();
void addFoods();
bool pauseGame();
void shiftBlocks(block_t *head);
void killSnake(block_t *);

void addchXCenter(char *str, int y, int start, int len);
void addchYCenter(char *str, int x, int start, int len);