/********************************************************************
 
 functions.c -- snakeGameの関数
 
 ********************************************************************/

#include <ncurses.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include "functions.h"

#define loop(num) for(int qazwsx = 0; qazwsx < num; qazwsx++)

const char* const GAME_NAME = "Snake Game";

const int UP = 0;
const int DOWN = 1;
const int LEFT = 2;
const int RIGHT = 3;

const int MARGIN_X = 5;
const int MARGIN_Y = 5;

/* *snakeの定義 */
block_t *snake = NULL;



/* ----------------------- staticな変数宣言ゾーン ----------------------- */

static int windowMin_X;
static int windowMax_X;
static int windowMin_Y;
static int windowMax_Y;

static int food_X;
static int food_Y;

static int score = 0;

static int width;
static int height;



/* ----------------------- staticな関数宣言ゾーン ----------------------- */


/*********************************************************
 addBlock(block_t *head, int *len) -- ヘビの体長を増やす
 
 説明: ヘビのbodyの最後尾に'x'を追加する関数
 
 パラメータ: *head -- ヘビの頭のアドレス
 *len -- ヘビの体長を格納している変数のアドレス
 *********************************************************/
static void addBlock(block_t *head, int *len);

/*********************************************************
 addFoods() -- 食べ物を設置する関数
 
 説明: ゲーム画面内にランダムに食べ物を配置
 *********************************************************/
static void addFoods();

/*********************************************************
 showScore(const int score) -- スコアを表示する関数
 
 説明: スコアを画面下部に表示させる関数
 
 パラメータ: score -- スコアを格納している変数を指定.
 *********************************************************/
static void showScore(const int score);

/*********************************************************
 updateScore(int *score) -- スコアをインクリメントする関数
 
 説明: スコアをインクリメントする関数. これ関数にする意味はあったのか...?
 
 パラメータ: *score -- スコアを格納する変数のアドレス:
 *********************************************************/
static void updateScore(int *score);

/*********************************************************
 shiftBlocks(block_t *head) -- ヘビのブロックをその親ブロックの座標にシフトする関数
 
 説明: へびが移動するときあるブロックnはnの親ブロックの座標に移動する.
 そのときにヘビ全体のブロックをシフトする関数.
 
 パラメータ: *head -- ヘビの頭のアドレス
 *********************************************************/
static void shiftBlocks(block_t *head);

/*********************************************************
 addchXCenter(char *str, int y, int start, int end) -- y座標におけるstartからendの間の中心にstrを表示する関数
 
 説明: とあるy座標のx座標startからendの間における真ん中に文字を出力する関数. centering的な.
 
 パラメータ: *str -- 出力したい文字列
 y -- 出力したいy座標
 start -- 出力をしたいy座標におけるx座標のスタート
 end -- 出力をしたいy座標におけるx座標のエンド
 *********************************************************/
static void addchXCenter(const char *str, int y, int start, int len);

/*********************************************************
 isBody(block_t *head, int x, int y) -- (x, y)座標にヘビがいるかどうか判別
 
 説明: (x, y)座標にヘビのbodyがあるかどうかの判断をする.
 
 パラメータ: *head -- ヘビの頭のアドレス
 x -- 調べたい座標のx座標
 y -- 調べたい座標のy座標
 
 戻り値:(x, y)座標にヘビがいるならtrueを返す.
 そうでないならfalseを返す.
 *********************************************************/
static bool isBody(block_t *head, int x, int y);



/* ----------------------- 関数定義ゾーン ----------------------- */


