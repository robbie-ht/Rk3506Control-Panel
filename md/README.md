# Lyra Shell

用于幸狐 RK3506 的模块化桌面 Shell，内置双栏文件管理器和主题系统。

## 功能特性

- 中文字体支持（文泉驿正黑）
- 主页：系统信息卡片（CPU温度/使用率、内存、运行时间、时钟）
- 设置：主题切换（深色/浅色），重启后自动恢复
- 文件管理器：双栏独立导航，长按弹出操作菜单
- 插件系统：AppInterface 接口，自动加载 .so 插件
- 主题系统：深色（紫色系）和浅色（橙色+米白色系），通过 QSS 文件加载

## 编译

### 本地编译（测试）

```bash
cd build_x86
/home/robbieht/Qt5.12.9/5.12.9/gcc_64/bin/qmake ../LyraShell.pro
make -j$(nproc)
./LyraShell
```

### 交叉编译（RK3506）

```bash
cd build
/home/robbieht/Lyra-sdk/buildroot/output/rockchip_rk3506_luckfox/host/bin/qmake ../LyraShell.pro
make -j$(nproc)
```

### 编译插件

```bash
cd helloworld
# x86
/home/robbieht/Qt5.12.9/5.12.9/gcc_64/bin/qmake helloworld.pro && make
# ARM
/home/robbieht/Lyra-sdk/buildroot/output/rockchip_rk3506_luckfox/host/bin/qmake helloworld.pro && make
```

## 目录结构

```
Rk3506Control-Panel/
├── main.cpp                    # 程序入口（含中文字体加载）
├── lyramainwindow.h/cpp        # 主窗口
├── LyraShell.pro               # 项目文件
├── resources.qrc               # 资源文件
├── resources/
│   └── power.png               # 电源按钮图标
├── fonts/
│   └── wqy-zenhei.ttc          # 文泉驿正黑中文字体
├── themes/                     # 主题文件
│   ├── dark.qss                # 深色主题
│   └── light.qss               # 浅色主题
├── utils/
│   └── thememanager.h/cpp      # 主题管理器
├── sidebar/                    # 侧边栏模块
├── content/                    # 内容区域模块
├── widgets/                    # 自定义控件
├── apps/                       # App 管理模块
├── sysinfo/                    # 系统信息模块
├── filemanager/                # 文件管理器模块
├── helloworld/                 # 插件示例
├── md/                         # 项目文档
├── build/                      # ARM 编译输出
└── build_x86/                  # x86 编译输出
```

## 添加新 App

1. 创建新的 App 类，实现 `AppInterface` 接口
2. 在 `lyramainwindow.cpp` 的 `loadBuiltInApps()` 中注册

## 添加新主题

1. 复制 `themes/dark.qss` 为新文件
2. 修改颜色值
3. 在 `resources.qrc` 中添加新文件
4. 在 `utils/thememanager.cpp` 的 `availableThemes()` 中添加主题名

## 部署到 RK3506

```bash
scp build/LyraShell root@<设备IP>:/opt/
scp -r build/plugins root@<设备IP>:/opt/
ssh root@<设备IP>
chmod +x /opt/LyraShell
/opt/LyraShell
```
