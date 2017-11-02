#include "snake.h"

block_t *snake;

int key = 0;
int oldKey;

int windowMin_X;
int windowMax_X;
int windowMin_Y;
int windowMax_Y;

int main(void) {

    initGameScreen();
    initGameConfig();

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

            case 'q' : /* 一時中断 */ sleep(2); key = oldKey; break;
            
            default : break;
        }
    }

    killSnake(snake);

    endwin();

    return 0;

}