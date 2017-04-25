#include <iostream>
#include "wikidb.h"
#include "contract.h"
#include "record_not_found.h"


int main()
{
    WikiDB db("/dev/shm/wiki-vis-data/pages");
    Article one = db.getArticle(1);
    std::cout << db.articleExists(one.index) << std::endl
    << db.articleExists(one.revid) << std::endl;
    std::cout << one.info();

    Category two = db.getCategory(1);
    std::cout << db.categoryExists(two.index) << std::endl
    << db.categoryExists(two.revid) << std::endl;
    std::cout << two;

}
