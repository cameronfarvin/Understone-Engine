#include <tests/tests.h>

/*
  [ cfarvin::TODO ]
  - Improve the presntly very drab and monotounous error messages
  - (better) uDAPush tests
  - (better) uDAPop tests
  - uDAFitToSize tests
  - uDAScalingFactor tests
  - uDADestroyTests
*/


#define arrayTestFailMessage "Failed dynamic array tests\n"
void runDynamicArrayTests()
{
    puts("\tRunning dynamic array tests...");

    uDynamicArray* daTest_u8 = uDAInit(u8);
    for (size_t ii = 0; ii < 0xFF; ii++)
    {
        uDAPush(daTest_u8, &ii);
        uDBGAssertMsg_v((u8)ii == *(u8*)uDAIndex(daTest_u8, ii), arrayTestFailMessage);
    }
    for (size_t ii = 0xFF; ii > 0; ii--)
    {
        size_t size_before = daTest_u8->length;
        uDAPop(daTest_u8);
        size_t size_after = daTest_u8->length;
        uDBGAssertMsg_v((size_after == (size_before - 1)), arrayTestFailMessage);
    }
    uDBGAssertMsg_v(uDADestroy(daTest_u8),
                    "Failed to deallocate for uDynamicArray u8 test");


    uDynamicArray* daTest_u16 = uDAInit(u16);
    for (size_t ii = 0; ii < 0xFFFF; ii++)
    {
        uDAPush(daTest_u16, &ii);
        uDBGAssertMsg_v((u16)ii == *(u16*)uDAIndex(daTest_u16, ii), arrayTestFailMessage);
    }
    for (size_t ii = 0xFFFF; ii > 0; ii--)
    {
        size_t size_before = daTest_u16->length;
        uDAPop(daTest_u16);
        size_t size_after = daTest_u16->length;
        uDBGAssertMsg_v((size_after == (size_before - 1)), arrayTestFailMessage);
    }
    uDBGAssertMsg_v(uDADestroy(daTest_u16),
                    "Failed to deallocate for uDynamicArray u16 test");


    uDynamicArray* daTest_u32 = uDAInit(u32);
    for (size_t ii = 0; ii < 0xFFFF; ii++)
    {
        uDAPush(daTest_u32, &ii);
        uDBGAssertMsg_v(((u32)ii == *(u32*)uDAIndex(daTest_u32, ii)), arrayTestFailMessage);
    }
    for (size_t ii = 0xFFFF; ii > 0; ii--)
    {
        size_t size_before = daTest_u32->length;
        uDAPop(daTest_u32);
        size_t size_after = daTest_u32->length;
        uDBGAssertMsg_v((size_after == (size_before - 1)), arrayTestFailMessage);
    }
    uDBGAssertMsg_v(uDADestroy(daTest_u32),
                    "Failed to deallocate for uDynamicArray u32 test");


    uDynamicArray* daTest_u64 = uDAInit(u64);
    for (size_t ii = 0; ii < 0xFFFF; ii++)
    {
        uDAPush(daTest_u64, &ii);
        uDBGAssertMsg_v(((u64)ii == *(u64*)uDAIndex(daTest_u64, ii)), arrayTestFailMessage);
    }
    for (size_t ii = 0xFFFF; ii > 0; ii--)
    {
        size_t size_before = daTest_u64->length;
        uDAPop(daTest_u64);
        size_t size_after = daTest_u64->length;
        uDBGAssertMsg_v((size_after == (size_before - 1)), arrayTestFailMessage);
    }
    uDBGAssertMsg_v(uDADestroy(daTest_u64),
                    "Failed to deallocate for uDynamicArray u64 test");


    uDynamicArray* daTest_r32 = uDAInit(r32);
    for (size_t ii = 0; ii < 0xFFFF; ii++)
    {
        r32 value = (r32)(ii - uPI);
        uDAPush(daTest_r32, (r32*)&value);
        uDBGAssertMsg_v((value == *(r32*)uDAIndex(daTest_r32, ii)), arrayTestFailMessage);
    }
    for (size_t ii = 0xFFFF; ii > 0; ii--)
    {
        size_t size_before = daTest_r32->length;
        uDAPop(daTest_r32);
        size_t size_after = daTest_r32->length;
        uDBGAssertMsg_v((size_after == (size_before - 1)), arrayTestFailMessage);
    }
    uDBGAssertMsg_v(uDADestroy(daTest_r32),
                    "Failed to deallocate for uDynamicArray r32 test");


    uDynamicArray* daTest_r64 = uDAInit(r64);
    for (size_t ii = 0; ii < 0xFFFF; ii++)
    {
        r64 value = ii - uPI;
        uDAPush(daTest_r64, &value);
        uDBGAssertMsg_v((value == *(r64*)uDAIndex(daTest_r64, ii)), arrayTestFailMessage);
    }
    for (size_t ii = 0xFFFF; ii > 0; ii--)
    {
        size_t size_before = daTest_r64->length;
        uDAPop(daTest_r64);
        size_t size_after = daTest_r64->length;
        uDBGAssertMsg_v((size_after == (size_before - 1)), arrayTestFailMessage);
    }
    uDBGAssertMsg_v(uDADestroy(daTest_r64),
                    "Failed to deallocate for uDynamicArray r64 test");
}


