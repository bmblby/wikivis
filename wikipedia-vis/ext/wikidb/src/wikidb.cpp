/*
 * Copyright (c) 2015 Bagrat Ter-Akopyan, bagrat.akopyan@gmail.com
 *                    Giuliano Castiglia, castiglia.giuliano@gmail.com
 */

#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <cstdlib>
#include <iostream>
#include <chrono>
#include <fstream>
#include <utility>
#include <vector>
#include <string>
#include <typeinfo>
#include <typeindex>

#include "wikidb.h"
#include "contract.h"
#include "record_not_found.h"

REGISTER(Page);
REGISTER(Article);
REGISTER(Category);


WikiDB::WikiDB(const std::string& DBfilename) {
    // TODO(bagrat): perform error handling if the DB files do not exist
    // Lookup error handling of fastDB
    _db.open(_T(DBfilename.c_str()));
    //_db.setFileSizeLimit(0); // unlimited filesize
}

WikiDB::~WikiDB() {
    _db.close();
}

bool
WikiDB::articleExistsRevid(uint32_t revid) const {
    //TODO change to two diffrent functions
    dbCursor<Article> articleCursor;
    dbQuery q;
    q = "revid=", revid;
    if(articleCursor.select(q) > 0)
        return true;
    else
        return false;
}
bool
WikiDB::articleExistsIndex(uint32_t index) const {
    dbCursor<Article> articleCursor;
    dbQuery q;
    q = "index=", index;
    if(articleCursor.select(q) > 0)
        return true;
    else
        return false;
}

bool
WikiDB::categoryExistsRevid(uint32_t revid) const {
    dbCursor<Category> categoryCursor;
    dbQuery q;
    q = "revid=", revid;
    if(categoryCursor.select(q) > 0)
        return true;
    else
        return false;
}
bool
WikiDB::categoryExistsIndex(uint32_t index) const {
    dbCursor<Category> categoryCursor;
    dbQuery q;
    q = "index=", index;
    if(categoryCursor.select(q) > 0)
        return true;
    else
        return false;
}

bool
WikiDB::articleExists(std::string title) const {
    dbCursor<Article> articleCursor;
    dbQuery q;
    q = "title=", title;
    return(articleCursor.select(q) > 0);
}

bool
WikiDB::categoryExists(std::string title) const {
    dbCursor<Category> categoryCursor;
    dbQuery q;
    q = "title=", title;
    return(categoryCursor.select(q) > 0);
}

void
WikiDB::insertArticle(uint32_t revid, std::string const& title) {
    // TODO(bagrat) is this desired behaviour?
    // if an article with this revid already exists, just bail out
    if (articleExistsRevid(revid)) {
        std::cerr << "Article with revid " << revid
                  << " already exists in table" << std::endl;
        return;
    }

    Article article;
    article.revid = revid;
    article.title = title.c_str();
    insert(article);

    _db.commit();

    return;
}

void
WikiDB::insertCategory(uint32_t revid, std::string const& title) {
    // TODO(bagrat) see above about behaviour
    if (categoryExistsRevid(revid)) {
        std::cerr << "Category with revid " << revid
                  << " already exists in table" << std::endl;
        return;
    }

    Category category;
    category.revid = revid;
    category.title = title.c_str();
    insert(category);

    _db.commit();

    return;
}

void
WikiDB::bulkInsertArticle(
    std::vector<std::pair<uint32_t, std::pair<std::string, uint32_t>>> const& articles) {

    std::chrono::high_resolution_clock::time_point t1, t2, t3;
    t1 = std::chrono::high_resolution_clock::now();

    for (auto const& el : articles) {
        Article article;
        article.revid = el.first;
        article.title = std::get<0>(el.second).c_str();
        article.words = std::get<1>(el.second);
         _db.insert(article);
    }

    auto duration = std::chrono::duration_cast<std::chrono::seconds>(t2 - t1);
    dbCursor<Article> articleCursor(dbCursorViewOnly);
    int articleNumber = articleCursor.select();
    std::cout << "Inserted " << articleNumber
    << " articles in " << duration.count() << " seconds" << std::endl;

    _db.commit();

    //time
    t3 = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::seconds>(t3 - t2);
    std::cout << "Time for commit: " << duration.count() << " seconds" << std::endl;

    return;
}

