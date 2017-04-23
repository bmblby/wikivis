### setup wikitool

Setup of wikitool to extract revid and titles from wikipedia xml-file.
Order of execution: first the extraction then sampling if wanted and at last
the cleaningprimary step.
histogram functionality missing!

To be able to execute .wikitool.py please run the commands below.

	mkdir venv
	virtualenv -p python3 venv
	source venv/bin/activate
	pip install -r requirements.txt

now .wikitool.py is executable

### setup createdb

Createdb is used to build fastdb and wikidb out of simMatrix and revid2<title | parent> files.
Project lies inside wikipedia-vis directory.
Execute steps below to build createdb tool.

	cd wikipedia-vis/ext/wikidb/build
	cmake ..
	cmake --build . --debug-output --Wno-dev
