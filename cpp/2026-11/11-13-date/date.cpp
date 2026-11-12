// 2026-11-13 · Date 类练习骨架
// 说明：先让下面这段能编译运行，再逐个把 TODO 补掉。
// 目标：默认/带参/拷贝构造、析构、this、const 成员、static 成员、相差天数
#include <iostream>

class Date {
public:
    Date();                                   // 默认构造：2026-11-13
    Date(int y, int m, int d);                // TODO: 校验年月日是否合法
    Date(const Date& other);                  // 拷贝构造
    ~Date();                                  // 析构函数（想想这里需要做什么？）

    void print() const;                       // const 成员函数
    int daysFrom(const Date& other) const;    // TODO: 与 other 相差多少天（想算法）

    static int created();                     // static 成员函数

private:
    int y_, m_, d_;
    static int count_;                        // 一共创建过多少个 Date
};

int Date::count_ = 0;

Date::Date() : y_(2026), m_(11), d_(13) { ++count_; }

Date::Date(int y, int m, int d) : y_(y), m_(m), d_(d) {
    // TODO: 校验 m 在 1~12、d 在该月合理范围（先写简单版）
    ++count_;
}

Date::Date(const Date& other) : y_(other.y_), m_(other.m_), d_(other.d_) {
    // 想一想：成员全是 int，编译器默认拷贝构造也能用，为什么这里还要写？
    ++count_;
}

Date::~Date() {
    // 想一想：这个析构函数必须写吗？
}

void Date::print() const {
    std::cout << y_ << "-" << m_ << "-" << d_ << "\n";
}

int Date::daysFrom(const Date& other) const {
    // TODO: 先转成"从某年起算的天数"再相减，比逐月循环简单
    (void)other;
    return 0;
}

int Date::created() { return count_; }

int main() {
    Date a;              // 默认构造
    Date b(2026, 12, 13);  // 带参构造
    Date c = a;          // 拷贝构造

    a.print();
    b.print();
    c.print();
    std::cout << "created = " << Date::created() << "\n";
    std::cout << "days between = " << a.daysFrom(b) << "（应为 -30 或 30，看你算法方向）\n";
    return 0;
}
