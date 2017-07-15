#!venv/bin/python

"""Pythonic tool

 * Copyright (c) 2015 Bagrat Ter-Akopyan, bagrat.akopyan@gmail.com

"""

__doc__ = """usage:
     wikitool.py extract <version> <xmlDumpFileName> <outDir> [--ns=<namespace>]...
     wikitool.py cleanprimary <simMatrixDir> <revid2title> <revid2parents> <outDir>
     wikitool.py sample <revid2title> <revid2parents> <sampleSize> <outDir> [<strata>...]
     wikitool.py histogramm <revid2title>


    Command:
        extract         Extract page information (revision ids, titles, category parents, lenght of article) from wikipedia xml dump for given ns
        sample          sample random subset from existing revid2title, size determined by number of Articles
        cleanprimary    write tsv file with categories and articles to be processed by createdb
        histogramm      save plot.png file with histogramm of revid2title

    Arguments:
        <xmlDumpFileName>       Input filename of the wikipedia xml dump
        <outDir>                Output directory to write pickle/tsv files
        <simMatrixDir>          Input directory with tsv-formatted similarity matrix (required gzip compressed tsv files)
        <revid2title>           Input filename of pickled dictionary with revision id from ns=0 as key and title as value (extracted from xml dump)
        <revid2parents>         Input filename of pickled dictionary with revision id from ns=14 as key and parents as value (extracted from xml dump)
        <sampleSize>      Size of articles to be randomly sampled
        <strata>                strata boundaries, number of words in article

    Options:
        -h --help               Show this screen
        --version               Show version
        --ns=<namespace>        Namespaces to extract [default: 0]
"""

import sys
import os
import random
import math

from docopt import docopt
import pickle
import operator
import pandas as pd
import numpy as np

import wikiutil

def main():

    arguments = docopt(__doc__, version='0.0.1')

    if arguments['histogramm']:
        revid2title = os.path.realpath(arguments['<revid2title>'])
        if not os.path.exists(revid2title):
            sys.stderr.write("sample: cannot open \'" + revid2title + "\': No such file or directory\n")
            sys.exit(1)
        histogramm(revid2title)

    if arguments['sample']:
        # Check if revid2title, revid2parents, outDir and sampleSize exist
        revid2title = os.path.realpath(arguments['<revid2title>'])
        if not os.path.exists(revid2title):
            sys.stderr.write("sample: cannot open \'" + revid2title + "\': No such file or directory\n")
            sys.exit(1)

        revid2parents = os.path.realpath(arguments['<revid2parents>'])
        if not os.path.exists(revid2parents):
            sys.stderr.write("sample: cannot open \'" + revid2parents + "\': No such file or directory\n")
            sys.exit(1)

        sampleSize = int(arguments['<sampleSize>'])
        if not type(sampleSize) == int:
            sys.stderr.write("sample: please add number of Articles to be sampled\n")
            sys.exit(1)

        outDir = os.path.realpath(arguments['<outDir>'])
        if not os.path.exists(outDir):
            sys.stderr.write("sample: cannot open \'" + outDir + "\': No such file or directory\n")
            sys.exit(1)
        #convert strata String to integers
        if arguments['<strata>']:
            stratas = list()
            for strata in arguments['<strata>']:
                stratas.append(int(strata))
            strataSampling(revid2title, revid2parents, sampleSize, outDir, stratas)
        else:
            randomSampling(revid2title, revid2parents, sampleSize, outDir)

    if arguments['extract']:
        # Check if xmlFilename and outDir exist
        if arguments['<version>']:
            version = arguments['<version>']
            # sys.stderr.write("""extract: Please insert number!\nSee first line in wikipedia xml file
            # http://www.mediawiki.org/xml/export-<VERSION-NUMBER>/\nPlease insert version number with point not commata!\n""")
            # sys.exit(1)

        xmlFileName = os.path.realpath(arguments['<xmlDumpFileName>'])
        if not os.path.exists(xmlFileName):
            sys.stderr.write("extract: cannot open \'" + xmlFileName + "\': No such file or directory\n")
            sys.exit(1)

        outDir = os.path.realpath(arguments['<outDir>'])
        if not os.path.exists(outDir):
            sys.stderr.write("extract: cannot open \'" + outDir + "\': No such file or directory\n")
            sys.exit(1)

        #convert namespace strings to integers
        listOfNS = list()
        for el in arguments['--ns']:
            listOfNS.append(int(el))
        extractPages(version, xmlFileName, listOfNS, outDir)

    if arguments['cleanprimary']:
        # Check if simMatrixDir, ns0fileName and outDir exist
        simMatrixDir = os.path.realpath(arguments['<simMatrixDir>'])
        if not os.path.exists(simMatrixDir):
            sys.stderr.write("cleanprimary: cannot open \'" + simMatrixDir + "\': No such file or directory\n")
            sys.exit(1)

        revid2title = os.path.realpath(arguments['<revid2title>'])
        if not os.path.exists(revid2title):
            sys.stderr.write("cleanprimary: cannot open \'" + revid2title + "\': No such file or directory\n")
            sys.exit(1)

        revid2parents = os.path.realpath(arguments['<revid2parents>'])
        if not os.path.exists(revid2parents):
            sys.stderr.write("cleanprimary: cannot open \'" + revid2parents + "\': No such file or directory\n")
            sys.exit(1)

        outDir = os.path.realpath(arguments['<outDir>'])
        if not os.path.exists(outDir):
            sys.stderr.write("cleanprimary: cannot open \'" + outDir + "\': No such file or directory\n")
            sys.exit(1)
        cleanprimary(simMatrixDir, revid2title, revid2parents, outDir)

