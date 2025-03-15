#ifndef _SINGLETON_
#define _SINGLETON_
#include <mutex>
#include <memory>

template <typename T>
class Singleton {
public:
    // 删除拷贝构造函数和赋值运算符
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    // 获取单例实例
    static T& getInstance() {
        static std::once_flag initFlag;
        std::call_once(initFlag, []() {
            _instance.reset(new T());
            });
        return *_instance;
    }

protected:
    // 构造函数和析构函数为 protected，防止外部直接实例化
    Singleton() = default;
    ~Singleton() = default;

private:
    static std::unique_ptr<T> _instance; // 单例实例
};

template <typename T>
std::unique_ptr<T> Singleton<T>::_instance = nullptr;

#endif 