
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


#ifndef GLOOST_MATRIX_H
#define GLOOST_MATRIX_H


// gloost includes
#include <gloost/gloostConfig.h>


// cpp includes
#include <iostream>
#include <cstdlib>


namespace gloost
{
  class Point3;
  class Vector3;
  class Ray;

  /// GL conform 4x4 matrix

class Matrix
{

	public:

  /// class constructor
  Matrix();

  /// copy constructor
  Matrix(const Matrix& m);

  ///
  Matrix(const mathType v[16]);

  ///
//  Matrix(const mathType* v);

  /// value constructor
  Matrix(const Vector3& col_1,
         const Vector3& col_2,
         const Vector3& col_3,
         const Point3&  col_4);

  /// class destructor
  virtual ~Matrix();



	/// get 4x4 matrix as an vector of 16 mathTypes
	const mathType* data() const;
	mathType* data();


	/// get/set within colum i and row j
	mathType get( unsigned int i, unsigned int j) const;
	void   set( unsigned int i, unsigned int j, mathType value);

  /// set all values of the matrix
  void setData(const mathType v[16]);


	/// set identity
	void setIdentity();

	/// invert matrix
	void invert ();

	/// returns a inverted version of this matrix
	Matrix inverted () const;

	/// transpose matrix
	void transpose ();

	/// returns transposed matrix
	Matrix transposed () const;

  ///  returns determinate of the rot/scale part of the matrix
//	mathType det3() const;

  ///  returns determinate of the matrix
  mathType det() const;

	/// mult matrix with [lhs]*[this]
	void mult (const Matrix& lhs);

  /// compare to matrices
  bool equals(const Matrix& rhs, mathType epsylon = GLOOST_MATHTYPE_MIN_EPSILON) const;


  /// translate
	void setTranslate(mathType x, mathType y, mathType z);
	void setTranslate(const Vector3& v);
	void setTranslate(const Point3& p);
	Vector3 getTranslate() const;

	/// scale
	void setScale(mathType x, mathType y, mathType z);
	void setScale(const Vector3& v);
	void setScale(const Point3& p);
	void setScale(mathType s);
	Vector3 getScale() const;

	/// rotate
	void setRotate(mathType x, mathType y, mathType z);
	void setRotate(const Vector3& v);
	void setRotate(const Vector3& components, mathType angle);
  void setRotateX(mathType angle);
  void setRotateY(mathType angle);
  void setRotateZ(mathType angle);

  // assignment
  Matrix& operator=(const Matrix& m);

  /// indexing (write, read)
  mathType& operator[](unsigned int i);
  const mathType& operator[](unsigned int i) const;


  // factory methode creates an identity matrix
  static Matrix createIdentity();

  // factory methode creates a translation matrix
  static Matrix createTransMatrix(const mathType x, const mathType y, const mathType z);
  static Matrix createTransMatrix(const Vector3& v);

  // factory methode creates a scale matrix
	static Matrix createScaleMatrix(mathType x, mathType y, mathType z);
	static Matrix createScaleMatrix(const Vector3& v);
	static Matrix createScaleMatrix(mathType s);

  // factory methode creates a rotation matrix
	static Matrix createRotMatrix(mathType x, mathType y, mathType z);
	static Matrix createRotMatrix(const Vector3& v);
	static Matrix createRotMatrix(const Vector3& axis, mathType angle);


private:

  mathType _data[16];

  ///  returns determinante of an particular matrix
  mathType det3 (const mathType a1, const mathType a2, const mathType a3,
	               const mathType b1, const mathType b2, const mathType b3,
	               const mathType c1, const mathType c2, const mathType c3) const;


};

/// matrix + matrix
Matrix  operator+(const Matrix& lhs, const Matrix& rhs);
/// matrix - matrix
Matrix  operator-(const Matrix& lhs, const Matrix& rhs);
/// matrix == matrix ?
bool    operator==(const Matrix& m, const Matrix& n);
bool    operator!=(const Matrix& m, const Matrix& n);

/// scalar * matrix
Matrix  operator*(mathType scalar,  const Matrix& m);
/// matrix * scalar
Matrix  operator*(const Matrix& m, mathType scalar);
/// matrix * vector
Vector3 operator*(const Matrix& m, const Vector3& v);
/// matrix * point
Point3  operator*(const Matrix& m, const Point3& p);
/// matrix * ray
Ray operator*(const Matrix& m, const Ray& r);
/// matrix * matrix
Matrix  operator*(const Matrix& lhs, const Matrix& rhs);

extern std::ostream& operator<< (std::ostream&, const Matrix&);


} // namespace gloost


#endif // GLOOST_MATRIX_H
