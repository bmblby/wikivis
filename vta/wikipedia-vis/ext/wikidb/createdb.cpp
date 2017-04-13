/*
 * Copyright (c) 2015 Bagrat Ter-Akopyan, bagrat.akopyan@gmail.com
 *                    Giuliano Castiglia, castiglia.giuliano@gmail.com
 */

#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
#include <stdint.h>
#include <iostream>
#include <chrono>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <iterator>
#include <utility>
#include <string>
#include <map>
#include <algorithm>
#include "contract.h"
#include "wikidb.h"
#include "SimPair.h"
#include "docopt.h"

#include <thread>
#include <future>

static const char USAGE[] =
R"(createdb.

	Usage:
	  createdb pages <DatabaseName> <revid2titleFilename>
	  createdb parents <DatabaseName> <revid2parentsFilename>
	  createdb comparisons <DatabaseName> <simMatrixDir>
      createdb debug <DatabaseName> <parents>

	Options:
	  -h --help	Show this screen.
	  --version	Show version.
)";


void debug(std::string const& db_filename, std::string const& parents_tsv_filename) {
    WikiDB wikidb(db_filename);
    dbQuery q;
    dbCursor<Article> articleCursor;
        //q = "index=", (4893 / 4);
        //articleCursor.select(q);
        articleCursor.last();
        articleCursor->info();
}

static const unsigned int k_similarity_multiplier = 1000;
/*
Input: wikidb and tsv containing articles and categories
Result: fill wikidb with pages: revid, name, words

*/
void pages2db(std::string const& db_filename,
	std::string const& tsv_filename) {
    std::ifstream infile(tsv_filename.c_str());
    if (!infile.is_open()) {
        std::cerr << "Couldn't open " << tsv_filename << std::endl;
        std::exit(1);
    }

    WikiDB wikidb(db_filename);

    // vector(pair(uint32_t, pair(string, uint32_t)))
    std::vector< std::pair<uint32_t, std::pair<std::string, uint32_t> > > articles(0);
    std::vector< std::pair<uint32_t, std::string> > categories(0);

    boost::char_separator<char> sep("\t");
    for (std::string line; getline(infile, line); ) {
        boost::tokenizer<boost::char_separator<char> > tokens(line, sep);

        auto it = std::begin(tokens);
        std::size_t ns = boost::lexical_cast<std::size_t>(*it);

        ++it;
        uint32_t revid = boost::lexical_cast<uint32_t>(*it);

        ++it;
        std::string title = *it;
        if (ns == 0) {
            ++it;
            uint32_t words = boost::lexical_cast<uint32_t>(*it);
            std::pair<std::string, uint32_t> pair = std::make_pair(title, words);
            std::pair<uint32_t, std::pair<std::string, int4> > article = std::make_pair(revid, pair);
            articles.push_back(article);
        }
        if (ns == 14) {
            std::pair<uint32_t, std::string> category = std::make_pair(revid, title);
            categories.push_back(category);
            ++it;
        }
    }

    wikidb.bulkInsertArticle(articles);
    wikidb.bulkInsertCategory(categories);
    infile.close();
}

void parents2db(std::string const& db_filename,
	std::string const& parents_tsv_filename) {
    std::ifstream infile(parents_tsv_filename.c_str());
    if (!infile.is_open()) {
        std::cerr << "Couldn't open " << parents_tsv_filename << std::endl;
        std::exit(1);
    }

    WikiDB wikidb(db_filename);

    std::vector<std::string> parents(0);

    // measure time for inserting parents for every record
    std::chrono::high_resolution_clock::time_point t1, t2, t3;
    t1 = std::chrono::high_resolution_clock::now();

    uint32_t parentNumber = 0;

    boost::char_separator<char> sep("\t");
    for (std::string line; getline(infile, line); ) {
        boost::tokenizer<boost::char_separator<char> > tokens(line, sep);

        auto it = std::begin(tokens);
        std::size_t ns = boost::lexical_cast<std::size_t>(*it);
        ++it;
        uint32_t revid = boost::lexical_cast<uint32_t>(*it);

        if (ns == 0) {
            ++it;
            while (it != std::end(tokens)) {
                parents.push_back(*it);
                ++it;
            }
            wikidb.bulkUpdateArticleParents(revid, parents);

        }
        if (ns == 14) {
            ++it;
            while (it != std::end(tokens)) {
                parents.push_back(*it);
                ++it;
            }
            wikidb.bulkUpdateCategoryParents(revid, parents);
        }
        parentNumber += parents.size();
        parents.clear();
    }

    t2 = std::chrono::high_resolution_clock::now();

    wikidb.commit();
    t3 = std::chrono::high_resolution_clock::now();

    auto insertDuration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    auto commitDuration = std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t2);

    std::cout << "Parents insertion time for " << parentNumber << " records: " << insertDuration.count()
	      << " milliseconds" << std::endl;
    std::cout << "Commit duration: " << commitDuration.count()
	      << " milliseconds" << std::endl;

    infile.close();
}