def randomSampling(revid2title, revid2parents, sampleSize, outDir):
    '''
        simple random Sampling of size: sampleSize
        shuffle article dict
        save shuffled dict to new pickle for further processing by cleanprimary

    '''
    no_parents = list()
    ns0_revid2title = dict()
    ns0_revid2parents = dict()
    sample_revid2title = dict()
    sample_revid2parents = dict()

    with open(revid2title, "rb") as inFile:
        ns0_revid2title = pickle.load(inFile)

    with open(revid2parents, "rb") as inFile:
        ns0_revid2parents = pickle.load(inFile)

    #shuffle revid keys to get random sample
    revids = list(ns0_revid2title.keys())
    random.shuffle(revids)
    revids = revids[0:sampleSize]
    for revid in revids:
        sample_revid2title[revid] = ns0_revid2title[revid]
        if revid in ns0_revid2parents:
            sample_revid2parents[revid] = ns0_revid2parents[revid]
        else:
        # add these articles also to parent dict bur with empty list??
            no_parents.append(revid)

    print('number if sampled articles without parents: ' + str(len(no_parents)))
    print("Saving pickle dicts in %s." % outDir)
    pickle.dump(sample_revid2title, open(outDir + "/sample_ns0_revid2title.p", "wb"))
    pickle.dump(sample_revid2parents, open(outDir + "/sample_ns0_revid2parents.p", "wb"))
    print("Done.")

