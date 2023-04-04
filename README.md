## C++ 单例模式介绍


####  一、单例是什么

单例模式是指在内存中只会创建且仅创建一次对象的设计模式，全局有且只有一个类的static实例。在程序中多次使用同一个对象且作用相同时，**为了防止频繁地创建对象使得内存飙升**，单例模式可以让程序仅在内存中创建一个对象，让所有需要调用的地方都共享这一单例对象。



#### 二、单例模式的类型

单例模式有两种类型：

- 懒汉式：在**真正需要使用**对象时才去创建该单例类对象
- 饿汉式：在**类加载**时已经创建好该单例对象，等待被程序使用

这里给出饿汉式的实现

```c++
// singleton.h
#ifndef __SINGLETON_H__
#define __SINGLETON_H__
 
class Singleton {
public:
        static Singleton* getInstance() {
            return instance;
        }   
private:
        static Singleton* instance;
}; 

#endif

// singleton.cpp
#include "singleton.h"
Singleton* Singleton::instance = new Singleton();
```

针对线程安全的改进：如果多个线程同时进入 getInstance() 时候，实例都为空的话，则各自都会创建实例

```c++
// singleton.h
#ifndef __SINGLETON_H__
#define __SINGLETON_H__
#include <mutex>

class Singleton {
public:
        static Singleton* getInstance() {
          // 双重检查锁DCLP 
            if (instance == nullptr) {
                std::lock_guard<std::mutex> lk(m_mutex);
                if (instance == nullptr) {
                    instance = new Singleton();
                }
            }
            return instance;
        }
private:
        static Singleton* instance;
        static std::mutex m_mutex;
};
#endif

// singleton.cpp
#include "singleton.h"
Singleton* Singleton::instance = new Singleton();
std::mutex Singleton::m_mutex; 
```



#### 三、C++实现单例

一个**好的单例**应该具备下面4点

1. 全局只有一个实例：static 特性，同时禁止用户自己声明并定义实例（把构造函数设为 private）
2. 线程安全：加锁或利用语言特性
3. 禁止赋值和拷贝：删除拷贝初始化和等于运算符重载函数
4. 用户通过接口获取实例：使用 static 类成员函数



对C++ 单例的常见写法进行了一个总结， 包括

1. 懒汉式版本-- v1
2. 线程安全版本智能指针加锁--v2
3. 线程安全版本Magic Static--v3

按照从简单到复杂，最终回归简单的的方式编写，并且对各种实现方法的局限进行了简单的阐述；

大量用到了C++ 11的特性如智能指针，magic static，线程锁。
