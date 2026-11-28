# 🧵 Qt 学习主线（一份就够）

> 之前的内容是“很多张卡片”，这一份把它们串成一条线。
> 读法：第 0 站 → 第 8 站，顺序读，每站 5 分钟。
> 细节在哪：`notes/bagu/qt-核心机制问答复盘-2026-11-14.md`（23 问答）、`notes/bagu/qt-model-view-2026-11-15.md`（Model/View 详解）；实验在本地 `E:\harness\qt-trace-lab`。

---

## 0. 全景：Qt 到底在解决什么

```text
C++ 基础（类、虚函数、RAII）
 └─ QObject 对象模型          ← 第 1 站：谁生谁死、怎么互相联系
     ├─ 元对象 moc            ← 第 2 站：运行期查对象
     ├─ 信号槽                ← 第 3 站：对象之间解耦通信
     ├─ 事件循环              ← 第 4 站：程序怎么一直响应
     └─ 线程 + 亲和性         ← 第 5 站：耗时活不卡界面
         └─ 生命周期          ← 第 6 站：对象什么时候死
             └─ Model/View    ← 第 7 站：大量数据怎么显示
```

一句话：**Qt = 一堆有“户籍”和“身份证”的对象，通过消息（信号 / 事件）互相通信。**

---

## 第 1 站｜QObject：一切的地基

- **解决什么问题**：对象生命周期谁负责？对象之间怎么联系？
- **一句话**：parent 管生管死；`Q_OBJECT` 给类办身份证。
- **流程**：`new QObject(parent)` 加入父子树 → `delete 父` → 父发 `destroyed` → 断开它的连接 → 递归删除子对象
- **关键代码**：
  ```cpp
  auto *btn = new QPushButton(this);   // btn 不需要手动 delete
  ```
- **常见坑**：栈对象设 parent（会 double delete）；该用 `deleteLater` 时用了 `delete`
- **对应**：问答 Q1、Q9 ｜ 实验 `trace04_lifetime`

---

## 第 2 站｜元对象 moc：Qt 的“反射”

- **解决什么问题**：C++ 没有反射，怎么在运行期按名字找信号 / 槽 / 属性？
- **一句话**：moc 扫描带 `Q_OBJECT` 的类，生成元对象表 + 信号函数体。
- **流程**：头文件写 `signals:` 声明 → moc 生成 `staticMetaObject`、`qt_static_metacall`，以及信号函数体：
  ```cpp
  // moc 生成，你从来没写过
  void TracedObject::pinged(int _t1)
  {
      QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
  }
  ```
- **怎么亲眼看到**：打开 `qt-trace-lab/build_direct/moc_tracedobject_trace01_metacall.cpp`
- **常见坑**：漏 `Q_OBJECT` → emit 时**链接错误**；connect 字符串版**运行期 No such signal**；`qobject_cast` 编译期报错
- **对应**：问答 Q1、Q21 ｜ 实验 `trace01_metacall`

---

## 第 3 站｜信号槽：对象通信

- **解决什么问题**：对象之间不想互相知道，怎么通知？
- **一句话**：**connect 登记，emit 上报，activate 查表分发。**
- **流程**：
  1. `connect` 把“谁收、槽是哪个、怎么送”登记到 sender 的连接表
  2. `emit` = 调用 moc 生成的函数 → `QMetaObject::activate`
  3. `activate` 查表：Direct 当场调槽；Queued 投事件到 receiver 线程；Auto 在 **emit 时**比较“执行 emit 的线程”与“receiver 所属线程”
- **关键代码**：
  ```cpp
  connect(&sender, &Sender::valueChanged, &receiver, &Receiver::onValue);
  emit sender.valueChanged(1);   // emit 本质就是普通函数调用
  ```
