
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



// gloost includes
#include <gloost/animation/EnvelopeSet.h>


// cpp includes
#include <fstream>
#include <iostream>



namespace gloost
{
namespace animation
{


///////////////////////////////////////////////////////////////////////////////////////////////


 /// class constructor

EnvelopeSet::EnvelopeSet(float framerate):
    _envelopes (),
    _transformDcs(),
    _recordedMatrix(),
    _filename("gloost_default.mot")
{
  init(framerate);
	_transformDcs.reset();
	_recordedMatrix.setIdentity();
}


///////////////////////////////////////////////////////////////////////////////////////////////


 /// class destructor

EnvelopeSet::~EnvelopeSet()
{
  deleteEnvelopes();
}


///////////////////////////////////////////////////////////////////////////////////////////////


 /// delete all envelopes

void
EnvelopeSet::deleteEnvelopes()
{
  delete _envelopes[GLOOST_ENVELOPESET_POSITION_X];
  delete _envelopes[GLOOST_ENVELOPESET_POSITION_Y];
  delete _envelopes[GLOOST_ENVELOPESET_POSITION_Z];

  delete _envelopes[GLOOST_ENVELOPESET_ROTATION_H];
  delete _envelopes[GLOOST_ENVELOPESET_ROTATION_P];
  delete _envelopes[GLOOST_ENVELOPESET_ROTATION_B];

  delete _envelopes[GLOOST_ENVELOPESET_SCALATION_X];
  delete _envelopes[GLOOST_ENVELOPESET_SCALATION_Y];
  delete _envelopes[GLOOST_ENVELOPESET_SCALATION_Z];

  delete _envelopes[GLOOST_ENVELOPESET_MATRIXPLUS_X];
  delete _envelopes[GLOOST_ENVELOPESET_MATRIXPLUS_Y];
  delete _envelopes[GLOOST_ENVELOPESET_MATRIXPLUS_Z];

  _envelopes.clear();
}


///////////////////////////////////////////////////////////////////////////////////////////////


 /// init the EnvelopSet

void
EnvelopeSet::init(float framerate)
{
  // position
  _envelopes.push_back(new Envelope(framerate));
  _envelopes.push_back(new Envelope(framerate));
  _envelopes.push_back(new Envelope(framerate));


  /// rotation
  _envelopes.push_back(new Envelope(framerate));
  _envelopes.push_back(new Envelope(framerate));
  _envelopes.push_back(new Envelope(framerate));


  /// scalation
  _envelopes.push_back(new Envelope(framerate));
  _envelopes.push_back(new Envelope(framerate));
  _envelopes.push_back(new Envelope(framerate));


  /// three envelopes more, in case you want record a matrix (so 12 values are needed)
  _envelopes.push_back(new Envelope(framerate));
  _envelopes.push_back(new Envelope(framerate));
  _envelopes.push_back(new Envelope(framerate));


  /// set the first key of the scalation envelopes to value 1.0
  _envelopes[GLOOST_ENVELOPESET_SCALATION_X]->addKeyframe(Keyframe(1.0, 0.0));
  _envelopes[GLOOST_ENVELOPESET_SCALATION_Y]->addKeyframe(Keyframe(1.0, 0.0));
  _envelopes[GLOOST_ENVELOPESET_SCALATION_Z]->addKeyframe(Keyframe(1.0, 0.0));
}


///////////////////////////////////////////////////////////////////////////////////////////////


 /// load a newtek lightwave 9DOF motion File

void
EnvelopeSet::loadLightwaveMotion(std::string filename, float framerate)
{

  _filename  = filename;

  // open motion file
  std::ifstream filestream(_filename.c_str());

  if (!filestream)
  {
    std::cout << std::endl;
    std::cout << std::endl << "ERROR in EnvelopeSet::load ( std::string filename, float framerate  ):" ;
		std::cout << std::endl << "         Unable to load file motion file\"" << filename <<  "\"";
		std::flush(std::cout);
    return;

  }
  else
  {
    std::cout << std::endl;
    std::cout << std::endl << "Message from EnvelopeSet::load ( std::string filename, float framerate  ):" ;
		std::cout << std::endl << "             Loading \"" << filename <<  "\"";
		std::flush(std::cout);
  }


  /// delete all envelopes and init them with the new framerate
  deleteEnvelopes();
  init(framerate);


  // Read the file
  std::string myWord;

  int keycounter   = 0;
  int channelIndex = 0;

  while (filestream >> myWord)
  {

    // Read Channel number
    if ( myWord == "Channel" )
    {
      filestream >> channelIndex;
    }
    // Load a Key
    else if ( myWord == "Key" )
    {
      double v0, v1, v2, v3, v4, v5, v6, v7, v8;

      filestream >> v0 >> v1 >> v2
                 >> v3 >> v4 >> v5
                 >> v6 >> v7 >> v8;


      /// Lightwave has a fliped x-axis
      if (channelIndex == 0 || channelIndex == 3)
      {
        v0 = -v0;
      }

      _envelopes[channelIndex]->addKeyframe( Keyframe (v0, v1*framerate, v2,
                                                  v3, v4, v5,
                                                  v6, v7, v8 ));

      keycounter++;
    }
  }

  filestream.close();


  std::cout << std::endl;
  std::cout << std::endl << "Message from EnvelopeSet::load ( std::string filename, float framerate  ):" ;
  std::cout << std::endl << "           " << keycounter << " Keys loaded from" << _filename << ".";
  std::cout << std::endl << "           " << channelIndex << " channels .";
  std::flush(std::cout);
}


///////////////////////////////////////////////////////////////////////////////////////////////


