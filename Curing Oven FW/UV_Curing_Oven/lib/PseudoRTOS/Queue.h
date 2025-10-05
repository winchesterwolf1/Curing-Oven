#ifndef PSEUDORTOS_QUEUE_H
#define PSEUDORTOS_QUEUE_H

class QueueBase
{
    public:
        QueueBase() : 
            head_(0), tail_(0), size_(0) 
        {

        }

        virtual bool Add(void* item) = 0;
        virtual bool Read(void* item) = 0;
        virtual bool IsFull() const = 0;
        virtual unsigned int GetCapacity() const = 0;

        bool IsEmpty() const { return size_ == 0; }
        unsigned int NumItems() const { return size_; }

    protected:
        unsigned int head_;
        unsigned int tail_;
        unsigned int size_;
};

/// @brief Queue class to use for communications between tasks
/// @tparam T type used for the queue buffer
/// @tparam Capacity max capacity of the queue buffer
template<typename T, unsigned int Capacity>
class Queue : public QueueBase 
{
    public:
        Queue() : QueueBase()
        {

        }

        bool Add(void* item) override
        {
            if (size_ == Capacity)
            {
                return false;
            }

            buffer_[tail_] = *(static_cast<T*>(item));
            tail_ = (tail_ + 1) % Capacity;
            ++size_;

            return true;
        }

        bool Read(void* item) override
        {
            if (size_ == 0)
            {
                return false;
            }

            *(T*)item = buffer_[head_];
            head_ = (head_ + 1) % Capacity;
            --size_;

            return true;
        }

        bool IsFull() const override 
        { 
            return size_ == Capacity; 
        }

        unsigned int GetCapacity() const override 
        { 
            return Capacity; 
        }

    private:
        T buffer_[Capacity];
};

#endif // PSEUDORTOS_QUEUE_H