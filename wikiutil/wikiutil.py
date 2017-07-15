"""
    parsing wikipedia xml-dump
    parsing very specific cosinus similarity matrix of wikipedia articles

 * Copyright (c) 2015 Bagrat Ter-Akopyan, bagrat.akopyan@gmail.com

"""

import sys
from lxml import etree
import re
import glob
from string import punctuation


class wikiPageParser:
    xmlFileName = ""
    listOfNS = list()

    global tag_list
    tag_list = ['title', 'ns', 'id', 'text']

    def __init__(self, version, xmlDumpFile, listOfNS):
        self.xmlFileName = xmlDumpFile
        self.listOfNS = listOfNS

    def articleLength(self, text):
        words = re.split(r'[^0-9A-Za-z]+', text)
        for word in words:
            if not word.isalpha():
                words.remove(word)
        return len(words)

    def is_redirect(self, page):
        cleanr = re.compile('<!--.*?-->')
        if isinstance(page['text'], str):
            page['text'] = re.sub(cleanr, '', page['text'])
            text_redirect = re.compile(r"\#REDIRECT?\s*\[\[([^\]]*)\]\]")
            if 'redirect' in page.keys():
                return True
            elif text_redirect.findall(page['text']):
                return True

        disambigTitle = re.compile(r"\(.*(disambiguation)\)")
    def is_disambig(self, page):
        cleanr = re.compile('<!--.*?-->')
        disTitle = re.compile(r"\(.*(?:disambiguation)\)")

        disTemplate1 = re.compile(r"\{\{(?:Disambiguation|Disambig|Disamb|Dab|DAB)\}\}|\{\{(?:Disambiguation|Disambig|Disamb|Dab|DAB)(?:\|human name|\|church|\|county|\|uscounty|\|fish|\|genus|\|geo|\|hospital|\|latin|\|letter number|\|math|\|number|\|plant|\|political|\|road|\|school|\|ship|\|township|\|airport|\|callsign|\|given name|\|surname|\|chinese|\|split)*\}\}")

        disTemplate2 = re.compile(r"\{\{(?:Airport|Biology|Caselaw|Call sign|Chinese title|Genus|Hospital|Letter|Mathematical|Phonetics|Road|School|Species Latin name|Wikipedia)(?:\sdisambiguation)\}\}|\{\{(?:Geodis|Hndis|Hndis-cleanup|Letter-NumberCombDisambig|Mil-unit-dis|Numberdis|)\}\}")

        if isinstance(page['text'], str):
            page['text'] = re.sub(cleanr, '', page['text'])
            if disTitle.findall(page['title']):
                return True
            elif disTemplate1.findall(page['text']) or disTemplate2.findall(page['text']):
                return True

    def get_parents(self, page):
        p = re.compile(r"\[\[Category:(.*?)\]\]")
        return p.findall(page['text'])

    def items(self):
        context = etree.iterparse(self.xmlFileName, events=("start", "end"))
        path = list()       # list containing start and end tags
        record = {}         # object containng page xml
        parents = list()

        redirect_c = 0
        disambig_c = 0
        pages = 0
        ns14_c = 0
        ns0_c = 0
        for event, elem in context:
            if event == 'start':
                path.append(elem.tag.split('}')[1])

            if event == 'end':
                if elem.tag.split('}')[1] == 'id' and 'revision' in path and 'contributor' not in path:
                    record['revid'] = elem.text
                    continue
                elif elem.tag.split('}')[1] == 'id' and 'contributor' in path:
                    continue
                elif elem.tag.split('}')[1] in tag_list:
                    record[elem.tag.split('}')[1]] = elem.text
                    continue
                if elem.tag.split('}')[1] == 'redirect':
                    record['redirect'] = True
                    continue

            if event == 'end' and elem.tag.split('}')[1] == 'page':
                if record['ns'] == '14' or record['ns'] == '0':
                    pages += 1
                    record['title'] = record['title'].strip()
                    if self.is_redirect(record):
                        redirect_c += 1
                        elem.clear()
                        while elem.getprevious() is not None:
                            del elem.getparent()[0]
                        path = []
                        record = {}
                        continue
                    if self.is_disambig(record):
                        disambig_c += 1
                        elem.clear()
                        while elem.getprevious() is not None:
                            del elem.getparent()[0]
                        path = []
                        record = {}
                        continue
                    if record['ns'] == '14':
                        ns14_c += 1
                        # strip 'category:' from begining of title
                        record['title'] = record['title'][9:]
                    else:
                        ns0_c += 1
                    if record['title'][0].isalpha():
                        record['title'] = record['title'].capitalize()

                    if isinstance(record['text'], str):
                        record['parents'] = self.get_parents(record)
                        yield record

                elem.clear()
                while elem.getprevious() is not None:
                    del elem.getparent()[0]
                path = []
                record = {}
        print("Pages: {}\nRedirect: {}".format(pages, redirect_c))
        print("Disambig: {}\nNS 14: {}\nNS 0: {}\n".format(disambig_c, ns14_c, ns0_c))


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