void
WikiDB::bulkInsertCategory(
        std::vector< std::pair<uint32_t, std::string> > const& categories) {
    std::chrono::high_resolution_clock::time_point t1, t2, t3;
    t1 = std::chrono::high_resolution_clock::now();

    for (auto const& el : categories) {
        Category category;
        category.revid = el.first;
        category.title = (el.second).c_str();
        _db.insert(category);
    }

    t2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(t2 - t1);
    dbCursor<Category> categoryCursor(dbCursorViewOnly);
    int categoryNumber = categoryCursor.select();
    std::cout << "Inserted " << categoryNumber
    << " categories in " << duration.count() << " seconds" << std::endl;

    _db.commit();
    t3 = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::seconds>(t3 - t2);
    std::cout << "Time for commit: " << duration.count() << " seconds" << std::endl;

    return;
}

void
WikiDB::updateArticleParents(uint32_t revid, std::string const& parent) {
    dbQuery q;
    q = "revid=", revid;
    dbCursor<Article> articleCursor(dbCursorForUpdate);

    if (articleCursor.select(q) > 0) {
        dbCursor<Category> parentCursor;
        dbQuery q;
        q = "title=", parent;

        if (parentCursor.select(q) > 0) {
            dbReference<Category> parentReference = parentCursor.currentId();
            articleCursor->parents.append(parentReference);

            articleCursor.update();
            _db.commit();
        }
    }

    return;
}

void
WikiDB::updateCategoryParents(uint32_t revid, std::string const& parent) {
    dbQuery q;
    q = "revid=", revid;
    dbCursor<Category> categoryCursor(dbCursorForUpdate);

    if (categoryCursor.select(q) > 0) {
        dbCursor<Category> parentCursor;
        dbQuery q;
        q = "title=", parent;

        if (parentCursor.select(q) > 0) {
            dbReference<Category> parentReference = parentCursor.currentId();
            categoryCursor->parents.append(parentReference);

            categoryCursor.update();
            _db.commit();
        }
    }

    return;
}

void
WikiDB::bulkUpdateArticleParents(uint32_t revid,
        std::vector<std::string> const& parents) {
    dbQuery q;
    q = "revid=", revid;
    dbCursor<Article> articleCursor(dbCursorForUpdate);

    if (articleCursor.select(q) > 0) {
        for (auto parent : parents) {
            dbCursor<Category> parentCursor;
            dbQuery q;
            q = "title=", parent;
            if (parentCursor.select(q) > 0) {
                dbReference<Category> parentReference = parentCursor.currentId();
                // TODO(bagrat) proove if parentReference already exists in articleCursor->parents
                (articleCursor->parents).append(parentReference);
            } else{} // TODO(bagrat) which parents were not found?
        }
        articleCursor.update();
    }
}

void
WikiDB::bulkUpdateCategoryParents(uint32_t revid,
        std::vector<std::string> const& parents) {
    dbQuery q;
    q = "revid=", revid;
    dbCursor<Category> categoryCursor(dbCursorForUpdate);
    if (categoryCursor.select(q) > 0) {
        for (auto parent : parents) {
            dbCursor<Category> parentCursor;
            dbQuery q;
            q = "title=", parent;
            if (parentCursor.select(q) > 0) {
                dbReference<Category> parentReference = parentCursor.currentId();
                // TODO(bagrat) see above in line 192
                (categoryCursor->parents).append(parentReference);
            } else {} // TODO(bagrat) which parents were not found?
        }
        categoryCursor.update();
    }
}

