/*
 * Copyright (c) 2015 Bagrat Ter-Akopyan, bagrat.akopyan@gmail.com
 *                    Giuliano Castiglia, castiglia.giuliano@gmail.com
 */

#include <iostream>
#include "contract.h"
#include "SimPair.h"

// Generic Print Function

// TODO(giuli) print func for categories
void
Page::info() const {
    std::cout << "Index: " << this->index << std::endl;
    std::cout << "Revid: " << this->revid << std::endl;
    std::cout << "Title: " << this->title << std::endl;
    std::cout << "Parents: ";

    if (this->parents.length() > 0) {
        std::cout << std::endl;
        for (std::size_t i =0; i < this->parents.length(); ++i) {
                dbCursor<Category> parentCursor;
                parentCursor.at(this->parents[i]);
                std::cout << "\tRevid: " << parentCursor->revid
                            << " Title: "<< parentCursor->title << std::endl;
        }
    } else {
        std::cout << "No Parents" << std::endl;
      }

    std::cout << std::endl;
}

std::vector<Category>
Page::getParents() const {
    std::vector<Category> parents(0);
    if ((this->parents).length() > 0) {
        parents.resize((this->parents).length());

        for (std::size_t i = 0; i < (this->parents).length(); ++i) {
            dbCursor<Category> parCur;
            parCur.at((this->parents)[i]);
            parents[i] = *parCur.get();
        }
    }

    return parents;
}

std::vector<uint32_t>
Category::getChildren() const {
    std::vector<uint32_t> children(0);

    if ((this->children).length() > 0) {
        children.resize((this->children).length());

        for (std::size_t i = 0; i < (this->children).length(); ++i) {
            dbCursor<Page> childCur;
            childCur.at((this->children)[i]);
            children[i] = childCur->revid;
        }
    }

    return children;
}

void
Article::info() const {
    this->Page::info();
    std::cout << "words: " << this->words << std::endl;

    std::cout << "Comparisons" << std::endl;
    if (this->comparisons.length() > 0) {
        for (std::size_t i = 0; i < this->comparisons.length(); ++i) {
            uint32_t sp = this->comparisons[i];

            SimPair simPair(sp);
            uint32_t index = simPair.getIndex();
            uint32_t sim  = simPair.getSim();

            dbCursor<Article> compCursor;
            dbQuery q;
            q = "index=", index;
            compCursor.select(q);

            std::cout << "\tRevid: " << compCursor->revid
                      << " Index: " << index
                      << " SimValue: " << sim << std::endl;
            }
    } else {
          std::cout << "No Comparisons" << std::endl;
      }
    std::cout << std::endl;
}

std::vector<SimPair>
Article::getComparisons() const {
    std::vector<SimPair> simpairs(0);

    if ((this->comparisons).length() > 0) {
        simpairs.resize((this->comparisons).length());

        for (std::size_t i = 0; i < (this->comparisons).length(); ++i) {
            SimPair sp((this->comparisons)[i]);
			simpairs[i] = sp;
			// simpairs[i] = sp.getData();
        }
    }
    return simpairs;
}
