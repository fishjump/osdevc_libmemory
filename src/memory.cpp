#include <memory/memory.hpp>

namespace {
system::memory::entity::GlobalMemoryDescriptor globalMemoryDescriptor;
} // namespace

namespace system::memory {
unsigned int entity::MemoryDescriptor::pages() const {
    void *start, *end;
    start = system::memory::pageAlign(this->address);
    end   = (void *)(((unsigned long)this->address + this->length)
                   & system::memory::PAGE_MASK);

    return start > end ? 0
                       : ((unsigned long)end - (unsigned long)start)
                             >> system::memory::PAGE_SHIFT;
}

void entity::GdtDescriptor::setDescriptor(uint32_t base, uint32_t limit,
                                          uint8_t access, uint8_t flags) {
    limit1       = (uint16_t)limit;
    limit2       = (uint8_t)(limit >> 16);
    base1        = (uint16_t)base;
    base2        = (uint8_t)(base >> 16);
    base3        = (uint8_t)(base >> 24);
    this->access = access;
    this->flags  = flags;
}

entity::GlobalMemoryDescriptor *getGlobalMemoryDescriptor() {
    return &globalMemoryDescriptor;
}

void *allocatePages(unsigned count) {
    unsigned memoryDescriptorsCount =
        getGlobalMemoryDescriptor()->memoryDescriptorsCount;
    if(memoryDescriptorsCount
       >= entity::GlobalMemoryDescriptor::MEMORY_DESCRIPTORS_COUNT_MAX) {
        // Cannot hold more descriptors
        return nullptr;
    }

    for(int i = 0; i < memoryDescriptorsCount; i++) {
        if(getGlobalMemoryDescriptor()->memoryDescriptors[i].type
               == entity::MemoryDescriptorType::RAM
           && getGlobalMemoryDescriptor()->memoryDescriptors[i].pages()
                  >= count) {
            void *addr = (void *)getGlobalMemoryDescriptor()
                             ->memoryDescriptors[i]
                             .address;
            entity::MemoryDescriptor newNode;
            newNode.address = addr;
            newNode.length  = count * PAGE_SIZE;
            newNode.type =
                getGlobalMemoryDescriptor()->memoryDescriptors[i].type;
            newNode.free = false;

            getGlobalMemoryDescriptor()->memoryDescriptors[i].address =
                (void *)((unsigned long)getGlobalMemoryDescriptor()
                             ->memoryDescriptors[i]
                             .address
                         + newNode.length);
            getGlobalMemoryDescriptor()->memoryDescriptors[i].length -=
                newNode.length;

            for(int j = memoryDescriptorsCount - 1; j >= i; j--) {
                getGlobalMemoryDescriptor()->memoryDescriptors[j] =
                    getGlobalMemoryDescriptor()->memoryDescriptors[j + 1];
            }

            getGlobalMemoryDescriptor()->memoryDescriptors[i] = newNode;

            return addr;
        }
    }
    return nullptr;
}

void freePages(void *addr) {
    entity::MemoryDescriptor *descriptors =
        getGlobalMemoryDescriptor()->memoryDescriptors;
    unsigned int count =
        getGlobalMemoryDescriptor()->memoryDescriptorsCount;

    for(int i = 0; i < count; i++) {
        if(descriptors[i].address == addr) {
            descriptors[i].free = true;
            return;
        }
    }
}
} // namespace system::memory
