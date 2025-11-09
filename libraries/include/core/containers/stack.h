#pragma once
#include "list.h"
#include "platforms/platforms.h"

namespace rpp
{
    template <typename T>
    class Stack
    {
    public:
        Stack()
            : m_list()
        {
        }

        ~Stack()
        {
        }

        /**
         * @brief Check if the stack is empty.
         * @return True if the stack is empty, false otherwise.
         */
        inline b8 Empty() const { return m_list.Size() == 0; }

        /**
         * @brief Get the current size of the stack.
         * @return Current size of the stack.
         */
        inline u32 Size() const { return m_list.Size(); }

        /**
         * @brief Push a new element to the top of the stack.
         * @param value The value to be pushed onto the stack.
         */
        inline void Push(const T &value) { m_list.Push(value); }

        /**
         * @brief Push a new element to the top of the stack using move semantics.
         * @param value The value to be pushed onto the stack.
         */
        inline void Push(T &&value) { m_list.Push(std::move(value)); }

        /**
         * @brief Pop the top element from the stack.
         */
        inline void Pop()
        {
            if (m_list.Size() == 0)
            {
                throw std::runtime_error("Stack is empty");
            }
            m_list.Erase(m_list.Size() - 1);
        }

        /**
         * @brief Get the top element of the stack.
         * @return Reference to the top element of the stack.
         */
        const T &Top() const
        {
            if (m_list.Size() == 0)
            {
                throw std::runtime_error("Stack is empty");
            }
            return m_list[m_list.Size() - 1];
        }

        /**
         * @brief Get the top element of the stack.
         * @return Reference to the top element of the stack.
         */
        T &Top()
        {
            if (m_list.Size() == 0)
            {
                throw std::runtime_error("Stack is empty");
            }
            return m_list[m_list.Size() - 1];
        }

    private:
        List<T> m_list; /// The underlying list to store stack elements.
    };
} // namespace rpp
