# Lyra Shell

用于幸狐 RK3606 Lyra Pi W 的模块化桌面 Shell。

## 特性

- 现代简约黑白风格设计
- 模块化侧边栏，方便添加新应用
- 浮动电源按钮（带光晕效果）
- 全解耦设计，易于维护和扩展
- CSS 样式分离，便于主题定制

## 编译

```bash
cd /home/robbieht/桌面/LyraShell
/home/robbieht/Qt5.12.9/5.12.9/gcc_64/bin/qmake LyraShell.pro
make
```

## 添加新 App

1. 创建新的 App 类，实现 `AppInterface` 接口
2. 在 `mainwindow.cpp` 的 `loadBuiltInApps()` 中注册

详细说明见代码注释。

## 目录结构

```
LyraShell/
├── main.cpp              # 程序入口
├── mainwindow.h/cpp      # 主窗口
├── sidebar/              # 侧边栏模块
├── content/              # 内容区域模块
├── widgets/              # 自定义控件
├── apps/                 # App 管理模块
├── styles/               # QSS 样式文件
│   └── main.qss          # 主样式表
├── icons/                # SVG 图标
└── LyraShell.pro         # 项目文件
```