void initGameScreen() {

    WINDOW *mainWin_Addr, *titleWin_Addr, *scoreWin_Addr;

    srand((unsigned int)time(NULL)); 

    initscr();
    start_color();
    curs_set(0);
    cbreak();
    noecho();
    keypad(stdscr,TRUE);

    width = COLS - 10;
    height = LINES - 10;

    if(width < MARGIN_X * 1.5 || height < MARGIN_Y * 1.5) {

        perror("画面サイズが小さすぎます");

        exit(EXIT_FAILURE);
    }

    windowMin_X = (COLS - width) / 2;
    windowMax_X = width + windowMin_X - 1;
    windowMin_Y = (LINES - height) / 2;
    windowMax_Y = height + windowMin_Y - 1;

    refresh();

    mainWin_Addr = newwin(height, width, windowMin_Y, windowMin_X);
    box(mainWin_Addr, 0 , 0);
    wrefresh(mainWin_Addr);

    titleWin_Addr = newwin(3, width, windowMin_Y - 2, windowMin_X);
    box(titleWin_Addr, 0 , 0);
    addchXCenter(GAME_NAME, windowMin_Y - 1, windowMin_X, width);
    wrefresh(titleWin_Addr);

    scoreWin_Addr = newwin(3, width, windowMax_Y, windowMin_X);
    box(scoreWin_Addr, 0 , 0);
    showScore(score);
    wrefresh(scoreWin_Addr); 
    
}

void initGameConfig(){

    snake = (block_t *)malloc(sizeof(block_t));

    snake -> x = rand() % (width - MARGIN_X * 2) + windowMin_X + MARGIN_X;
    snake -> y = rand() % (height - MARGIN_Y * 2) + windowMin_Y + MARGIN_Y;
    snake -> next = NULL;

    timeout(200);

    addFoods();

}

bool gameStartScreen() {
    
    int tmpKey, oldTmpKey, tmpNum = 0;
    int option_Y = (windowMin_Y + windowMax_Y) / 2;
    
    char *gameLogo[] = {
        "                       #                                               ",
        " ####                  #               #####                           ",
        "#   ##                 #              ##   ##                          ",
        "#       # ###    ###   #  ##   ####  ##         ###   #### ###    #### ",
        "##      ##  ##  #   #  # ##   ##  #  #         #   #  ##  ##  #  ##  # ",
        "  ###   #    #      #  # #    #   ## #    ###      #  #   #   #  #   ##",
        "    ##  #    #   ####  ###    ###### #      #   ####  #   #   #  ######",
        "     #  #    #  #   #  # ##   #      ##     #  #   #  #   #   #  #     ",
        "#    #  #    #  #  ##  #  #   ##      ##   ##  #  ##  #   #   #  ##    ",
        " ####   #    #   ####  #   #   ####    #####    ####  #   #   #   #### "
    };
    
    if(COLS >= (int)strlen(gameLogo[0]) + MARGIN_X * 2 + 2 && LINES > 24){
    
        for(tmpNum = 1; tmpNum < 10; tmpNum++)
            addchXCenter(gameLogo[tmpNum], windowMin_Y + tmpNum, windowMin_X, width);
        
        option_Y = ((windowMin_Y + windowMax_Y) / 2 + windowMax_Y) / 2;
        
    }
    
    /* 選択肢の表示 */
    addchXCenter("[ ] Start", option_Y, windowMin_X, width);
    addchXCenter("[ ] Exit ", option_Y + 1, windowMin_X, width);
    
    /* 選択している方を示すための'*'を表示. デフォルトはStart側 */
    move(option_Y, windowMin_X + (width - 9) / 2 + 1);
    addch('*');
    
    /* 画面の再描画 */
    refresh();
    
    while(1) {
        
        tmpKey = getch();
        
        /* 決定キーが押されるまでループ */
        if(tmpKey != KEY_RIGHT && tmpKey != 'd') {
            
            switch(tmpKey) {
                    
                case 'w' : /* KEY_UPと同じ処理 */
                case KEY_UP :
                    move(option_Y + 1, windowMin_X + (width - 9) / 2 + 1);
                    addch(' ');
                    move(option_Y, windowMin_X + (width - 9) / 2 + 1);
                    addch('*');
                    oldTmpKey = tmpKey;
                    break;
                    
                case 's' : /* KEY_DOWNと同じ処理 */
                case KEY_DOWN :
                    move(option_Y, windowMin_X + (width - 9) / 2 + 1);
                    addch(' ');
                    move(option_Y + 1, windowMin_X + (width - 9) / 2 + 1);
                    addch('*');
                    oldTmpKey = tmpKey;
                    break;
                    
                default :
                    tmpKey = oldTmpKey;
                    break;
                    
            }
            
        }else{
            
            /* 選択肢表示を消す */
            addchXCenter("         ", option_Y, windowMin_X, width);
            addchXCenter("         ", option_Y + 1, windowMin_X, width);
            
            /* SnakeGameのロゴを消す */
            if(tmpNum != 0){
                
                loop(10)
                    addchXCenter("                                                                       ", windowMin_Y + (tmpNum--), windowMin_X, width);
                
            }
            
            break;
        }
    }
    
    if(oldTmpKey == KEY_DOWN || oldTmpKey == 's') return true;
    
    return false;
    
}

