#pragma once

#include "entity.hpp"

#define VIRTUAL_TO_PHYICAL_ADDR(v_addr) ((v_addr) - (system::memory::KERNEL_ADDR_BASE))
#define PHYICAL_TO_VIRTUAL_ADDR(p_addr) ((p_addr) + (system::memory::KERNEL_ADDR_BASE))

namespace system::memory
{
    // virtual addr offset
    // x86_64 addr structure:
    // 64 bits length, but only 48 bits are valid.
    // the higher 16 bits must be all 0s or all 1s, and must be same as the 17th highest bit.
    // so, we can let the higher 17 bits all 1s to be kernel addr space
    // and let the higher 17 bits all 0s as user addr space
    const uint64_t KERNEL_ADDR_BASE = 0xffff800000000000ul;
    const uint64_t USER_ADDR_BASE = 0x0000000000000000ul;

    // internal page bits' length
    const uint32_t PAGE_1G_SHIFT = 30;
    const uint32_t PAGE_2M_SHIFT = 21;
    const uint32_t PAGE_4K_SHIFT = 12;
    const uint32_t PAGE_SHIFT = PAGE_2M_SHIFT;

    // size pre page
    const uint64_t PAGE_1G_SIZE = 1ul << PAGE_1G_SHIFT;
    const uint64_t PAGE_2M_SIZE = 1ul << PAGE_2M_SHIFT;
    const uint64_t PAGE_4K_SIZE = 1ul << PAGE_4K_SHIFT;
    const uint64_t PAGE_SIZE = PAGE_2M_SIZE;

    // page mask
    const uint64_t PAGE_1G_MASK = ~(PAGE_1G_SIZE - 1);
    const uint64_t PAGE_2M_MASK = ~(PAGE_2M_SIZE - 1);
    const uint64_t PAGE_4K_MASK = ~(PAGE_4K_SIZE - 1);
    const uint64_t PAGE_MASK = PAGE_2M_MASK;

    inline void *virtualToPhyicalAddr(void *addr)
    {
        return (void *)((unsigned long)addr - KERNEL_ADDR_BASE);
    }

    inline void *phyicalToVirtualAddr(void *addr)
    {
        return (void *)((unsigned long)addr + KERNEL_ADDR_BASE);
    }

    inline void *page1gAlign(void *addr)
    {
        // find out an addr with is equal or higher than addr, and it's lower bits are all 0s.
        // we cannot use (addr & PAGE_1G_MASK) because the result may be lower than addr which is illegal.
        // this method may waste some space but it's necessary.
        return (void *)(((unsigned long)addr + PAGE_1G_SIZE - 1) & PAGE_1G_MASK);
    }

    inline void *page2mAlign(void *addr)
    {
        // find out an addr with is equal or higher than addr, and it's lower bits are all 0s.
        // we cannot use (addr & PAGE_2M_MASK) because the result may be lower than addr which is illegal.
        // this method may waste some space but it's necessary.
        return (void *)(((unsigned long)addr + PAGE_2M_SIZE - 1) & PAGE_2M_MASK);
    }

    inline void *page4kAlign(void *addr)
    {
        // find out an addr with is equal or higher than addr, and it's lower bits are all 0s.
        // we cannot use (addr & PAGE_4K_MASK) because the result may be lower than addr which is illegal.
        // this method may waste some space but it's necessary.
        return (void *)(((unsigned long)addr + PAGE_4K_SIZE - 1) & PAGE_4K_MASK);
    }

    inline void *pageAlign(void *addr)
    {
        return page2mAlign(addr);
    }

    entity::GlobalMemoryDescriptor *getGlobalMemoryDescriptor();
    void *allocatePages(unsigned count);
    void freePages(void *addr);

} // namespace system::memory
