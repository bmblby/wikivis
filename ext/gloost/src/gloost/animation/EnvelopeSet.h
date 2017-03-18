
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



#ifndef GLOOST_ENVELOPE_SET_H
#define GLOOST_ENVELOPE_SET_H



// gloost includes
#include <gloost/animation/Keyframe.h>
#include <gloost/animation/Envelope.h>
#include <gloost/Dcs.h>
#include <gloost/Point3.h>



// cpp includes
#include <string>
#include <vector>



/// defines for the components of a 9-Degrees-of-freedom motion + 3
#define GLOOST_ENVELOPESET_POSITION_X 0
#define GLOOST_ENVELOPESET_POSITION_Y 1
#define GLOOST_ENVELOPESET_POSITION_Z 2

#define GLOOST_ENVELOPESET_ROTATION_H 3
#define GLOOST_ENVELOPESET_ROTATION_P 4
#define GLOOST_ENVELOPESET_ROTATION_B 5

#define GLOOST_ENVELOPESET_SCALATION_X 6
#define GLOOST_ENVELOPESET_SCALATION_Y 7
#define GLOOST_ENVELOPESET_SCALATION_Z 8

#define GLOOST_ENVELOPESET_MATRIXPLUS_X 9
#define GLOOST_ENVELOPESET_MATRIXPLUS_Y 10
#define GLOOST_ENVELOPESET_MATRIXPLUS_Z 11



namespace gloost
{
namespace animation
{

  ///  A Set of nine envelopes for translation, rotation and scalation

class EnvelopeSet
{
	public:

    /// class constructor
    EnvelopeSet(float framerate = 60.0f);

    /// class destructor
    ~EnvelopeSet();



    /// initialize the EnvelopeSet
    void init(float framerate);



    /// load a newtek lightwave 9DOF motion File
    void loadLightwaveMotion( std::string filename, float framerate = 60.0f);


    /// save all nine Envelopes to a newtek lightwave 9DOF motion File
    void saveLightwaveMotion( std::string filename );



    /// get vector with keys for indices like GLOOST_ENVELOPESET_POSITION_X or GLOOST_ENVELOPESET_SCALATION_Z
    Envelope* getEnvelope( unsigned int index );


    /// get the dcs with all transformations and their components as matrices
    Dcs* getDcs();



    /// get a matrix with the complete transformation for a specific time
    const Matrix& getMatrixLinear(const float time);

    /// returns a vector with the position information (x, y, z) for a specific time
    Point3 getPositionLinear(const float time);

    /// returns a vector with the position information (H, P, B) for a specific time
    Vector3 getRotationLinear(const float time);

    /// returns a vector with the scalation information (x, y, z) for a specific time
    Vector3 getScalationLinear(const float time);



    /// get a matrix with the complete transformation for a specific time
//    const Matrix& getMatrixTCB(const float time);

    /// returns a vector with the position information (x, y, z) for a specific time
//    Point3 getPositionTCB(const float time);

    /// returns a vector with the position information (H, P, B) for a specific time
//    Vector3 getRotationTCB(const float time);

    /// returns a vector with the scalation information (x, y, z) for a specific time
//    Vector3 getScalationTCB(const float time);



    /// adds Keyframes to all 9 Envelopes to save a whole Matrix
    void recordMatrixState(const Matrix& matrix, const double& time);

    /// returns a Matrix with
    const Matrix& getRecordedMatrix(const double& time);



	private:


    /// Keyframes for 9 degres of freedom (dof)
    std::vector< Envelope* > _envelopes;

    /// Dcs holding all transfomations as Translate-, Rotate- and Scale Components or all together
    Dcs _transformDcs;

    /// recorded matrix
    Matrix _recordedMatrix;

    /// filename of the loaded motion file
    std::string _filename;

     /// delete all envelopes
    void deleteEnvelopes();


};

} /// namespace animation
} /// namespace gloost

#endif // GLOOST_ENVELOPE_SET_H