bool crawl(int udlr) {

    static int snakeLen = 1;
    int lentmp;
    int headchar;
    bool gameOver = false;
    block_t *block;
    block_t *tmp;
    block = snake;

    /* 蛇の先端がウインドウ内ではないとき */
    if(((windowMin_X < snake -> x && snake -> x < windowMax_X) && (windowMin_Y < snake -> y && snake -> y < windowMax_Y)) == false) {

        gameOver = true;

    }

    while(block != NULL && gameOver == false){

        if(block == snake){

            //1マス進むためにすべてのブロックをずらす準備
            shiftBlocks(snake);

            //押されたボタンで進行方向決定
            switch(udlr) {
                
                case UP     : block -> y -= 1; headchar = ACS_UARROW; break;
                case DOWN   : block -> y += 1; headchar = ACS_DARROW; break;
                case LEFT   : block -> x -= 1; headchar = ACS_LARROW; break;
                case RIGHT  : block -> x += 1; headchar = ACS_RARROW; break;

            }

            gameOver = isBody(snake -> next, snake -> x, snake -> y);

            move(block -> y, block -> x);

            switch(*unctrl(inch())) {

                case 'n' : 
                    /* 増やす */
                    addBlock(snake, &snakeLen);
                    addFoods();
                    updateScore(&score);
                    showScore(score);

                    break;

                case 'm' : 
                    /* 蛇の長さを2倍にする */
                    lentmp = snakeLen;
                    loop(lentmp) {
                        addBlock(snake, &snakeLen);
                    }

                    addFoods();
                    updateScore(&score);
                    showScore(score);

                    break;

                case 'd' :
                    /* 蛇の長さを半分にする */
                    if(snakeLen > 1) {
                        tmp = snake;
                        loop((snakeLen / 2) - 1) {
                            tmp = tmp -> next;
                        }
                        killSnake(tmp -> next);
                        tmp -> next = NULL;

                        snakeLen /= 2;
                    }

                    addFoods();
                    updateScore(&score);
                    showScore(score);

                    break;

                default : break;
            
            }

        }

        move(block -> y, block -> x);

        if(block == snake){
            addch(headchar);
        }else{
            addch('x');
        }

        block = block -> next;

    }

    oldKey = key;
    key = getch();
    usleep(10000);

    block = snake;

    while(block != NULL) {

        move(block -> y, block -> x);
        addch(' ');
        block = block -> next;

    }

    return gameOver;

}

