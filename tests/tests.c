#include <tests/tests.h>
#include <inttypes.h>

/*
  [ cfarvin::TODO ]
  # (better) uDAPush tests
  # (better) uDAPop tests
  # uDAFitToSize tests
  # uDAScalingFactor tests
  # uDADestroyTests
*/

void runDynamicArrayTests()
{
    puts("\tRunning dynamic array tests...");

    uDynamicArray* daTest_u8 = uDAInit(u8);
    for (size_t ii = 0; ii < 0xFF; ii++)
    {
        uDAPush(daTest_u8, &ii);
        assert( (u8)ii == *(u8*)uDAIndex(daTest_u8, ii) );
    }
    for (size_t ii = 0xFF; ii > 0; ii--)
    {
        size_t size_before = daTest_u8->length;
        uDAPop(daTest_u8);
        size_t size_after = daTest_u8->length;
        assert(size_after == (size_before - 1));
    }
    uDADestroy(daTest_u8);


    uDynamicArray* daTest_u16 = uDAInit(u16);
    for (size_t ii = 0; ii < 0xFFFF; ii++)
    {
        uDAPush(daTest_u16, &ii);
        assert( (u16)ii == *(u16*)uDAIndex(daTest_u16, ii) );
    }
    for (size_t ii = 0xFFFF; ii > 0; ii--)
    {
        size_t size_before = daTest_u16->length;
        uDAPop(daTest_u16);
        size_t size_after = daTest_u16->length;
        assert(size_after == (size_before - 1));
    }
    uDADestroy(daTest_u16);


    uDynamicArray* daTest_u32 = uDAInit(u32);
    for (size_t ii = 0; ii < 0xFFFF; ii++)
    {
        uDAPush(daTest_u32, &ii);
        assert( (u32)ii == *(u32*)uDAIndex(daTest_u32, ii) );
    }
    for (size_t ii = 0xFFFF; ii > 0; ii--)
    {
        size_t size_before = daTest_u32->length;
        uDAPop(daTest_u32);
        size_t size_after = daTest_u32->length;
        assert(size_after == (size_before - 1));
    }
    uDADestroy(daTest_u32);


    uDynamicArray* daTest_u64 = uDAInit(u64);
    for (size_t ii = 0; ii < 0xFFFF; ii++)
    {
        uDAPush(daTest_u64, &ii);
        assert( (u64)ii == *(u64*)uDAIndex(daTest_u64, ii) );
    }
    for (size_t ii = 0xFFFF; ii > 0; ii--)
    {
        size_t size_before = daTest_u64->length;
        uDAPop(daTest_u64);
        size_t size_after = daTest_u64->length;
        assert(size_after == (size_before - 1));
    }
    uDADestroy(daTest_u64);


    uDynamicArray* daTest_r32 = uDAInit(r32);
    for (size_t ii = 0; ii < 0xFFFF; ii++)
    {
        r32 value = (r32)(ii - uPI);
        uDAPush(daTest_r32, (r32*)&value);
        assert( value == *(r32*)uDAIndex(daTest_r32, ii) );
    }
    for (size_t ii = 0xFFFF; ii > 0; ii--)
    {
        size_t size_before = daTest_r32->length;
        uDAPop(daTest_r32);
        size_t size_after = daTest_r32->length;
        assert(size_after == (size_before - 1));
    }
    uDADestroy(daTest_r32);


    uDynamicArray* daTest_r64 = uDAInit(r64);
    for (size_t ii = 0; ii < 0xFFFF; ii++)
    {
        r64 value = ii - uPI;
        uDAPush(daTest_r64, &value);
        assert( value == *(r64*)uDAIndex(daTest_r64, ii) );
    }
    for (size_t ii = 0xFFFF; ii > 0; ii--)
    {
        size_t size_before = daTest_r64->length;
        uDAPop(daTest_r64);
        size_t size_after = daTest_r64->length;
        assert(size_after == (size_before - 1));
    }
    uDADestroy(daTest_r64);
}

void
runMemoryArenaTests()
{
    size_t allocation_size = 0;

    puts("\tRunning memory arena tests...");
    uMemoryArena* memory_arena_zero_alloc_test = uInitMemoryArena(allocation_size);
    assert(memory_arena_zero_alloc_test == NULL);

    allocation_size = 512;
    uMemoryArena* memory_arena_ptr_test = uInitMemoryArena(allocation_size);
    assert(memory_arena_ptr_test != NULL);
    assert(memory_arena_ptr_test->data != NULL);
    assert(memory_arena_ptr_test->data == memory_arena_ptr_test->data_ptr);
    assert(memory_arena_ptr_test->arena_size == allocation_size);
}

void
runAllTests()
{
    puts("[ TESTING ] Running All Tests...");
    runDynamicArrayTests();
    runMemoryArenaTests();
    puts("[ TESTING::SUCCESS ]");
}
