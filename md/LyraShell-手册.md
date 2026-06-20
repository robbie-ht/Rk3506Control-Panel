# Lyra Shell 参数修改手册

> 每个参数标注了 **文件路径** 和 **行号附近的关键字**，方便快速定位修改。

---

## 一、主题系统

主题样式已从 `lyramainwindow.cpp` 移至独立的 QSS 文件，通过 ThemeManager 加载。

### 主题文件

| 文件 | 说明 |
|------|------|
| `themes/dark.qss` | 深色主题（深紫色系） |
| `themes/light.qss` | 浅色主题（橙色+米白色系） |
| `utils/thememanager.h/cpp` | 主题管理器单例 |

### 配置文件
- 路径：`~/.lyrashell/theme.conf`
- 内容：主题名（`dark` 或 `light`）

### 添加新主题
1. 复制 `themes/dark.qss` 为 `themes/新主题名.qss`
2. 修改颜色值
3. 在 `resources.qrc` 中添加新文件
4. 在 `utils/thememanager.cpp` 的 `availableThemes()` 中添加主题名

### 深色主题主要颜色

| 选择器 | 颜色 | 说明 |
|--------|------|------|
| QWidget `background-color` | `#0d0d1a` | 整体背景 |
| `#sidebar` 渐变 | `#0f0f1e` → `#141428` | 侧边栏背景 |
| `#sidebar` `border-right` | `#1a1a3a` | 侧边栏边框 |
| `#toolbar` 渐变 | `#141428` → `#0f0f1e` | 工具栏背景 |
| `#navButton` 渐变 | `#1e1e3a` → `#161630` | 导航按钮背景 |
| `#navButton:disabled` | `#0f0f1e` | 按钮禁用态 |
| `#pathLabel` | `#8888bb` | 路径文字 |
| QTreeView 背景 | `#0d0d1a` / `#10102a` | 文件列表/交替行 |
| QTreeView:selected | `#3a3a7a` → `#2a2a6a` | 选中项 |
| QTreeView:hover | `#1a1a3a` | 悬停项 |
| QHeaderView | `#0f0f1e` / `#8888aa` | 表头背景/文字 |
| `#panelPathLabel` | `#10102a` / `#8888bb` | 面板路径标签 |
| QMenu | `#141428` | 右键菜单背景 |
| QMenu:selected | `#3a3a7a` | 菜单选中项 |
| `#statusbar` | `#0f0f1e` | 状态栏 |
| QPushButton 渐变 | `#1e1e3a` → `#161630` | 对话框按钮 |
| `#deleteButton` | `#8b2252` → `#6b1242` | 删除按钮 |

### 浅色主题主要颜色

| 选择器 | 颜色 | 说明 |
|--------|------|------|
| QWidget `background-color` | `#fdf6ec` | 米白背景 |
| `#sidebar` 渐变 | `#f5e6d0` → `#f0dcc4` | 浅驼色侧边栏 |
| `#toolbar` 渐变 | `#f5e6d0` → `#f0dcc4` | 浅驼色工具栏 |
| `#navButton` 渐变 | `#fffcf5` → `#f5ead8` | 奶白按钮 |
| QTreeView:selected | `#f0a040` → `#e08820` | 橙色选中项 |
| QMenu:selected | `#f0a040` | 橙色菜单选中 |
| `#themeButton[selected]` | `#f0a040` → `#e08820` | 主题按钮选中态 |
| `#deleteButton` | `#e86840` → `#d04820` | 红橙删除按钮 |

---

## 二、侧边栏（sidebar/）

### sidebaritem.cpp

