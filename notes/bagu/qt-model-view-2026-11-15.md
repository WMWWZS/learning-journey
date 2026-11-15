# 🗂 Qt Model/View · 从「改数据不刷新」讲起（2026-11-15）

> 承接《Qt 核心机制问答复盘》Q23。
> 环境：Qt 6.10.1 + MinGW 13.1；配套实验 `trace05_modelview`（本地 `qt-trace-lab`）。

---

## 0. 一句话

**Model 存数据和规则，View 负责显示和交互，Delegate 负责每个格子怎么画。View 不轮询，只等 Model 发通知。**

---

## 1. 先回答 Q23（改数据不通知，界面会怎样）

- **问题条件**：自定义 model 把内部数据改了，但没有 `emit dataChanged`。
- **结论**：界面**不会刷新**，继续显示旧内容；直到因为别的原因（窗口重绘、滚动、其他通知）重新要数据，才可能更新。
- **专业说法**：View 不会定时调用 `data()` 轮询，它只在需要绘制/编辑时向 Model 要数据；数据变化必须通过 `dataChanged` 等信号通知。
- **人话**：显示屏是“等通知的”，不是“主动查岗的”。
- **验证**：运行实验 `trace05_modelview`，点“改第一行，但不通知”——列表文字不动；点“改第一行，并通知”——立刻刷新。

---

## 2. 三件套的职责

| 角色 | 职责 | 典型类 | 它不负责 |
| :-- | :-- | :-- | :-- |
| **Model** | 回答“有多少行 / 每格显示什么” | `QAbstractListModel`、`QAbstractTableModel`、`QStandardItemModel` | 不画界面、不管选择 |
| **View** | 显示、滚动、选择、编辑 | `QListView`、`QTableView`、`QTreeView` | 不存数据 |
| **Delegate** | 每个格子怎么画、怎么编辑 | `QStyledItemDelegate` | 不做布局 |

> 关系：View 拿着 Model 的指针，显示时向它要数据；Model 不知道 View 长什么样（一个 Model 可以同时被多个 View 显示）。

---

## 3. 一次显示的数据流

```text
View 要画某一行
  → model->rowCount()                         // 一共多少行
  → model->data(index(row), Qt::DisplayRole)  // 这一行显示什么文字
  → delegate 按 role 画到屏幕上
```

两个核心概念：

- **index（坐标）**：`QModelIndex` 是 `(行, 列, 父)`。列表模型只有“行”，列固定 0；无效索引用 `QModelIndex()` 表示“没有”。
- **role（问法）**：同一个格子，View 会换不同“问法”来问 Model：

| role | 问什么 |
| :-- | :-- |
| `Qt::DisplayRole` | 显示的文字 |
| `Qt::DecorationRole` | 图标 |
| `Qt::ForegroundRole` | 文字颜色 |
| `Qt::TextAlignmentRole` | 对齐方式 |
| 自定义 role（`Qt::UserRole + n`） | 业务数据（排序键、ID 等） |

`data()` 要按 role 分别回答；返回空 `QVariant` 表示“这个 role 我没有”。

---

## 4. 最小可用列表模型（可直接跑）

### model.h

```cpp
#pragma once

#include <QAbstractListModel>
#include <QStringList>

class StringListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit StringListModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index,
                  int role = Qt::DisplayRole) const override;

    void changeWithoutNotify(int row, const QString &text);
    void changeWithNotify(int row, const QString &text);

private:
    QStringList m_items;
};
```

### model.cpp（核心实现）

```cpp
StringListModel::StringListModel(QObject *parent)
    : QAbstractListModel(parent)
    , m_items{QStringLiteral("第一行"), QStringLiteral("第二行"), QStringLiteral("第三行")}
{
}

int StringListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())   // 列表模型：只有根节点下面有数据
        return 0;
    return m_items.size();
}

QVariant StringListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_items.size())
        return {};
    if (role == Qt::DisplayRole)
        return m_items.at(index.row());
    return {};
}

void StringListModel::changeWithoutNotify(int row, const QString &text)
{
    m_items[row] = text;                       // 数据变了，但 View 不知道
}

void StringListModel::changeWithNotify(int row, const QString &text)
{
    m_items[row] = text;
    const QModelIndex idx = index(row);
    emit dataChanged(idx, idx, {Qt::DisplayRole});   // 通知 View
}
```

### 接上 View

```cpp
auto *model = new StringListModel(&window);
auto *view  = new QListView;
view->setModel(model);      // 这一行把 Model 和 View 接起来
```

---

## 5. 三条“必须通知”的规则

