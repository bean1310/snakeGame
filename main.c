#include "snake.h"

block_t *snake;

int food_X;
int food_Y;

int key = 0;
int oldKey;

int windowMin_X;
int windowMax_X;
int windowMin_Y;
int windowMax_Y;

int main(void) {

    snake = (block_t *)malloc(sizeof(block_t));

    food_X = rand() % 50 + 6;
    food_Y = rand() % 50 + 1;

    initGameScreen();

    timeout(200);

    key = ERR;
    oldKey = KEY_DOWN;
    while( (windowMin_X < snake -> x && snake -> x < windowMax_X) && (windowMin_Y < snake -> y && snake -> y < windowMax_Y) ) {
        if(key == ERR) {
            key = oldKey;
        }
        switch (key) {
            case 'w' : /* 上キーと同じ処理 */
            case KEY_UP : crawl(UP); break;

            case 's' : /* 下キーと同じ処理 */
            case KEY_DOWN : crawl(DOWN); break;

            case 'a' : /* 左キーと同じ処理 */
            case KEY_LEFT : crawl(LEFT); break;

            case 'd' : /* 右キーと同じ処理 */
            case KEY_RIGHT : crawl(RIGHT); break;

            case 'q' : /* 一時中断 */ break;
            
            default : break;
        }
    }

    killsnake(snake);

    endwin();

    return 0;

}