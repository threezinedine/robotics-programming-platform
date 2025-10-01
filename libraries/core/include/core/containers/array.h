#pragma once
#include "platforms/platforms.h"
#include <string>
#include <stdexcept>

#define RPP_ARRAY_DEFAULT_CAPACITY 2

namespace rpp
{
    /**
     * @brief Self-defining dynamic array class. The array will be resized automatically when needed.
     */
    template <typename T>
    class Array
    {
    public:
        /**
         * @brief Default constructor for empty array.
         */
        Array()
        {
            m_capacity = RPP_ARRAY_DEFAULT_CAPACITY;
            m_data = (T *)RPP_MALLOC(m_capacity * sizeof(T));
            m_size = 0;
        }

        /**
         * @brief Constructor with initial capacity.
         * @param capacity Initial capacity of the array.
         * @note The capacity must be greater than zero.
         */
        Array(u32 capacity)
        {
            m_capacity = capacity;
            m_data = (T *)RPP_MALLOC(m_capacity * sizeof(T));
            m_size = 0;
        }

        /**
         * @brief Copy constructor. Performs a deep copy of the other array.
         */
        Array(const Array &other)
        {
            m_capacity = other.m_capacity;
            m_size = other.m_size;
            m_data = (T *)RPP_MALLOC(m_capacity * sizeof(T));
            for (u32 i = 0; i < m_size; i++)
            {
                RPP_NEW_REPLACE(&other.m_data[i], T(other.m_data[i]));
            }
        }

        ~Array()
        {
            if (m_data != nullptr)
            {
                Clear();
                RPP_FREE(m_data);
                m_data = nullptr;
            }
        }

    public:
        /**
         * @brief Get the current size of the array.
         * @return Current size of the array.
         */
        inline u32 Size() const { return m_size; }

        /**
         * @brief Get the current capacity of the array.
         * @return Current capacity of the array.
         */
        inline u32 Capacity() const { return m_capacity; }

        /**
         * @brief Access operator for the array.
         * @param index Index of the element to access. If the index is out of bounds, an exception will be thrown.
         */
        T &operator[](u32 index)
        {
            if (index >= m_size)
            {
                throw std::runtime_error("Array index out of bounds");
            }

            return m_data[index];
        }

        /**
         * @brief Const access operator for the array (for read-only access).
         * @param index Index of the element to access. If the index is out of bounds, an exception will be thrown.
         */
        T operator[](u32 index) const
        {
            if (index >= m_size)
            {
                throw std::runtime_error("Array index out of bounds");
            }

            return m_data[index];
        }

        /**
         * @brief Resize the array to the new capacity (not number of elements).
         * @param newCapacity New capacity of the array. The new capacity must be greater than the current size else
         *      an exception will be thrown.
         */
        void Reallocate(u32 newCapacity)
        {
            if (newCapacity <= m_size)
            {
                throw std::runtime_error("New capacity must be greater than current size");
            }

            T *newData = (T *)RPP_MALLOC(newCapacity * sizeof(T));

            for (u32 i = 0; i < m_size; i++)
            {
                RPP_NEW_REPLACE(&newData[i], T(std::move(m_data[i])));
            }

            for (u32 i = 0; i < m_size; i++)
            {
                m_data[i].~T();
            }

            RPP_FREE(m_data);
            m_data = newData;
            m_capacity = newCapacity;
        }

        /**
         * @brief Add an element to the end of the array. The array will be resized if needed.
         * @param value Value to add to the array.
         * @param index Optional index to insert the value at. If the index is -1 (default), the value will be added to the end of the array.
         *      If the index is not -1 or greater than the current size of the array, an exception will be thrown.
         */
        void Push(const T &value, i32 index = -1)
        {
            if (index != -1 && (index < 0 || index > static_cast<i32>(m_size)))
            {
                throw std::runtime_error("Invalid index, must be -1 or less than than the size of the size of the array");
            }

            if (m_size >= m_capacity)
            {
                Reallocate(m_capacity * 2);
            }

            u32 modifiedIndex = 0;
            if (index == -1)
            {
                modifiedIndex = m_size;
            }
            else
            {
                modifiedIndex = index;
            }
            m_size++;

            for (u32 i = m_size - 1; i >= u32(modifiedIndex) + 1; i--)
            {
                RPP_NEW_REPLACE(&m_data[i], T(std::move(const_cast<T &>(m_data[i - 1]))));
            }

            RPP_NEW_REPLACE(&m_data[modifiedIndex], T(value));
        }

        void Push(T &&value, i32 index = -1)
        {
            if (index != -1 && (index < 0 || index > static_cast<i32>(m_size)))
            {
                throw std::runtime_error("Invalid index, must be -1 or less than than the size of the size of the array");
            }

            if (m_size >= m_capacity)
            {
                Reallocate(m_capacity * 2);
            }

            u32 modifiedIndex = 0;
            if (index == -1)
            {
                modifiedIndex = m_size;
            }
            else
            {
                modifiedIndex = index;
            }
            m_size++;

            for (u32 i = m_size - 1; i >= u32(modifiedIndex) + 1; i--)
            {
                RPP_NEW_REPLACE(&m_data[i], T(std::move(const_cast<T &>(m_data[i - 1]))));
            }

            RPP_NEW_REPLACE(&m_data[modifiedIndex], T(std::move(value)));
        }

        /**
         * @brief Erase the element at the specified index. The size of the array will be decreased by one.
         * @param index Index of the element to erase. If the index is out of bounds
         *     an exception will be thrown.
         */
        void Erase(i32 index = -1)
        {
            if ((index < 0 || index >= static_cast<i32>(m_size)) && index != -1)
            {
                throw std::runtime_error("Invalid index, must be between 0 and size-1 of the array or -1");
            }

            u32 modifiedIndex = 0;
            if (index == -1)
            {
                modifiedIndex = m_size - 1;
            }
            else
            {
                modifiedIndex = index;
            }

            m_size--;

            m_data[modifiedIndex].~T();
            for (u32 i = modifiedIndex; i < m_size; i++)
            {
                RPP_NEW_REPLACE(&m_data[i], T(std::move(const_cast<T &>(m_data[i + 1]))));
            }
        }

        /**
         * @brief Clear the array. The size will be set to zero but the capacity will remain unchanged.
         */
        void Clear()
        {
            for (u32 i = 0; i < m_size; i++)
            {
                m_data[i].~T();
            }
            m_size = 0;
        }

        /**
         * @brief Get the pointer to the array data.
         * @return Pointer to the array data.
         */
        inline T *Data() { return m_data; }

    private:
        T *m_data = nullptr; ///< Pointer to the array data.
        u32 m_capacity = 0;  ///< Current capacity of the array. The array will be resized when the size exceeds the capacity.
        u32 m_size = 0;      ///< Current size of the array. The actual number of elements in the array.
    };
} // namespace rpp
