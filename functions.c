#include "snake.h"

int key;
int oldKey;
int snakelen = 1;

int i;  //要変更

block_t *snake, *block, *pAddr, *newAddr, *tmp;


void initGameScreen() {

    initscr();
    curs_set(0);
    cbreak();
    noecho();
    keypad(stdscr,TRUE);

    addstr(
        "     +--------------------------------------------------+     \n"
        "     |                                                  |     \n"
        "     |                                                  |     \n"
        "     |                                                  |     \n"
        "     |                                                  |     \n" 
        "     |                                                  |     \n" 
        "     |           n        m         d                   |     \n" 
        "     |                                                  |     \n" 
        "     |                   n                              |     \n" 
        "     |                                      n           |     \n" 
        "     |            d                                     |     \n" 
        "     |                                                  |     \n" 
        "     |                                                  |     \n" 
        "     |                      m                           |     \n" 
        "     |                                                  |     \n"
        "     |                                                  |     \n"
        "     |                                                  |     \n" 
        "     |                                                  |     \n" 
        "     |                         n                        |     \n" 
        "     |                                                  |     \n" 
        "     |                                                  |     \n"
        "     |                                                  |     \n"
        "     |                                                  |     \n"
        "     |                                                  |     \n"
        "     |                                                  |     \n"
        "     |                                                  |     \n"
        "     |                                                  |     \n"
        "     |                                                  |     \n"
        "     |                    m                             |     \n"
        "     |                                        d         |     \n"
        "     |                                                  |     \n"
        "     |                                                  |     \n" 
        "     |              n                                   |     \n" 
        "     |                                                  |     \n" 
        "     |                                                  |     \n"
        "     |                                                  |     \n"
        "     |                                                  |     \n" 
        "     |                                                  |     \n" 
        "     |                                                  |     \n" 
        "     |                                                  |     \n" 
        "     |                                   m              |     \n"
        "     |                                                  |     \n"
        "     |                                                  |     \n"
        "     |                                                  |     \n"
        "     |                                                  |     \n"
        "     |                                                  |     \n"
        "     |                  n                               |     \n"
        "     |                                                  |     \n"
        "     |                                                  |     \n"
        "     |                                                  |     \n"
        "     |                          n                       |     \n"
        "     +--------------------------------------------------+     "
    );
    
}

void crawl(int udlr) {

    block = snake;

    bool parent = true;
    while(block != NULL){

        if(parent){

            shiftBlocks(snake);

            if(udlr == UP)
                block -> y -= 1;
            else if(udlr == DOWN)
                block -> y += 1;
            else if(udlr == LEFT)
                block -> x -= 1;
            else
                block -> x += 1;
        }

        move(block -> y, block -> x);

        switch (*unctrl(inch())) {
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