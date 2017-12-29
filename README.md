# Snake Game

## Build
```git clone https://github.com/bean1310/snakeGame.git```

して

```make```

すると./snakegameという実行ファイルができる.

## 遊び方

### ゲームの基本
./snakegame を実行するとゲームが始まります.  
キーボードの上, 下, 左, 右キー or w, s, a, dキーで蛇を操作.ひたすら食べ物を食って（ヘビの先頭を食べ物上に持っていくだけ）いきましょう.
またゲーム中にqキーを押すと一時中断ができます.
上下 or wsキーで行いたい操作を選んで右 or d キーで決定です.

### 食べ物の種類
食べ物は3種類.  
* n : 普通の食べ物という認識で, これを食べるとヘビの体長が1ブロック伸びます.
* m : この食べ物を食べるとヘビの体長が現在の体長の2倍になります.
* d : この食べ物を食べるとヘビの体長が現在の体長の半分になります.

## Branches
開発はdevelopブランチで行います.
機能が追加されちゃんと動くものはmasterにマージします.

## 環境
READMEを描いてる時にの動作確認環境は
- Apple MacBook Pro (Retina, 13-inch, Early 2015): macOS High Sierra 10.13.2
- DELL Latitude E6230: Ubuntu 16.04 LTS