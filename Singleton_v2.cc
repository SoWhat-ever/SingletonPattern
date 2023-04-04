#include <iostream>
using namespace std;
#include <memory> 
#include <mutex>  

/**
 * @brief 改进的懒汉模式
 * 解决了V1的问题
 * 线程不安全: 加锁
 * 内存泄露: 使用智能指针
 */

class SingletonPattern_v2 {
public:
	~SingletonPattern_v2() {
		std::cout << "Destructor called!" << std::endl;
	}
	SingletonPattern_v2(SingletonPattern_v2&) = delete;
	SingletonPattern_v2& operator=(const SingletonPattern_v2&) = delete;
 
	//实例化
	static std::shared_ptr<SingletonPattern_v2> Instance() {
		//双重检查锁DCLP
		if (m_instance == nullptr) {
			std::lock_guard<std::mutex> lk(m_mutex);
			if (m_instance == nullptr) {
				m_instance = std::shared_ptr<SingletonPattern_v2>(new SingletonPattern_v2());
			}
		}
		return m_instance;
	}
private:
	SingletonPattern_v2() {
		std::cout << "Constructor called!" << std::endl;
	}
    // 智能指针
	static std::shared_ptr<SingletonPattern_v2> m_instance;
    // 加锁
	static std::mutex m_mutex;
};
 
//在类外初始化静态变量
std::shared_ptr<SingletonPattern_v2> SingletonPattern_v2::m_instance = nullptr;
std::mutex SingletonPattern_v2::m_mutex;
 
int main() {
	std::shared_ptr<SingletonPattern_v2> p1 = SingletonPattern_v2::Instance();
	std::shared_ptr<SingletonPattern_v2> p2 = SingletonPattern_v2::Instance();
 
	system("pause");
	return 0;
}

/**
 * @details
 * 执行结果是 Constructor called!  Destructor called!
 * 
 * 优点
 * 1. 基于 shared_ptr，基于引用计数的智能指针，在内部的析构中判断引用计数为0的时候会调用真正的delete。
 *    用对象管理资源的思想，当 shared_ptr析构的时候，new出来的对象也会被delete掉。以此避免内存泄漏。
 * 2. 加了锁，使用互斥锁来达到线程安全。
 *    这里使用了两个if判断语句的技术称为双重检测锁；
 *    好处是，只有判断指针为空的时候才加锁，避免每次调用 get_instance的方法都加锁，减少锁的开销。
 *    第二次检查是必不可少的，因为在第一次检查instance和加锁之间，可能有另一个线程对instance进行初始化。
 * 
 * 缺点
 * 1. 使用智能指针会要求外部调用也得使用智能指针，就算用typedef也是一长串代码不好维护且不美观。非必要不应该提出这种约束; 
 * 2. 使用锁也有开销; 同时代码量也增多了，实际上设计最简单的才是最好的。
 * 3. 另外，双重检测锁DCLP某种程度上也是不可靠的，原因如下：
 * 
 * instance = new Singleton;
 * 第一步：为Singleton对象分配一片内存
 * 第二步：构造一个Singleton对象，存入已分配的内存区
 * 第三步：将instance指向这片内存区
 * 编译器生成可并行执行的代码，使得处理器在一个时钟周期内能够处理两条甚至更多指令。
 * 编译器优化代码可能会交换步骤2和步骤3的执行顺序
 * 1. 线程A进入instance()，检查出instance为空，请求加锁，而后执行由步骤1和步骤3组成的语句。之后线程A被挂起。
 *    此时，instance已为非空指针，但instance指向的内存里的Singleton对象还未被构造出来。
 * 2. 线程B进入instance(), 检查出instance非空，直接将pInstance返回(return)给调用者。
 *    之后，调用者使用该返回指针去访问Singleton对象————但显然这个Singleton对象实际上还未被构造出来呢！
 */