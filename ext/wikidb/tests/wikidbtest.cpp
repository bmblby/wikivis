#include <iostream>
#include "wikidb.h"
#include "contract.h"
#include "record_not_found.h"


int main()
{
    WikiDB wikidb("/dev/shm/wiki-vis-data/pages");
    Article one = wikidb.getArticle(1);
    std::cout << one.info();

    Category one_c = wikidb.getCategory(1);
    std::cout << one_c;


}
