#include "test_common.h"
#include "modules/storage.h"

namespace
{
    /**
     * Used only inside the project for testing the storage functionality.
     */
    class TestObject
    {
    public:
        static u32 count;                 ///< Should be reset each time before the test starts.
        static u32 constructorCallCounts; ///< Counts how many times the constructor was called.

    public:
        TestObject()
            : TestObject(0)
        {
        }

        TestObject(u32 v) : value(v)
        {
            count++;
            constructorCallCounts++;
        }

        ~TestObject()
        {
            count--;
        }

    public:
        u32 value; ///< Just a test value.
    };

    u32 TestObject::count = 0;
    u32 TestObject::constructorCallCounts = 0;
} // namespace

class StorageTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        TestObject::count = 0; // Reset the count before each test.
    }

    void TearDown() override
    {
        // Any cleanup code can go here.
    }
};

TEST_F(StorageTest, SimpleObjectLifetime)
{
    {
        rpp::Storage<TestObject> storage;
        EXPECT_EQ(TestObject::count, 0);

        EXPECT_EQ(storage.Create(), 0); // Create an object, should return 0 as the first id.
        EXPECT_EQ(TestObject::count, 1);
        EXPECT_EQ(TestObject::constructorCallCounts, 1);
        EXPECT_EQ(storage.Get(0)->value, 0); // The default value should be 0.

        EXPECT_EQ(storage.Create(42), 1); // Create another object with value 42, should return 1 as the second id.
        EXPECT_EQ(TestObject::count, 2);
        EXPECT_EQ(TestObject::constructorCallCounts, 2);
        EXPECT_EQ(storage.Get(1)->value, 42); // The value should be 42.
    }

    EXPECT_EQ(TestObject::count, 0); // After the storage is destroyed, the count should be 0.
}

TEST_F(StorageTest, FreeObject)
{
    rpp::Storage<TestObject> storage;
    EXPECT_EQ(storage.Create(), 0); // Create an object, should return 0 as the first id.
    storage.Free(0);

    EXPECT_EQ(TestObject::count, 0); // After freeing the object, the count should be 0.

    // Reuse the id
    EXPECT_EQ(storage.Create(100), 0); // Create another object with value 100, should return 1 as the second id.
}