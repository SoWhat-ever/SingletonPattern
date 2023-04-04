#include <iostream>

using namespace std;

/**
 * @brief 有缺陷的懒汉模式
 * 问题：
 * 1. 线程不安全
 * 2. 内存泄露
 */

class  SingletonPattern_v1 {
public:
    ~SingletonPattern_v1() {
        cout << "Destructor called!" << endl;
    }
    // 实例化
    static SingletonPattern_v1* Instance() {
        if(!m_instance) {
            m_instance = new SingletonPattern_v1();
        }
        return m_instance;
    }
private:
    SingletonPattern_v1() {
        cout << "Constructor called!" << endl;
    }
    SingletonPattern_v1(SingletonPattern_v1&) = delete;
    SingletonPattern_v1& operator=(const SingletonPattern_v1&) = delete;

    static SingletonPattern_v1* m_instance;
};

// 类外初始化静态变量
SingletonPattern_v1* SingletonPattern_v1::m_instance = nullptr;

// 函数入口
int main() {
    //测试
	SingletonPattern_v1* p1 = SingletonPattern_v1::Instance();
	SingletonPattern_v1* p2 = SingletonPattern_v1::Instance();
 
	// system("pause");
	return 0;
}

/**
 * @details
 * 执行结果是 Constructor called!
 * 获取两次类的实例，构造函数只用了一次，只生成一个实例
 * 
 * 线程安全：
 * 当多线程获取单例时有可能引发竞争：第一个线程在if中判断 m_pInstance是空的，于是开始实例化单例;
 * 同时第2个线程也尝试获取单例，这个时候判断m_pInstance还是空的，于是也开始实例化单例;
 * 这样就会实例化出两个对象,线程不安全; 
 * 解决办法: 加锁
 * 
 * 内存泄漏：
 * 类中只负责new出对象，却没有负责delete对象；因此只有构造函数被调用，析构函数却没有被调用；导致内存泄漏。
 * 解决办法：
 * 可以手动调用delete来进行释放，但是维护在何处释放又成了问题。
 * 正确解决办法：使用智能指针;
 */