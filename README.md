# MCTS

可以在const.h设置棋盘大小和连子数

selection中公式可以调参

目前内存消耗巨大，应该可以优化为外存消耗巨大的版本

文件结构：
 - json.hpp: nlohmann/json
 - const.h: 游戏基础设置
 - game.h/.cpp: 游戏胜负判断
 - log.h/.cpp: 日志
 - main.cpp: 自对战循环
 - mcts.h/.cpp: 主算法
 - utils.h: 工具库

编译：
```
xmake
```

运行：
```
xmake run
```