void
WikiDB::bulkUpdateComparisons(
        uint32_t refRevid, std::vector<int64_t> const& comparisons) {
    dbQuery q;
    q = "revid=", refRevid;
    dbCursor<Article> articleCursor(dbCursorForUpdate);
    if (articleCursor.select(q) > 0) {
        (articleCursor->comparisons).assign(comparisons.data(), comparisons.size(), true);
        articleCursor.update();
    }
}

void
WikiDB::commit() {
	_db.commit();
}

Article
WikiDB::getArticle(uint32_t index) const {
    if (index > 0) {
        dbQuery q;
        q = "index=", index;
        dbCursor<Article> articleCursor;
        articleCursor.select(q);

        if (articleCursor.get() == NULL) {
            throw record_not_found("article index: "
                    + std::to_string(index) + " not in Table");
        } else {
            return *articleCursor.get();
        }
    } else {
        throw record_not_found("Index starts from 1 not 0!");
    }
}

Category
WikiDB::getCategory(uint32_t index) const {
    if (index > 0) {
        dbQuery q;
        q = "index=", index;
        dbCursor<Category> categoryCursor;
        categoryCursor.select(q);

        if (categoryCursor.get() == NULL) {
            throw record_not_found("category revid: "
                    + std::to_string(index) + " not found in Table");
        } else {
            return *categoryCursor.get();
        }
    } else {
      throw record_not_found("Index starts from 1 not 0!");
    }
}

Article
WikiDB::getArticleByRevid(uint32_t revid) const {
	dbQuery q;
	q = "revid=", revid;
	dbCursor<Article> artCur;
	artCur.select(q);
	return *artCur.get();
}

Category
WikiDB::getCategoryByRevid(uint32_t revid) const {
	dbQuery q;
	q = "revid=", revid;
	dbCursor<Category> catCur;
	catCur.select(q);
	return *catCur.get();
}

Article
WikiDB::getArticleByName(std::string name) const {
    dbQuery q;
    q = "title=", name;
    dbCursor<Article> artCur;
    if(!artCur.select(q))
        std::cerr << "article: " << name << " not found!" << std::endl;
    return *artCur.get();
}

Category
WikiDB::getCategoryByName(std::string name) const {
    dbQuery q;
    q = "title=", name;
    dbCursor<Category> catCur;
    if(!catCur.select(q))
        std::cerr << "Category: " << name << " not found!" << std::endl;
    return *catCur.get();
}

std::vector<Article>
WikiDB::getArticleChildren(uint32_t index) const {
	dbQuery q;
	q = "index=", index;
	dbCursor<Category> catCur;

	catCur.select(q);
	std::vector<uint32_t> children = catCur->getChildren();
	std::vector<Article> artChildren(0);
	for (auto i : children) {
		if(this->articleExistsIndex(i)) {
			artChildren.push_back(getArticleByRevid(i));
		}
	}
	return artChildren;
}

std::vector<Category>
WikiDB::getCategoryChildren(uint32_t index) const {
	dbQuery q;
	q = "index=", index;
	dbCursor<Category> catCur;

	catCur.select(q);
	std::vector<uint32_t> children = catCur->getChildren();
	std::vector<Category> catChildren(0);
	for (auto i : children) {
		if(this->categoryExistsIndex(i)) {
			catChildren.push_back(getCategoryByRevid(i));
		}
	}
	return catChildren;
}

std::vector<SimPair>
WikiDB::getComparisons(uint32_t index) const {
    dbCursor<Article> articleCursor;
    dbQuery q;
    q = "index=", index;

    std::vector<SimPair> simPairs;
    if (articleCursor.select(q) > 0) {
        simPairs.resize((articleCursor->comparisons).length());
        for (std::size_t i = 0; i < (articleCursor->comparisons).length(); ++i) {
            int64_t simpair = (articleCursor->comparisons)[i];
            SimPair sp(simpair);

            simPairs[i] = sp;
        }
    }
    return simPairs;
}

int
WikiDB::sizeArticles() const {
	dbCursor<Article> artCur;
	return artCur.select();
}

int
WikiDB::sizeCategories() const {
	dbCursor<Category> catCur;
	return catCur.select();
}
