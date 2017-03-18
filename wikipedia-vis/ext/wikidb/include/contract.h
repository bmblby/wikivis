/*
 * Copyright (c) 2015 Bagrat Ter-Akopyan, bagrat.akopyan@gmail.com
 *                    Giuliano Castiglia, castiglia.giuliano@gmail.com
 */

#ifndef WIKIDB_CONTRACT_H_
#define WIKIDB_CONTRACT_H_

#include <vector>
// #include <string>
#include <fastdb.h>
#include "SimPair.h"

USE_FASTDB_NAMESPACE

class Category;

class Page {
 public:
    std::string info() const;
    std::vector<Category> getParents() const;

    // Menber
    int4 index;
    int4 revid;
    char const* title;
    dbArray< dbReference<Category> > parents;

    TYPE_DESCRIPTOR((KEY(index, AUTOINCREMENT|INDEXED|HASHED),
                     KEY(revid, INDEXED|HASHED), KEY(title, INDEXED|HASHED),
                     RELATION(parents, children)));
};

class Article : public Page {
 public:
    std::string info() const;
    std::vector<SimPair> getComparisons() const;

    //member
    int4 words;
    dbArray<uint32_t> comparisons;

    TYPE_DESCRIPTOR((SUPERCLASS(Page), KEY(words, INDEXED|HASHED),
                    FIELD(comparisons)));
};

class Category : public Page {
 public:
    std::string info() const;
    dbArray< dbReference<Page> > children;

    // get revision ids of children
    std::vector<uint32_t> getChildren() const;

    TYPE_DESCRIPTOR((SUPERCLASS(Page), RELATION(children, parents)));
};

inline
std::ostream& operator<<(std::ostream& os, Category& cat)
{
    os << cat.info();
    return os;
}

inline
std::ostream& operator<<(std::ostream& os, Article& art)
{
    os << art.info();
    return os;
}

#endif  // WIKIDB_CONTRACT_H_
