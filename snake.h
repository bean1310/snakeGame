/* ヘッダファイル */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ncurses.h>
#include <sys/time.h>
#include <stdbool.h>

#define UP      0
#define DOWN    1
#define LEFT    2
#define RIGHT   3

typedef struct blockData{
    int x;
    int y;
    struct blockData *next;
} block_t;

extern block_t *snake;

extern int key;
extern int oldKey;
extern int snaklen;

/* 関数一覧 */
void initGameScreen();

void crawl(int udlr);

void shiftBlocks(block_t *head);
void killsnake(block_t *);