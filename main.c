/*
 *
 * [snake Game]
 *
 */


#include "snake.h"

int main(void) {
    
    bool gameOver = false;
    bool stopGame = false;
    
    initGameScreen();
    initGameConfig();
    
    gameStartScreen();
    
    key = ERR;
    oldKey = KEY_DOWN;
    
    while(gameOver == false && stopGame == false) {
        
        if(key == ERR || keysAreRev(key, oldKey) == true) {
            
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
                stopGame = pauseGame();  break;
                
            default :
                key = oldKey; break;
        }
    }
    
    if(gameOver) {
        
        gameOverScreen();
        
    }
    
    killSnake(snake);
    
    endwin();
    
    return 0;
    
}


