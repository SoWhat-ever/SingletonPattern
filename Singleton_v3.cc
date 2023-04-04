#include <iostream>
using namespace std;

/**
 * @brief 推荐使用的懒汉模式
 * 使用magic static 解决了v2的问题
 * 1. 代码简洁 无智能指针调用
 * 2. 也没有双重检查锁定模式的风险
 */

class SingletonPattern_v3
{
public:
	~SingletonPattern_v3() {
		std::cout << "Destructor called!" << std::endl;
	}
	SingletonPattern_v3(const SingletonPattern_v3&) = delete;
	SingletonPattern_v3& operator=(const SingletonPattern_v3&) = delete;
	static SingletonPattern_v3& Instance() {
		static SingletonPattern_v3 m_instance;
		return m_instance; 
	}
private:
	SingletonPattern_v3() {
		std::cout << "Constructor called!" << std::endl;
	}
};
 
int main() {
	SingletonPattern_v3& instance_1 = SingletonPattern_v3::Instance();
    SingletonPattern_v3& instance_2 = SingletonPattern_v3::Instance();

    system("pause");
	return 0;
}

/**
 * @details
 * 执行结果是 Constructor called! Destructor called!
 * 
 * 魔法静态变量(magic static)是C++11的核心语言功能特性
 * 如果当变量在初始化的时候，并发同时进入声明语句，并发线程将会阻塞等待初始化结束。
 * 这样保证了并发线程在获取静态局部变量的时候一定是初始化过的，所以具有线程安全性。
 * 
 * 这是最推荐的一种单例实现方式：
 * 通过局部静态变量的特性保证了线程安全
 * 不需要使用共享指针，代码简洁；不需要使用互斥锁。
 * 注意在使用的时候需要声明单例的引用 SingletonPattern_v3& 才能获取对象。
 */