- **连接类型**：Auto（默认：发射线程 vs receiver 所属线程）、Direct、Queued、BlockingQueued（有死锁风险）、Unique（去重）
- **为什么不是“sender 对象的线程”**：`QThread::started` 从新线程发出，但 QThread 对象户籍在创建它的线程；按“sender 户籍”会误判成 Queued，实际是 Direct。判断依据永远是“**emit 执行线程** vs **receiver 所属线程**”。
- **常见坑**：同一槽连两次会执行两次；lambda 记得传 context object；Queued 依赖接收线程有事件循环
- **对应**：问答 Q3、Q4、Q13、Q17、Q22 ｜ 实验 `trace01`、`trace03`

---

## 第 4 站｜事件循环：程序的心脏

- **解决什么问题**：GUI 程序怎么“一直活着”响应鼠标键盘？
- **一句话**：`exec()` 是个循环：从线程的事件队列取事件 → notify → `receiver->event()` 分发。
- **流程**：
  ```text
  main → app.exec() → QEventLoop::exec
       → 取事件（原生消息 / postEvent / 定时器）
       → QCoreApplication::notify → receiver->event() → 具体处理函数

  update() → 标记脏区 + 投递 UpdateRequest → 事件循环
           → paintEvent → 刷到屏幕
  ```
- **三条关键结论**：
  - 事件属于**线程**的队列，不属于对象
  - `update()` 只记账，paint 结账时读当前状态，中间状态会被跳过
  - 槽里 `msleep` → 事件循环停摆 → 重绘请求没人处理；睡眠期间的状态变化会被合并，中间帧不出现
  - 界面卡 = 槽占住事件循环线程；`processEvents()` 只是插队且会重入
- **常见坑**：在槽里 `QThread::msleep`；把 `processEvents()` 当解药
- **对应**：问答 Q5、Q6、Q14 ｜ 实验 `trace02_events`

---

## 第 5 站｜线程：让界面不卡

- **解决什么问题**：耗时任务怎么办？
- **一句话**：QThread 是一条带事件循环的“前台”；亲和性决定事件投给谁。
- **标准模板**：
  ```cpp
  QThread *thread = new QThread(this);
  Worker  *worker = new Worker;          // 关键：不能有 parent
  worker->moveToThread(thread);

  connect(thread, &QThread::started, worker, &Worker::work);
  connect(worker, &Worker::done, this, [this](const Result &r) {
      ui->label->setText(r.text);        // 回到主线程才碰 UI
  });
  connect(worker, &Worker::done, thread, &QThread::quit);
  connect(thread, &QThread::finished, worker, &QObject::deleteLater);
  thread->start();
  ```
- **核心规则**：
  - 亲和性只管事件 / 信号投递，**不管普通函数调用**（直接调用仍在调用者线程）
  - 跨线程信号 Auto → Queued → 槽在接收者线程执行
  - 没有 `exec()` 的线程：Queued 槽、QTimer、deleteLater 都不会执行
  - 带 parent 的对象不能 `moveToThread`；否则线程白开（活被排回主线程）
- **对应**：问答 Q7、Q8、Q15、Q16、Q17、Q18 ｜ 实验 `trace03_thread`

---

## 第 6 站｜生命周期：谁负责删

- **一句话**：有 parent 父管；没 parent 自管；跨线程 / 槽内删除用 `deleteLater`。
- **三条规则**：
  1. 有 parent → parent 析构时连带删除
  2. 没 parent → 谁 new 谁 delete；忘了就是内存泄漏
  3. `deleteLater` = 提交离职申请（事件循环 / 线程收尾时安全删除）；`delete` = 当场开除
- **自动断连**：receiver 析构时，连到它的信号槽自动断开，再 emit 不会崩
- **对应**：问答 Q9、Q19、Q20 ｜ 实验 `trace04_lifetime`

---

## 第 7 站｜Model/View：大量数据

