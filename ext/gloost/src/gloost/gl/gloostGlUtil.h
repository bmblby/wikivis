
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



#ifndef H_GLOOST_GLOOST_GL_UTIL
#define H_GLOOST_GLOOST_GL_UTIL


#include <gloost/gloostConfig.h>
#include <gloost/gloostMath.h>
#include <gloost/Matrix.h>
#include <gloost/Vector3.h>
#include <gloost/Viewport.h>



namespace gloost
{

namespace gl
{


void gloostPerspective(Matrix& matrix, mathType fieldOfView, mathType aspectRatio, mathType nearPlaneZ, mathType farPlaneZ);
void gloostFrustum(Matrix& matrix,mathType left,mathType right,mathType bottom,mathType top,mathType nearPlaneZ,mathType farPlaneZ);
void gloostLookAt(Matrix& matrix, const Vector3& eyePosition3D, const Vector3& center3D, const Vector3& upVector3D);
void gloostOrtho(Matrix& matrix, mathType left, mathType right, mathType bottom, mathType top, mathType nearPlaneZ,mathType farPlaneZ);


Matrix modelViewMatrixToNormalMatrix(const Matrix& modelViewMatrix);


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   Creates a Matrix for a perspective projection
  \remarks Code was taken from http://www.opengl.org/wiki/GluPerspective_code and
           is licensed by terms of the LGPL
*/

inline
Matrix
gloostPerspective(mathType fieldOfView,
                  mathType aspectRatio,
                  mathType nearPlaneZ,
                  mathType farPlaneZ)
{
  Matrix matrix;
  gloostPerspective(matrix,
                    fieldOfView,
                    aspectRatio,
                    nearPlaneZ,
                    farPlaneZ);
  return matrix;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   Creates a Matrix for a perspective projection
  \remarks Code was taken from http://www.opengl.org/wiki/GluPerspective_code and
           is licensed by terms of the LGPL
*/

inline
void
gloostPerspective(Matrix& matrix,
                  mathType fieldOfView,
                  mathType aspectRatio,
                  mathType nearPlaneZ,
                  mathType farPlaneZ)
{
  float ymax, xmax;
  ymax = nearPlaneZ * tan(fieldOfView * math::PI / 360.0);
  //ymin = -ymax;
  //xmin = -ymax * aspectRatio;
  xmax = ymax * aspectRatio;
  gloostFrustum(matrix, -xmax, xmax, -ymax, ymax, nearPlaneZ, farPlaneZ);
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \remarks Code was taken from http://www.opengl.org/wiki/GluPerspective_code and
           is licensed by terms of the LGPL

           From: http://www.khronos.org/message_boards/viewtopic.php?t=1014
            glFrustum multiplies the current matrix with the following matrix:
            ( 2n/(r-l)    0       (r+l)/(r-l)     0      )
            (   0       2n/(t-b)  (t+b)/(t-b)     0      )
            (   0         0       (f+n)/(n-f)  2fn/(n-f) )
            (   0         0           -1          0      )
*/

inline
void
gloostFrustum(Matrix& matrix,
              mathType left,
              mathType right,
              mathType bottom,
              mathType top,
              mathType nearPlaneZ,
              mathType farPlaneZ)
{
  mathType temp, temp2, temp3, temp4;
  temp = 2.0 * nearPlaneZ;
  temp2 = right - left;
  temp3 = top - bottom;
  temp4 = farPlaneZ - nearPlaneZ;
  matrix[0] = temp / temp2;
  matrix[1] = 0.0;
  matrix[2] = 0.0;
  matrix[3] = 0.0;
  matrix[4] = 0.0;
  matrix[5] = temp / temp3;
  matrix[6] = 0.0;
  matrix[7] = 0.0;
  matrix[8] = (right + left) / temp2;
  matrix[9] = (top + bottom) / temp3;
  matrix[10] = (-farPlaneZ - nearPlaneZ) / temp4;
  matrix[11] = -1.0;
  matrix[12] = 0.0;
  matrix[13] = 0.0;
  matrix[14] = (-temp * farPlaneZ) / temp4;
  matrix[15] = 0.0;
}

///////////////////////////////////////////////////////////////////////////////


/**
  \brief   Creates a Matrix for a orthogonal projection
  \remarks Code was taken from https://sourceforge.net/projects/glhlib and
           is licensed by terms of the LGPL
*/

inline
void
gloostOrtho(Matrix& matrix,
            mathType left,
            mathType right,
            mathType bottom,
            mathType top,
            mathType nearPlaneZ,
            mathType farPlaneZ)
{
  mathType temp, temp2, temp3;
	temp  = right - left;
	temp2 = top - bottom;
	temp3 = farPlaneZ - nearPlaneZ;
	matrix[0]  = 2.0 / temp;
	matrix[1]  = 0.0;
	matrix[2]  = 0.0;
	matrix[3]  = 0.0;
	matrix[4]  = 0.0;
	matrix[5]  = 2.0 / temp2;
	matrix[6]  = 0.0;
	matrix[7]  = 0.0;
	matrix[8]  = 0.0;
	matrix[9]  = 0.0;
	matrix[10] = -2.0 / temp3;
	matrix[11] = 0.0;
	matrix[12] = (-right - left) / temp;
	matrix[13] = (-top - bottom) / temp2;
	matrix[14] = (-farPlaneZ - nearPlaneZ) / temp3;
	matrix[15] = 1.0;
}


///////////////////////////////////////////////////////////////////////////////

/**
  \brief   Creates a Matrix for a orthogonal projection
  \remarks Code was taken from https://sourceforge.net/projects/glhlib and
           is licensed by terms of the LGPL
*/

inline
Matrix
gloostOrtho(mathType left,
            mathType right,
            mathType bottom,
            mathType top,
            mathType nearPlaneZ,
            mathType farPlaneZ)
{
  Matrix matrix;
  gloostOrtho(matrix,
              left,
              right,
              bottom,
              top,
              nearPlaneZ,
              farPlaneZ);

  return matrix;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   Computes a modelview matrix for a viewer, looking to a point of interest
  \remarks Code was taken from http://www.opengl.org/wiki/GluPerspective_code and
           is licensed by terms of the LGPL
*/

inline
void
gloostLookAt(Matrix&        matrix,
             const Vector3& eyePosition3D,
             const Vector3& center3D,
             const Vector3& upVector3D )
{
  Vector3 forward, side, up;
  //------------------
  forward = center3D - eyePosition3D;
  forward.normalize();
  //------------------
  //Side = forward x up
  side = cross(forward, upVector3D);
  side.normalize();
  //------------------
  //Recompute up as: up = side x forward
  up = cross(side, forward);
  up.normalize();
  //------------------
  matrix[0]  = side[0];
  matrix[4]  = side[1];
  matrix[8]  = side[2];
//  matrix[12] = 0.0;
  //------------------
  matrix[1]  = up[0];
  matrix[5]  = up[1];
  matrix[9]  = up[2];
//  matrix[13] = 0.0;
  //------------------
  matrix[2]  = -forward[0];
  matrix[6]  = -forward[1];
  matrix[10] = -forward[2];
//  matrix[14] = 0.0;
  //------------------
  matrix[3]  = matrix[7] = matrix[11] = 0.0;
  matrix[15] = 1.0;
  //------------------
//  resultMatrix = matrix * matrix2;

  Matrix trans;
  trans.setIdentity();
  trans.setTranslate(eyePosition3D*-1.0);

  matrix = matrix*trans;

//  matrix[12] -= eyePosition3D[0];
//  matrix[13] -= eyePosition3D[1];
//  matrix[14] -= eyePosition3D[2];
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   Computes a modelview matrix for a viewer, looking to a point of interest
  \remarks Code was taken from http://www.opengl.org/wiki/GluPerspective_code and
           is licensed by terms of the LGPL
*/

inline
Matrix
gloostLookAt(const Vector3& eyePosition3D,
             const Vector3& center3D,
             const Vector3& upVector3D )
{
  Matrix matrix;
  gloostLookAt(matrix,
               eyePosition3D,
               center3D,
               upVector3D );
  return matrix;
}



///////////////////////////////////////////////////////////////////////////////


/**
  \brief   Converts a ModelViewMatrix into a NormalMatrix
  \remarks
*/

inline
Matrix
modelViewMatrixToNormalMatrix(const Matrix& modelViewMatrix)
{
  return modelViewMatrix.inverted().transposed();
}



///////////////////////////////////////////////////////////////////////////////


/**
  \brief   Creates a 3D coordinate in object space from a 2d screen coordinate
  \remarks Code was taken from http://www.opengl.org/wiki/GluProject_and_gluUnProject_code
           and adapted by henning gruendle
*/
inline
gloost::Point3
gloostProject(const Point3&   ojectSpacePoint,      // object coordinates
              const Matrix&   modelView,  // model view
              const Matrix&   projection, // projection
              const Viewport& viewport)   // viewport
{
  // Model View Projection transform
  gloost::Point3 screenSpace = projection * modelView * ojectSpacePoint;
  // the final row of projection matrix is always [0 0 -1 0]
  // -> optimize for that

  // The result normalizes between -1 and 1
  if ( screenSpace[3] == 0.0)        // The w value
  {
    return gloost::Point3(0.0,0.0,0.0);
  }

  screenSpace[3] = 1.0 / screenSpace[3];
  // Perspective division
  screenSpace[0] *= screenSpace[3];
  screenSpace[1] *= screenSpace[3];
  screenSpace[2] *= screenSpace[3];
  // Window coordinates

  // Map x, y to range 0-1
  gloost::Point3 screenSpacePoint;
  screenSpacePoint[0] = (screenSpace[0] * 0.5 + 0.5) * viewport.getWidth()  + viewport.getX();
  screenSpacePoint[1] = (screenSpace[1] * 0.5 + 0.5) * viewport.getHeight() + viewport.getY();
  // This is only correct when glDepthRange(0.0, 1.0)
  screenSpacePoint[2] = (1.0 + screenSpace[2]) * 0.5;  // Between 0 and 1
  screenSpacePoint[3] = 1.0f;

  return screenSpacePoint;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   Creates a 3D coordinate in object space from a 2d screen coordinate
  \remarks Code was taken from http://www.opengl.org/wiki/GluProject_and_gluUnProject_code
*/

inline
gloost::Point3
gloostUnproject(const gloost::Point3& onScreenPosition,
                const gloost::Matrix& modelview,
                const gloost::Matrix& projectionMatrix,
                int viewportX, int viewportY,
                int viewportWidth, int viewportHeight)
{

  // Transformation of normalized coordinates between -1 and 1
  gloost::Vector3 normPoint;
  normPoint[0]=(onScreenPosition[0]-(float)viewportX)/(float)viewportWidth*2.0-1.0;
  normPoint[1]=(onScreenPosition[1]-(float)viewportY)/(float)viewportHeight*2.0-1.0;
  normPoint[2]=2.0*onScreenPosition[3]-1.0;
//  normPoint[3]=1.0;

  //Transformation matrices
  //Now compute the inverse of matrix A
  gloost::Matrix modelviewProjectionInv = projectionMatrix*modelview;
  modelviewProjectionInv.inverted();

  //Objects coordinates
  gloost::Point3 objectCoord = modelviewProjectionInv*normPoint;
  if (!objectCoord[3])
  {
    return gloost::Point3(0.0,0.0,0.0,0.0);
  }
  objectCoord[3] = (mathType)1.0/objectCoord[3];
  objectCoord[0]=objectCoord[0]*objectCoord[3];
  objectCoord[1]=objectCoord[1]*objectCoord[3];
  objectCoord[2]=objectCoord[2]*objectCoord[3];

  return objectCoord;
}

////////////////////////////////////////////////////////////////////////////////


  /// for mathType float

#ifdef GLOOST_SYSTEM_MATH_WITH_FLOAT

  /// openGL function replacement macros
  #define gloostMultMatrix    glMultMatrixf
  #define gloostLoadMatrix    glLoadMatrixf
  #define gloostGetv          glGetFloatv

  #define gloostVertex2v      glVertex2fv
  #define gloostVertex3v      glVertex3fv
  #define gloostNormal2v      glNormal2fv
  #define gloostNormal3v      glNormal3fv
  #define gloostTexCoord1v    glTexCoord1fv
  #define gloostTexCoord2v    glTexCoord2fv
  #define gloostTexCoord3v    glTexCoord3fv
  #define gloostTexCoord4v    glTexCoord4fv

  #define gloostColor4v       glColor4fv
  #define gloostColor3v       glColor3fv

#endif


////////////////////////////////////////////////////////////////////////////////


  /// for mathType double

#ifdef GLOOST_SYSTEM_MATH_WITH_DOUBLE

  /// openGL function replacement macros
  #define gloostMultMatrix    glMultMatrixd
  #define gloostLoadMatrix    glLoadMatrixd
  #define gloostGetv          glGetDoublev

  #define gloostVertex2v      glVertex2dv
  #define gloostVertex3v      glVertex3dv
  #define gloostNormal2v      glNormal2dv
  #define gloostNormal3v      glNormal3dv
  #define gloostTexCoord1v    glTexCoord1dv
  #define gloostTexCoord2v    glTexCoord2dv
  #define gloostTexCoord3v    glTexCoord3dv
  #define gloostTexCoord4v    glTexCoord4dv

  #define gloostColor4v       glColor4dv
  #define gloostColor3v       glColor3dv

#endif



















}  // namespace gl
}  // namespace gloost


#endif // #ifndef H_GLOOST_GLOOST_GL_UTIL
