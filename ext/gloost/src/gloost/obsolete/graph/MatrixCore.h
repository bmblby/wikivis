
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



#ifndef GLOOST_MATRIXCORE_H
#define GLOOST_MATRIXCORE_H



// gloost includes
#include <gloost/graph/Core.h>

#include <gloost/Vector3.h>
#include <gloost/Matrix.h>


// cpp includes


namespace gloost
{
namespace graph
{



class Action;


/// MatrixCore for transformations

class MatrixCore : public Core
{
	public:

		// class constructor
		MatrixCore();

		// class constructor
		MatrixCore(const Matrix& matrix);


		~MatrixCore();



		/* virtual */ void enterCore (const Action* action);
		/* virtual */ void exitCore  (const Action* action);


    /// set the matrix of this MatrixCore
		void setMatrix (const Matrix& matrix /*, bool threadsafe = true*/);


		/// returns the matrix of this MatrixCore
		const Matrix&  getMatrix () const;
		/// returns the matrix of this MatrixCore
    Matrix&  getMatrix ();


  private:

    Matrix _matrix;
};


} // namespace graph
} // namespace gloost


#endif // GLOOST_MATRIXCORE_H