| 参数 | 行号附近 | 说明 |
|------|----------|------|
| `setFixedSize(60, 58)` | 构造函数 | **侧边栏项尺寸（宽×高）** |
| `QPen(..., 1.6, ...)` | `drawIcon()` | 图标线条粗细 |
| `QColor(200, 200, 255)` | `drawIcon()` | 选中时图标颜色 |
| `QColor(120, 120, 160)` | `drawIcon()` | 未选中时图标颜色 |
| `iconRect(width()/2-10, 8, 20, 20)` | `paintEvent()` | **图标区域位置和大小** |
| `font.setPixelSize(10)` | `paintEvent()` | **侧边栏文字大小** |
| `QColor(200, 200, 255)` | `paintEvent()` | 选中时文字颜色 |
| `QColor(120, 120, 160)` | `paintEvent()` | 未选中时文字颜色 |
| `QRect(0, 34, width(), 16)` | `paintEvent()` | **文字位置** |
| `QColor(74, 74, 138, 80)` | 选中渐变 start | 选中背景渐变起始色 |
| `QColor(58, 58, 122, 40)` | 选中渐变 end | 选中背景渐变结束色 |
| `QColor(140, 140, 220)` | 左侧指示条 | 选中指示条颜色 |
| `QRectF(...+14, 3, 26)` | 左侧指示条 | 指示条位置和长度 |

### 添加新图标

在 `sidebaritem.cpp` 的 `drawIcon()` 函数中，用 `else if (iconType == "xxx")` 添加新的图标绘制逻辑。

---

## 三、电源按钮（widgets/powerbutton.cpp）

| 参数 | 行号附近 | 说明 |
|------|----------|------|
| `setFixedSize(52, 52)` | 构造函数 | **电源按钮尺寸** |
| `QColor(0, 0, 0, 80)` | 阴影 | 阴影颜色和透明度 |
| `shadow->setBlurRadius(20)` | 阴影 | 阴影模糊半径 |
| `shadow->setOffset(0, 4)` | 阴影 | 阴影偏移 |
| `QColor(255, 255, 255, 15)` | 光晕 | hover 光晕颜色 |
| `QColor(42, 42, 42)` → `QColor(26, 26, 26)` | hover 渐变 | hover 时背景渐变 |
| `QColor(32, 32, 32)` → `QColor(18, 18, 18)` | normal 渐变 | 正常时背景渐变 |
| `QColor(45, 45, 45)` | 边框 | 按钮边框颜色 |
| `1.5` | 边框 | 按钮边框粗细 |
| `btnRect.width() * 0.55` | 图标大小 | **图标占按钮的比例** |
| `":/resources/power.png"` | 图标路径 | 电源图标 PNG 资源路径 |
| `qApp->quit()` | 点击事件 | 点击执行的操作 |

### 电源按钮位置

在 `lyramainwindow.cpp` 的 `updatePowerButtonPosition()` 中：
| 参数 | 说明 |
|------|------|
| `width() - btn->width() - 24` | 距右边距 |
| `height() - btn->height() - 24` | 距下边距 |

---

## 四、首页（sysinfo/homepage.cpp）

| 参数 | 行号附近 | 说明 |
|------|----------|------|
| `setContentsMargins(30, 20, 30, 20)` | `setupUI()` | **页面整体边距** |
| `setSpacing(20)` | `setupUI()` | 模块间距 |
| **时间卡片** | | |
| `setFixedWidth(320)` | timeWidget | **时间卡片宽度** |
| `setMinimumHeight(160)` | timeWidget | **时间卡片高度** |
| `border-radius: 16px` | timeWidget 样式 | 时间卡片圆角 |
| `stop:0 #1a1a2e, stop:1 #16213e` | timeWidget 样式 | 时间卡片背景渐变 |
| `setContentsMargins(30, 20, 30, 20)` | timeLayout | 时间卡片内边距 |
| `font-size: 40px` | m_timeLabel | **时钟字号** |
| `color: #ffffff` | m_timeLabel | 时钟颜色 |
| `font-family: "Noto Sans Mono"` | m_timeLabel | 时钟字体 |
| `letter-spacing: 2px` | m_timeLabel | 时钟字间距 |
| `font-size: 16px` | m_dateLabel | **日期字号** |
| `color: #8888aa` | m_dateLabel | 日期颜色 |
| **欢迎文字** | | |
| `font-size: 28px` | m_welcomeLabel | **欢迎标题字号** |
| `font-size: 14px` | deviceInfo | 设备信息字号 |
| `setSpacing(40)` | topLayout | 时间卡片与右侧间距 |
| **系统信息卡片区域** | | |
| `font-size: 16px` | sectionTitle | "系统状态"标题字号 |
| `setSpacing(15)` | cardsLayout | 卡片间距 |

