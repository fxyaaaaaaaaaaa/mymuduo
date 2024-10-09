#pragma once

//用于作为父类 防止派生类进行拷贝构造 和 赋值运算符操作
class  noncopyable
{
public:
    noncopyable(const noncopyable&)=delete;
    noncopyable& operator=(const noncopyable&)=delete;
protected:
    noncopyable()=default;
    ~noncopyable()=default;
};