//void comparisons2db(WikiDB &wikidb,
//	std::string const& simpairs_tsv_filename, double threshold = 0.1) {
//    // structure of matrix:     revid   revid   sim revid   sim ...
//
//    std::ifstream infile(simpairs_tsv_filename.c_str());
//    if (!infile.is_open()) {
//        std::cerr << "Couldn't open file " << simpairs_tsv_filename << std::endl;
//        std::exit(1);
//    }
//
//    //std::cout << "Processing file: " << simpairs_tsv_filename << std::endl;
//
//    auto t1 = std::chrono::high_resolution_clock::now();
//
//    std::vector<uint32_t> comparisons(0);
//
//    static std::size_t count(0);
//
//    boost::char_separator<char> sep("\t");
//    for (std::string line; getline(infile, line); ) {
//        boost::tokenizer<boost::char_separator<char> > tokens(line, sep);
//
//        auto it = std::begin(tokens);
//        uint32_t refRevid = boost::lexical_cast<uint32_t>(*it);
//
//        ++it;
//        if (wikidb.articleExists(refRevid)) {      // does the reference revision id exist in the db?
//            while (it != std::end(tokens)) {
//                uint32_t revid = boost::lexical_cast<uint32_t>(*it);
//                ++it;
//                uint32_t sim = boost::numeric_cast<uint32_t>
//                                (boost::lexical_cast<float>(*it) * k_similarity_multiplier);
//                if (sim >= threshold) {
//                    dbQuery q1;
//                    q1 = "revid=", revid;
//                    dbCursor<Article> comparisonCursor;
//                    if (comparisonCursor.select(q1) > 0) {
//                        uint32_t comparisonIdx = comparisonCursor->index;
//
//                        SimPair sp(comparisonIdx, sim);
//
//                        uint32_t comparison = sp.getData();
//                        comparisons.push_back(comparison);
//                    }
//                }
//                else {
//                    break;
//                }
//                ++it;
//            }
//            //wikidb.bulkUpdateComparisons(refRevid, comparisons);
//            comparisons.clear();
//		} else{ /* nothing */ }
//    }
//
//    infile.close();
//    auto t2 = std::chrono::high_resolution_clock::now();
//    auto duration = std::chrono::duration_cast<std::chrono::seconds>(t2 - t1);
//    std::cout << "File " << simpairs_tsv_filename << " finished in "
//	      << duration.count() << " seconds." << std::endl;
//}

std::vector< std::pair<uint32_t, std::vector<uint32_t> > >
comparisons2vec(WikiDB &wikidb,
		std::string const& simpairs_tsv_filename, uint32_t threshold = 500) {

    std::ifstream infile(simpairs_tsv_filename.c_str());
    if (!infile.is_open()) {
        std::cerr << "Couldn't open file " << simpairs_tsv_filename << std::endl;
        std::exit(1);
    }

    std::vector< std::pair<uint32_t, std::vector<uint32_t> > > ret;
    std::vector<uint32_t> comparisons(0);
    static std::size_t count(0);

    wikidb._db.attach();

    boost::char_separator<char> sep("\t");
    for (std::string line; getline(infile, line); ) {
        boost::tokenizer<boost::char_separator<char> > tokens(line, sep);
        auto it = std::begin(tokens);
        uint32_t refRevid = boost::lexical_cast<uint32_t>(*it);
        ++it;

        if (wikidb.articleExists(refRevid)) {       // does the reference revision id exist in the db?
            while (it != std::end(tokens)) {
                uint32_t revid = boost::lexical_cast<uint32_t>(*it);
                ++it;
                uint32_t sim = boost::numeric_cast<uint32_t>
                                (boost::lexical_cast<float>(*it) * k_similarity_multiplier);
				if (sim >= threshold) {             // insert only simpairs to vec higher then treshold. Attention when sampling
					dbQuery q1;
					q1 = "revid=", revid;
					dbCursor<Article> comparisonCursor;
					if (comparisonCursor.select(q1) > 0) {
						uint32_t comparisonIdx = comparisonCursor->index;

						SimPair sp(comparisonIdx, sim);

						uint32_t comparison = sp.getData();
						comparisons.push_back(comparison);
					}
				} else { break; }
				++it;
			}
			ret.push_back(std::make_pair(refRevid, comparisons));

            //wikidb.bulkUpdateComparisons(refRevid, comparisons);
            comparisons.clear();
		} else { /* nothing 8 */ }
    }

    wikidb._db.detach();
    infile.close();
    return ret;
}