### 系统信息卡片（sysinfo/sysinfocard.cpp）

| 参数 | 行号附近 | 说明 |
|------|----------|------|
| `setFixedSize(220, 130)` | `setupUI()` | **卡片尺寸（宽×高）** |
| `setContentsMargins(18, 14, 18, 14)` | mainLayout | 卡片内边距 |
| `setSpacing(6)` | mainLayout | 元素间距 |
| `m_iconLabel->setFixedSize(36, 36)` | 图标 | **图标区域大小** |
| `font-size: 20px` | 图标样式 | 图标字号 |
| `border-radius: 10px` | 图标样式 | 图标背景圆角 |
| `font-size: 13px` | 标题 | 卡片标题字号 |
| `color: #8888aa` | 标题 | 卡片标题颜色 |
| `font-size: 28px` | 值 | **数值字号** |
| `color: #ffffff` | 值 | 数值颜色 |
| `font-size: 12px` | 副标题 | 副标题字号 |
| `color: #666688` | 副标题 | 副标题颜色 |
| `14, 14` | 圆角 | 卡片圆角半径 |
| `QColor(30, 30, 50)` → `QColor(20, 20, 40)` | 背景渐变 | 卡片背景渐变色 |

---

## 五、文件管理器

### 工具栏（filemanager/toolbar.cpp）

| 参数 | 行号附近 | 说明 |
|------|----------|------|
| `setFixedHeight(44)` | `setupUI()` | **工具栏高度** |
| `setFixedSize(32, 32)` | 按钮 | **导航按钮尺寸** |
| `"↑"` `"H"` `"/"` `"R"` | 按钮文字 | **按钮显示文字** |

### 长按菜单（filemanager/filelistview.cpp）

| 参数 | 行号附近 | 说明 |
|------|----------|------|
| `m_longPressTimer->setInterval(600)` | 构造函数 | **长按触发时间（毫秒）** |
| `manhattanLength() > 20` | `eventFilter()` | **移动取消距离（像素）** |

### 双栏布局（filemanager/filemanagerwidget.cpp）

| 参数 | 行号附近 | 说明 |
|------|----------|------|
| `m_leftFileManager->goHome()` | `initialize()` | 左栏默认目录 |
| `m_rightFileManager->goRoot()` | `initialize()` | 右栏默认目录 |
| `setFixedHeight(24)` | pathLabel | **路径标签高度** |
| `setFixedWidth(1)` | separator | 面板分隔线宽度 |

### 文件列表列设置（filemanager/filelistview.cpp）

| 参数 | 行号附近 | 说明 |
|------|----------|------|
| `setColumnHidden(2, true)` | `setupUI()` | 隐藏"类型"列（0=名称, 1=大小, 2=类型, 3=日期） |
| `setSectionResizeMode(0, Stretch)` | `setupUI()` | 名称列拉伸填充 |
| `setSectionResizeMode(1, ResizeToContents)` | `setupUI()` | 大小列自适应 |

---

## 六、对话框（filemanager/dialogs.cpp）

| 对话框 | 参数 | 说明 |
|--------|------|------|
| NewFolderDialog | `setFixedSize(300, 150)` | 新建文件夹对话框尺寸 |
| RenameDialog | `setFixedSize(300, 150)` | 重命名对话框尺寸 |
| ConfirmDeleteDialog | `setFixedSize(400, 200)` | 确认删除对话框尺寸 |
| MessageDialog | `setFixedSize(350, 150)` | 消息对话框尺寸 |

