
/*
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

                             http://www.gloost.org

    This file is part of the gloost framework. You can use it in parts or as
       whole under the terms of the GPL (http://www.gnu.org/licenses/#GPL).
*/



#ifndef H_GLOOST_RECT
#define H_GLOOST_RECT



// gloost system includes
#include <gloost/gloostConfig.h>
#include <gloost/gloostMath.h>


// cpp includes
#include <string>
#include <memory>



namespace gloost
{


  //  An axis aligned rectangle

class Rect
{
	public:

    /// a std::shared_ptr of an Rect instance
    typedef std::shared_ptr<Rect>       shared_ptr;
    typedef std::shared_ptr<const Rect> const_shared_ptr;


    // class constructor
    Rect();

    Rect(const mathType& posX,  const mathType&  posY,
         const mathType& width, const mathType&  height);

    Rect(const Rect& rect);


    // class destructor
	  ~Rect();


    // ...
	  const mathType& getPosX() const;
	  const mathType& getPosY() const;

    // ...
	  mathType getPosX2 () const;
	  mathType getPosY2 () const;

    // ...
	  const mathType& getWidth() const;
	  const mathType& getHeight() const;


	private:

    mathType _posX;
    mathType _posY;
    mathType _width;
    mathType _height;

};


} // namespace gloost


#endif // GLOOST_EMPTYCLASS_H


