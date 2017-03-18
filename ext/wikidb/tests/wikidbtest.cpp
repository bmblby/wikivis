#include "wikidb.h"
#include "contract.h"
#include "record_not_found.h"


int main()
{
    WikiDB wikidb("/dev/shm/wiki-vis-data/pages");
    Article one = wikidb.getArticle(1);
    one.info();


}
