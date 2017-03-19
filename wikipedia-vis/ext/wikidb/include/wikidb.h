/*
 * Copyright (c) 2015 Bagrat Ter-Akopyan, bagrat.akopyan@gmail.com
 *                    Giuliano Castiglia, castiglia.giuliano@gmail.com
 */

#ifndef WIKIDB_WIKIDB_H_
#define WIKIDB_WIKIDB_H_

#include <fastdb.h>
#include <stdint.h>
#include <vector>
#include <utility>
#include <iosfwd>
#include <string>

#include "./contract.h"
#include "./SimPair.h"

class WikiDB {
 public:
    explicit WikiDB(std::string const& DBfilename);
    ~WikiDB();

    // create database
    void insertArticle(uint32_t revid, std::string const& title);
    void insertCategory(uint32_t revid, std::string const& title);

    void bulkInsertArticle(std::vector<std::pair<uint32_t,
                        std::pair<std::string, uint32_t> > >
                        const& articles);
    void bulkInsertCategory(std::vector<std::pair<uint32_t, std::string> >
                        const& categories);

    void updateArticleParents(uint32_t revid, std::string const& parent);
    void updateCategoryParents(uint32_t revid, std::string const& parent);

    void bulkUpdateArticleParents(uint32_t revid,
            std::vector<std::string> const& parents);
    void bulkUpdateCategoryParents(uint32_t revid,
            std::vector<std::string> const& parents);
    void bulkUpdateComparisons(uint32_t revid,
            std::vector<uint32_t> const& comparisons);

    void commit();

    // get page by index
    Article getArticle(uint32_t index) const;
    Category getCategory(uint32_t index) const;

    Article getArticleByRevid(uint32_t revid) const;
    Category getCategoryByRevid(uint32_t revid) const;

    Article getArticleByName(std::string name) const;
    Category getCategoryByName(std::string name) const;

	std::vector<Article> getArticleChildren(uint32_t index) const;
	std::vector<Category> getCategoryChildren(uint32_t index) const;

	int sizeArticles() const;
	int sizeCategories() const;

    std::vector<SimPair> getComparisons(uint32_t idx) const;

    bool articleExists(uint32_t revid) const;
    bool categoryExists(uint32_t revid) const;
    bool articleExists(std::string title) const;
    bool categoryExists(std::string title) const;

 public:
    dbDatabase _db;
};

#endif  // WIKIDB_WIKIDB_H_