bool pauseGame() {

    char title[] = "pause";
    char cpyKey;
    int tmp, oldTmp;

    //GameTitleを一旦隠す
    addchXCenter("          ", windowMin_Y - 1, windowMin_X, width);
    addchXCenter(title, windowMin_Y - 1, windowMin_X, width);

    //foodを一旦隠す
    move(food_Y, food_X);
    cpyKey = *unctrl(inch());
    addch(' ');

    //選択肢の提示
    addchXCenter("[ ] Start", (windowMin_Y + windowMax_Y) / 2, windowMin_X, width);
    addchXCenter("[ ] Exit ", (windowMin_Y + windowMax_Y) / 2 + 1, windowMin_X, width);

    //選択している方を示すための'*'を表示. デフォルトはStart側
    move((windowMin_Y + windowMax_Y) / 2, windowMin_X + (width - 9) / 2 + 1);
    addch('*');

    //画面の再描画
    refresh();

    while(1) {

        tmp = getch();

        //決定キーが押されるまでループ
        if(tmp != KEY_RIGHT && tmp != 'd') {

            switch(tmp) {

                case 'w' : /* KEY_UPと同じ処理 */
                case KEY_UP : 
                    move((windowMin_Y + windowMax_Y) / 2 + 1, windowMin_X + (width - 9) / 2 + 1);
                    addch(' ');
                    move((windowMin_Y + windowMax_Y) / 2, windowMin_X + (width - 9) / 2 + 1);
                    addch('*');
                    oldTmp = tmp;
                    break;

                case 's' : /* KEY_DOWNと同じ処理 */
                case KEY_DOWN :
                    move((windowMin_Y + windowMax_Y) / 2, windowMin_X + (width - 9) / 2 + 1);
                    addch(' ');
                    move((windowMin_Y + windowMax_Y) / 2 + 1, windowMin_X + (width - 9) / 2 + 1);
                    addch('*');
                    oldTmp = tmp;
                    break;

                default :
                    tmp = oldTmp;
                    break;

            }

        }else{

            //選択肢表示を消す
            addchXCenter("         ", (windowMin_Y + windowMax_Y) / 2, windowMin_X, width);
            addchXCenter("         ", (windowMin_Y + windowMax_Y) / 2 + 1, windowMin_X, width);

            break;
        }

    }

    //GameTitle再描画
    addchXCenter(GAME_NAME, windowMin_Y - 1, windowMin_X, width);

    //食べ物再描画
    move(food_Y, food_X);
    addch(cpyKey);

    //最後に押されたキーを元にreturn値を決定
    if(oldTmp == KEY_DOWN || oldTmp == 's') return true;

    return false;
}


void gameOverScreen() {

    addchXCenter("-- Game Over --", windowMin_Y + height / 10, windowMin_X, width);
    
    refresh();
    sleep(2);

}



/* ----------------------- staticな関数定義ゾーン ----------------------- */


void addBlock(block_t *head, int *len) {
    
        block_t *newAddr = (block_t *)malloc(sizeof(block_t));
        block_t *tmp;

        *len += 1;
        tmp = head;
    
        while(tmp -> next != NULL)
            tmp = tmp -> next;
    
        newAddr -> next = NULL;
        newAddr -> x = tmp -> x;
        newAddr -> y = tmp -> y;
        tmp -> next = newAddr;
    
}


void addFoods() {
    
    int foodType = rand() % 10;
    
    do{

        food_X = rand() % (width - 2) + 1 + windowMin_X;
        food_Y = rand() % (height - 2) + 1 + windowMin_Y;

    }while(isBody(snake, food_X, food_Y) == true);  //ヘビの体上に座標が決まればもう一度ランダムに決定
    
    move(food_Y, food_X);
    
    if(foodType < 6){

        addch('n');

    }else if(foodType < 8){

        addch('m');

    }else{

        addch('d');

    }
    
}


void showScore(const int score) {

    move(windowMax_Y + 1, windowMin_X + (width - 10) / 2);
    printw("score : %d", score);

}


void updateScore(int *score) {
    (*score)++;
}


void shiftBlocks(block_t *head) {

    if(head -> next != NULL) {

        if(head -> next -> next != NULL) {

            shiftBlocks(head -> next);

        }

        (head -> next) -> y = head -> y;
        (head -> next) -> x = head -> x;

    }

}


void killSnake(block_t *head){

    if(head -> next != NULL) {

        killSnake(head -> next);

    }

    free(head);

}


void addchXCenter(const char *str, int y, int start, int end) {

    move(y, start + (end - (int)strlen(str)) / 2);
    addstr(str);

}


bool areKeysRev(const int key1, const int key2) {

    if(key1 == KEY_UP && key2 == KEY_DOWN) return true;

    if(key1 == KEY_DOWN && key2 == KEY_UP) return true;

    if(key1 == KEY_LEFT && key2 == KEY_RIGHT) return true;

    if(key1 == KEY_RIGHT && key2 == KEY_LEFT) return true;

    return false;
}


bool isBody(block_t *head, int x, int y) {

    block_t *block = head;

    while(block != NULL){

        if(x == block -> x && y == block -> y) {

            return true;

        }

        block = block -> next;

    }

    return false;

}
