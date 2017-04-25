
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



#ifndef H_GLOOST_MATRIXSTACK
#define H_GLOOST_MATRIXSTACK



// gloost system includes
#include <gloost/Matrix.h>
#include <gloost/Dcs.h>
#include <gloost/Vector3.h>



// cpp includes
#include <string>
#include <stack>



namespace gloost
{


  //  This is a reimplementation of the openGL matrix stack

class MatrixStack
{
	public:
    /// class constructor
    MatrixStack();

    /// class destructor
	  ~MatrixStack();


	  // pushs a copy of the current top matrix into the stack
	  void push();

	  // removes the top matrix from the stack
	  void pop();

	  /// returns current matrix
	  Matrix& top();

	  /// returns current matrix
	  const Matrix& top() const;

	  /// clears the matrix stack
	  void clear();


	  // sets the current top matrix to identity
	  void loadIdentity();

	  // multiplicate a matrix to the top matrix
	  void multMatrix(const Matrix& matrix);

	  // replaces the top matrix with another matrix
	  void loadMatrix(const Matrix& matrix);


	  // multiplicate the current top matrix with a translation matrix
	  void translate(mathType x, mathType y, mathType z);

	  // multiply the current top matrix with a translation matrix
	  void translate(Vector3 offset);


	  // multiply the current top matrix with a rotation matrix
	  void rotate(mathType radAngle, mathType x, mathType y, mathType z);

	  // multiply the current top matrix with a rotation matrix
	  void rotate(mathType radAngleX, mathType radAngleY, mathType radAngleZ);

	  // multiply the current top matrix with a rotation matrix
	  void rotate(Vector3 rotations);


	  // multiply the current top matrix with a scale matrix
	  void scale(mathType x, mathType y, mathType z);

	  // multiply the current top matrix with a scale matrix
	  void scale(Vector3 scale);

	  // multiply the current top matrix with a scale matrix
	  void scale(mathType scale);


	  // returns the number of elements in the stack
    unsigned int getSize() const;

	  // returns true if the MatrixStack is empty
    bool isEmpty() const;

	private:

	  /// stack of matrices
	  std::stack<Matrix> _stack;

	  Matrix _tempMatrix;
};


} // namespace gloost


#endif // H_GLOOST_MATRIXSTACK



