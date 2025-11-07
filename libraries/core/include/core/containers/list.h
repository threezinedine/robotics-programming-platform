#pragma once
#include "platforms/platforms.h"

namespace rpp
{
    /**
     * @brief A simple linked list implementation. The list allows for dynamic insertion and deletion of elements.
     */
    template <typename T>
    class List
    {
    private:
        /**
         * internal data for each element inside the list.
         */
        struct Node
        {
            T data;                ///< The data stored in the node.
            Node *pNext = nullptr; ///< Pointer to the next node in the list.
        };

    public:
        List()
            : m_pHead(nullptr), m_pTail(nullptr), m_size(0)
        {
        }

        ~List()
        {
            Clear();
        }

        /**
         * @brief Get the current size of the list.
         * @return Current size of the list.
         */
        u32 Size() const
        {
            return m_size;
        }

        /**
         * @brief Insert a new element at the end of the list.
         * @param value The value to be inserted.
         * @param index The index at which to insert the new element. Default is -1, which means to insert at the end.
         */
        void Push(const T &value, i32 index = -1)
        {
            Node *newNode = RPP_NEW(Node);
            m_size++;
            newNode->data = T(value);

            u32 modifiedIndex = 0;

            if (m_pTail == nullptr)
            {
                if (index != -1 && index != 0)
                {
                    throw std::runtime_error("Invalid index, must be 0 when the list is empty or -1");
                }

                m_pTail = newNode;
                m_pHead = newNode;
                return;
            }

            if (index == -1)
            {
                modifiedIndex = m_size - 1;
            }
            else if (index < 0 || index >= static_cast<i32>(m_size))
            {
                throw std::runtime_error("Invalid index, must be between 0 and size-1 of the list or -1");
            }
            else
            {
                modifiedIndex = index;
            }

            Node *current = m_pHead;
            Node *previous = nullptr;
            for (u32 i = 0; i < modifiedIndex; ++i)
            {
                previous = current;
                current = current->pNext;
            }

            newNode->pNext = current;

            if (previous)
            {
                previous->pNext = newNode;
            }
            else
            {
                m_pHead = newNode;
            }
        }

        void Push(T &&value, i32 index = -1)
        {
            Node *newNode = RPP_NEW(Node);
            m_size++;
            newNode->data = std::move(value);

            u32 modifiedIndex = 0;

            if (m_pTail == nullptr)
            {
                if (index != -1 && index != 0)
                {
                    throw std::runtime_error("Invalid index, must be 0 when the list is empty or -1");
                }

                m_pTail = newNode;
                m_pHead = newNode;
                return;
            }

            if (index == -1)
            {
                modifiedIndex = m_size - 1;
            }
            else if (index < 0 || index >= static_cast<i32>(m_size))
            {
                throw std::runtime_error("Invalid index, must be between 0 and size-1 of the list or -1");
            }
            else
            {
                modifiedIndex = index;
            }

            Node *current = m_pHead;
            Node *previous = nullptr;
            for (u32 i = 0; i < modifiedIndex; ++i)
            {
                previous = current;
                current = current->pNext;
            }

            newNode->pNext = current;

            if (previous)
            {
                previous->pNext = newNode;
            }
            else
            {
                m_pHead = newNode;
            }
        }

        /**
         * @brief Access an element by its index.
         * @param index The index of the element to access.
         * @return Reference to the element at the specified index.
         */
        T &operator[](u32 index)
        {
            Node *current = m_pHead;
            for (u32 i = 0; i < index; ++i)
            {
                if (current)
                {
                    current = current->pNext;
                }
                else
                {
                    throw std::out_of_range("Index out of range");
                }
            }
            if (current)
            {
                return current->data;
            }
            else
            {
                throw std::out_of_range("Index out of range");
            }
        }

        /**
         * @brief Erase the element at the specified index. The size of the list will be decreased by one.
         * @param index Index of the element to erase. If the index is out of bounds, an exception will be thrown.
         * If index is -1, the last element will be removed.
         */
        void Erase(i32 index = -1)
        {
            if (m_pTail == nullptr)
            {
                throw std::runtime_error("List is empty");
            }

            u32 deletedIndex = 0;

            if (index == -1)
            {
                deletedIndex = m_size - 1;
            }
            else if (index < 0 || index >= static_cast<i32>(m_size))
            {
                throw std::runtime_error("Invalid index, must be between 0 and size-1 of the list or -1");
            }
            else
            {
                deletedIndex = index;
            }

            Node *current = m_pHead;
            Node *previous = nullptr;
            for (u32 i = 0; i < deletedIndex; ++i)
            {
                previous = current;
                current = current->pNext;
            }

            if (previous)
            {
                previous->pNext = current->pNext;
            }
            else
            {
                m_pHead = current->pNext;
            }

            RPP_DELETE(current);
            m_size--;
        }

        /**
         * @brief Clear the list and free all allocated memory. The size will be set to zero.
         */
        void Clear()
        {
            if (m_pHead == nullptr)
            {
                return;
            }

            Node *current = m_pHead;
            while (current)
            {
                Node *next = current->pNext;
                RPP_DELETE(current);
                current = next;
            }

            m_pHead = nullptr;
            m_pTail = nullptr;
            m_size = 0;
        }

    private:
        Node *m_pHead = nullptr; ///< Pointer to the head of the list.
        Node *m_pTail = nullptr; ///< Pointer to the tail of the list.
        u32 m_size = 0;          ///< Current size of the list.
    };
} // namespace rpp
