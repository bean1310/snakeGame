#include "snake.h"

int windowMin_X;
int windowMax_X;
int windowMin_Y;
int windowMax_Y;

int food_X;
int food_Y;

int score = 0;

int width;
int height;

/* 
 *
 * ゲーム画面の描画 (移植しやすい)
 * ^^^^^^^^^^^^^^^^^^^^^^^^^^^
 * 定数WIDTHが横幅, HEIGHTが縦幅, GAME_NAMEがゲーム名入れるとそれに従ってウィンドウ作成
 * 
 */
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

/* 
 *
 * ゲームの設定(移植不可)
 * ^^^^^^^^^^^^^^^^^^^^^^^
 * 蛇の初期位置とキー入力待ち時間の初期設定を行う.その後food()関数で食べ物設置
 * 
 */
void initGameConfig(){

    snake = (block_t *)malloc(sizeof(block_t));

    snake -> x = rand() % (width - MARGIN_X * 2) + windowMin_X + MARGIN_X;
    snake -> y = rand() % (height - MARGIN_Y * 2) + windowMin_Y + MARGIN_Y;
    snake -> next = NULL;

    timeout(200);

    addFoods();

}

/* 
 * 
 * 蛇を操作するキーを押された時の処理(移植不可)
 * ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
 * ゲームの核になる関数. ヘビのhead以外のblockの座標ををその親blockの座標にシフトする.
 * 操作キーによりヘビのheadの次の座標を決定してheadのx, yに代入.
 * またそのx, yに食べ物があればヘビの体長を変更する.
 * 座標に記録が終わってからヘビを描画.
 * 
 */

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

/*  
 * 
 * pause画面を出力する関数
 * ^^^^^^^^^^^^^^^^^^^^^
 * ゲームタイトルが書かれている部分を"pause"に変更し, mainWindow中央に再開か終了の選択肢を提示.
 * また上(w)下(s)キーで'*'を選択肢の"[ ]"内どちらかに表示.
 * 
 */

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

                default : break;

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

/* GameOver画面を出力する関数 */
void gameOverScreen() {

    addchXCenter("-- Game Over --", windowMin_Y + height / 10, windowMin_X, width);
    
    refresh();
    sleep(2);

}

/* ヘビのbodyの最後尾に'x'を追加する関数 */
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
    
/* 食べ物を設置する関数 */
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

/* スコアを表示する関数 */
void showScore(const int score) {

    move(windowMax_Y + 1, windowMin_X + (width - 10) / 2);
    printw("score : %d", score);

}

/* スコアをインクリメントする関数 */
void updateScore(int *score) {
    (*score)++;
}

/* ヘビのブロックをその親ブロックの座標にシフトする関数 */
void shiftBlocks(block_t *head) {

    if(head -> next != NULL) {

        if(head -> next -> next != NULL) {

            shiftBlocks(head -> next);

        }

        (head -> next) -> y = head -> y;
        (head -> next) -> x = head -> x;

    }

}

/* 動的メモリ確保されたblock_t型要素の解放 */
void killSnake(block_t *head){

    if(head -> next != NULL) {

        killSnake(head -> next);

    }

    free(head);

}

/* y座標におけるstartからlenの間の中心にstrを表示する関数 */
void addchXCenter(char *str, int y, int start, int len) {

    move(y, start + (len - (int)strlen(str)) / 2);
    addstr(str);

}

/* 上の後に下キーが押されたか, またその逆, 左の後に右が押されたか, またその逆を判定する関数 */
bool keysAreRev(const int key1, const int key2) {

    if(key1 == KEY_UP && key2 == KEY_DOWN) return true;

    if(key1 == KEY_DOWN && key2 == KEY_UP) return true;

    if(key1 == KEY_LEFT && key2 == KEY_RIGHT) return true;

    if(key1 == KEY_RIGHT && key2 == KEY_LEFT) return true;

    return false;
}

/* (x, y)座標はheadから始まるヘビのbodyかどうか判定する関数 */
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