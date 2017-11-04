#include "snake.h"

block_t *snake;

int key = 0;
int oldKey;

int windowMin_X;
int windowMax_X;
int windowMin_Y;
int windowMax_Y;

int main(void) {

    bool gameOver = false;
    bool stopGame = false;

    initGameScreen();
    initGameConfig();

    key = ERR;
    oldKey = KEY_DOWN;
    while( ((windowMin_X < snake -> x && snake -> x < windowMax_X) && (windowMin_Y < snake -> y && snake -> y < windowMax_Y)) && gameOver == false && stopGame == false) {
        if(key == ERR) {
            key = oldKey;
        }
        switch (key) {
            case 'w' : /* 上キーと同じ処理 */
            case KEY_UP : gameOver = crawl(UP);    break;

            case 's' : /* 下キーと同じ処理 */
            case KEY_DOWN : gameOver = crawl(DOWN);  break;

            case 'a' : /* 左キーと同じ処理 */
            case KEY_LEFT : gameOver = crawl(LEFT);  break;

            case 'd' : /* 右キーと同じ処理 */
            case KEY_RIGHT : gameOver = crawl(RIGHT); break;

                        /* 一時中断 */
            case 'q' : stopGame = pauseGame();  break;
            
            default : break;
        }
    }

    killSnake(snake);

    endwin();

    return 0;

}