### Wikipedia Visualization

Application to visualize categories and articles from wikipedia.
This app renders a small subset of the wikipedia categorygraph with an initial
category as seed. The graph is explorable for the user to dig deeper in different
subcategories. With a chosen set ob categories the user is able to browse
through the underlying articles inside the categries.
A huge matrix of article similarities based on paragraph similarities is
explorable by this application.

#### building

before building with cmake
* build gloost
* build wikidb
	* copy libfastbd into contrib/lib
* get nanogui submodule

mkdir build
cd build
cmake ..
make


#### Common Tasks

##### add primitve to application
* setup vao and vbo handle in renderer.h
* generate vao and vbo inside initialize()
* inside draw() bind vao and vbo
	* call vertexAttribPointer()
	* call drawArrays()
