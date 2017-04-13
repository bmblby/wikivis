
                       ___                            __      
                      /\_ \                          /\ \__   
                   __ \//\ \     ___     ___     ____\ \  _\  
                 /'_ `\ \ \ \   / __`\  / __`\  /  __\\ \ \/  
                /\ \ \ \ \_\ \_/\ \ \ \/\ \ \ \/\__   \\ \ \_ 
                \ \____ \/\____\ \____/\ \____/\/\____/ \ \__\
                 \/___/\ \/____/\/___/  \/___/  \/___/   \/__/
                   /\____/                                    
                   \_/__/     
                
                     OpenGL framework for fast demo programming

///////////////////////////////////////////////////////////////////////////////


Table of contents:

    01.  ....... About
    02   ....... Developer
    03.  ....... Contributors and Sources
    04.  ....... How to start
    04.1  ...... How to start with GNU/Linux


///////////////////////////////////////////////////////////////////////////////


  01. About:


gloost is a free software, platform independent framework for OpenGL programming.
gloost provides you with a solid math library, a large number of wrappers for
OpenGL entities, an extendable scenegraph, keyframe-based animationsystem,
GLSL support, texture loading and managing, handy GUI system, texture-based
text support, geometry loading, realtime input from keyboard, mouse and joypads,
handy support classes for developers, doxygen support and much more.

The framework has proven itself for prototyping, programming of graphical
demonstrations, games, visualizers and just for fun.


gloost directly supports techniques like:
    - GLSL
    - shadow mapping,
    - FBO,
    - frustum culling,
    - anaglyph stereo,
    - scene hierarchies,
    - PLY, OBJ and GBM loading,
    - VBO
    - multi context redering up to 256 devices

Visit http://www.sourceforge.net/projects/gloost/ to checkout gloost.


gloost is free software (http://www.fsf.org/about/what-is-free-software). By
downloading and using it you agree to the terms of the GNU GENERAL PUBLIC LICENSE
Version 3.

Read the LICENSE.TXT file comming with this file for more details.


///////////////////////////////////////////////////////////////////////////////


  02. Developer:

  gloost is being created by Felix Weißig
  

  Felix Weißig
  
    thesleeper@gmx.net
    http://www.otaco.de


///////////////////////////////////////////////////////////////////////////////


  03. Contributors and Sources:
  

Contributors to the gloost framework:

  Stephan Beck
  Konstantin Silin
  Mathias Schneider
  Henning Gruendl
  Benjamin Bendig
  dynardo GmbH (http://www.dynardo.de)


Source and Ideas taken from:
(references can be found at the particular places in the source):
  
  Mark Harris:     http://www.gpgpu.org
  Ryhor Spivak:    http://blog.rusteddreams.net
  Dominik Göddeke: http://www.mathematik.uni-dortmund.de/~goeddeke/gpgpu/
  Eddy Luten       http://www.http://openglbook.com
  

Thanks to:

  Dr. Jan P. Springer:  http://create.louisiana.edu/index.php/people/research-staff/60-dr-jan-p-springer


gloost uses:

FreeType   ... http://www.freetype.org
FreeImage  ... http://freeimage.sourceforge.net
SDL_Sound  ... http://www.libsdl.org
zeromq     ... http://www.zeromq.org


ASCII Logo created with:
  
  http://www.network-science.de/ascii/
  

///////////////////////////////////////////////////////////////////////////////


  04. How to start


Getting started with gloost is as easy as it gets. For GNU/Linux we provide
project files for the popular Code::Blocks IDE and makefiles to build the lib
and all demos Alternatively there are makefiles available.
  

///////////////////////////////////////////////////////////////////////////////


  04.1 How to start with GNU/Linux


In order to start with gloost you should verify that your GNU/Linux box has the
required dependencies installed. For Ubuntu and derivates just open a Terminal and execute:

  sudo apt-get install freeglut3-dev libglew1.5-dev libglfw-dev libsdl1.2-dev libsdl-image1.2-dev libsdl-mixer1.2-dev libfreeimage-dev libboost-all-dev codeblocks libfreetype6-dev libzmq-dev

Open the the code::blocks ide from Applications/Programming from your start
menu. Inside codeblocks open the workspace file
  
  gloost/tutorials/examples/examples_gnulinux.workspace

Compile and execute the demos one by one by selecting them in the left
project list and pressing F9.

Alternatively you can open a terminal and write

  cd gloost/gnulinux/bin

and

  make


Happy Hacking


///////////////////////////////////////////////////////////////////////////////













