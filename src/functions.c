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

#define loop(num) for(int qazwsx = 0; qazwsx < (num); qazwsx++)

const char* GAME_NAME = "Snake Game";

const int MARGIN_X = 5;
const int MARGIN_Y = 5;

/* *snakeの定義 */
block_t *snake = NULL;



/* ----------------------- staticな変数ゾーン ----------------------- */

static int windowMin_X;
static int windowMax_X;
static int windowMin_Y;
static int windowMax_Y;

static int score;
static int snakeLen;

static int width;
static int height;

static const char* saveFileName = "./saves/score.dat";
static int highScore;
static int magicNum;

static foodCdnt_t foodCoordinate;



/* ----------------------- staticな関数宣言ゾーン ----------------------- */


/*********************************************************
 addBlock(block_t *head, int *len) -- ヘビの体長を増やす
 
 説明: ヘビのbodyの最後尾に'x'を追加する関数
 
 パラメータ: *head -- ヘビの頭のアドレス
 *len -- ヘビの体長を格納している変数のアドレス
 *********************************************************/
static void addBlock(block_t *head, int *len);

/*********************************************************
 showScore(const int score) -- スコアを表示する関数
 
 説明: スコアを画面下部に表示させる関数
 
 パラメータ: score -- スコアを格納している変数を指定.
 *********************************************************/
static void showScore(const int score, bool highScoreFlag);

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
static void addchXCenter(const char *str, int y, int start, int end);

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

static int getHighScore();

static int mkMagicNum(const char* str);

static void overWriteSpace(char *str, unsigned int len);



/* ----------------------- 関数定義ゾーン ----------------------- */


void initGameConfig(){

    WINDOW *mainWin_Addr, *titleWin_Addr, *scoreWin_Addr;

    snake = (block_t *)malloc(sizeof(block_t));

    score = 0;
    snakeLen = 1;

    srand((unsigned int)time(NULL)); 

    initscr();
    start_color();
    curs_set(0);
    cbreak();
    noecho();
    keypad(stdscr,TRUE);
    timeout(200);

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

    snake -> x = rand() % (width - MARGIN_X * 2) + windowMin_X + MARGIN_X;
    snake -> y = rand() % (height - MARGIN_Y * 2) + windowMin_Y + MARGIN_Y;
    snake -> next = NULL;

    magicNum = mkMagicNum(GAME_NAME);

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
    highScore = getHighScore();
    showScore(highScore, true);
    wrefresh(scoreWin_Addr);

}