| 场景 | 必须调用 | 说明 |
| :-- | :-- | :-- |
| 改某几格的数据 | `emit dataChanged(topLeft, bottomRight, roles)` | 只重画指定范围；列表里就是同一 index 传两次 |
| 插入 / 删除行 | `beginInsertRows(...) / endInsertRows()`（成对） | 真正改数据要放在这对函数**中间** |
| 整体重建数据 | `beginResetModel() / endResetModel()` | 最省事但最粗暴：会丢选择、滚动位置、展开状态 |

补充：结构变化但想保留“持久索引”时可用 `layoutChanged`；能用细粒度信号就别用 reset。

---

## 6. 常见坑

| 现象 | 原因 | 修法 |
| :-- | :-- | :-- |
| 界面永远不更新 | 忘了 `emit dataChanged` | 补通知 |
| 程序崩溃 / 断言 | `begin*/end*` 不配对、顺序错 | 严格成对，中间再改数据 |
| 界面卡顿 | `data()` 里做耗时操作（读文件、查数据库） | `data()` 只做快速取值，耗时逻辑提前算好 |
| 选择、滚动位置丢失 | 频繁 `beginResetModel` | 用 `dataChanged` / insert/remove 等细粒度信号 |
| 显示错位、越界 | `rowCount()` 与 `data()` 的行数不一致 | 两个函数用同一份数据、同一套边界检查 |

---

## 7. 实验 `trace05_modelview` 怎么玩

```powershell
Set-ExecutionPolicy -Scope Process Bypass
.\build_direct.ps1
$env:Path = 'E:\Qt\6.10.1\mingw_64\bin;' + $env:Path
.\build_direct\trace05_modelview.exe
```

窗口里有两个按钮：

1. **“改第一行，但不通知”** —— 列表文字不变（数据其实已经变了）
2. **“改第一行，并通知”** —— 列表立刻刷新

下方 QLabel 显示 `data()` 被调用次数，可以直观看到：**View 只在需要时来问数据，不会定时轮询**。

---

## 8. 下一问（先想一想）

> 同一个格子，View 画一次时可能调用 `data()` 好几次（不同 role）。
> 如果我想让**第二行文字显示成红色**，应该实现哪个 role？
> 如果我想让第二行**按隐藏的 ID 排序**，又该用哪个 role？

提示：看看第 3 节那张 role 表。

---

## 9. 补充：3 个 role / 通知问题（讲解版）

> ⚠️ 这三题出题超纲（当时还没讲 `QSortFilterProxyModel` 和范围通知），改为直接讲解，**不计入答题记录**。

### Q2｜让第二行文字变红 → `Qt::ForegroundRole`

```cpp
QVariant StringListModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::ForegroundRole && index.row() == 1)
        return QColor(Qt::red);        // 返回 QColor 或 QBrush
    if (role == Qt::DisplayRole)
        return m_items.at(index.row());
    return {};
}
```

- 专业：颜色属于 `Qt::ForegroundRole`，返回 `QColor` / `QBrush`。
- 人话：View 换了个问法——“这行字什么颜色？”——你在这个 role 里回答。

### Q3｜隐藏 ID 排序 → 自定义 role + 代理的 `sortRole`

```cpp
enum ItemRole { IdRole = Qt::UserRole + 1 };

// data() 里为 IdRole 返回编号
if (role == IdRole)
    return m_items.at(index.row()).id;

// 排序交给代理：告诉它按 IdRole 排，而不是按显示文字排
auto *proxy = new QSortFilterProxyModel(this);
proxy->setSourceModel(model);
proxy->setSortRole(IdRole);
view->setModel(proxy);
```

- 专业：`Qt::UserRole + n` 之后都是自定义 role，用来携带业务数据；`QSortFilterProxyModel::setSortRole()` 指定排序依据。
- 人话：给每行发一张“隐藏身份证”，排序时按身份证号排，不按显示文字排。

### Q4｜一次改 2~5 行怎么通知 → `dataChanged` 的 topLeft / bottomRight

```cpp
// 0 基索引：第 2~5 行 = index(1) .. index(4)
emit dataChanged(index(1), index(4), {Qt::DisplayRole});
```

- 专业：`topLeft` 是左上角索引、`bottomRight` 是右下角索引，两端都包含；`roles` 指明哪些 role 变了，只传 `DisplayRole` 就只重画文字。
- 人话：告诉 View“第 1 到第 4 行（含）变了，只重画这几行”。

---

> 📌 出题复盘：上面三题需要「自己推理 + 没学过的 API」，不是当天能答的。以后每天 4 题只考“看笔记能复述”的级别；答不上就回“不会”，我直接讲，不追问、不扣分。