 /// save all nine Envelopes to a newtek lightwave 9DOF motion File

void
EnvelopeSet::saveLightwaveMotion (std::string filename)
{
  std::ofstream filestream;
  filestream.open(filename.c_str());

  if (!filestream)
  {
    std::cout << std::endl;
    std::cout << std::endl << "ERROR in EnvelopeSet::saveLightwaveMotion(std::string filename):" ;
		std::cout << std::endl << "         Unable to open envelope file\"" << filename <<  "\"";
		std::flush(std::cout);
    return;

  }
  else
  {
    std::cout << std::endl;
    std::cout << std::endl << "Message from EnvelopeSet::saveLightwaveMotion(std::string filename):" ;
		std::cout << std::endl << "             Writing \"" << filename <<  "\"";
		std::flush(std::cout);
  }


  // write the file
  filestream << "LWMO" << std::endl;
  filestream << "3" << std::endl;
  filestream << std::endl;

  filestream << "NumChannels 9" << std::endl;


  for(unsigned int i=0; i != 9; ++i)
  {
    filestream << "Channel " << i << std::endl;
    _envelopes[i]->writeToLightwaveMotion(filestream);
  }


  filestream.close();

  std::cout << std::endl;
  std::cout << std::endl << "Message from EnvelopeSet::saveLightwaveMotion(std::string filename):" ;
  std::cout << std::endl << "             Motion file with 9 Envelopes written";
  std::flush(std::cout);


  filestream.close();
}


///////////////////////////////////////////////////////////////////////////////////////////////


 /// get vector with keys for indices like GLOOST_ENVELOPESET_POSITION_X or GLOOST_ENVELOPESET_SCALATION_Z

Envelope*
EnvelopeSet::getEnvelope (unsigned int index)
{
  return _envelopes[index];
}



///////////////////////////////////////////////////////////////////////////////////////////////


 /// get the dcs with all transformations and their components as matrices

Dcs*
EnvelopeSet::getDcs()
{
  return &_transformDcs;
}


///////////////////////////////////////////////////////////////////////////////////////////////


 /// get a matrix with the complete transformation for a specific time

const Matrix&
EnvelopeSet::getMatrixLinear(const float time)
{

  double posX, posY, posZ;
  double rotX, rotY, rotZ;
  double scaX, scaY, scaZ;

  // benchmark
  //for(unsigned int i=0; i != 300; ++i)
  {

    posX = _envelopes[GLOOST_ENVELOPESET_POSITION_X]->getValueLinear(time);
    posY = _envelopes[GLOOST_ENVELOPESET_POSITION_Y]->getValueLinear(time);
    posZ = _envelopes[GLOOST_ENVELOPESET_POSITION_Z]->getValueLinear(time);

    rotX = _envelopes[GLOOST_ENVELOPESET_ROTATION_P]->getValueLinear(time);
    rotY = _envelopes[GLOOST_ENVELOPESET_ROTATION_H]->getValueLinear(time);
    rotZ = _envelopes[GLOOST_ENVELOPESET_ROTATION_B]->getValueLinear(time);

    scaX = _envelopes[GLOOST_ENVELOPESET_SCALATION_X]->getValueLinear(time);
    scaY = _envelopes[GLOOST_ENVELOPESET_SCALATION_Y]->getValueLinear(time);
    scaZ = _envelopes[GLOOST_ENVELOPESET_SCALATION_Z]->getValueLinear(time);
  }


  _transformDcs.setTranslate(posX, posY, posZ);
  _transformDcs.setRotate(rotX, rotY, rotZ);
  _transformDcs.setScale(scaX, scaY, scaZ);
  _transformDcs.validate();

  return _transformDcs.getMatrix();
}


///////////////////////////////////////////////////////////////////////////////


