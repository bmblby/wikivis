#include <iostream>
#include "wikidb.h"
#include "contract.h"
#include "record_not_found.h"


int main()
{
    WikiDB db("/media/HDD2/data/database/enwiki2016-full");

    //Write test to check right conversionfrom simpair to revid + simval
    uint32_t revid = 714857385;
    uint32_t simval = 478;
    std::cout << "Revid: " << revid
            << " Sim: " << simval << std::endl;

    SimPair sp(revid, simval);
    std::cout
            << " get data: " << sp.getData()
            << " SP: Revid: " << sp.getIndex()
            << " SP: Sim: " << sp.getSim()
            << std::endl;

    assert(revid == sp.getIndex());
    assert(simval == sp.getSim());

    uint64_t data = sp.getData();
    SimPair sp2(data);
    std::cout << "SimPair two:\n"
            << " get data: " << sp.getData()
            << " SP: Revid: " << sp.getIndex()
            << " SP: Sim: " << sp.getSim()
            << std::endl;

    Article one = db.getArticle(1);
    std::cout << db.articleExistsIndex(one.index) << std::endl
    << db.articleExistsRevid(one.revid) << std::endl;
    std::cout << one.info();

    Category two = db.getCategory(1);
    std::cout << db.categoryExistsIndex(two.index) << std::endl
    << db.categoryExistsRevid(two.revid) << std::endl;
    std::cout << two;

}