def strataSampling(revid2title, revid2parents, sampleSize, outDir, strata):
    '''
        Input:  two pickles with articles
                dir with simMatrix tsv files
                size of subset to be sampled
                dir to pickle sampled dataFrames
                bucket sizes to be stratified

        sample stratified subset of all articles
        save to sampled dict for further proccesing by cleanprimary
    '''

    # loading article pickle and create DataFrames from dicts
    title = pd.read_pickle(revid2title)
    parents = pd.read_pickle(revid2parents)
    titleFrame = pd.DataFrame({'revid' : [el for el in iter(title.keys())],
                                'Article Name' : [el[0] for el in iter(title.values())],
                                'words' : [el[1] for el in iter(title.values())]})
    titleFrame = titleFrame.set_index('revid')
    parentsFrame = pd.DataFrame.from_dict(parents , orient='index')
    parentsFrame.index.name = 'revid'

    print('finished building DataFrames: ')
    print('length titleFrame: ' + str(len(titleFrame.index)))
    print('length parentsFrame: ' + str(len(parentsFrame.index)))

    #loop over strata an get list with dataframe subsets
    #hardcoded numbers to get right boundaries for min and max
    subsets = list()
    for index, el in enumerate(strata):
        if index + 1 < len(strata):
            if index == 0:
                subsets.append(getstrata(0, strata[index], titleFrame, parentsFrame))
            subsets.append(getstrata(strata[index], strata[index+1], titleFrame, parentsFrame))
        else:
            subsets.append(getstrata(strata[index], titleFrame.max(axis=1).max(), titleFrame, parentsFrame))

    # calculate sampling size according to strata
    subSizes = [len(el) for el in subsets]
    subPercentage = pd.Series([(el * 100 / len(titleFrame.index)) for el in subSizes])
    subSampleSize = pd.Series([math.ceil((sampleSize * el) / 100) for el in subPercentage])

    sampleTitle = pd.DataFrame()
    for index, el in enumerate(subSampleSize):
        #subsets = list of dataFrames by strata size
        sub = subsets[index]
        __ = pd.DataFrame(sub.sample(n=el, random_state=np.random.RandomState(), replace=True))
        sampleTitle = sampleTitle.append(__)

    #slice of parentFrame containing only sampled data
    sampleParents = parentsFrame.loc[sampleTitle.index]

    #convert dataFrames to dicts for cleanprimary to proccess
    sample_ns0_revid2title = sampleTitle.T.to_dict('list')
    sample_ns0_revid2parents = sampleParents.T.to_dict('list')


    #save sampled Frames to tsv and pickle for further proccesing
    pickle.dump(sample_ns0_revid2title, open(outDir + "/sample_ns0_revid2title.p", "wb"))
    pickle.dump(sample_ns0_revid2parents, open(outDir + "/sample_ns0_revid2parents.p", "wb"))
    sampleTitle.to_csv(outDir + '/sample_ns0_revid2title.tsv', sep='\t', encoding='utf-8')
    sampleParents.to_csv(outDir + '/sample_ns0_revid2parents.tsv', sep='\t', encoding='utf-8')
    print('\n# finished saving sampled dict:\n'+ \
            'sample_ns0_revid2title\n' + 'sample_ns0_revid2parents\n' + 'to directory: ' + outDir )

#helper functions get subsets from DataFrame
def getstrata(mini, maxi, titleFrame, parentsFrame):
    titleFrame.sort_values('words', ascending=True, inplace=True)
    return titleFrame[(titleFrame.words >= mini) & (titleFrame.words <= maxi)]

def extractPages(version, xmlFileName, listOfNS, outDir):
    '''
        Save all Categories (pages ns 14) found in xml dump in tsv files
        Save all Articles (pages ns 0) found in xml dump in  pickle files

        Result: tsv file with Categories

    '''
    ns0_pageid2revid = dict()
    ns0_revid2title = dict()
    ns0_revid2parents = dict()

    wikiparser = wikiutil.wikiPageParser(version, xmlFileName, listOfNS)
    count = 0
    for elem in wikiparser.items():
        # print('id', elem['id'], 'title', elem['title'], 'revid', elem['revid'])
        ns = elem['ns']
        pageid = elem['id']
        revid = elem['revid']
        title = elem['title']
        parents = elem['parents']
        text = elem['text']
        words = wikiparser.articleLength(text)

        if not parents:
            with open(outDir + "/no_parents.log", "a") as logFile:
                 logFile.write(str(ns) + "\t" + str(revid) + "\t" + title + "\n")

        if (ns == '0'):
            with open(outDir + "/pageid2revid.tsv", "a") as outF:
                outF.write(str(pageid) + "\t" + str(revid) + "\t" + str(title) + "\n")
            ns0_pageid2revid.setdefault(pageid, revid)
            ns0_revid2title.setdefault(revid, [title, words])
            #BUG: if parents list is empty -> no entry in dict
            # why not save directly ns 0 to revid2*.tsvs ??
            for el in parents:
                ns0_revid2parents.setdefault(revid, []).append(el)

        elif (ns == '14'):
            with open(outDir + "/revid2title.tsv", "a") as outF:
                outF.write(str(ns) + "\t" + str(revid) + "\t" + title + "\t" + str(words) + "\n")
            with open(outDir + "/revid2parents.tsv", "a") as outF:
                outF.write(str(ns) + "\t" + str(revid))
                for el in parents:
                    outF.write("\t" +  el)
                outF.write("\n")

    print("Saving pickle dicts in %s." % outDir)
    pickle.dump(ns0_pageid2revid, open(outDir + "/pageid2revid.p", "wb"))
    pickle.dump(ns0_revid2title, open(outDir + "/revid2title.p", "wb"))
    pickle.dump(ns0_revid2parents, open(outDir + "/revid2parents.p", "wb"))
    print("Done.")

