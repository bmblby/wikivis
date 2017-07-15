"""Pythonic tool for:
    parsing wikipedia xml-dump
    parsing very specific cosinus similarity matrix of wikipedia articles

 * Copyright (c) 2015 Bagrat Ter-Akopyan, bagrat.akopyan@gmail.com

"""

import sys
import xml.etree.cElementTree as ET
import re
import glob
from string import punctuation

class Page:
    title = ""
    ns = -1
    pageid = -1
    revid = -1
    parents = list()
    text = ""

    def __init__(self, title, ns, pageid, revid, parents, text):
        self.title = title
        self.ns = ns
        self.pageid = pageid
        self.revid = revid
        self.updateParents(parents)
        self.text = text

    def updateParents(self, parents):
        self.parents.clear()
        for el in parents:
            self.parents.append(el)

    def getTitle(self):
        return self.title

    def getNS(self):
        return self.ns

    def getPageid(self):
        return self.pageid

    def getRevid(self):
        return self.revid

    def getParents(self):
        return self.parents

    def getText(self):
        return self.text

class wikiPageParser:
    xmlFileName = ""
    listOfNS = list()

    # namespace for every tag
    # NOTE xmlns number missing input from user. version = "0.8"
    xmlns = "{http://www.mediawiki.org/xml/export-"
    pagetag = xmlns + "page"
    titletag = xmlns + "title"
    nstag = xmlns + "ns"
    revisiontag = xmlns + "revision"
    idtag = xmlns + "id"
    redirecttag = xmlns + "redirect"
    texttag = xmlns + "text"

    def __init__(self, version, xmlDumpFile, listOfNS):
        self.xmlns = self.xmlns + str(version) + "/}"
        self.pagetag = self.xmlns + "page"
        self.titletag = self.xmlns + "title"
        self.nstag = self.xmlns + "ns"
        self.revisiontag = self.xmlns + "revision"
        self.idtag = self.xmlns + "id"
        self.redirecttag = self.xmlns + "redirect"
        self.texttag = self.xmlns + "text"
        self.xmlFileName = xmlDumpFile
        self.updateNS(listOfNS)

    def updateNS(self, listOfNS):
        self.listOfNS.clear()
        for el in listOfNS:
            self.listOfNS.append(el)

    def articleLength(self, text):
        words = re.split(r'[^0-9A-Za-z]+', text)
        for word in words:
            if not word.isalpha():
                words.remove(word)
        return len(words)


    def items(self):
        context = ET.iterparse(self.xmlFileName, events=("start", "end"))
        cit = iter(context)

        path = list()
        title = ""
        text = ""
        ns = -1
        pageid = -1
        revid = -1
        parents = list()

        p = re.compile(r"\[\[Category:(.*?)\]\]")
        # regex to get pages that are redirects, if no redirect tag
        # see wikimedia source code
        r = re.compile(r"\#REDIRECT(?:S|ED|ION)?\s*(?: :|\sTO|=)?\s*\[\[([^\]]*)\]\]", re.IGNORECASE)

        # regex from wikimedia to match disambiguous pages |REMOVED from regex: |dab|surname|
        disambigTitle = re.compile(r"\(.*(disambiguation)\)")
        disambigTemplate = re.compile(r"{\{\s*disambiguation|disambig|disambig-cleanup|disamb|shipindex|hndis|geodis|schooldis|hospitaldis|mathdab|numberdis|given name\s*(?:\|.*)?\s*\}\}", re.IGNORECASE)

        count = 0
        for event, elem in cit:
            if event == 'start':
                path.append(elem.tag)

            if event == 'end':
                if elem.tag == self.pagetag:
                    ns = -1
                    pageid = -1
                    revid = -1
                    elem.clear()

                if elem.tag == self.titletag:
                    title = elem.text

                if elem.tag == self.nstag:
                    ns = int(elem.text)

                if elem.tag == self.idtag and path[-2] == self.pagetag:
                    pageid = int(elem.text)

                if elem.tag == self.texttag:
                    text = elem.text

                if elem.tag == self.idtag and self.revisiontag == path[-2]:
                    revid = int(elem.text)

                if elem.tag == self.redirecttag:
                    count += 1
                    continue

                if elem.tag == self.texttag and ns in self.listOfNS:
                    if not elem.text:
                        continue

                    title = title.strip()
                    if ns == 14:
                        title = title[9:]

                    #remove white spaces at the beginning and the end of the title
                    title = title.strip()
                    disTitle = disambigTitle.findall(title)
                    if disTitle:
                        count += 1
                        continue
                    if record['ns'] == '14':
                        ns14_c += 1
                        # strip 'category:' from begining of title
                        record['title'] = record['title'][9:]
                    else:
                        ns0_c += 1
                    if isinstance(record['text'], str):
                        record['parents'] = self.get_parents(record)
                        yield record

                    redirect = r.findall(elem.text)
                    if redirect:
                        count += 1
                        continue

                    if title[0].isalpha():
                        title = title.capitalize()

                    parents = p.findall(elem.text)
                    for i in range(len(parents)):
                        parents[i] = parents[i].split("|")[0]
                        parents[i] = parents[i].strip()
                        if len(parents[i]) and parents[i][0].isalpha():
                                parents[i] = parents[i].capitalize()

                    page = Page(title, ns, pageid, revid, parents, text)
                    yield page
                path.pop()
        print("Total number of pages skipped:", count)

class matrixParser:
    simMatrixDir = ""
    filelist = list()

    def __init__(self, simMatrixDir):
        self.simMatrixDir = simMatrixDir
        self.filelist = glob.glob(self.simMatrixDir + "/*.tsv.*")

    def items(self):
        for filename in sorted(self.filelist):
            with open(filename, "r") as inFile:
                print("Processing", filename, "...")
                for line in inFile:
                    line = line.split()
                    yield (line, filename)
