#include "snake.h"

int key;
int oldKey;

int i;  //要変更

int snakelen = 1;

int windowMin_X;
int windowMax_X;
int windowMin_Y;
int windowMax_Y;

block_t *snake, *block, *newAddr, *tmp;


void initGameScreen() {

    WINDOW *win_Addr;

    initscr();
    curs_set(0);
    cbreak();
    noecho();
    keypad(stdscr,TRUE);

    windowMin_X = (COLS - WIDTH) / 2;
    windowMax_X = WIDTH + windowMin_X - 1;
    windowMin_Y = (LINES - HEIGHT) / 2;
    windowMax_Y = HEIGHT + windowMin_Y - 1;

// #ifdef TEST

//     addstr(
//         "     +--------------------------------------------------+     \n"
//         "     |                                                  |     \n"
//         "     |                                                  |     \n"
//         "     |                                                  |     \n"
//         "     |                                                  |     \n" 
//         "     |                                                  |     \n" 
//         "     |           n        m         d                   |     \n" 
//         "     |                                                  |     \n" 
//         "     |                   n                              |     \n" 
//         "     |                                      n           |     \n" 
//         "     |            d                                     |     \n" 
//         "     |                                                  |     \n" 
//         "     |                                                  |     \n" 
//         "     |                      m                           |     \n" 
//         "     |                                                  |     \n"
//         "     |                                                  |     \n"
//         "     |                                                  |     \n" 
//         "     |                                                  |     \n" 
//         "     |                         n                        |     \n" 
//         "     |                                                  |     \n" 
//         "     |                                                  |     \n"
//         "     |                                                  |     \n"
//         "     |                                                  |     \n"
//         "     |                                                  |     \n"
//         "     |                                                  |     \n"
//         "     |                                                  |     \n"
//         "     |                                                  |     \n"
//         "     |                                                  |     \n"
//         "     |                    m                             |     \n"
//         "     |                                        d         |     \n"
//         "     |                                                  |     \n"
//         "     |                                                  |     \n" 
//         "     |              n                                   |     \n" 
//         "     |                                                  |     \n" 
//         "     |                                                  |     \n"
//         "     |                                                  |     \n"
//         "     |                                                  |     \n" 
//         "     |                                                  |     \n" 
//         "     |                                                  |     \n" 
//         "     |                                                  |     \n" 
//         "     |                                   m              |     \n"
//         "     |                                                  |     \n"
//         "     |                                                  |     \n"
//         "     |                                                  |     \n"
//         "     |                                                  |     \n"
//         "     |                                                  |     \n"
//         "     |                  n                               |     \n"
//         "     |                                                  |     \n"
//         "     |                                                  |     \n"
//         "     |                                                  |     \n"
//         "     |                          n                       |     \n"
//         "     +--------------------------------------------------+     \n"
//         "     |                 --- Test Map ---                 |     \n"
//         "     +--------------------------------------------------+     "
//     );

// #else

    refresh();
    win_Addr = newwin(HEIGHT, WIDTH, windowMin_Y, windowMin_X);
    box(win_Addr, 0 , 0);
    srand((unsigned int)time(NULL));
    
    snake -> x = rand() % (WIDTH - 1) + windowMin_X;
    snake -> y = rand() % (HEIGHT - 1) + windowMin_Y;
    snake -> next = NULL;
    wrefresh(win_Addr);

//#endif
    
}

void crawl(int udlr) {

    block = snake;

    bool parent = true;
    while(block != NULL){

        if(parent){

            //1マス進むためにすべてのブロックをずらす準備
            shiftBlocks(snake);

            //
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

                break;

            case 'm' : 
                /* 蛇の長さを2倍にする */
                for(i = 0; i < snakelen; i++) {
                    newAddr = (block_t *)malloc(sizeof(block_t));
                    tmp = snake;

                    while(tmp -> next != NULL){

                        tmp = tmp -> next;

                    } 

                    newAddr -> next = NULL;
                    newAddr -> x = tmp -> x;
                    newAddr -> y = tmp -> y;
                    tmp -> next = newAddr;
                }

                snakelen *= 2;

                break;

            case 'd' :
                /* 蛇の長さを半分にする */
                if(snakelen > 1) {
                    tmp = snake;
                    for (i = 0; i < (snakelen / 2) - 1; i++) {
                        tmp = tmp -> next;
                    }
                    killsnake(tmp -> next);
                    tmp -> next = NULL;

                    snakelen /= 2;
                }

                break;

            case 'x' : //GameOver
                break;

            default : break;
            
        }

        addch('x');
        block = block -> next;
        parent = false;

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

void shiftBlocks(block_t *head) {

    if(head -> next != NULL) {
        if(head -> next -> next != NULL) {
            shiftBlocks(head -> next);
        }

        (head -> next) -> y = head -> y;
        (head -> next) -> x = head -> x;
    }

}

void killsnake(block_t *head){

    if(head -> next != NULL) {
        killsnake(head -> next);
    }

    free(head);

}