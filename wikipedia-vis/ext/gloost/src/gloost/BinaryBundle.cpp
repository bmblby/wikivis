
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



// gloost system includes
#include <gloost/BinaryBundle.h>
#include <gloost/BinaryFile.h>



// cpp includes
#include <string>
#include <cstring>
#include <iostream>
#include <omp.h>



namespace gloost
{

/**
  \class   BinaryBundle

  \brief   Bundle of binary data aka serialized objects

  \author  Felix Weiszig
  \date    November 2011
  \remarks ...
*/


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   creates a BinaryBundle instance
  \param   ...
  \remarks ...
*/

/*static*/
BinaryBundle::shared_ptr
BinaryBundle::create(size_t capacityInByte)
{
	return shared_ptr(new BinaryBundle(capacityInByte));
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   creates a BinaryBundle instance
  \param   ...
  \remarks ...
*/

/*static*/
BinaryBundle::shared_ptr
BinaryBundle::create(const unsigned char* data, size_t size)
{
	return shared_ptr(new BinaryBundle(data, size));
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   copies a BinaryBundle instance
  \param   ...
  \remarks ...
*/

/*static*/
BinaryBundle::shared_ptr
BinaryBundle::create(BinaryBundle::const_shared_ptr bundle)
{
	auto newBundle = shared_ptr(new BinaryBundle(bundle->getCapacity()));
  newBundle->put(bundle->data(), bundle->getCapacity());
  newBundle->rewind();
  return newBundle;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   copies a BinaryBundle instance
  \param   ...
  \remarks ...
*/

/*static*/
BinaryBundle::shared_ptr
BinaryBundle::create(std::vector<BinaryBundle::const_shared_ptr> bundleVector)
{
  size_t capacitySum = 0u;
  for (auto bundle : bundleVector)
  {
    capacitySum += bundle->getCapacity();
  }

	auto newBundle = shared_ptr(new BinaryBundle(capacitySum));

  for (auto bundle : bundleVector)
  {
    newBundle->putUnsave(bundle->data(), bundle->getCapacity());
  }

  newBundle->rewind();
  return newBundle;
}

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class constructor
  \remarks ...
*/

BinaryBundle::BinaryBundle(size_t capacity):
    _capacity(capacity),
    _buffer(new unsigned char[capacity]),
    _startPtr(_buffer),
    _endPtr(_buffer+capacity),
    _currentPtr(_buffer)
{

}

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class constructor
  \remarks ...
*/

BinaryBundle::BinaryBundle(const unsigned char* buffer, size_t size):
    _capacity(size),
    _buffer(new unsigned char[size]),
    _startPtr(_buffer),
    _endPtr(_buffer+size),
    _currentPtr(_buffer)
{
  put(buffer, size);
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class destructor
  \remarks ...
*/

BinaryBundle::~BinaryBundle()
{
	delete [] _buffer;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns the capacity (in bytes)
  \param   ...
  \remarks ...
*/

size_t
BinaryBundle::getCapacity() const
{
	return  _capacity;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns the size from start to current put position (in bytes)
  \param   ...
  \remarks ...
*/

size_t
BinaryBundle::getSize() const
{
	return _currentPtr-_startPtr;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns the free elements left in the bundle
  \param   ...
  \remarks ...
*/

size_t
BinaryBundle::getFreeSpace() const
{
	return _endPtr-_currentPtr;
}

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   appends a BinaryBundle
  \param   ...
  \remarks returns FALSE if buffers free space is to small to accommodate the new data
*/

bool
BinaryBundle::put(std::shared_ptr<const BinaryBundle> bundle)
{
  if (getFreeSpace() >= bundle->getSize())
  {
    memcpy(_currentPtr, bundle->data(), bundle->getSize());
    _currentPtr+=bundle->getSize();
    return true;
  }
  return false;
}

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   inserts sizeInBytes bytes of a BinaryFiles Current read position into the bundle
  \param   ...
  \remarks 1. returns FALSE if the bundle has not enough space left or the BinaryFile has not
           enough bytes after its current read position
           2. read position of the BinaryFile will be raised by sizeInBytes
*/

bool
BinaryBundle::put(BinaryFile& sourceFile, size_t sizeInBytes)
{
  if (sourceFile.getBytesLeft() >= sizeInBytes && getFreeSpace() >=  sizeInBytes)
  {
    putUnsave(sourceFile.getCurrent(), sizeInBytes);
    sourceFile.advanceBy(sizeInBytes);
    return true;
  }
  return false;
}

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   inserts sizeInBytes bytes of a BinaryFiles Current read position into the bundle
  \param   ...
  \remarks read position of the BinaryFile will be raised by sizeInBytes
*/

void
BinaryBundle::putUnsave(BinaryFile& sourceFile, size_t sizeInBytes)
{
  putUnsave(sourceFile.getCurrent(), sizeInBytes);
  sourceFile.advanceBy(sizeInBytes);
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   appends a generic buffer
  \param   ...
  \remarks returns FALSE if buffers free space is to small to accommodate the new data
*/

bool
BinaryBundle::put(const unsigned char* buffer, size_t sizeInBytes)
{
  if (getFreeSpace() >= sizeInBytes)
  {
    memcpy(_currentPtr, buffer, sizeInBytes);
    _currentPtr+=sizeInBytes;
    return true;
  }
  return false;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   appends a generic buffer, does not check if there is enough space left in the bundle
  \param   ...
  \remarks returns FALSE if buffers free space is to small to accommodate the new data
*/

void
BinaryBundle::putUnsave(const unsigned char* buffer, size_t sizeInBytes)
{
  memcpy(_currentPtr, buffer, sizeInBytes);
  _currentPtr+=sizeInBytes;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   appends a single character
  \param   ...
  \remarks returns FALSE if buffers free space is to small to accommodate the new data
*/

bool
BinaryBundle::putChar(unsigned char character)
{
  bool result = put((const unsigned char*)&character, 1);
  return result;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   appends a unsigned int
  \param   ...
  \remarks returns FALSE if buffers free space is to small to accommodate the new data
*/

bool
BinaryBundle::putUnsigned(unsigned number)
{
  bool result = put((const unsigned char*)&number, 4);
  return result;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   inserts a double at the current position and increments the position
  \param   ...
  \remarks returns FALSE if buffers free space is to small to accommodate the new data
*/

bool
BinaryBundle::putFloat(float number)
{
  bool result = put((const unsigned char*)&number, 4);
  return result;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   inserts a double at the current position and increments the position
  \param   ...
  \remarks returns FALSE if buffers free space is to small to accommodate the new data
*/

bool
BinaryBundle::putDouble(double number)
{
  bool result = put((const unsigned char*)&number, 8);
  return result;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   sets the current put position lengthInBytes back
  \remarks 1. If lengthInBytes == 0, the current put pointer will be set to the start
  \remarks 2. This methode is unsave, no boundary checks are being made
*/

void
BinaryBundle::rewind(size_t lengthInBytes)
{
  if (lengthInBytes)
  {
    _currentPtr -= lengthInBytes;
    return;
  }
  _currentPtr = _startPtr;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   sets the current put position lengthInBytes forward
  \remarks 1. If lengthInBytes == 0, the current put pointer will be set to the end
  \remarks 2. This methode is unsave, no boundary checks are being made
*/

void
BinaryBundle::forward(size_t lengthInBytes)
{
  if (lengthInBytes)
  {
    _currentPtr += lengthInBytes;
    return;
  }
  _currentPtr = _endPtr;
}

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   sets the current put position relative to the begining
  \remarks ...
*/

void
BinaryBundle::setPutPosition(size_t relativePositionInBytes)
{
  if (_startPtr + relativePositionInBytes > _endPtr)
  {
    _currentPtr = _endPtr;
    return;
  }
  _currentPtr += relativePositionInBytes;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns a pointer to the first buffer element
  \param   ...
  \remarks ...
*/

unsigned char*
BinaryBundle::data()
{
	return _startPtr;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns a const pointer to the first buffer element
  \param   ...
  \remarks ...
*/

const unsigned char*
BinaryBundle::data() const
{
	return _startPtr;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns a pointer to the current put position within the BinaryBundle
  \param   ...
  \remarks ...
*/

unsigned char*
BinaryBundle::getCurrentPtr()
{
	return _currentPtr;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns a const pointer to the current put position within the BinaryBundle
  \param   ...
  \remarks ...
*/

const unsigned char*
BinaryBundle::getCurrentPtr() const
{
	return _currentPtr;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Factory methode creates and returns a BinaryFile object which was openend with this BinaryBundle to read from it
  \param   ...
  \remarks The BinaryFile instance has to be deleted after use
*/

std::shared_ptr<BinaryFile>
BinaryBundle::createReader() const
{
  auto reader = new BinaryFile();
  reader->openAndLoad(const_shared_ptr(shared_from_this()));
	return std::shared_ptr<BinaryFile>(reader);
}

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns a const pointer to the first buffer element
  \param   ...
  \remarks ...
*/

BinaryBundle&
BinaryBundle::operator= (const BinaryBundle& bundle)
{
  if (getCapacity())
  {
    delete [] _buffer;
    _buffer = new unsigned char[bundle.getCapacity()];
    _startPtr = _buffer;
    _endPtr   = _buffer+bundle.getCapacity();
    _startPtr = _buffer;
  }
  memcpy(_startPtr, bundle._startPtr, bundle.getCapacity());

  return (*this);
}

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   join two bundles to create a third
  \param   ...
  \remarks ...
*/

/*extern*/
BinaryBundle::shared_ptr
operator+(BinaryBundle::const_shared_ptr bundle1, BinaryBundle::const_shared_ptr bundle2)
{
  BinaryBundle::shared_ptr bundle3 = BinaryBundle::create(bundle1->getCapacity() + bundle2->getCapacity());
  bundle3->putUnsave(bundle1->data(), bundle1->getCapacity());
  bundle3->putUnsave(bundle2->data(), bundle2->getCapacity());
  return bundle3;
}


////////////////////////////////////////////////////////////////////////////////





} // namespace gloost


