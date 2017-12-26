/********************************************************************
 
 functions.h -- snakeGameのfunctions.cのヘッダファイル
 
 ********************************************************************/


#ifndef _FUCTIONS_H__
#define _FUCTIONS_H__



/* ----------------------- 種類定義ゾーン ----------------------- */


#define START_SCREEN    1
#define GAMEOVER_SCREEN 2
#define PAUSE_SCREEN    3

#define UP      1
#define DOWN    2
#define LEFT    3
#define RIGHT   4



/* ----------------------- 型定義ゾーン ----------------------- */


/* ヘビの体1ブロックを格納する構造体 */
typedef struct blockData{
    int x;
    int y;
    struct blockData *next;
} block_t;



/* ----------------------- 他ファイルから参照可能な変数ゾーン ----------------------- */


/* ヘビの頭のアドレスを格納する変数 */
extern block_t *snake;

/* 入力されたキーを格納する変数 */
extern int key;

/* 前の段階で入力されたキーを格納する変数 */
extern int oldKey;



/* ----------------------- 他ファイルから参照可能な関数ゾーン ----------------------- */


/*********************************************************
 initGameScreen() -- ゲーム画面の初期化
 
 説明: シェルの画面に合わせた画面を描画する.
      小さ過ぎればstderrにその旨を出力.
 *********************************************************/
extern void initGameScreen();

/*********************************************************
 initGameConfig() -- ゲーム設定の初期化
 
 説明: 蛇の初期位置とキー入力待ち時間の初期設定を行う.
      その後food()関数で食べ物設置
 *********************************************************/
extern void initGameConfig();

/*********************************************************
 selectionScreen(const int scrType) -- ゲーム開始/停止/オーベー時の画面の出力
 
 説明: mainWindow中央に開始か終了の選択肢を提示.
      また上(w)下(s)キーで'*'を選択肢の"[ ]"内どちらかに表示.
 
 パラメータ: scrType -- 出力する画面の種類. functions.hで定義される.
            START_SCREEN: ゲーム開始時の画面
            GAMEOVER_SCREEN: ゲームオーバー時の画面
            PAUSE_SCREEN: ゲーム開始時の画面
 
 戻り値: ゲームを終了させるならtrue.プレイするならfalse.
 *********************************************************/
extern bool selectionScreen(const int scrType);

/*********************************************************
 crawl(int udlr) -- 各キー操作の処理
 
 説明: ゲームの核になる関数.なぜcrawlって名前にしたか忘れた.
 
 パラメータ: udlr -- 押されたキーの数値(snake.hの13から16行目で定義されてるもの)
 
 戻り値: ゲームオーバーになるような動作をするとtrueを返す.
        そうでないならfalse.
 *********************************************************/
extern bool crawl(int udlr);

/*********************************************************
 pauseGame() -- pause画面を出力する関数
 
 説明: pause(一時停止)画面をだす.
 プレイヤはゲームを続行するか終了させるか選択できる.
 
 戻り値: ゲームを続行するならfalseを返す.
 中止するならtrueを返す.
 *********************************************************/
extern bool pauseGame();

/*********************************************************
 gameOverScreen() -- ゲームオーバ画面を出力する関数
 
 説明: ゲームオーバの時にその画面を出力する.
 表示が貧相すぎる問題.
 *********************************************************/
extern void gameOverScreen();

/*********************************************************
 killSnake(block_t *) -- ヘビを殺す関数
 
 説明: ヘビの体に使うメモリを解放する関数. ヘビに対する動的メモリ解放.
 
 パラメータ: ヘビの頭のアドレス.
 *********************************************************/
extern void killSnake(block_t *);

/*********************************************************
 areKeysRev(const int key1, const int key2) -- 逆向きのキーか判別
 
 説明: key1とkey2が逆向きのキーか判断. 上キーと下キーは逆, 左キーと右キーは逆.
      上キーと左キーは逆ではない.
 
 パラメータ: key1, key2 -- ncursesで定義されているUP_KEY, DOWN_KEY, LEFT_KEY, RIGHT_KEY
 
 戻り値: 逆であればtrue.
        そうでないならfalse.
 *********************************************************/
extern bool areKeysRev(const int key1, const int key2);

#endif /* _FUCTIONS_H__ */
