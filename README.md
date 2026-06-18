# Lyra Shell

用于幸狐 RK3506 的模块化桌面 Shell，内置文件管理器。

## 功能特性

- 🈶 中文字体支持（文泉驿正黑）
- 🏠 主页
- ⚙️ 系统设置
- 📁 文件管理器
  - 浏览文件系统
  - 返回上级目录
  - 复制/剪切/粘贴
  - 删除文件/目录
  - 新建文件夹
  - 重命名
  - 列表/图标视图切换

## 编译

### 本地编译（测试）

```bash
cd /home/robbieht/桌面/Rk3506Control-Panel
/home/robbieht/Qt5.12.9/5.12.9/gcc_64/bin/qmake LyraShell.pro
make
./LyraShell
```

### 交叉编译（RK3506）

```bash
cd /home/robbieht/桌面/Rk3506Control-Panel
/home/robbieht/Lyra-sdk/buildroot/output/rockchip_rk3506_luckfox/host/bin/qmake LyraShell.pro
make
```

## 目录结构

```
Rk3506Control-Panel/
├── main.cpp                    # 程序入口（含中文字体加载）
├── lyramainwindow.h/cpp        # LyraShell 主窗口
├── LyraShell.pro               # 项目文件
├── resources.qrc               # 资源文件
├── README.md                   # 说明文档
├── LICENSE                     # 许可证
│
├── fonts/                      # 字体目录
│   └── wqy-zenhei.ttc          # 文泉驿正黑中文字体
│
├── sidebar/                    # 侧边栏模块
│   ├── sidebar.h/cpp
│   └── sidebaritem.h/cpp
│
├── content/                    # 内容区域模块
│   └── contentarea.h/cpp
│
├── widgets/                    # 自定义控件
│   └── powerbutton.h/cpp
│
├── apps/                       # App 管理模块
│   ├── appinterface.h
│   └── appmanager.h/cpp
│
└── filemanager/                # 文件管理器模块
    ├── filemanager.h/cpp       # 核心逻辑
    ├── filelistview.h/cpp      # 文件列表视图
    ├── filemanagerwidget.h/cpp # 文件管理器组件
    ├── filemanagerapp.h/cpp    # App接口
    ├── toolbar.h/cpp           # 工具栏
    ├── statusbar.h/cpp         # 状态栏
    └── dialogs.h/cpp           # 对话框
```

## 添加新 App

1. 创建新的 App 类，实现 `AppInterface` 接口
2. 在 `lyramainwindow.cpp` 的 `loadBuiltInApps()` 中注册

## 部署到 RK3506

```bash
scp LyraShell root@<设备IP>:/opt/
ssh root@<设备IP>
chmod +x /opt/LyraShell
/opt/LyraShell
```
