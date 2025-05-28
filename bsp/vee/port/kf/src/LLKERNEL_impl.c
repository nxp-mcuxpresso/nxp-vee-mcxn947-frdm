#include <stdlib.h>
#include <string.h>

#include "LLKERNEL_impl.h"
#include "BESTFIT_ALLOCATOR.h"


// Define size to allocate for KF working buffer
#ifndef KERNEL_WORKING_BUFFER_SIZE
#define KERNEL_WORKING_BUFFER_SIZE 0x17000
#endif

// Declare MicroEJ KF working buffer
uint8_t kernel_working_buffer[KERNEL_WORKING_BUFFER_SIZE] __attribute__((section(".m_data")));

// Your implementation of malloc()
#define KERNEL_MALLOC(size) BESTFIT_ALLOCATOR_allocate(&allocatorInstance, (int32_t)(size))

// Your implementation of free()
#define KERNEL_FREE(addr) BESTFIT_ALLOCATOR_free(&allocatorInstance, (void*)(addr))

// Your implementation of 'ASSERT(0)'
#define KERNEL_ASSERT_FAIL() while(1)

// Utility macros for allocating RAM and ROM areas with required alignment constraints
#define KERNEL_AREA_GET_MAX_SIZE(size, alignment) ((size)+((alignment)-1))
#define KERNEL_AREA_GET_START_ADDRESS(addr, alignment) ((void*)((((int32_t)(addr))+(alignment)-1)&~((alignment)-1)))

typedef struct installed_feature{
   void* ROM_area;
   void* RAM_area;
} installed_feature_t;

BESTFIT_ALLOCATOR allocatorInstance;
uint32_t KERNEL_allocationInitialized;

int32_t LLKERNEL_IMPL_allocateFeature(int32_t size_ROM, int32_t size_RAM) {
   int32_t ret = 0;

   if(KERNEL_allocationInitialized == 0){
      // lazy init
      BESTFIT_ALLOCATOR_new(&allocatorInstance);
      BESTFIT_ALLOCATOR_initialize(&allocatorInstance, (int32_t)(&kernel_working_buffer[0]), (int32_t)(&kernel_working_buffer[KERNEL_WORKING_BUFFER_SIZE]));
      KERNEL_allocationInitialized = 1;
   }

   int total_size = sizeof(installed_feature_t);
   total_size += KERNEL_AREA_GET_MAX_SIZE(size_ROM, LLKERNEL_ROM_AREA_ALIGNMENT);
   total_size += KERNEL_AREA_GET_MAX_SIZE(size_RAM, LLKERNEL_RAM_AREA_ALIGNMENT);

   void* total_area = KERNEL_MALLOC(total_size);
   if(NULL != total_area){
      installed_feature_t* f = (installed_feature_t*)total_area;
      f->ROM_area = KERNEL_AREA_GET_START_ADDRESS((void*)(((int32_t)f)+((int32_t)sizeof(installed_feature_t))), LLKERNEL_ROM_AREA_ALIGNMENT);
      f->RAM_area = KERNEL_AREA_GET_START_ADDRESS((void*)(((int32_t)f->ROM_area)+size_ROM), LLKERNEL_RAM_AREA_ALIGNMENT);
      ret = (int32_t)f;
   } // else out of memory

   return ret;
}

void LLKERNEL_IMPL_freeFeature(int32_t handle) {
   if(KERNEL_allocationInitialized == 0){
      // lazy init
      BESTFIT_ALLOCATOR_new(&allocatorInstance);
      BESTFIT_ALLOCATOR_initialize(&allocatorInstance, (int32_t)(&kernel_working_buffer[0]), (int32_t)(&kernel_working_buffer[KERNEL_WORKING_BUFFER_SIZE]));
      KERNEL_allocationInitialized = 1;
   }
   KERNEL_FREE(handle);
}

int32_t LLKERNEL_IMPL_getAllocatedFeaturesCount(void) {
   // No persistency support
   return 0;
}

int32_t LLKERNEL_IMPL_getFeatureHandle(int32_t allocation_index) {
   // No persistency support
   KERNEL_ASSERT_FAIL();
}

void* LLKERNEL_IMPL_getFeatureAddressRAM(int32_t handle) {
   return ((installed_feature_t*)handle)->RAM_area;
}

void* LLKERNEL_IMPL_getFeatureAddressROM(int32_t handle) {
   return ((installed_feature_t*)handle)->ROM_area;
}

int32_t LLKERNEL_IMPL_copyToROM(void* dest_address_ROM, void* src_address, int32_t size) {
   memcpy(dest_address_ROM, src_address, size);
   return LLKERNEL_OK;
}

int32_t LLKERNEL_IMPL_flushCopyToROM(void) {
   return LLKERNEL_OK;
}

int32_t LLKERNEL_IMPL_onFeatureInitializationError(int32_t handle, int32_t error_code) {
   // No persistency support
   KERNEL_ASSERT_FAIL();
   return 0;
}