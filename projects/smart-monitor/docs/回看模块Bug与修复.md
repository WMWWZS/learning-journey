# 回看模块 Bug 记录与修复方案

> 记录时间：2026-11-16
> 范围：普通视频回看（VideoWidget / PlayerWidget / PlaybackTask / VideoModel）
> 测试方式：Windows UI 自动化（UIA）实际点击回看页，记录时间、页码、列表数量、数据库与文件变化

## 一、环境

- Windows 10/11
- Qt 6.10.1 MinGW 64-bit（qmake 工程 `Qt20.pro`）
- OpenCV 4.10（MinGW）
- MySQL 5.7.29，库 `monitor`
- 数据：当天真实录像若干条（`bin/records/chX/YYYYMMDD/`），另有 165 条 `TEST%` 测试数据（路径为演示用）

## 二、测试结果概览

| 功能 | 结果 |
| :-- | :-- |
| 启动 / 进入回看页 | ✅ 正常，无崩溃 |
| 列表加载 | ✅ 每页 10 条，页码 `1/4` |
| 翻页 | ✅ 下一页 → `2/4` |
| 双击播放 | ✅ 时间从 `0:00` 递增 |
| 时间显示 | ✅ `0:02 / 0:30` 正常刷新 |
| **暂停** | ✅ 时间冻结（连续 4 次采样均为 `0:03`） |
| **继续播放** | ❌ 点 ▶ 后时间不恢复 |
| **倍速（2.0x）** | ❌ 时间不加速 |
| **进度条跳转** | ❌ 松手后位置不变 |
| **切换视频** | ❌ 暂停后双击另一条仍播放旧视频 |
| **抓拍** | ⚠️ 播放中可用（`feature_image` +1、生成 jpg）；但暂停状态下切换视频后失败 |
| 稳定性 | ✅ 全程未崩溃 |

## 三、Bug 列表

### BUG-1：暂停后无法继续 / 倍速 / 切换视频（P0）

**现象**

- 点“暂停”后时间冻结（正常）；
- 再点播放器里的 ▶ 无反应；
- 切换倍速 2.0x 无反应；
- 暂停状态下双击其他录像，播放器仍停留在旧视频。

**原因**

`PlaybackTask::startDecode()` 在暂停时阻塞在条件变量：

```cpp
// service/playbacktask.cpp
while (m_paused && !m_stop)
{
    m_pauseCond.wait(&m_pauseMutex);   // 线程睡在这里
}
```

解码线程的事件循环被这个 `wait()` 完全占住，因此通过
`QMetaObject::invokeMethod(..., Qt::QueuedConnection)` 投递的
`play` / `setSpeed` / `seekTo` / `open` 事件**永远不会被执行**。

暂停本身能生效，是因为它在进入 `wait()` 之前就被处理了；
一旦进入等待，后续所有排队调用全部失效。

**修复**

在暂停等待中也要处理事件，注意必须先解锁再 `processEvents`（否则 `play()` 里锁同一个互斥量会死锁）：

```cpp
// playbacktask.cpp，暂停等待处
{
    QMutexLocker locker(&m_pauseMutex);
    while (m_paused && !m_stop)
    {
        locker.unlock();
        QCoreApplication::processEvents(QEventLoop::AllEvents, 10);
        locker.relock();
    }
}
```

```cpp
// 文件顶部
#include <QEventLoop>
```

**验证**：暂停 → 点 ▶ 应继续；暂停时切 2x 应加速；暂停时双击另一条应切换。

---

### BUG-2：进度条拖动跳转无效（P0）

**现象**：拖动进度条松手后，播放位置不跳转。

**原因（两处）**

1. 声明与调用不匹配：头文件是 `void seekTo();`（无参），
   而 `PlayerWidget` 调用的是
   `QMetaObject::invokeMethod(m_task, "seekTo", Qt::QueuedConnection, Q_ARG(int, value));`
   ——签名对不上，永远匹配不到；
2. 实现写错：`PlaybackTask::seekTo()` 里把 `m_paused = true`，把“跳帧”写成了“暂停”，没有记录目标帧。

```cpp
// 错误实现（当前）
void PlaybackTask::seekTo()
{
    QMutexLocker locker(&m_pauseMutex);
    m_paused = true;
}
```

**修复**

```cpp
// playbacktask.h
void seekTo(int frameNo);
```

```cpp
// playbacktask.cpp
void PlaybackTask::seekTo(int frameNo)
{
    if (frameNo < 0) return;
    m_seekFrame = frameNo;    // 记录目标帧
    m_pendingSeek = true;     // 交给解码循环执行 set(CAP_PROP_POS_FRAMES, ...)
    // 不要修改 m_paused
}
```

> 解码循环里已有 `if (m_pendingSeek.exchange(false)) m_cap.set(...)` 的处理，无需额外改动。

**验证**：播放中拖动进度条，时间应跳到对应位置；暂停时拖动也应能跳转。

---

### BUG-3：编译警告（P3）

| 位置 | 警告 | 修复 |
| :-- | :-- | :-- |
| `model/videomodel.cpp:23` | `setQuery(const QSqlQuery&)` 已弃用 | `#include <utility>`，改为 `setQuery(std::move(query))` |
| `model/videomodel.cpp:117` | 同上 | 同上 |
| `model/usermodel.cpp:44` | 非 void 函数缺少返回值 | 函数末尾补 `return false;` |

---

### BUG-4：体验问题（P3，非阻塞）

1. **列表时间重复**：列表显示 `yyyyMMddHHmm`，同分钟内多条录像显示相同文本（实测 10 条都是 `202611162308`）。
   建议改为 `yyyyMMddHHmmss` 或显示文件名。
2. **切回回看页不刷新**：`loadPage()` 只在构造、改日期/通道、翻页时调用；从其他页面切回来不会重新查询。
   建议重写 `VideoWidget::showEvent()` 调用 `loadPage(m_page)`，或加“刷新”按钮。
3. **图片预览页仍是占位**：`m_tabs` 第二个页签只有提示文字。
4. **TEST 测试数据双击打不开**：`TEST%` 的 `video_path` 是演示路径，文件不存在；真实录像可正常播放。

## 四、涉及文件

| 文件 | 说明 |
| :-- | :-- |
| `service/playbacktask.h/.cpp` | 解码循环、暂停/继续、seekTo、倍速 |
| `view/playerwidget.h/.cpp` | 播放器 UI、时间/进度条、抓拍 |
| `view/videowidget.h/.cpp` | 回看页列表、翻页、双击播放 |
| `model/videomodel.cpp` | 录像检索 SQL、分页 |
| `service/videoservice.cpp` | 业务层转发 |
| `model/usermodel.cpp` | 登录查询（返回值警告） |

## 五、修复后回归清单

- [ ] 暂停后点 ▶ 能继续播放
- [ ] 暂停中切换 2x / 4x，播放明显加速
- [ ] 拖动进度条松手后位置跳转
- [ ] 暂停中双击另一条录像能切换
- [ ] 播放中抓拍：`feature_image` +1，`bin/features/manual/` 生成 jpg
- [ ] 编译 0 warning（setQuery / return）
- [ ] 关闭窗口无 `QThread: Destroyed while thread is still running`