  /// returns a vector with the position information (x, y, z) for a specific time

Point3
EnvelopeSet::getPositionLinear(const float time)
{
  Point3 pos;

  pos[0] = _envelopes[GLOOST_ENVELOPESET_POSITION_X]->getValueLinear(time);
  pos[1] = _envelopes[GLOOST_ENVELOPESET_POSITION_Y]->getValueLinear(time);
  pos[2] = _envelopes[GLOOST_ENVELOPESET_POSITION_Z]->getValueLinear(time);

  return pos;
}


///////////////////////////////////////////////////////////////////////////////


  /// returns a vector with the position information (H, P, B) for a specific time

Vector3
EnvelopeSet::getRotationLinear(const float time)
{
  Vector3 rot;

  rot[0] = _envelopes[GLOOST_ENVELOPESET_ROTATION_P]->getValueLinear(time);
  rot[1] = _envelopes[GLOOST_ENVELOPESET_ROTATION_H]->getValueLinear(time);
  rot[2] = _envelopes[GLOOST_ENVELOPESET_ROTATION_B]->getValueLinear(time);

  return rot;
}


///////////////////////////////////////////////////////////////////////////////


  /// returns a vector with the scalation information (x, y, z) for a specific time

Vector3
EnvelopeSet::getScalationLinear(const float time)
{
  Vector3 scale;

  scale[0] = _envelopes[GLOOST_ENVELOPESET_SCALATION_X]->getValueLinear(time);
  scale[1] = _envelopes[GLOOST_ENVELOPESET_SCALATION_Y]->getValueLinear(time);
  scale[2] = _envelopes[GLOOST_ENVELOPESET_SCALATION_Z]->getValueLinear(time);

  return scale;
}


///////////////////////////////////////////////////////////////////////////////////////////////


 /// get a matrix with the complete transformation for a specific time

//const Matrix&
//EnvelopeSet::getMatrixTCB(const float time)
//{
//
//  double posX, posY, posZ;
//  double rotX, rotY, rotZ;
//  double scaX, scaY, scaZ;
//
//  // benchmark
//  //for(unsigned int i=0; i != 300; ++i)
//  {
//
//    posX = _envelopes[GLOOST_ENVELOPESET_POSITION_X]->getValueTCB(time);
//    posY = _envelopes[GLOOST_ENVELOPESET_POSITION_Y]->getValueTCB(time);
//    posZ = _envelopes[GLOOST_ENVELOPESET_POSITION_Z]->getValueTCB(time);
//
//    rotX = _envelopes[GLOOST_ENVELOPESET_ROTATION_P]->getValueTCB(time);
//    rotY = _envelopes[GLOOST_ENVELOPESET_ROTATION_H]->getValueTCB(time);
//    rotZ = _envelopes[GLOOST_ENVELOPESET_ROTATION_B]->getValueTCB(time);
//
//    scaX = _envelopes[GLOOST_ENVELOPESET_SCALATION_X]->getValueTCB(time);
//    scaY = _envelopes[GLOOST_ENVELOPESET_SCALATION_Y]->getValueTCB(time);
//    scaZ = _envelopes[GLOOST_ENVELOPESET_SCALATION_Z]->getValueTCB(time);
//  }
//
//
//  _transformDcs.setTranslate(posX, posY, posZ);
//  _transformDcs.setRotate(rotX, rotY, rotZ);
//  _transformDcs.setScale(scaX, scaY, scaZ);
//  _transformDcs.validate();
//
//  return _transformDcs.getMatrix();
//}


///////////////////////////////////////////////////////////////////////////////


  /// returns a vector with the position information (x, y, z) for a specific time

//Point3
//EnvelopeSet::getPositionTCB(const float time)
//{
//  Point3 pos;
//
//  pos[0] = _envelopes[GLOOST_ENVELOPESET_POSITION_X]->getValueTCB(time);
//  pos[1] = _envelopes[GLOOST_ENVELOPESET_POSITION_Y]->getValueTCB(time);
//  pos[2] = _envelopes[GLOOST_ENVELOPESET_POSITION_Z]->getValueTCB(time);
//
//  return pos;
//}


///////////////////////////////////////////////////////////////////////////////


  /// returns a vector with the position information (H, P, B) for a specific time

//Vector3
//EnvelopeSet::getRotationTCB(const float time)
//{
//  Vector3 rot;
//
//  rot[0] = _envelopes[GLOOST_ENVELOPESET_ROTATION_H]->getValueTCB(time);
//  rot[1] = _envelopes[GLOOST_ENVELOPESET_ROTATION_P]->getValueTCB(time);
//  rot[2] = _envelopes[GLOOST_ENVELOPESET_ROTATION_B]->getValueTCB(time);
//
//  return rot;
//}


///////////////////////////////////////////////////////////////////////////////


