//
//
//
//
#ifndef _CIRCLE_QUEUE_
#define _CIRCLE_QUEUE_
#include <stdio.h>
#include <string.h>
#include <stdint.h>
namespace type
{
template<typename T>
class CircleQueue
{
    public:
        CircleQueue(int32_t size);
        void push(T t);
        void pop();
        T front();
        void clear();
        void PrintQueue();
        int32_t size()
        {
            return size_;
        }
        T* GetData();
        ~CircleQueue()
        {
            if (data_) delete [] data_;
        }
    private:
        void Realloc(int32_t new_size);

        T* data_;
        int32_t size_;
        int32_t capacity_;
        int32_t front_;
        int32_t end_;
};

template<typename T>
void CircleQueue<T>::Realloc(int32_t new_size)
{
    capacity_ = new_size;
    T* origin = data_;
    data_ = new T[new_size];
    if (front_ < end_) {
        // printf("front_ < end_ \n");
        memmove(data_, origin + front_, size_ * sizeof(T));
    } else if (front_ > end_) {
        // printf("front_ > end_ \n");
        memmove(data_, origin + front_, (capacity_ - front_) * sizeof(T));
        memmove(data_ + capacity_ - front_, origin, end_ * sizeof(T));
    }
    front_ = 0;
    end_ = size_;
    delete [] origin;
}
template<typename T>
CircleQueue<T>::CircleQueue(int32_t size):data_(new T[size + 10]),
    size_(0), capacity_(size + 10), front_(0), end_(0)
{
}
template<typename T>
void CircleQueue<T>::push(T t)
{
    if (size_ >= capacity_ - 1) {
        Realloc(capacity_ * 2);
    }
    data_[end_] = t;
    if (end_ >= capacity_ - 1) {
        end_ = 0;
    } else {
        ++end_;
    }
    ++size_;
}
template<typename T>
void CircleQueue<T>::pop()
{
    if (size_ <= 0) return;
    if (size_ < capacity_ / 3 && capacity_ > 15) {
        Realloc(capacity_ / 2);
    }
    if (front_ >= capacity_ - 1) {
        front_ = 0;
    } else {
        ++front_;
    }
    --size_;
}
template<typename T>
T CircleQueue<T>::front()
{
    return data_[front_];
}
template<typename T>
void CircleQueue<T>::clear()
{
    front_ = 0;
    size_ = 0;
    end_ = 0;
}
template<typename T>
void CircleQueue<T>::PrintQueue()
{
    printf("front_: %d, end_: %d, capacity_: %d, size_: %d\n",
            front_, end_, capacity_, size_);
    for (int i = 0; i < capacity_; ++i) {
        printf("[%d]:%d ", i, data_[i]);
    }
    printf("\n");
}
template<typename T>
T* CircleQueue<T>::GetData()
{
    Realloc(capacity_);
    return data_;
}
};
#endif
