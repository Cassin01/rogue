# Rogue
A Rogue like game


## Compile and Run

```c++
clang++ main.cpp -lcurses -std=c++14 && ./a.out
```

## Command (コマンド)
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
| Symbol | Meaning  |
|--------|----------|
| @      | yourself |
| .      | floor    |
| +      | door     |
| A      | enemy    |
| \|     | wall     |
| -      | wall     |

## 参考
夢枕獏 上弦の月を喰べる獅子
