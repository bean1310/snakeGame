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

After, you can play snakeGame !!

## How to play

### Basis of game
You type ```./snakegame``` then, start game.  
You manipulate snake with UP, UNDER, LEFT, RIGHT or W, S, A, D keys.
If you put 'q' key, game is paused.
Select key is RIGHT and 'd' keys in this game.

### Kinds of food
There are 3 foods.  
* n : If snake eat this food, increased lengh of snake by one block.
* m : If snake eat this food, twohold lengh of snake.
* d : If snake eat this food, halve lengh of snake.

## Branches
I develop this game on develop branch
If new feature is stable, I marge master branch

## Environment

I confirmed with the following environments.

- Apple MacBook Pro (Retina, 13-inch, Early 2015): macOS High Sierra 10.13.2
- DELL Latitude E6230: Ubuntu 16.04 LTS