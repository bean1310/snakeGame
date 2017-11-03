#include "snake.h"

int key;
int oldKey;

int i;  //要変更

int snakelen = 1;

int windowMin_X;
int windowMax_X;
int windowMin_Y;
int windowMax_Y;

int food_X;
int food_Y;

block_t *snake, *block, *newAddr, *tmp;

/* 
 *
 * ゲーム画面の描画 (移植しやすい)
 * ^^^^^^^^^^^^^^^^^^^^^^^^^^^
 * 定数WIDTHが横幅, HEIGHTが縦幅, GAME_NAMEがゲーム名入れるとそれに従ってウィンドウ作成
 * 
 */
void initGameScreen() {

    WINDOW *mainWin_Addr, *titleWin_Addr;

    initscr();
    start_color();
    curs_set(0);
    cbreak();
    noecho();
    keypad(stdscr,TRUE);

    windowMin_X = (COLS - WIDTH) / 2;
    windowMax_X = WIDTH + windowMin_X - 1;
    windowMin_Y = (LINES - HEIGHT) / 2;
    windowMax_Y = HEIGHT + windowMin_Y - 1;

    refresh();
    mainWin_Addr = newwin(HEIGHT, WIDTH, windowMin_Y, windowMin_X);
    titleWin_Addr = newwin(3, WIDTH, windowMin_Y - 2, windowMin_X);
    addchXCenter(GAME_NAME, windowMin_Y - 1, windowMin_X, WIDTH);
    box(mainWin_Addr, 0 , 0);
    box(titleWin_Addr, 0 , 0);
    srand((unsigned int)time(NULL));
    
    wrefresh(mainWin_Addr);
    wrefresh(titleWin_Addr);    
    
}

/* 
 *
 * ゲームの設定(移植不可)
 * ^^^^^^^^^^^^^^^^^^^^^^^
 * 蛇の初期位置とキー入力待ち時間の初期設定を行う.その後food()関数で食べ物設置
 * 
 */
void initGameConfig(){

    snake = (block_t *)malloc(sizeof(block_t));

    snake -> x = rand() % (WIDTH - 1) + windowMin_X;
    snake -> y = rand() % (HEIGHT - 1) + windowMin_Y;
    snake -> next = NULL;

    timeout(200);

    addFoods();

}

/* 
 * 
 * 蛇を操作するキーを押された時の処理(移植不可)
 * ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
 * 
 * 
 */

void crawl(int udlr) {

    int lentmp;
    block = snake;

    while(block != NULL){

        if(block == snake){

            //1マス進むためにすべてのブロックをずらす準備
            shiftBlocks(snake);

            //押されたボタンで進行方向決定
            switch(udlr) {
                
                case UP     : block -> y -= 1; break;
                case DOWN   : block -> y += 1; break;
                case LEFT   : block -> x -= 1; break;
                case RIGHT  : block -> x += 1; break;

            }

        }

        move(block -> y, block -> x);

        switch(*unctrl(inch())) {
            case 'n' : 
                /* 増やす */
                addBlock();
                addFoods();

                break;

            case 'm' : 
                /* 蛇の長さを2倍にする */
                lentmp = snakelen;
                for(i = 0; i < lentmp; i++) {
                    addBlock();
                }

                addFoods();

                break;

            case 'd' :
                /* 蛇の長さを半分にする */
                if(snakelen > 1) {
                    tmp = snake;
                    for (i = 0; i < (snakelen / 2) - 1; i++) {
                        tmp = tmp -> next;
                    }
                    killSnake(tmp -> next);
                    tmp -> next = NULL;

                    snakelen /= 2;
                }

                addFoods();

                break;

            case 'x' : //GameOver
                break;

            default : break;
            
        }

        move(block -> y, block -> x);
        addch('x');
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

}

void addBlock() {

    newAddr = (block_t *)malloc(sizeof(block_t));
    snakelen += 1;
    tmp = snake;

    while(tmp -> next != NULL){

        tmp = tmp -> next;

    }

    newAddr -> next = NULL;
    newAddr -> x = tmp -> x;
    newAddr -> y = tmp -> y;
    tmp -> next = newAddr;

}

void addFoods() {

    int foodType = rand() % 10;

    food_X = rand() % (WIDTH - 2) + 1 + windowMin_X;
    food_Y = rand() % (HEIGHT - 2) + 1 + windowMin_Y;

    move(food_Y, food_X);

    if(foodType < 6){
        addch('n');
    }else if(foodType < 8){
        addch('m');
    }else{
        addch('d');
    }

}

bool pauseGame() {

    char title[] = "pause";
    char cpyKey;
    int tmp, oldTmp;
    int y;

    addchXCenter("          ", windowMin_Y - 1, windowMin_X, WIDTH);
    addchXCenter(title, windowMin_Y - 1, windowMin_X, WIDTH);

    move(food_Y, food_X);
    cpyKey = *unctrl(inch());
    addch(' ');

    addchXCenter("[ ] Start", (windowMin_Y + windowMax_Y) / 2, windowMin_X, WIDTH);
    addchXCenter("[ ] Exit ", (windowMin_Y + windowMax_Y) / 2 + 1, windowMin_X, WIDTH);

    move((windowMin_Y + windowMax_Y) / 2, windowMin_X + (WIDTH - 9) / 2 + 1);
    addch('*');

    refresh();

    while(1) {
        tmp = getch();
        if(tmp != KEY_RIGHT) {

            switch(tmp) {

                case KEY_UP : 
                    move((windowMin_Y + windowMax_Y) / 2 + 1, windowMin_X + (WIDTH - 9) / 2 + 1);
                    addch(' '); 
                    move((windowMin_Y + windowMax_Y) / 2, windowMin_X + (WIDTH - 9) / 2 + 1);
                    addch('*');
                    oldTmp = tmp;
                    break;

                case KEY_DOWN :
                    move((windowMin_Y + windowMax_Y) / 2, windowMin_X + (WIDTH - 9) / 2 + 1);
                    addch(' ');
                    move((windowMin_Y + windowMax_Y) / 2 + 1, windowMin_X + (WIDTH - 9) / 2 + 1);
                    addch('*');
                    oldTmp = tmp;
                    break;

                default : break;

            }

        }else{

            addchXCenter("         ", (windowMin_Y + windowMax_Y) / 2, windowMin_X, WIDTH);
            addchXCenter("         ", (windowMin_Y + windowMax_Y) / 2 + 1, windowMin_X, WIDTH);

            break;
        }

    }

    addchXCenter(GAME_NAME, windowMin_Y - 1, windowMin_X, WIDTH);

    move(food_Y, food_X);
    addch(cpyKey);

    key = oldKey;

    if(oldTmp == KEY_DOWN){
        return true;
    }

    return false;
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

void addchXCenter(char *str, int y, int start, int len) {

    move(y, start + (len - (int)strlen(str)) / 2);
    addstr(str);

}

void addchYCenter(char *str, int x, int start, int len) {
    
        move(start + (len - (int)strlen(str)) / 2, x);
        addstr(str);
    
    }