- **解决什么问题**：几千行数据怎么显示不卡、怎么解耦？
- **一句话**：Model 存数据和规则，View 显示和交互，Delegate 画格子；**View 不轮询，等 Model 通知**。
- **三个角色**：Model（`rowCount()` / `data()`）、View（`QListView` / `QTableView`）、Delegate（默认 `QStyledItemDelegate`）
- **数据流**：`View → rowCount() → data(index(row), role) → delegate 画`
- **三条通知规则**：
  1. 改数据：`emit dataChanged(topLeft, bottomRight, roles)` —— topLeft / bottomRight 是变化范围（两端都包含），列表里同一 index 传两次；roles 是变化的 role 列表（如 `{Qt::DisplayRole}`）
  2. 插入 / 删除行：`beginInsertRows / endInsertRows` 成对，中间改数据
  3. 整体重建：`beginResetModel / endResetModel`（会丢选择 / 滚动位置）
- **role**：`DisplayRole` 文字、`DecorationRole` 图标、`ForegroundRole` 颜色、`Qt::UserRole + n` 自定义
- **常见坑**：改数据不通知；begin/end 不配对；`data()` 里做耗时操作
- **对应**：问答 Q10、Q23、11-16 四题 ｜ 实验 `trace05_modelview`

---

## 第 8 站｜构建与部署（还没学，先挂着）

- **最小 CMake**：
  ```cmake
  find_package(Qt6 REQUIRED COMPONENTS Widgets)
  qt_standard_project_setup()            # 只打开 AUTOMOC/AUTOUIC
  set(CMAKE_AUTORCC ON)                  # AUTORCC 要单独开（或 qt_add_resources）
  qt_add_executable(app main.cpp)
  target_link_libraries(app PRIVATE Qt6::Widgets)
  ```
- **moc/uic/rcc 去哪看**：构建目录 `<target>_autogen/` 下——`moc_*.cpp` 在 `EWIEGA46WW/` 之类子目录，`ui_*.h` 在 `include/`，`qrc_*.cpp` 在同一个 hash 子目录
- **发布**：`windeployqt` 复制 DLL / 插件 / 翻译；缺 `platforms/qwindows.dll` 会报“no Qt platform plugin could be initialized”
- **对应**：问答 Q12（首轮空白，待学）

---

## 9. 通关清单（能答上就算过）

| 站 | 三个必答问题 | 实验 |
| :-- | :-- | :-- |
| 1 QObject | parent 作用？删父时子对象怎样？栈对象设 parent 会怎样？ | `trace04` |
| 2 moc | Q_OBJECT 生成什么？漏写会出哪两种错？信号函数体谁写的？ | `trace01` |
| 3 信号槽 | connect/emit 各做什么？Auto 怎么判定？同槽连两次执行几次？ | `trace01`、`trace03` |
| 4 事件循环 | 事件存在哪？update 和 repaint 区别？界面为什么卡？ | `trace02` |
| 5 线程 | moveToThread 条件？直接调用在哪个线程？没 exec 会怎样？ | `trace03` |
| 6 生命周期 | 谁负责删？delete vs deleteLater？为什么要 wait？ | `trace04` |
| 7 Model/View | View 会不会轮询？改数据发什么？插行调哪两个函数？ | `trace05` |

---

## 10. 资料索引（细节在这里）

| 内容 | 位置 |
| :-- | :-- |
| 23 个问答（条件 / 我的回答 / 判定 / 正确理解） | `notes/bagu/qt-核心机制问答复盘-2026-11-14.md` |
| Model/View 详解（含 3 个 role 题的讲解） | `notes/bagu/qt-model-view-2026-11-15.md` |
| 每日记录（11-14 / 11-15 / 11-16） | `notes/` |
| 实验包（本地，不在仓库） | `E:\harness\qt-trace-lab`（trace01~05 + errors） |

---

## 11. 下一步

1. 把第 7 站的两个动作亲手写一遍：`dataChanged`、`beginInsertRows/endInsertRows`
2. 补第 8 站：CMake 最小工程 + `windeployqt` 发一个 exe
3. 每天 4 题，只考“看本文能复述”的级别；答不上就写“不会”，我直接讲
