# Snake Game

Japanese version: [README_JP](https://github.com/bean1310/snakeGame/blob/develop/README_JP.md "日本語バージョン")

## Build

You can build with the following commands.

```bash
git clone https://github.com/bean1310/snakeGame.git
cd snakeGame
mkdir build
cmake ..
make
```
## How to play

### Basis of game
You can start game with  ```./snakegame``` command.
You manipulate snake with UP, UNDER, LEFT, RIGHT or W, S, A, D keys.
If you put 'q' key, game is paused.
Select key is RIGHT and 'd' keys in this game.

### Kinds of food
In this game, 3 foods are available.
* n : increase length of snake by one block.
* m : increase twohold length of snake.
* d : decrease halve length of snake.

## Branches
I'm  developping  this game on develop branch.
If new feature is stable, I'm going to  marge develop into  master.

## Environment

I confirmed with the following environments.

- Apple MacBook Pro (Retina, 13-inch, Early 2015): macOS High Sierra 10.13.2
- DELL Latitude E6230: Ubuntu 16.04 LTS