def histogramm(revid2title):
    '''
        plot histogramm according to sturges formula and save to pdf
    '''
    title = pd.read_pickle(revid2title)
    titleFrame = pd.DataFrame({'revid' : [el for el in iter(title.keys())],
                                'Article Name' : [el[0] for el in iter(title.values())],
                                'words' : [el[1] for el in iter(title.values())]})
    titleFrame = titleFrame.set_index('revid')

    plot = titleFrame.plot.hist(by='words')
    fig = plot.get_figure()
    fig.savefig('plot.png')

def cleanprimary(simMatrixDir, revid2title, revid2parents, outDir):
    '''
        Check if first colum of matrix is in ns 0, if yes append revid with title and parents
        to existing tsv with ns 0 prefix
        ATTENTION! outDir should contain preprocessed tsv with categories

        append articles to existing tsv and save pickle with list of all articles

        Result: tsv file with Categories and Articles

    '''
    ns0_set = set()
    ns0_revid2title = dict()
    ns0_revid2parents = dict()

    with open(revid2title, "rb") as inFile:
        ns0_revid2title = pickle.load(inFile)
    with open(revid2parents, "rb") as inFile:
        ns0_revid2parents = pickle.load(inFile)

    matrixParser = wikiutil.matrixParser(simMatrixDir)
    for line, filename in matrixParser.items():
        revid = int(line[0])
        # uncomment code for rewriting simMatrixDir in this case all pages ns 0
#        if revid in ns0_revid2title:
#            #write new tsv sim matrix dir only containing namespace 0
#            filepath, filename = os.path.split(filename)
#            filepath = os.path.join(filepath + '/sampling/')
#            os.makedirs(filepath, exist_ok=True)
#            filename = os.path.join(filepath + filename)
#            with open(filename, 'a+') as outFile:
#                for el in line:
#                    outFile.write(str(el)+ '\t')
#                outFile.write('\n')

        if revid not in ns0_set:
            ns0_set.add(revid)
            if revid in ns0_revid2parents:
                with open(outDir + "/revid2title.tsv", "a") as outF:
                    outF.write(str(0) + "\t" + str(revid) + "\t" + ns0_revid2title[revid][0] \
                    + '\t' + str(ns0_revid2title[revid][1]) + "\n")
                with open(outDir + "/revid2parents.tsv", "a") as outF:
                    outF.write(str(0) + "\t" + str(revid))
                    for el in ns0_revid2parents[revid]:
                        if el is not None:
                            outF.write("\t" + str(el))
                    outF.write("\n")

            elif revid in ns0_revid2title and revid not in ns0_revid2parents:
                with open(outDir + "/revid2title.tsv", "a") as outF:
                    outF.write(str(0) + "\t" + str(revid) + "\t" + ns0_revid2title[revid][0] \
                    + '\t' + str(ns0_revid2title[revid][1]) + "\n")

                with open(outDir + "/revid2parents.tsv", "a") as outF:
                    outF.write(str(0) + "\t" + str(revid) + "\n")
        else:
            with open(outDir + "/duplicate_lines.log", "a") as f:
                f.write(str(line) + "\n")


    print("Found", len(ns0_set), "in similarity matrix")
    print("Saving pickle dicts in %s." % outDir)
    pickle.dump(ns0_set, open(outDir + "/ns0_set.p", "wb"))
    print("Done.")

if __name__ == '__main__':
    main()
