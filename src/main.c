/*
 *
 * [snake Game] by BEAN
 *
 */

#include <ncurses.h>
#include "functions.h"

int key = ERR;
int oldKey = KEY_DOWN;

int main(void) {
    
    bool gameOver = false;
    bool stopGame = false;
    
    initGameConfig();
    stopGame = selectionScreen(START_SCREEN);
    addFoods(NULL);
    
    while(gameOver == false && stopGame == false) {
        
        if(key == ERR || areKeysRev(key, oldKey) == true) {
            
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
                
            case 'q' : /* 一時中断 */
                stopGame = selectionScreen(PAUSE_SCREEN);
                key = oldKey;
                break;
                
            default :
                key = oldKey; break;
        }
    
        if(gameOver) {

            if(saveHighScore())
                perror("Failed to save high score.\n");

            killSnake(snake);
                
            initGameConfig();
            
            if(!selectionScreen(GAMEOVER_SCREEN)) {

                gameOver = false;
                addFoods(NULL);

            }
        
        }

    }
    
    killSnake(snake);
    if(saveHighScore())
        perror("Failed to save high score.\n");
    
    endwin();
    
    return 0;
    
}


