#include "snake.h"

int key = 0;
int oldKey;
int snaklen = 2;
block_t *snake;

int main(void) {

    snake = (block_t *)malloc(sizeof(block_t));

    srand((unsigned int)time(NULL));



    snake -> x = rand() % 50 + 6;
    snake -> y = rand() % 50 + 1;
    snake -> next = NULL;
    
    initGameScreen();

    timeout(200);

    key = ERR;
    oldKey = KEY_DOWN;
    while(5 < snake -> x && snake -> x < 56 && 0 < snake -> y && snake -> y < 51) {
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
            
            default : break;
        }
    }

    killsnake(snake);

    endwin();

    return 0;

}