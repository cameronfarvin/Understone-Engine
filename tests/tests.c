#include <tests/tests.h>
#include <inttypes.h>

void
runAllTests()
{
    printf("[ TESTING ] Running All Tests...\n");

    printf("\tTesting uDynamicArray<u8>...\n");
    uDynamicArray* daTest_u8 = uDAInit(u8);
    for (size_t ii = 0; ii < 0xFF; ii++)
    {
        uDAPush(daTest_u8, (void* const) ii);
        assert( (u8)ii == *(u8*)uDAIndex(daTest_u8, ii) );
    }
    uDADestroy(daTest_u8);

    printf("\tTesting uDynamicArray<u16>...\n");
    uDynamicArray* daTest_u16 = uDAInit(u16);
    for (size_t ii = 0; ii < 0xFFFF; ii++)
    {
        uDAPush(daTest_u16, (void* const) ii);
        assert( (u16)ii == *(u16*)uDAIndex(daTest_u16, ii) );
    }
    uDADestroy(daTest_u16);

    printf("\tTesting uDynamicArray<u32>...\n");
    uDynamicArray* daTest_u32 = uDAInit(u32);
    for (size_t ii = 0; ii < 0xFFFF; ii++)
    {
        uDAPush(daTest_u32, (void* const) ii);
        assert( (u32)ii == *(u32*)uDAIndex(daTest_u32, ii) );
    }
    uDADestroy(daTest_u32);

    printf("\tTesting uDynamicArray<u64>...\n");
    uDynamicArray* daTest_u64 = uDAInit(u64);
    //for (size_t ii = 0; ii < 0xFFFF; ii++)
    for (size_t ii = 0; ii < 10; ii++)
    {
        u64 value = ii;
        //printf("value: %" PRIu64 "\n", ii);
        printf("%d\n", value);
        uDAPush(daTest_u64, (void* const) &value);
        assert( (u64)ii == *(u64*)uDAIndex(daTest_u64, ii) );
    }
    uDADestroy(daTest_u64);

    printf("\tTesting uDynamicArray<r32>...\n");
    uDynamicArray* daTest_r32 = uDAInit(r32);
    for (size_t ii = 0; ii < 0xFFFF; ii++)
    {
        r32 value = ii - uPI;
        uDAPush(daTest_r32, (void* const) &value);
        assert( (r32)(ii - uPI) == *(r32*)uDAIndex(daTest_r32, ii) );
    }
    uDADestroy(daTest_r32);

    printf("\tTesting uDynamicArray<r64>...\n");
    uDynamicArray* daTest_r64 = uDAInit(r64);
    for (size_t ii = 0; ii < 0xFFFF; ii++)
    {
        r64 value = ii - uPI;
        uDAPush(daTest_r64, (void* const) &value);
        assert( (r64)(ii - uPI) == *(r64*)uDAIndex(daTest_r64, ii) );
    }
    uDADestroy(daTest_r64);

    printf("[ TESTING::SUCCESS ]\n\n");
}