bool selectionScreen(const int scrType){
    
    int tmpKey, oldTmpKey, tmpNum, lines = 0;
    int option_Y = (windowMin_Y + windowMax_Y) / 2;
    int optionTxtLen;
    
    const char *startScreen[] = {
        "                       #                                               ",
        " ####                  #               #####                           ",
        "#   ##                 #              ##   ##                          ",
        "#       # ###    ###   #  ##   ####  ##         ###   #### ###    #### ",
        "##      ##  ##  #   #  # ##   ##  #  #         #   #  ##  ##  #  #    #",
        "  ###   #    #      #  # #    #   ## #    ###      #  #   #   #  #    #",
        "    ##  #    #   ####  ###    ###### #      #   ####  #   #   #  ######",
        "     #  #    #  #   #  # ##   #      ##     #  #   #  #   #   #  #     ",
        "#    #  #    #  #   #  #  #   ##      ##   ##  #   #  #   #   #  #     ",
        " ####   #    #   ### # #   #   ####    #####    ### # #   #   #   #### "
    };
    
    const char *gameoverScreen[] = {
        "  #####                              #####                    ",
        " ##   ##                            ##   ##                   ",
        "##         ###   #### ###    ####  ##     ## #   #  ####  #  #",
        "#         #   #  ##  ##  #  #    # #       # #   # #    # # # ",
        "#    ###      #  #   #   #  #    # #       # #   # #    # #   ",
        "#      #   ####  #   #   #  ###### #       # #   # ###### #   ",
        "##     #  #   #  #   #   #  #      ##     ## #   # #      #   ",
        " ##   ##  #   #  #   #   #  #       ##   ##   # #  #      #   ",
        "  #####    ### # #   #   #   ####    #####     #    ####  #   "
    };
    
    const char *pauseScreen[] = {
        "#####                              ",
        "#   ##                             ",
        "#    #   ###   #    #   ###   #### ",
        "#   ##  #   #  #    #  #  #  #    #",
        "#####       #  #    #  #     #    #",
        "#        ####  #    #   ##   ######",
        "#       #   #  #    #     #  #     ",
        "#       #   #  #   ##  #  #  #     ",
        "#        ### #  ### #  ###    #### "
    };
    
    /* もっとも幅の必要なAA文字が表示できないとそれ以外のAA文字以外も表示しないようにする */
    if(COLS >= (int)strlen(startScreen[0]) + MARGIN_X * 2 + 2 && LINES > 24){

        switch(scrType) {

            case START_SCREEN:

                lines = (int)(sizeof(startScreen) / sizeof(char *));
            
                for(tmpNum = 0; tmpNum < lines; tmpNum++)
                    addchXCenter(startScreen[tmpNum], windowMin_Y + tmpNum + 1, windowMin_X, width);
            
                option_Y = ((windowMin_Y + windowMax_Y) / 2 + windowMax_Y) / 2;

                break;

            case GAMEOVER_SCREEN:
        
                lines = (int)(sizeof(gameoverScreen) / sizeof(char *));
            
                for(tmpNum = 0; tmpNum < lines; tmpNum++)
                    addchXCenter(gameoverScreen[tmpNum], windowMin_Y + tmpNum + 1, windowMin_X, width);
            
                option_Y = ((windowMin_Y + windowMax_Y) / 2 + windowMax_Y) / 2;

                break;

            case PAUSE_SCREEN:

                lines = (int)(sizeof(pauseScreen) / sizeof(char *));
            
                for(tmpNum = 0; tmpNum < lines; tmpNum++)
                    addchXCenter(pauseScreen[tmpNum], windowMin_Y + tmpNum + 1, windowMin_X, width);
            
                option_Y = ((windowMin_Y + windowMax_Y) / 2 + windowMax_Y) / 2;

                break;
        
            default:
        
                perror("オプション画面でエラーが発生しました.");
                exit(EXIT_FAILURE);
        
        }

    }

    
    /* 選択肢の表示 */
    switch(scrType) {

        case START_SCREEN:

            addchXCenter("[ ] Start", option_Y, windowMin_X, width);
            addchXCenter("[ ] Exit ", option_Y + 1, windowMin_X, width);

            optionTxtLen = 9;

            break;

        case GAMEOVER_SCREEN: /* ゲームオーバーと一時停止では同じ選択肢を表示 */
        case PAUSE_SCREEN:

            /* foodを隠す */
            move(foodCoordinate.y, foodCoordinate.x);
            addch(' ');

            /* 選択肢の表示 */
            addchXCenter("[ ] ReStart", option_Y, windowMin_X, width);
            addchXCenter("[ ] Exit   ", option_Y + 1, windowMin_X, width);

            optionTxtLen = 11;

            break;

        default:

            perror("選択肢表示でのエラーが発生しました.");
            exit(EXIT_FAILURE);

    }

    
    /* 選択している方を示すための'*'を表示. デフォルトはStart側 */
    move(option_Y, windowMin_X + ( width - optionTxtLen ) / 2 + 1);
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
                    move(option_Y + 1, windowMin_X + ( width - optionTxtLen ) / 2 + 1);
                    addch(' ');
                    move(option_Y, windowMin_X + ( width - optionTxtLen ) / 2 + 1);
                    addch('*');
                    oldTmpKey = tmpKey;
                    break;
                    
                case 's' : /* KEY_DOWNと同じ処理 */
                case KEY_DOWN :
                    move(option_Y, windowMin_X + ( width - optionTxtLen ) / 2 + 1);
                    addch(' ');
                    move(option_Y + 1, windowMin_X + ( width - optionTxtLen ) / 2 + 1);
                    addch('*');
                    oldTmpKey = tmpKey;
                    break;
                    
                default :
                    tmpKey = oldTmpKey;
                    break;
                    
            }
            
        }else{
            
            break;

        }

    }


    /* 選択肢表示を消す */
    if(scrType == START_SCREEN) {

        addchXCenter("         ", option_Y, windowMin_X, width);
        addchXCenter("         ", option_Y + 1, windowMin_X, width);

    }else{

        addchXCenter("           ", option_Y, windowMin_X, width);
        addchXCenter("           ", option_Y + 1, windowMin_X, width);

    }

    /* ハイスコア表示をスコアの表示に変更 */
    showScore(score, false);

            
    /* ロゴ表示された時SnakeGameのロゴを消す */
    if(lines != 0){

        while(tmpNum--)
            addchXCenter("                                                                       ", windowMin_Y + tmpNum + 1, windowMin_X, width);
        
    }
    
    /* ゲーム終了ならtrueを返す */
    if(oldTmpKey == KEY_DOWN || oldTmpKey == 's') return true;

    if(scrType == PAUSE_SCREEN) addFoods(&foodCoordinate);
    
    /* ゲーム続行ならfalseを返す */
    return false;
    
}

