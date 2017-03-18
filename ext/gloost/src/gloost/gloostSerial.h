
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



#ifndef H_GLOOST_GLOOST_SERIALIZERS
#define H_GLOOST_GLOOST_SERIALIZERS


#include <gloost/BinaryBundle.h>
#include <gloost/BinaryFile.h>
#include <cstring>

#include <cstring>

namespace gloost
{
namespace serial
{

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   creates a BinaryBundle from a generic type
  \remarks Good luck ;-)
*/

template <class inType>
std::shared_ptr<BinaryBundle>
serialize(const inType& generic)
{
  return BinaryBundle::create((const unsigned char*)&generic, sizeof(inType));
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   creates a BinaryBundle from a generic type
  \remarks Good luck ;-)
*/

template <class inType>
std::shared_ptr<BinaryBundle>
serialize(const inType& generic, unsigned sizeInBytes)
{
  return BinaryBundle::create((const unsigned char*)&generic, sizeInBytes);
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   unserialises a generic type from a BinaryFile
  \remarks This will advance the current read position of the binary file with sizeof(genericType)
*/

template <class Type>
bool
unserialize(Type& generic, BinaryFile& sourceFile)
{
  size_t sizeOfGeneric = sizeof(Type);

  if (sourceFile.getBytesLeft() >= sizeOfGeneric)
  {
    std::memcpy((void*)&generic, sourceFile.getCurrent(), sizeOfGeneric);
    sourceFile.advanceBy(sizeOfGeneric);
    return true;
  }

  return false;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   unserialises a generic type from a binary blob
  \remarks
*/

template <class Type>
bool
unserialize(Type& generic, const BinaryBundle::const_shared_ptr blob, size_t offsetInByte = 0u)
{
  size_t sizeOfGeneric = sizeof(Type);

  if (blob->getCapacity() >= offsetInByte+sizeOfGeneric)
  {
    std::memcpy((void*)&generic, (blob->data()+offsetInByte), sizeOfGeneric);
    return true;
  }

  return false;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   unserialises a number of bytes from a BinaryFile
  \remarks This will advance the current read position of the binary file with sizeInBytes
*/

inline
bool
unserialize(void* destination, BinaryFile& sourceFile, size_t sizeInBytes)
{
  if (sourceFile.getBytesLeft() >= sizeInBytes)
  {
    std::memcpy(destination, sourceFile.getCurrent(), sizeInBytes);
    sourceFile.advanceBy(sizeInBytes);
    return true;
  }
  return false;
}

///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////// Math //////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

  // ehm..., this is work in progress

///**
//  \brief creates a BinaryBundle from a Point3
//*/
//
//BinaryBundle*
//serialize(const Point3& object)
//{
//  return new BinaryBundle((unsigned char*)&object, sizeof(mathType*3));
//}
//
//
/////////////////////////////////////////////////////////////////////////////////
//
//
///**
//  \brief creates a BinaryBundle from a Vector3
//*/
//
//BinaryBundle*
//serialize(const Vector3& object)
//{
//  return new BinaryBundle((unsigned char*)&object, sizeof(Vector3)-1);
//}

///////////////////////////////////////////////////////////////////////////////

/**
//  \brief creates a BinaryBundle from a Matrix
*/

//BinaryBundle*
//serialize(const Matrix& object)
//{
//  return new BinaryBundle((unsigned char*)object.data(), sizeof(mathType)*16);
//}

///////////////////////////////////////////////////////////////////////////////


/**
  \brief   serializes a std::string in a gloost::BinaryBundle by storing number of chars and the chars them self
  \remarks
*/

inline
gloost::BinaryBundle::shared_ptr
stringBundle(const std::string& string)
{
  // calc memory size
  size_t memSize = sizeof(unsigned) + string.length()*sizeof(unsigned char);
  // create a bundle
  gloost::BinaryBundle::shared_ptr bundle = gloost::BinaryBundle::create(memSize);

  // store length
  bundle->putUnsigned(string.length());
  // store chars
  bundle->putUnsave((unsigned char*)string.c_str(), string.length()*sizeof(unsigned char));

  return bundle;
}

inline std::shared_ptr<BinaryBundle> serialize(const std::string& string)
{
  return stringBundle(string);
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   unserializes a std::string from a BinaryBundle
  \remarks
*/

inline
std::string
stringUnbundle(BinaryFile& reader)
{
  size_t numChars = reader.readUInt32();

  unsigned char* chars = new unsigned char[numChars+1u];
  reader.readBuffer(chars, numChars);
  chars[numChars] = '\0';

  std::string theString((char*)chars);

  delete [] chars;

  return std::string(theString);
}

inline
bool
unserialize(std::string& string, const BinaryBundle::shared_ptr blob)
{
  auto reader = blob->createReader();
  string = stringUnbundle(*reader);
  return true;
}

///////////////////////////////////////////////////////////////////////////////


}  /// namespace serial
}  /// namespace gloost


#endif // H_GLOOST_GLOOST_SERIALIZERS
