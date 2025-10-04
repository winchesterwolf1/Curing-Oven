#ifndef PSEUDORTOS_QUEUE_H
#define PSEUDORTOS_QUEUE_H

/// @brief Queue class to use for communications between tasks
/// @tparam T type used for the queue buffer
/// @tparam Capacity max capacity of the queue buffer
template<typename T, unsigned int Capacity>
class Queue {
    public:
        Queue() :
            head_(0), tail_(0), size_(0)
        {

        }

        bool Add(T item)
        {
            if (size_ == Capacity)
            {
                return false;
            }

            buffer_[tail_] = item;
            tail_ = (tail_ + 1) % Capacity;
            ++size_;

            return true;
        }

        bool Read(T* item)
        {
            if (size_ == 0)
            {
                return false;
            }

            *item = buffer_[head_];
            head_ = (head_ + 1) % Capacity;
            --size_;

            return true;
        }

        bool IsEmpty() const { return size_ == 0; }
        bool IsFull() const { return size_ == Capacity; }
        unsigned int NumItems() const { return size_; }
        unsigned int GetCapacity() const { return Capacity; }

    private:
        T buffer_[Capacity];
        unsigned int head_;
        unsigned int tail_;
        unsigned int size_;
};

#endif // PSEUDORTOS_QUEUE_H