std::vector<std::string>
create_files_vec(std::string const& matrixDir) {
    std::vector<std::string> vec;
    boost::filesystem::path path(matrixDir);
    boost::filesystem::directory_iterator end_itr;
    for (boost::filesystem::directory_iterator itr(path); itr != end_itr; ++itr) {
        if (is_regular_file(itr->path())) {
            std::string current_file = itr->path().string();
            vec.push_back(current_file);
        }
    }
    std::sort(vec.begin(), vec.end());

    return vec;
}


int main(int argc, char* argv[]) {
    std::map<std::string, docopt::value> args = docopt::docopt(USAGE,
		    { argv + 1, argv + argc },
		    true,
		    "wikidbcreate 0.1");

    for (auto const& arg : args) {
		std::cout << arg.first << ":" << arg.second << std::endl;
    }


    if (args["debug"].asBool()) {
        debug(args["<DatabaseName>"].asString(), args["<parents>"].asString());
    }
    if (args["pages"].asBool()) {
		pages2db(args["<DatabaseName>"].asString(), args["<revid2titleFilename>"].asString());
    }
    if (args["parents"].asBool()) {
		parents2db(args["<DatabaseName>"].asString(), args["<revid2parentsFilename>"].asString());
    }
    if (args["comparisons"].asBool()) {
	std::vector<std::string> filename_vec = create_files_vec( args["<simMatrixDir>"].asString());

	WikiDB wikidb(args["<DatabaseName>"].asString());

		std::size_t count = 0;
		std::size_t filesNum = filename_vec.size();			// 5639 files uncleaned simMatrixDir
		std::size_t chunks = 64;                            // number of threads, try 64 threads

        // for loop for every file
		for(auto it = filename_vec.begin(); it != filename_vec.end(); ) {
			auto t1 = std::chrono::high_resolution_clock::now();

			std::vector< std::vector< std::pair< uint32_t, std::vector<uint32_t> > > > workVecs;
            int threshold = 100;                             // set to 100 lowest threshold
			typedef decltype(std::async(comparisons2vec, std::ref(wikidb), *it, threshold)) future_t;
			std::vector<future_t> futuresVec;

			if (filesNum - (chunks * count) > chunks) {		// count = 0..351 for 16 threads
                // for loop for
				for (std::size_t i = 0; i < chunks; ++i) {
					std::cout << *it << std::endl;
					futuresVec.push_back(std::async(std::launch::async, comparisons2vec, std::ref(wikidb), *it, threshold));
					++it;
				}
			} else {										// count = 352 for 16 threads
				while (it != filename_vec.end()) {
					std::cout << *it << std::endl;
					futuresVec.push_back(std::async(std::launch::async, comparisons2vec, std::ref(wikidb), *it, threshold));
					++it;
				}
			}

			for(auto &future : futuresVec) {
				workVecs.push_back(future.get());
			}

			// update already processed files count
			count++;

			for( auto const& workVec : workVecs ) {
				for( auto const& element : workVec ) {
					wikidb.bulkUpdateComparisons(element.first, element.second);
				}
			}
			wikidb.commit();

			auto t2 = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::seconds>(t2 - t1);
			std::cout << "successfully processed " << chunks << " files in " << duration.count() << " sec\n"
            << "processed: " << chunks * count << " files of " << filesNum << " total files" << std::endl;

		}
		wikidb.commit();
    }

    return 0;
}
