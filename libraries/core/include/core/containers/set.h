#pragma once
#include "platforms/platforms.h"

namespace rpp
{
    /**
     * @brief The comparator function type for comparing two elements.
     * @param a The element to compare (pointer inserted value).
     * @param b The incoming element to compare (pointer in array).
     * @return Returns 0 if equal, 1 if a > b, and -1 if a < b.
     */
    typedef i8 (*SetComparator)(const void *a, const void *b);

    /**
     * @brief The container for managing the list of unique elements with a specific order (customed by the comparator).
     */
    template <typename T, SetComparator Comparator>
    class Set
    {
    private:
        /**
         * Storiing need information of 1 elements
         */
        struct Node
        {
            T data;      ///< The data of the element
            Node *pNext; ///< The pointer to the next element

            Node() : pNext(nullptr) {}

            Node(const T &d, Node *n = nullptr)
                : data(d), pNext(n)
            {
            }

            Node(Node &&other) noexcept
                : data(std::move(other.data)), pNext(other.pNext)
            {
                other.pNext = nullptr;
            }

            ~Node()
            {
                pNext = nullptr;
            }
        };

    public:
        Set()
            : m_pHead(nullptr), m_size(0)
        {
        }

        ~Set()
        {
            Clear();
        }

    public:
        /**
         * @brief Get the number of elements in the set
         * @return The number of elements in the set
         */
        inline u32 Size() const { return m_size; }

    public:
        /**
         * @brief Add an element to the set.
         * This function adds an element to the set, ensuring that it is unique.
         * If the element already exists, it will not be added again.
         * @param element The element to add to the set.
         */
        void Add(const T &element)
        {
            if (m_pHead == nullptr)
            {
                Node *pNewNode = RPP_NEW(Node(T(element), nullptr));
                pNewNode->data = element;
                pNewNode->pNext = nullptr;

                m_pHead = pNewNode;
                m_size++;
                return;
            }

            Node *pCurrent = m_pHead;
            Node *pPrevious = nullptr;

            while (pCurrent)
            {
                i8 compareValue = Comparator(&pCurrent->data, &element);

                if (compareValue < 0)
                {
                    pPrevious = pCurrent;
                    pCurrent = pCurrent->pNext;
                    continue;
                }

                if (compareValue == 0)
                {
                    return;
                }

                Node *pNewNode = RPP_NEW(Node(T(element), nullptr));
                pNewNode->data = element;

                if (pPrevious == nullptr)
                {
                    pNewNode->pNext = m_pHead;
                    m_pHead = pNewNode;
                }
                else
                {
                    pNewNode->pNext = pCurrent;
                    pPrevious->pNext = pNewNode;
                }

                m_size++;
                return;
            }

            Node *pNewNode = RPP_NEW(Node(T(element), nullptr));
            pNewNode->data = element;

            if (pPrevious)
            {
                pPrevious->pNext = pNewNode;
            }

            pNewNode->pNext = nullptr;
            m_size++;
            return;
        }

        /**
         * @brief Access an element in the set by its index.
         * This function provides access to an element in the set based on its index.
         * Note that the order of elements is determined by the comparator used when adding elements.
         * @param index The index of the element to access (0-based).
         * @return A reference to the element at the specified index.
         * @throws std::out_of_range if the index is out of bounds.
         */
        T &operator[](u32 index)
        {
            if (index >= m_size)
            {
                throw std::out_of_range("Index out of range");
            }

            Node *pCurrent = m_pHead;
            for (u32 i = 0; i < index; ++i)
            {
                pCurrent = pCurrent->pNext;
            }

            return pCurrent->data;
        }

        /**
         * @brief Remove an element from the set by its index.
         * This function removes an element from the set based on its index.
         * @param index The index of the element to remove (0-based). If index = -1 removes the last element.
         * @throws std::out_of_range if the index is out of bounds.
         */
        void Remove(i32 index = -1)
        {
            if ((index >= m_size || index < 0) && index != -1)
            {
                throw std::out_of_range("Index out of range");
            }

            i32 actualIndex = index;
            if (index == -1)
            {
                actualIndex = m_size - 1;
            }

            Node *pCurrent = m_pHead;
            Node *pPrevious = nullptr;

            for (u32 i = 1; i <= actualIndex; ++i)
            {
                pPrevious = pCurrent;
                pCurrent = pCurrent->pNext;
            }

            if (pPrevious == nullptr)
            {
                m_pHead = pCurrent->pNext;
            }
            else
            {
                if (pCurrent)
                {
                    pPrevious->pNext = pCurrent->pNext;
                }
                else
                {
                    pPrevious->pNext = nullptr;
                }
            }

            RPP_DELETE(pCurrent);
            m_size--;
        }

        /**
         * @brief Clear all elements from the set.
         * This function removes all elements from the set, effectively resetting it to an empty state.
         */
        void Clear()
        {
            Node *pCurrent = m_pHead;
            while (pCurrent)
            {
                Node *pNext = pCurrent->pNext;
                RPP_DELETE(pCurrent);
                pCurrent = pNext;
            }
            m_pHead = nullptr;
            m_size = 0;
        }

    private:
        Node *m_pHead = nullptr; ///< The pointer to the head element
        u32 m_size = 0;          ///< The number of elements in the set
    };

    template <typename T>
    i8 defaultComparator(const void *a, const void *b)
    {
        if (*(T *)a == *(T *)b)
            return 0;
        else if (*(T *)a > *(T *)b)
            return 1;
        else
            return -1;
    }
} // namespace rpp
