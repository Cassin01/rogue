# Rogue
A Rogue like game


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

## Symbol (記号)
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
