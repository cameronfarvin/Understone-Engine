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

void
runAllTests()
{
    printf("[ TESTING ] Running All Tests...\n");

    printf("\tTesting uDynamicArray<u8>...\n");
    uDynamicArray* daTest_u8 = uDAInit(u8);
    for (size_t ii = 0; ii < 0xFF; ii++)
    {
        uDAPush(daTest_u8, (void** const) &ii);
        assert( (u8)ii == *(u8*)uDAIndex(daTest_u8, ii) );
    }

    for (size_t ii = 0xFF; ii > 0; ii--)
    {
        size_t size_before = daTest_u8->num_elements;
        uDAPop(daTest_u8);
        size_t size_after = daTest_u8->num_elements;
        assert(size_after == (size_before - 1));
    }
    uDADestroy(daTest_u8);

    printf("\tTesting uDynamicArray<u16>...\n");
    uDynamicArray* daTest_u16 = uDAInit(u16);
    for (size_t ii = 0; ii < 0xFFFF; ii++)
    {
        uDAPush(daTest_u16, (void** const) &ii);
        assert( (u16)ii == *(u16*)uDAIndex(daTest_u16, ii) );
    }

    for (size_t ii = 0xFFFF; ii > 0; ii--)
    {
        size_t size_before = daTest_u16->num_elements;
        uDAPop(daTest_u16);
        size_t size_after = daTest_u16->num_elements;
        assert(size_after == (size_before - 1));
    }
    uDADestroy(daTest_u16);

    printf("\tTesting uDynamicArray<u32>...\n");
    uDynamicArray* daTest_u32 = uDAInit(u32);
    for (size_t ii = 0; ii < 0xFFFF; ii++)
    {
        uDAPush(daTest_u32, (void** const) &ii);
        assert( (u32)ii == *(u32*)uDAIndex(daTest_u32, ii) );
    }

    for (size_t ii = 0xFFFF; ii > 0; ii--)
    {
        size_t size_before = daTest_u32->num_elements;
        uDAPop(daTest_u32);
        size_t size_after = daTest_u32->num_elements;
        assert(size_after == (size_before - 1));
    }
    uDADestroy(daTest_u32);

    printf("\tTesting uDynamicArray<u64>...\n");
    uDynamicArray* daTest_u64 = uDAInit(u64);
    for (size_t ii = 0; ii < 0xFFFF; ii++)
    {
        uDAPush(daTest_u64, (void** const) &ii);
        assert( (u64)ii == *(u64*)uDAIndex(daTest_u64, ii) );
    }

    for (size_t ii = 0xFFFF; ii > 0; ii--)
    {
        size_t size_before = daTest_u64->num_elements;
        uDAPop(daTest_u64);
        size_t size_after = daTest_u64->num_elements;
        assert(size_after == (size_before - 1));
    }
    uDADestroy(daTest_u64);

    printf("\tTesting uDynamicArray<r32>...\n");
    uDynamicArray* daTest_r32 = uDAInit(r32);
    for (size_t ii = 0; ii < 0xFFFF; ii++)
    {
        r32 value = (r32)(ii - uPI);
        uDAPush(daTest_r32, (void** const)(r32*)&value);
        assert( value == *(r32*)uDAIndex(daTest_r32, ii) );
    }

    for (size_t ii = 0xFFFF; ii > 0; ii--)
    {
        size_t size_before = daTest_r32->num_elements;
        uDAPop(daTest_r32);
        size_t size_after = daTest_r32->num_elements;
        assert(size_after == (size_before - 1));
    }
    uDADestroy(daTest_r32);

    printf("\tTesting uDynamicArray<r64>...\n");
    uDynamicArray* daTest_r64 = uDAInit(r64);
    for (size_t ii = 0; ii < 0xFFFF; ii++)
    {
        r64 value = ii - uPI;
        uDAPush(daTest_r64, (void** const) &value);
        assert( value == *(r64*)uDAIndex(daTest_r64, ii) );
    }

    for (size_t ii = 0xFFFF; ii > 0; ii--)
    {
        size_t size_before = daTest_r64->num_elements;
        uDAPop(daTest_r64);
        size_t size_after = daTest_r64->num_elements;
        assert(size_after == (size_before - 1));
    }
    uDADestroy(daTest_r64);

    printf("[ TESTING::SUCCESS ]\n\n");
}
