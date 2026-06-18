# Hello World 插件

这是一个示例插件，演示如何为 Lyra Shell 创建动态插件。

## 编译

### 交叉编译（用于 Lyra Pi W）

```bash
cd /home/robbieht/桌面/LyraShell/helloworld
/home/robbieht/Lyra-sdk/buildroot/output/rockchip_rk3506_luckfox/host/bin/qmake helloworld.pro
make
```

### 本地编译（用于测试）

```bash
cd /home/robbieht/桌面/LyraShell/helloworld
/home/robbieht/Qt5.12.9/5.12.9/gcc_64/bin/qmake helloworld.pro
make
```

## 安装

编译后会生成 `libhelloworld.so` 文件，将其复制到 LyraShell 的 plugins 目录：

```bash
mkdir -p /home/robbieht/桌面/LyraShell/plugins
cp libhelloworld.so /home/robbieht/桌面/LyraShell/plugins/
```

## 运行

启动 LyraShell 后，插件会自动加载，侧边栏会显示 "Hello World" 项目。

## 目录结构

```
helloworld/
├── helloworld.h      # 插件头文件
├── helloworld.cpp    # 插件实现
├── helloworld.pro    # 项目文件
└── README.md         # 说明文档
```

## 创建自己的插件

1. 复制此目录作为模板
2. 修改类名和实现
3. 实现 `AppInterface` 接口：
   - `appName()` - 返回应用名称
   - `iconPath()` - 返回图标类型（home/settings/files/hello）
   - `contentWidget()` - 返回内容 Widget
   - `initialize()` - 初始化逻辑
4. 编译并放置到 plugins 目录
