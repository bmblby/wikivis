#ifndef WIKIDB_SIM_PAIR_H_
#define WIKIDB_SIM_PAIR_H_

#include <stdint.h>
#include <bitset>

//#define SIMPAIR_DEBUG

class SimPair {
 public:
    SimPair();
    SimPair(uint32_t id, uint32_t similarity);
    SimPair(uint64_t data);

    uint32_t getIndex() const;
    uint32_t getSim();
    uint64_t getData() const;

    void setIndex(uint32_t id);
    void setSim(uint32_t similarity);

#ifdef SIMPAIR_DEBUG
    void print_debug() const;
#endif

 private:
    // only 40 Bits used for storage
    uint64_t _data;
    // Data Layout:
    // Bit 10..40: id (max id: 2^30 or 1073741824)
    // Bit   0..9: similarity value (between 0 and 999)
};

#endif // WIKIDB_SIM_PAIR_H_
