#include "SimPair.h"
#include <cassert>

#ifdef SIMPAIR_DEBUG
#include <iostream>
#endif

SimPair::SimPair()
 : _data(0)
{}

SimPair::SimPair(uint32_t id, uint32_t similarity)
 : _data(0)
{
            assert(id < 1 << 30);
            assert(similarity < 1 << 10);
            _data = (id << 10);
            _data |= similarity;
}

SimPair::SimPair(uint64_t data)
 : _data(0)
{
    assert(data < 0x1FFFFFFFFF3);
    // NOTE switch first 14 bits to zero
    data &= 0xFFFFFF0000000000;
    _data = data;
}

uint32_t
SimPair::getIndex() const {
        return (uint32_t)(_data >> 10);
}

uint32_t
SimPair::getSim() {
        _data &= 0xFFFFFFFC00;
        return (uint32_t)(_data);
}

uint64_t
SimPair::getData() const {
    return _data;
}

void
SimPair::setIndex(uint32_t id) {
        assert(id < 1 << 30);
        _data &= 0xFFFFFFFC00;
        _data |= id << 10;
}

void
SimPair::setSim(uint32_t similarity) {
        assert(similarity < 1 << 10);
        _data &= 0x3FF;
        _data |= similarity;
}

#ifdef SIMPAIR_DEBUG
void
SimPair::print_debug() const {
        std::string string_data = std::to_string(_data);
        std::cout << "_data: " << string_data << std::endl;
        std::cout << "ID: " << this->getIndex() << " - Similarity: " << this->getSim() << std::endl;
}
#endif

