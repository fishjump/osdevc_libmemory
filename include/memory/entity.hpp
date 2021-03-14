#pragma once

#include <std/stdcxx.hpp>

namespace system::memory::entity
{
    class GdtTableDescriptor
    {
    public:
        uint16_t length;
        uint64_t addr;
    } __attribute__((packed));

    class GdtDescriptor
    {
    public:
        uint16_t limit1;
        uint16_t base1;
        uint8_t base2;
        uint8_t access;
        uint8_t limit2 : 4;
        uint8_t flags : 4;
        uint8_t base3;

        void setDescriptor(uint32_t base, uint32_t limit, uint8_t access, uint8_t flags);
    } __attribute__((packed));

    enum MemoryDescriptorType
    {
        RAM = 1,
        ROM = 2,
        ACPI_RECLAIM = 3,
        ACPI_NVS = 4,
        UNUSABLE = 5
    };

    class MemoryDescriptor
    {
    public:
        void *address;
        unsigned long length;
        MemoryDescriptorType type;
        bool free = false;
        unsigned int pages() const;
    };

    // Assume we only have 2 GB physical memory with 2MB page size
    class GlobalMemoryDescriptor
    {
    public:
        static const unsigned MEMORY_DESCRIPTORS_COUNT_MAX = 1024;

        MemoryDescriptor memoryDescriptors[MEMORY_DESCRIPTORS_COUNT_MAX];
        unsigned long memoryDescriptorsCount;

        unsigned long usableMemory;
        unsigned int usablePages;
    };

} // namespace system::memory::entity