bool crawl(int udlr) {

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
                case DOWN   : block -> y += 1; headchar = 'v'; break;
                case LEFT   : block -> x -= 1; headchar = ACS_LARROW; break;
                case RIGHT  : block -> x += 1; headchar = ACS_RARROW; break;

            }

            gameOver = isBody(snake -> next, snake -> x, snake -> y);

            move(block -> y, block -> x);

            switch(*unctrl(inch())) {

                case 'n' : 
                    /* 増やす */
                    addBlock(snake, &snakeLen);
                    addFoods(NULL);
                    score++;
                    showScore(score, false);

                    break;

                case 'm' : 
                    /* 蛇の長さを2倍にする */
                    lentmp = snakeLen;
                    loop(lentmp) {
                        addBlock(snake, &snakeLen);
                    }
                    addFoods(NULL);
                    score++;
                    showScore(score, false);

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

                    addFoods(NULL);
                    score++;
                    showScore(score, false);

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


void addFoods(foodCdnt_t *foodPointer) {
    
    int foodType = rand() % 10;

    if(foodPointer == NULL) {
    
        do{

            foodCoordinate.x = rand() % (width - 2) + 1 + windowMin_X;
            foodCoordinate.y = rand() % (height - 2) + 1 + windowMin_Y;

        }while(isBody(snake, foodCoordinate.x, foodCoordinate.y) == true);  //ヘビの体上に座標が決まればもう一度ランダムに決定
    
    
    
        if(foodType < 6){

            foodCoordinate.type = 'n';

        }else if(foodType < 8){

            foodCoordinate.type = 'm';

        }else{

            foodCoordinate.type = 'd';

        }

    }

    move(foodCoordinate.y, foodCoordinate.x);
    addch(foodCoordinate.type);
    
}

int saveHighScore() {
    
    FILE *fp;
    int buffer[2];
    int dataNum = 0;

    /* バイナリで読み書きモード */
    if((fp = fopen(saveFileName, "wb+")) == NULL) {
        perror("Failed to open save file\n");
        return 1;
    }

    dataNum = fread(&buffer[0], 4, 1, fp);

    if(dataNum != 0) {

        if(buffer[0] != magicNum) {
            perror("Readed file is not save file of this game.\n");
            fclose(fp);
            return 1;
        }
    
    /* saveファイルがなかった時 */
    }else{
        buffer[0] = magicNum;
    }

    /* データ書き込み */
    buffer[1] = score >= highScore ? score : highScore;
    if(fwrite(buffer, 4, 2, fp) != 2) {
        perror("Failed to save data.\n");
        fclose(fp);
        return 1;
    }

    fclose(fp);

    return 0;

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

void showScore(const int score, bool highScoreFlag) {

    char spaceStr[width - 3];

    overWriteSpace(spaceStr, width - 3);

    move(windowMax_Y + 1, windowMin_X + 1);
    addnstr(spaceStr, width - 3);

    move(windowMax_Y + 1, windowMin_X + (width - 10) / 2);
    if(highScoreFlag)
        printw("High Score : %d", score);    
    else
        printw("Score : %d", score);

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

    if( ( ( key1 == KEY_UP) || ( key1 == 'w' ) ) && ( ( key2 == KEY_DOWN) || ( key2 == 's' ) )) return true;

    if( ( ( key2 == KEY_UP) || ( key2 == 'w' ) ) && ( ( key1 == KEY_DOWN) || ( key1 == 's' ) )) return true;

    if( ( ( key1 == KEY_LEFT) || ( key1 == 'a' ) ) && ( ( key2 == KEY_RIGHT) || ( key2 == 'd' ) )) return true;

    if( ( ( key2 == KEY_LEFT) || ( key2 == 'a' ) ) && ( ( key1 == KEY_RIGHT) || ( key1 == 'd' ) )) return true;

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

int getHighScore() {

    FILE *fp;
    int buffer[2];
    int i = 0;

    /* バイナリで読み書きモード */
    if((fp = fopen(saveFileName, "rb")) == NULL)
        return 0;                                   //もしファイルがなければ, 開なければ, highScoreは0ということにする.

    /* bufferにデータを格納 */
    while(fread(&buffer[i], 4, 1, fp)) {
        i++;
        if(i > 1) break;
    }

    /* saveファイルらしきものがあった時 */
    if(i != 0) {

        if(i < 2) {
            //perror("Failed to read save data.\n");
            fclose(fp);
            return 0;
        }

        if(buffer[0] != magicNum) {
            //perror("Readed file is not save file of this game.\n");
            fclose(fp);
            return 0;
        }

    }

    fclose(fp);

    return buffer[1];

}

int mkMagicNum(const char* str) {

    int base = 0;

    for(int i = 0; i < 4; i++) {
        base += (str[i] << (8 * (3 - i)));
    }

    return (base + 0x80808080);
}

void overWriteSpace(char *str, unsigned int len) {

    while(len) {
        str[--len] = ' ';
    }

}