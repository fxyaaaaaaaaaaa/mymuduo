#pragma once
#include <unistd.h>
#include <sys/syscall.h>
namespace CurrentThread
{ 
    //__thread 保证每个线程都有一份属于自己的pid
    extern __thread int t_cachedTid;
    void cacheTid();
    inline int tid()
    {
        if (__builtin_expect(t_cachedTid == 0, 0))
        {
            cacheTid();
        }
        return t_cachedTid;
    }
}