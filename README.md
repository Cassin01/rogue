# 真夏の夜のサンサーラ
A Rogue like game

![プレイ画面1](https://github.com/Cassin01/rogue/blob/images/01.png)
![プレイ画面2](https://github.com/Cassin01/rogue/blob/images/02.png)
![プレイ画面3](https://github.com/Cassin01/rogue/blob/images/03.png)


## Compile and Run

```c++
clang++ main.cpp -lcurses -std=c++14 && ./a.out
```

## Commands (コマンド)
### Move (移動)

| Direciton | Key |
|-----------|-----|
|Up         | w   |
|Down       | s   |
|Left       | a   |
|Right      | d   |

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
