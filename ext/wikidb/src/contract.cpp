/*
 * Copyright (c) 2015 Bagrat Ter-Akopyan, bagrat.akopyan@gmail.com
 *                    Giuliano Castiglia, castiglia.giuliano@gmail.com
 */

#include <iostream>
#include "contract.h"
#include "SimPair.h"

// Generic Print Function

// TODO(giuli) print func for categories
std::string
Page::info() const {
    std::string message =
    + "Index: " + std::to_string(this->index) + "\n"
    + "Revid: " + std::to_string(this->revid) + "\n"
    + "Title: " + (std::string)this->title + "\n"
    + "Parents: ";

    if (this->parents.length() > 0) {
        message += "\n";
        for (std::size_t i =0; i < this->parents.length(); ++i) {
                dbCursor<Category> cur;
                cur.at(this->parents[i]);
                message += "\tRevid: " + std::to_string(cur->revid)
                            + " Title: "+ cur->title + "\n";
        }
    } else {
        message += "No Parents\n";
      }
    return message;
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

std::string
Category::info() const {
    std::string final = this->Page::info();

    auto children_revids = this->getChildren();
    if (children_revids.size() > 0) {
        int art_num(0), cat_num(0);
        std::string message;
        for(auto revid : children_revids) {
            dbCursor<Category> Cursor;
            dbQuery q;
            q = "revid=", revid;
            Cursor.select(q);
            if(Cursor.isEmpty())
            {
                art_num++;
                dbCursor<Article> Cursor;
                dbQuery q;
                q = "revid=", revid;
                Cursor.select(q);
                Page p = *Cursor.get();
                message += "\tA Revid: " + std::to_string(p.revid) + " "
                          + "title: " + p.title + "\n";
            }
            else {
                cat_num++;
                Page p = *Cursor.get();
                message += "\tC Revid: " + std::to_string(p.revid) + " "
                          + "title: " + p.title + "\n";
            }
        }
        final += std::to_string(cat_num) + " Sub categories and "
                  + std::to_string(art_num) + " Sub articles:\n"
                  + message;
        return final;
    } else {
          return "No Sub-/ Categories or Articles found!\n";
      }
}

std::string
Article::info() const {
    std::string message = this->Page::info();
    message += "words: " + std::to_string(this->words) + "\n";

    message += "Comparisons\n";
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

            message += "\tRevid: " +    std::to_string(compCursor->revid)
                      + " Index: " +    std::to_string(index)
                      + " SimValue: " + std::to_string(sim) + "\n";
            }
    } else {
          return message += "No Comparisons\n\n";
      }
    return message;
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