#define memoryArenaTestFailMessage "Failed memory arena tests\n"
void
runMemoryArenaTests()
{
    puts("\tRunning memory arena tests...");

    u16 allocation_size = 0;

    // Test allocation fail on zero size
    uMemoryArena* memory_arena_zero_alloc_test = uMAInit(allocation_size);
    uDBGAssertMsg_v((memory_arena_zero_alloc_test == NULL), memoryArenaTestFailMessage);
    // [ cfarvin::NOTE ] We *want* this uMADestroy to fail
    uDBGAssertMsg_v(!(uMADestroy(memory_arena_zero_alloc_test)),
                    "Failed to return false from uMADestroy on zero size allocation");

    // Test standard allocation/initialization
    allocation_size = 512;
    uMemoryArena* memory_arena_ptr_test = uMAInit(allocation_size);
    uDBGAssertMsg_v((memory_arena_ptr_test->data != NULL), memoryArenaTestFailMessage);
    uDBGAssertMsg_v((memory_arena_ptr_test->data == (memory_arena_ptr_test->data +
                                                     memory_arena_ptr_test->offset)),
                    memoryArenaTestFailMessage);
    uDBGAssertMsg_v((memory_arena_ptr_test->arena_size == allocation_size), memoryArenaTestFailMessage);
    uDBGAssertMsg_v(uMADestroy(memory_arena_ptr_test), "Failed to deallocate on uMemoryArena ptr test");

    // Test standard push
    allocation_size = 1;
    uMemoryArena* memory_arena_push_data_test = uMAInit(allocation_size);
    uDBGAssertMsg_v((memory_arena_push_data_test->data != NULL), memoryArenaTestFailMessage);
    uDBGAssertMsg_v((memory_arena_push_data_test->data == (memory_arena_push_data_test->data +
                                                           memory_arena_push_data_test->offset)),
                    memoryArenaTestFailMessage);
    uDBGAssertMsg_v(memory_arena_push_data_test->arena_size == allocation_size, memoryArenaTestFailMessage);
    u8 standard_test_target = 5;
    u8* standard_test_target_result = uMAPushData(memory_arena_push_data_test, standard_test_target, u8);
    uDBGAssertMsg_v((*standard_test_target_result == 5), memoryArenaTestFailMessage);
    uDBGAssertMsg_v(uMADestroy(memory_arena_push_data_test),
                    "Fialed to destroy arena on standard data push test.\n");

    // Test over-size push
    /* allocation_size = 1; */
    /* uMemoryArena* memory_arena_overflow_test = uMAInit(allocation_size); */
    /* uDBGAssertMsg_v((memory_arena_overflow_test->data != NULL), memoryArenaTestFailMessage); */
    /* uDBGAssertMsg_v((memory_arena_overflow_test->data == (memory_arena_overflow_test->data + */
    /*                                                       memory_arena_overflow_test->offset)), */
    /*                 memoryArenaTestFailMessage); */
    /* uDBGAssertMsg_v(memory_arena_overflow_test->arena_size == allocation_size, memoryArenaTestFailMessage); */
    /* u16 oversize_test_target = 500; */
    /* u16* oversize_test_target_result = uMAPushData(memory_arena_overflow_test, oversize_test_target, u16); */
    /* uDBGAssertMsg_v((oversize_test_target_result == (u16*)(memory_arena_overflow_test->data)), */
    /*                 memoryArenaTestFailMessage); */
    /* uDBGAssertMsg_v(uMADestroy(memory_arena_overflow_test), "Fialed to destroy arena on overflow test.\n"); */

    // Test push data (struct) feature
    typedef struct
    {
        int*  iptr;
        int   i;
        float f;
    } uMATestStruct;

    uMATestStruct testStruct = { NULL, 0, 0.0 };

    allocation_size = sizeof(testStruct);
    uMemoryArena* memory_arena_struct_test = uMAInit(allocation_size);
    uDBGAssertMsg_v((memory_arena_struct_test->data != NULL), memoryArenaTestFailMessage);
    uDBGAssertMsg_v((memory_arena_struct_test->data == (memory_arena_struct_test->data +
                                                        memory_arena_struct_test->offset)),
                    memoryArenaTestFailMessage);
    uDBGAssertMsg_v(memory_arena_struct_test->arena_size == allocation_size, memoryArenaTestFailMessage);
    uMATestStruct* test_struct_result = uMAPushData(memory_arena_struct_test, testStruct, uMATestStruct);
    uDBGAssertMsg_v((test_struct_result->iptr == testStruct.iptr), memoryArenaTestFailMessage);
    uDBGAssertMsg_v((test_struct_result->i == testStruct.i), memoryArenaTestFailMessage);
    uDBGAssertMsg_v((test_struct_result->f == testStruct.f), memoryArenaTestFailMessage);
    uDBGAssertMsg_v(uMADestroy(memory_arena_struct_test), "Failed to deallocate on uMemoryArena struct test");

    // Test multi-type push
    allocation_size = 336;
    uMemoryArena* memory_arena_multi_type_test = uMAInit(allocation_size);
    uDBGAssertMsg_v((memory_arena_multi_type_test->data != NULL), memoryArenaTestFailMessage);
    uDBGAssertMsg_v((memory_arena_multi_type_test->data ==
                     (memory_arena_multi_type_test->data + memory_arena_multi_type_test->offset)),
                    memoryArenaTestFailMessage);
    uDBGAssertMsg_v(memory_arena_multi_type_test->arena_size == allocation_size, memoryArenaTestFailMessage);
    u8* testStruct_u8    = uMAPushData(memory_arena_multi_type_test, testStruct.i, u8);
    u16* testStruct_u16  = uMAPushData(memory_arena_multi_type_test, testStruct.i, u16);
    u32* testStruct_u32  = uMAPushData(memory_arena_multi_type_test, testStruct.i, u32);
    u64* testStruct_u64  = uMAPushData(memory_arena_multi_type_test, testStruct.i, u64);
    s8* testStruct_s8    = uMAPushData(memory_arena_multi_type_test, testStruct.i, s8);
    s16* testStruct_s16  = uMAPushData(memory_arena_multi_type_test, testStruct.i, s16);
    s32* testStruct_s32  = uMAPushData(memory_arena_multi_type_test, testStruct.i, s32);
    s64* testStruct_s64  = uMAPushData(memory_arena_multi_type_test, testStruct.i, s64);
    r32* testStruct_r32  = uMAPushData(memory_arena_multi_type_test, testStruct.i, r32);
    r64* testStruct_r64  = uMAPushData(memory_arena_multi_type_test, testStruct.i, r64);
    uDBGAssertMsg_v((*testStruct_u8  == (u8)  testStruct.i), memoryArenaTestFailMessage);
    uDBGAssertMsg_v((*testStruct_u16 == (u16) testStruct.i), memoryArenaTestFailMessage);
    uDBGAssertMsg_v((*testStruct_u32 == (u32) testStruct.i), memoryArenaTestFailMessage);
    uDBGAssertMsg_v((*testStruct_u64 == (u64) testStruct.i), memoryArenaTestFailMessage);
    uDBGAssertMsg_v((*testStruct_s8  == (s8)  testStruct.i), memoryArenaTestFailMessage);
    uDBGAssertMsg_v((*testStruct_s16 == (s16) testStruct.i), memoryArenaTestFailMessage);
    uDBGAssertMsg_v((*testStruct_s32 == (s32) testStruct.i), memoryArenaTestFailMessage);
    uDBGAssertMsg_v((*testStruct_s64 == (s64) testStruct.i), memoryArenaTestFailMessage);
    uDBGAssertMsg_v((*testStruct_r32 == (r32) testStruct.i), memoryArenaTestFailMessage);
    uDBGAssertMsg_v((*testStruct_r64 == (r64) testStruct.i), memoryArenaTestFailMessage);
    uDBGAssertMsg_v(uMADestroy(memory_arena_multi_type_test),
                    "Failed to deallocate on uMemoryArena multi-type test");

    // Test array push
    uMemoryArena* memory_arena_array_test = uMAInit(256);
    uDBGAssertMsg_v((memory_arena_array_test), memoryArenaTestFailMessage);
    uDBGAssertMsg_v((memory_arena_array_test->data), memoryArenaTestFailMessage);
    uDBGAssertMsg_v((memory_arena_array_test->offset == 0), memoryArenaTestFailMessage);
    uDBGAssertMsg_v((memory_arena_array_test->arena_size == 256), memoryArenaTestFailMessage);

    u8 test_array[256];
    for (size_t ii = 0; ii < 256; ii++)
    {
        test_array[ii] = (u8) ii;
    }
    u8* test_array_result =
        uMAPushArray(memory_arena_array_test, &test_array, u8, 256);
    for (size_t ii = 0; ii < 256; ii++)
    {
        uDBGAssertMsg_v((test_array_result[ii] == test_array[ii] &&
                         test_array[ii] == (u8)ii),
                        memoryArenaTestFailMessage);
    }
    uDBGAssertMsg_v(uMADestroy(memory_arena_array_test),
                    "Failed to deallocate on uMemoryArena array test");
}

void
runAllTests()
{
    puts("[ TESTING ] Running All Tests...");
    runDynamicArrayTests();
    runMemoryArenaTests();
    puts("[ TESTING::SUCCESS ]");
}
