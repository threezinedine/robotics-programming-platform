#pragma once
#include "platforms/platforms.h"
#include "list.h"

namespace rpp
{
    /**
     * @brief A simple queue implementation using a linked list.
     * The queue allows for dynamic insertion and deletion of elements in a FIFO manner.
     */
    template <typename T>
    class Queue
    {
    public:
        Queue()
            : m_list()
        {
        }

        ~Queue()
        {
        }

        /**
         * @brief Check if the queue is empty.
         * @return True if the queue is empty, false otherwise.
         */
        inline b8 Empty() const { return m_list.Size() == 0; }

        /**
         * @brief Get the current size of the queue.
         * @return Current size of the queue.
         */
        inline u32 Size() const { return m_list.Size(); }

        /**
         * @brief Push a new element to the back of the queue.
         * @param value The value to be pushed into the queue.
         */
        inline void Push(const T &value) { m_list.Push(value); }

        /**
         * @brief Push a new element to the back of the queue using move semantics.
         * @param value The value to be pushed into the queue.
         */
        inline void Push(T &&value) { m_list.Push(std::move(value)); }

        /**
         * @brief Check the front element of the queue.
         * @return Reference to the front element of the queue.
         */
        const T &Front() const
        {
            if (m_list.Size() == 0)
            {
                throw std::runtime_error("Queue is empty");
            }
            return m_list[0];
        }

        /**
         * @brief Check the front element of the queue.
         * @return Reference to the front element of the queue.
         */
        T &Front()
        {
            if (m_list.Size() == 0)
            {
                throw std::runtime_error("Queue is empty");
            }
            return m_list[0];
        }

        void Pop()
        {
            if (m_list.Size() == 0)
            {
                throw std::runtime_error("Queue is empty");
            }
            m_list.Erase(0);
        }

    private:
        List<T> m_list; ///< Internal list to store the queue elements.
    };
} // namespace rpp