  /// returns a vector with the scalation information (x, y, z) for a specific time

//Vector3
//EnvelopeSet::getScalationTCB(const float time)
//{
//  Vector3 scale;
//
//  scale[0] = _envelopes[GLOOST_ENVELOPESET_SCALATION_X]->getValueTCB(time);
//  scale[1] = _envelopes[GLOOST_ENVELOPESET_SCALATION_Y]->getValueTCB(time);
//  scale[2] = _envelopes[GLOOST_ENVELOPESET_SCALATION_Z]->getValueTCB(time);
//
//  return scale;
//}


///////////////////////////////////////////////////////////////////////////////


  /// adds Keyframes to all 12 Envelopes to save a whole Matrix

void
EnvelopeSet::recordMatrixState(const Matrix& matrix, const double& time)
{
  _envelopes[GLOOST_ENVELOPESET_POSITION_X]->addKeyframe(matrix[0], time);
  _envelopes[GLOOST_ENVELOPESET_POSITION_Y]->addKeyframe(matrix[1], time);
  _envelopes[GLOOST_ENVELOPESET_POSITION_Z]->addKeyframe(matrix[2], time);

  _envelopes[GLOOST_ENVELOPESET_ROTATION_H]->addKeyframe(matrix[4], time);
  _envelopes[GLOOST_ENVELOPESET_ROTATION_P]->addKeyframe(matrix[5], time);
  _envelopes[GLOOST_ENVELOPESET_ROTATION_B]->addKeyframe(matrix[6], time);

  _envelopes[GLOOST_ENVELOPESET_SCALATION_X]->addKeyframe(matrix[8], time);
  _envelopes[GLOOST_ENVELOPESET_SCALATION_Y]->addKeyframe(matrix[9], time);
  _envelopes[GLOOST_ENVELOPESET_SCALATION_Z]->addKeyframe(matrix[10], time);

  _envelopes[GLOOST_ENVELOPESET_MATRIXPLUS_X]->addKeyframe(matrix[12], time);
  _envelopes[GLOOST_ENVELOPESET_MATRIXPLUS_Y]->addKeyframe(matrix[13], time);
  _envelopes[GLOOST_ENVELOPESET_MATRIXPLUS_Z]->addKeyframe(matrix[14], time);
}


///////////////////////////////////////////////////////////////////////////////


  /// returns a Matrix with the actual values of the 9 Envelopes

const Matrix&
EnvelopeSet::getRecordedMatrix(const double& time)
{
  _recordedMatrix[0] = _envelopes[GLOOST_ENVELOPESET_POSITION_X]->getValueLinear(time);
  _recordedMatrix[1] = _envelopes[GLOOST_ENVELOPESET_POSITION_Y]->getValueLinear(time);
  _recordedMatrix[2] = _envelopes[GLOOST_ENVELOPESET_POSITION_Z]->getValueLinear(time);
  _recordedMatrix[3] = 0.0;

  _recordedMatrix[4] = _envelopes[GLOOST_ENVELOPESET_ROTATION_H]->getValueLinear(time);
  _recordedMatrix[5] = _envelopes[GLOOST_ENVELOPESET_ROTATION_P]->getValueLinear(time);
  _recordedMatrix[6] = _envelopes[GLOOST_ENVELOPESET_ROTATION_B]->getValueLinear(time);
  _recordedMatrix[7] = 0.0;

  _recordedMatrix[8]  = _envelopes[GLOOST_ENVELOPESET_SCALATION_X]->getValueLinear(time);
  _recordedMatrix[9]  = _envelopes[GLOOST_ENVELOPESET_SCALATION_Y]->getValueLinear(time);
  _recordedMatrix[10] = _envelopes[GLOOST_ENVELOPESET_SCALATION_Z]->getValueLinear(time);
  _recordedMatrix[11] = 0.0;

  _recordedMatrix[12] = _envelopes[GLOOST_ENVELOPESET_MATRIXPLUS_X]->getValueLinear(time);
  _recordedMatrix[13] = _envelopes[GLOOST_ENVELOPESET_MATRIXPLUS_Y]->getValueLinear(time);
  _recordedMatrix[14] = _envelopes[GLOOST_ENVELOPESET_MATRIXPLUS_Z]->getValueLinear(time);
  _recordedMatrix[15] = 1.0;

  return _recordedMatrix;
}


///////////////////////////////////////////////////////////////////////////////






} /// namespace animation

} /// namespace gloost