---

## 七、主窗口（lyramainwindow.cpp）

| 参数 | 位置 | 说明 |
|------|------|------|
| `setMinimumSize(1024, 600)` | `setupUI()` | **窗口最小尺寸（匹配 RK3506 屏幕）** |
| `showFullScreen()` | `main.cpp` | 全屏显示 |
| `QColor(10, 10, 10)` | `paintEvent()` | 窗口绘制背景色 |

---

## 八、字体配置（main.cpp）

| 参数 | 说明 |
|------|------|
| `":/fonts/wqy-zenhei.ttc"` | 资源文件中的字体路径 |
| `"/usr/lib/fonts/wqy-zenhei/wqy-zenhei.ttc"` | 系统字体回退路径 |
| `font.setPixelSize(14)` | **全局默认字号** |

---

## 九、编译命令

### x86 版本（本地测试）
```bash
cd build_x86
/home/robbieht/Qt5.12.9/5.12.9/gcc_64/bin/qmake ../LyraShell.pro
make -j$(nproc)
./LyraShell
```

### ARM 版本（RK3506）
```bash
cd build
/home/robbieht/Lyra-sdk/buildroot/output/rockchip_rk3506_luckfox/host/bin/qmake ../LyraShell.pro
make -j$(nproc)
```

### 插件编译
```bash
cd helloworld
# x86
/home/robbieht/Qt5.12.9/5.12.9/gcc_64/bin/qmake helloworld.pro
# ARM
/home/robbieht/Lyra-sdk/buildroot/output/rockchip_rk3506_luckfox/host/bin/qmake helloworld.pro
make
```

---

## 十、常用修改速查

| 想改什么 | 改哪里 |
|----------|--------|
| 深色主题颜色 | `themes/dark.qss` |
| 浅色主题颜色 | `themes/light.qss` |
| 添加新主题 | 复制 `.qss` 文件 + 更新 `resources.qrc` |
| 主题切换持久化路径 | `utils/thememanager.cpp` `configPath()` |
| 侧边栏宽度 | `themes/*.qss` `#sidebar` 的 `min/max-width` |
| 侧边栏项大小 | `sidebaritem.cpp` `setFixedSize(60, 58)` |
| 侧边栏文字大小 | `sidebaritem.cpp` `font.setPixelSize(10)` |
| 侧边栏图标颜色 | `sidebaritem.cpp` `drawIcon()` 和 `paintEvent()` 中的 `isLight` 判断 |
| 电源按钮大小 | `powerbutton.cpp` `setFixedSize(52, 52)` |
| 电源按钮位置 | `lyramainwindow.cpp` `updatePowerButtonPosition()` 的 `-24` |
| 时钟字号 | `themes/*.qss` `#homeTimeLabel` 的 `font-size` |
| 时间卡片大小 | `homepage.cpp` `setFixedWidth(320)` 和 `setMinimumHeight(160)` |
| 系统卡片大小 | `sysinfocard.cpp` `setFixedSize(220, 130)` |
| 卡片背景色 | `sysinfocard.cpp` `paintEvent()` 中的 `isLight` 判断 |
| 工具栏高度 | `toolbar.cpp` `setFixedHeight(44)` |
| 长按触发时间 | `filelistview.cpp` `setInterval(600)` |
| 双栏默认目录 | `filemanagerwidget.cpp` `goHome()` / `goRoot()` |
| 文件列表隐藏列 | `filelistview.cpp` `setColumnHidden(2, true)` |
| 全局默认字号 | `main.cpp` `font.setPixelSize(14)` |
| 设置页面主题按钮 | `lyramainwindow.cpp` `createSettingsPage()` |
