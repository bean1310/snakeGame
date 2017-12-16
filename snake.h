/* test */
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

#define MARGIN_X 5
#define MARGIN_Y 5

#define loop(num) for(int qazwsx = 0; qazwsx < num; qazwsx++)

typedef struct blockData{
    int x;
    int y;
    struct blockData *next;
} block_t;

/* 複数のファイルで使う変数 */
block_t *snake;

int key;
int oldKey;

/* 関数一覧 */
void initGameScreen();
void initGameConfig();
void gameStartScreen();

bool crawl(int udlr);

void addBlock(block_t *head, int *len);
void addFoods();
bool pauseGame();
void gameOverScreen();

void showScore(const int score);
void updateScore(int *score);
void shiftBlocks(block_t *head);
void killSnake(block_t *);

void addchXCenter(char *str, int y, int start, int len);
bool keysAreRev(const int key1, const int key2);

bool isBody(block_t *head, int x, int y);
