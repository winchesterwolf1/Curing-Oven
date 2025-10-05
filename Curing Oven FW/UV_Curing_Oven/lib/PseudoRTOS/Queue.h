#ifndef PSEUDORTOS_QUEUE_H
#define PSEUDORTOS_QUEUE_H

/// @brief Base Generic abstract class for a queue
/// @attention Cast to this class when handling queue references
class QueueBase
{
    public:
        /// @brief Constructor initialises default values
        QueueBase() : 
            head_(0), tail_(0), size_(0) 
        {

        }

        /// @brief Add a queue item to the queue
        /// @param item pointer to the item to add to the queue
        /// @return true if there was enough room to add this item to the queue
        virtual bool Add(void* item) = 0;

        /// @brief Read the next item in the queue
        /// @param item pointer of the place in memory to place the received item
        /// @return false if there is no items in the queue to read
        virtual bool Read(void* item) = 0;

        /// @brief Check to see if the queue is full
        /// @return true if queue is at capacity
        virtual bool IsFull() const = 0;

        /// @brief Get the capacity of the queue
        /// @return the capacity of this queue
        virtual unsigned int GetCapacity() const = 0;

        /// @brief Check to see if this queue is empty
        /// @return true if the queue is empty
        bool IsEmpty() const { return size_ == 0; }

        /// @brief Gets the number of items currently sitting in the queue
        /// @return the number of items in the queue
        unsigned int NumItems() const { return size_; }

    protected:
        /// @brief index of the read head in the circular buffer
        unsigned int head_;
        /// @brief index of the write tail in the circuilar buffer
        unsigned int tail_;
        /// @brief number of items in the circular buffer
        unsigned int size_;
};

/// @brief Type and Capacity specific implementation of the queue class to use for communications between tasks
/// @tparam T type used for the queue buffer
/// @tparam Capacity max capacity of the queue buffer
template<typename T, unsigned int Capacity>
class Queue : public QueueBase 
{
    public:
        /// @brief Constructor used to initialise default values
        Queue() : QueueBase()
        {

        }

        /// @brief Add a queue item to the queue
        /// @param item pointer to the item to add to the queue
        /// @return true if there was enough room to add this item to the queue
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

        /// @brief Read the next item in the queue
        /// @param item pointer of the place in memory to place the received item
        /// @return false if there is no items in the queue to read
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

        /// @brief Check to see if the queue is full
        /// @return true if queue is at capacity
        bool IsFull() const override 
        { 
            return size_ == Capacity; 
        }

        /// @brief Get the capacity of the queue
        /// @return the capacity of this queue
        unsigned int GetCapacity() const override 
        { 
            return Capacity; 
        }

    private:
        /// @brief Buffer to store the queue items in
        T buffer_[Capacity];
};

#endif // PSEUDORTOS_QUEUE_H