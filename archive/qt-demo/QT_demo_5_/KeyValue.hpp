#ifndef KEYVALUE_HPP
#define KEYVALUE_HPP
#include <iostream>
using namespace std;
template <typename K, typename V>
class KeyValue
{
private:
    K Key;
    V Val;
public:
    KeyValue(K k, V v) : Key(k),Val(v) {};   //±ðÍü¼Ç{}
    void show()
    {
         cout << "¼ü£º" << Key << " | Öµ£º" << Val << endl;
    }
};
#endif // KEYVALUE_HPP
