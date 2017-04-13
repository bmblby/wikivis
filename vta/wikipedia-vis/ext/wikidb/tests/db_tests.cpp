#include "gtest/gtest.h"

#include <vector>
#include <chrono>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <fstream>
#include <time.h>
#include "wikidb.h"
#include "contract.h"

#include <stdlib.h>
#include <iostream>

class WikiDBFixture : public ::testing::Test {
	public:
		WikiDBFixture() 
			: _wikiDB("/dev/shm/wiki-vis-data/pages")
		{
		};

	protected:

	virtual ~WikiDBFixture() 
		{};

	virtual void SetUp() 
		{};

	virtual void TearDown()
		{};

	WikiDB _wikiDB;
};

TEST_F(WikiDBFixture, IterateRevIDs) {
	
	srand (time(NULL));
	std::size_t sample = 100;
	int random;

	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
	int sizeArticles = _wikiDB.sizeArticles();
	int sizeCategories = _wikiDB.sizeCategories();
	std::cout << "Number Articles " << sizeArticles << std::endl;
	std::cout << "Number Categories " << sizeCategories << std::endl;

	int sizeComp(0);
	for (int i = 1; i < sizeArticles; ++i) {
		Article a = _wikiDB.getArticle(i);
		std::vector<SimPair> compVec = a.getComparisons();
		sizeComp += compVec.size();
	}
	std::cout << "Number Comparisons " << sizeComp << std::endl;

	//for (int i = 0; i < sample; ++i) {
	//	for (int j = 0; j < 100000; ++j) {
	//		random = rand() % sizeArticles + 1;
	//		Article a = _wikiDB.getArticle(random);
	//		//std::vector<SimPair> sim = a.getComparisons();
	//		//artVec[i] = a;
	//	}
	//}
	//std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
	//auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1);
	//duration = duration / sample;
	//std::cout << "Duration Database: " << duration.count() << " microseconds" << std::endl;

	/////////////////////////////////////////////////////////////////
	// change contract to unsigned return value for testing	
#if 0
	std::vector<uint32_t> index2revid(0);
	std::vector<std::string> index2title(0);
	std::vector<std::vector<uint32_t>> index2parents(0);
	std::vector<std::vector<uint32_t>> index2comparisons(0);

	std::ifstream i2r_dat("/dev/shm/index2revid.dat");
	boost::archive::binary_iarchive ia_i2r(i2r_dat);
	ia_i2r & index2revid;

	std::ifstream i2t_dat("/dev/shm/index2title.dat");
	boost::archive::binary_iarchive ia_i2t(i2t_dat);
	ia_i2t & index2title;

	std::ifstream i2p_dat("/dev/shm/index2parents.dat");
	boost::archive::binary_iarchive ia_i2p(i2p_dat);
	ia_i2p & index2parents;

	std::ifstream i2c_dat("/dev/shm/index2comparisons.dat");
	boost::archive::binary_iarchive ia_i2c(i2c_dat);
	ia_i2c & index2comparisons;

	t1 = std::chrono::high_resolution_clock::now();
	for (std::size_t a = 0; a < sample; ++a) {
		for (std::size_t b = 0; b < 100000; ++b) {
			random = rand() % index2revid.size();
			std::size_t revid = index2revid[random];
			std::string title = index2title[random];
			std::vector<uint32_t> parents = index2parents[random];
			std::vector<uint32_t> comparisons = index2comparisons[random];
		}
	}
	t2 = std::chrono::high_resolution_clock::now();
	duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1);
	duration = duration / sample;
	std::cout << "Duration Vector: " << duration.count() << " microseconds" << std::endl;

#endif
	
}
