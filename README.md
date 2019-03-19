# 真夏の夜のサンサーラ
A Rogue like game

![png00](https://github.com/Cassin01/rogue/blob/images/00.png)
![png02](https://github.com/Cassin01/rogue/blob/images/02.png)
![png03](https://github.com/Cassin01/rogue/blob/images/03.png)

## Compile and Run

```c++
clang++ main.cpp -lcurses -std=c++17 && ./a.out
```

## Commands (コマンド)
### Move (移動)

|コマンド|意味|
|-|-|
|h| 	左へ1歩移動する。|
|j| 	下へ1歩移動する。|
|k| 	上へ1歩移動する。|
|l| 	右へ1歩移動する。|
|y| 	左上へ1歩移動する。|
|u| 	右上へ1歩移動する。|
|b| 	左下へ1歩移動する。|
|n| 	右下へ1歩移動する。|


### Quit (ゲームの終了)
Press ``q`` or ``Q``

### forced termination (ゲームの強制終了)
Press ``CTRL-z`` or ``CTRL-c``

## Symbols (記号)
| Symbol | Meaning  |意味するもの |
|--------|----------|-----------|
| @      | yourself |あなた自身  |
| .      | floor    |床   |      
| +      | door     |扉|
| A      | enemy    |敵|
| \|     | wall     |壁|
| -      | wall     |壁|
| #      | root     |道|

## 参考
夢枕獏 上弦の月を喰べる獅子
