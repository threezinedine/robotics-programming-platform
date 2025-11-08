#pragma once
#include "platforms/platforms.h"
#include "array.h"
#include "set.h"
#include <functional>

namespace rpp
{
    /**
     * The datatype which is specified for final deallocation of the object in the storage.
     */
    template <typename T>
    using StorageDeallocator = std::function<void(T *)>;

    /**
     * @brief The invalid id for the storage object. This id is used to indicate that the object is not found or not allocated.
     * @note This is a constant value, so it should not be changed.
     */
    constexpr u32 INVALID_ID = u32(-1);

    /**
     * Main purpose of this class is to provide the way to managing the allocated memory for the objects. Each object will be managed via `id` rather than pointer
     * (for easy to bind to Python). Other class could use this object as the attribute to store the data.
     */
    template <typename T>
    class Storage
    {
    public:
        Storage(StorageDeallocator<T> deallocator = nullptr)
            : m_deallocator(deallocator), m_elements(), m_freeIds(), m_count(0), m_capacity(0)
        {
        }

        ~Storage()
        {
            u32 size = m_elements.Size();
            for (u32 i = 0; i < size; i++)
            {
                T *object = m_elements[i];
                if (object != nullptr)
                {
                    if (m_deallocator)
                    {
                        m_deallocator(object);
                    }
                    else
                    {
                        delete object;
                    }
                }
            }
            m_elements.~Array();
        }

        /**
         * The method to allocate the memory for the object of type T and store it in the storage.
         *
         * @param args The arguments to pass to the constructor of the object of type T.
         * @return The id of the created object in the storage. The id is the indexa of the object in the storage array, starting from `0`.
         *      else `INVALID_ID` if the object could not be created.
         *
         */
        template <typename... Args>
        u32 Create(Args &&...args)
        {
            m_count++;
            if (m_freeIds.Size() == 0)
            {
                m_capacity++;
                T *object = RPP_NEW(T, std::forward<Args>(args)...);
                m_elements.Push(object);
                return m_elements.Size() - 1;
            }
            else
            {
                u32 id = m_freeIds[0];
                m_freeIds.Remove(0);
                T *object = RPP_NEW(T, std::forward<Args>(args)...);
                m_elements[id] = object;
                return id;
            }
        }

        /**
         * Retrieve the object from the storage by its id.
         */
        T *Get(u32 id) const
        {
            return m_elements[id];
        }

        /**
         * Set the deallocator function to be used when the object is removed from the storage or when the storage is destroyed.
         * The id of the deleted object can be reused for the new object.
         */
        void Free(u32 id)
        {
            m_count--;
            T *object = m_elements[id];
            if (object != nullptr)
            {
                if (id == m_capacity - 1)
                {
                    m_capacity--;
                }

                if (m_deallocator)
                {
                    m_deallocator(object);
                }
                else
                {
                    RPP_DELETE(object);
                    m_freeIds.Add(id);
                }
                m_elements[id] = nullptr;
            }
        }

        /**
         * Retrieve the number of elements in the storage (including the deleted elements).
         */
        inline u32 GetNumberOfElements() const { return m_count; }

        /**
         * Retrieve the capacity of the storage (the highest id which is currently allocated in the storage).
         *
         * TODO: Use another mechanism for looping through the storage, not the capacity like this.
         */
        inline u32 GetCapacity() const { return m_capacity; }

    private:
        Array<T *> m_elements;                      ///< The storage for the objects of type T. Each object will be deleted manually (not by Array) when the storage is destroyed.
        Set<u32, defaultComparator<u32>> m_freeIds; ///< The free ids for the objects in the storage. The id of the deleted object can be reused for the new object.
        StorageDeallocator<T> m_deallocator;        ///< The deallocator function to free the memory of the stored object.
        u32 m_count;                                ///< The number of currently allocated objects in the storage.
        u32 m_capacity;                             ///< The highest id which is currently allocated in the storage.
    };
} // namespace rpp
