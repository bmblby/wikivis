
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
#include <gloost/BinaryFile.h>
//#include <gloost/gloostHelper.h>
#include <gloost/gloostMath.h>
#include <gloost/BinaryBundle.h>

// boost
#include <boost/filesystem.hpp>

// cpp includes
#include <string>
#include <iostream>
#include <cstring>

namespace gloost
{

/**
  \class BinaryFile

  \brief Reads and writes binary files

  \author  Felix Weiszig
  \date    Juli 2010
  \remarks ...
*/

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   creates a BinaryFile instance
  \param   ...
  \remarks ...
*/

/*static*/
BinaryFile::shared_ptr
BinaryFile::create()
{
	return shared_ptr(new BinaryFile());
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief Class constructor

  \remarks ...
*/

BinaryFile::BinaryFile():
    _filePath(),
    _fileBuffer(0),
    _fileSize(0),
    _bufferStart(0),
    _currentPosition(0),
    _bufferEnd(0),
    _deleteBufferInDestructor(true),
    _fileToWriteTo(nullptr)
{

}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief Class destructor
  \remarks ...
*/

BinaryFile::~BinaryFile()
{
  unload();
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns the size of a file in bytes if the file exists
  \param   filename Path to the file

*/

/*static*/
size_t
BinaryFile::getFileSize(const std::string& filename)
{
  return getFileSize(gloost::PathType(filename));
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns the size of a file in bytes if the file exists
  \param   filename Path to the file

*/

/*static*/
size_t
BinaryFile::getFileSize(const gloost::PathType& filename)
{
  if(!fileExists(filename))
  {
    return 0;
  }

  return boost::filesystem::file_size(filename);
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   compatibility call to pathExists(const gloost::PathType& filename)
  \param   filename Path to the file

*/

/*static*/
bool
BinaryFile::pathExists(const std::string& filename)
{
  return pathExists(gloost::PathType(filename));
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns if a path exists - makes no difference between file and folder
  \param   filename Path to the file

*/

/*static*/
bool
BinaryFile::pathExists(const gloost::PathType& filename)
{
  return (fileExists(filename) || folderExists(filename));
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns if a file exists
  \param   filename Path to the file

*/

/*static*/
bool
BinaryFile::fileExists(const gloost::PathType& filename)
{
  return (boost::filesystem::exists(filename) && boost::filesystem::is_regular_file(filename));
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns if a folder exists
  \param   filename Path to the file

*/

/*static*/
bool
BinaryFile::folderExists(const gloost::PathType& filename)
{
  return (boost::filesystem::exists(filename) && boost::filesystem::is_directory(filename));
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   compatibility call
*/

/*static*/
bool
BinaryFile::write(const std::string& filename,
                  const BinaryBundle& bundle)
{
  return write(gloost::PathType(filename), bundle);
}


////////////////////////////////////////////////////////////////////////////////

/**
  \brief   writes a BinaryBundle to a file
  \param   filename path to the file to write to
  \param   bundle a gloost::BinaryBundle full of the finest data
  \remarks If the file does not exist it will be created. Existing files will be
           overwriten.
*/
/*static*/
bool
BinaryFile::write(const gloost::PathType& filename,
                  const BinaryBundle& bundle)
{

#ifndef GLOOST_SYSTEM_DISABLE_OUTPUT_MESSAGES
  std::cerr << std::endl << "Message from BinaryFile::write(): ";
  std::cerr << std::endl << "             Writing " << filename << ".";
  std::cerr << std::endl << "            " << bundle.getCapacity() << " Bytes to write...";
  std::cerr << std::endl;

#endif

  boost::filesystem::ofstream myFile (filename, std::ios::out | std::ios::binary);

  if (!myFile.is_open())
  {
#ifndef GLOOST_SYSTEM_DISABLE_OUTPUT_WARNINGS
    std::cerr << std::endl << "ERROR in BinaryFile::write(): ";
    std::cerr << std::endl << "         Could NOT open file " << filename << " to write to.";
    std::cerr << std::endl;
#endif
    return false;
  }

  myFile.write ((char*)bundle.data(), bundle.getCapacity());
  myFile.close();

  return true;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief    compatibility call
*/

/*static*/
bool
BinaryFile::read(const std::string&  filename,
                 unsigned char*&     buffer,
                 size_t&             bufferSizeInByte)
{
  return read(gloost::PathType(filename), buffer, bufferSizeInByte);
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   reads a whole file and fills a buffer with its content
  \param   filename Path to the file to read from
  \param   buffer Pointer of type unsigned char*. No memory has
           to be allocated befor calling read()
  \param   bufferSizeInByte Will be the length of buffer in Byte after calling
           read()
  \remarks This is an example how to read a buffer:

           \code
              std::string    filename         = "somefile.bin";
              unsigned char* buffer           = 0;
              unsigned int   bufferSizeInByte = 0;

              if (!gloost::BinaryFile::read(filename, buffer, bufferSizeInByte))
              {
                std::cerr << std::endl << "ERROR: Could not read file: " << filename;
              }
              else
              {
                std::cerr << std::endl << "bufferSizeInByte: " << bufferSizeInByte;

                // use buffer ...
              }
           \endcode
*/

/*static*/
bool
BinaryFile::read(const gloost::PathType&  filename,
                 unsigned char*&          buffer,
                 size_t&                  bufferSizeInByte)
{
  if(!fileExists(filename))
  {

#ifndef GLOOST_SYSTEM_DISABLE_OUTPUT_WARNINGS
    std::cerr << std::endl << "ERROR in BinaryFile::read(): ";
    std::cerr << std::endl << "         While probing \"" << filename << "\"...";
    std::cerr << std::endl << "         File does not exist!";
    std::cerr << std::endl;
#endif

    return false;
  }

  // estimate file size
  auto size = getFileSize(filename);
  bufferSizeInByte = size;
#ifndef GLOOST_SYSTEM_DISABLE_OUTPUT_MESSAGES
    std::cerr << std::endl << "Message from BinaryFile::read(): ";
    std::cerr << std::endl << "             Reading \"" << filename << "\".";
    std::cerr << std::endl << "             " << bufferSizeInByte << " Bytes  (" << bufferSizeInByte/1048576.0f << " MB) to load...";
    std::cerr << std::endl;
#endif
  if (!size)
  {
    buffer = nullptr;
    return true;
  }

  // allocate
  buffer = new unsigned char[bufferSizeInByte];

  // read file
  boost::filesystem::ifstream myFile (filename, std::ios::in | std::ios::binary);
  if (!myFile.read ((char*)buffer, bufferSizeInByte))
  {

#ifndef GLOOST_SYSTEM_DISABLE_OUTPUT_WARNINGS
    std::cerr << std::endl << "ERROR in BinaryFile::read(): ";
    std::cerr << std::endl << "         While reading \"" << filename << "\"...";
    std::cerr << std::endl << "         Could not estimate filesize! Maybe the file";
    std::cerr << std::endl << "         does not exist in this universe?";
    std::cerr << std::endl;

#endif
    myFile.close();
    delete[] buffer;
    return false;
  }

  // close file
  myFile.close();
  return true;

}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   compatibility call
*/

/*static*/
bool
BinaryFile::write(const std::string& filename,
                  unsigned char*     buffer,
                  size_t             bufferSizeInByte)
{
  return write(gloost::PathType(filename), buffer, bufferSizeInByte);
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   writes a whole buffer to a file
  \param   filename path to the file to write to
  \param   buffer Pointer to an array of data
  \param   bufferSizeInByte Length of buffer in Bytes
  \remarks If the file does not exist it will be created. Existing files will be
           overwriten.
  \remarks This is an example how to write a buffer:
           \code
            struct Vertex{ float x; float y; float z; };

            // generate test data
            unsigned int numVertices = 100;
            std::vector<Vertex> vertices(numVertices, Vertex());

            for (unsigned int i=0; i!=numVertices; ++i)
            {
              Vertex v;
              v.x = i; v.y = i+1; v.z = i+2;
              vertices[i] = v;
            }

            // write
            gloost::BinaryFile::write("binaryFile_test.bin",
                                      (unsigned char*) &(*vertices.begin()),
                                      sizeof(Vertex)*vertices.size());
          \endcode
*/

/*static*/
bool
BinaryFile::write(const gloost::PathType& filename,
                  unsigned char*     buffer,
                  size_t             bufferSizeInByte)
{

#ifndef GLOOST_SYSTEM_DISABLE_OUTPUT_MESSAGES
  std::cerr << std::endl << "Message from BinaryFile::write(): ";
  std::cerr << std::endl << "             Writing \"" << filename << "\".";
  std::cerr << std::endl << "            " << bufferSizeInByte << " Bytes to write...";
  std::cerr << std::endl;

#endif


   boost::filesystem::ofstream myFile (filename, std::ios::out | std::ios::binary);

  if (!myFile.is_open())
  {
#ifndef GLOOST_SYSTEM_DISABLE_OUTPUT_WARNINGS
    std::cerr << std::endl << "ERROR in BinaryFile::write(): ";
    std::cerr << std::endl << "         Could NOT open file \"" << filename << "\" to write to.";
    std::cerr << std::endl;
#endif
    return false;
  }

  myFile.write ((char*)buffer, bufferSizeInByte);
  myFile.close();

  return true;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   compatibility call

*/

/*static*/
BinaryBundle::shared_ptr
BinaryFile::read(const std::string& filename)
{
  return read(gloost::PathType(filename));
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   reads a whole file and returns a gloost::BinaryBundle with its data
  \param   filename Path to the file to read from
  \remarks If the file could not be read, this methode will return 0

*/

/*static*/
BinaryBundle::shared_ptr
BinaryFile::read(const gloost::PathType& filename)
{
  if(!fileExists(filename))
  {
    gloost::log::GlobalLog::postDebug(GLOOST_CURRENT_LOCATION,
                                      "File not found: " + filename.string());
    return nullptr;
  }

  BinaryBundle::shared_ptr bundle = nullptr;

  // estimate file size
  auto size = boost::filesystem::file_size(filename);
  if ((size_t)size > (size_t)0)
  {
    bundle = BinaryBundle::create(size);

#ifndef GLOOST_SYSTEM_DISABLE_OUTPUT_MESSAGES
    std::cerr << std::endl << "Message from BinaryFile::read(): ";
    std::cerr << std::endl << "             Reading \"" << filename << "\".";
    std::cerr << std::endl << "             " << bundle->getCapacity() << " Bytes (" << bundle->getCapacity()/1048576.0f << " MB) to load...";
    std::cerr << std::endl;
#endif
  }
  else
  {
#ifndef GLOOST_SYSTEM_DISABLE_OUTPUT_WARNINGS
    std::cerr << std::endl << "ERROR in BinaryFile::read(): ";
    std::cerr << std::endl << "         While reading \"" << filename << "\"...";
    std::cerr << std::endl << "         Could not estimate filesize! Maybe the file";
    std::cerr << std::endl << "         does not exist in this universe?";
    std::cerr << std::endl;
#endif

    return nullptr;
  }


  // read file
  boost::filesystem::ifstream myFile (filename, std::ios::in | std::ios::binary);

  if (!myFile.read ( (char*)(bundle->data()),
                     bundle->getCapacity()))
  {
#ifndef GLOOST_SYSTEM_DISABLE_OUTPUT_WARNINGS
    std::cerr << std::endl;
    std::cerr << std::endl << "ERROR in BinaryFile::read(): ";
    std::cerr << std::endl << "         While reading \"" << filename << "\"...";
    std::cerr << std::endl << "         Could not estimate filesize! Maybe the file";
    std::cerr << std::endl << "         does not exist in this universe?";
    std::cerr << std::endl;
#endif

    myFile.close();
    return nullptr;
  }

  bundle->setPutPosition(bundle->getCapacity());


  // close file
  myFile.close();
  return bundle;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   compatibility call
*/

bool
BinaryFile::openAndLoad(const std::string& filename)
{
  return openAndLoad(gloost::PathType(filename));
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   opens and loads a binary file for reading
  \param   filename Path to the file to read from
  \remarks This loads the complete file into your Ram. Call BinaryFile::unload()
           to free the memory after use.
*/

bool
BinaryFile::openAndLoad(const gloost::PathType& filename)
{
  _filePath   = filename;
  _fileBuffer = 0;
  _fileSize   = 0;

  if (!gloost::BinaryFile::read(_filePath, _fileBuffer, _fileSize))
  {
    return false;
  }

//  std::cerr << std::endl << "filename: " << filename;
//  std::cerr << std::endl << "_fileSize: " << _fileSize;

  _bufferStart     = _fileBuffer;
  _currentPosition = _fileBuffer;
  _bufferEnd       = _fileBuffer + _fileSize;

  return true;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   "opens" a BinaryBundle to read from
  \param   bundle A gloost::BinaryBundle
  \remarks ...
*/

bool
BinaryFile::openAndLoad(BinaryBundle::shared_ptr bundle)
{
//  _filePath   = "BinaryBundle";
  _deleteBufferInDestructor = false;
  _fileBuffer               = bundle->data();
  _fileSize                 = bundle->getSize();

  _bufferStart     = _fileBuffer;
  _currentPosition = _fileBuffer;
  _bufferEnd       = _fileBuffer + _fileSize;

  return true;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   "opens" a BinaryBundle to read from
  \param    bundle A gloost::BinaryBundle
  \remarks This will create a copy of the BundleData within the reader
*/

bool
BinaryFile::openAndLoad(BinaryBundle::const_shared_ptr bundle)
{
  _deleteBufferInDestructor = true;
  _fileBuffer               = new unsigned char[bundle->getCapacity()];
  memcpy(_fileBuffer, bundle->data(), bundle->getCapacity());
  _fileSize                 = bundle->getCapacity();

  _bufferStart     = _fileBuffer;
  _currentPosition = _fileBuffer;
  _bufferEnd       = _fileBuffer + _fileSize;

  return true;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   "opens" custom binary data to read from
  \remarks ...
*/

bool
BinaryFile::openAndLoad(unsigned char* data, size_t dataSizeInBytes)
{
//  _filePath   = "BinaryBundle";
  _deleteBufferInDestructor = false;
  _fileBuffer               = data;
  _fileSize                 = dataSizeInBytes;

  _bufferStart     = _fileBuffer;
  _currentPosition = _fileBuffer;
  _bufferEnd       = _fileBuffer + _fileSize;

  return true;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Unloads the file from RAM
  \remarks ...
*/

void
BinaryFile::unload()
{
  if (_fileBuffer && _deleteBufferInDestructor)
  {
    _fileBuffer = _bufferStart;
	  delete[] _fileBuffer;
  }

  _fileBuffer      = 0;
  _bufferStart     = 0;
  _currentPosition = 0;
  _bufferEnd       = 0;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns the next word as string
  \remarks Words are seperated by ' ' (SPACE), '\t' (TAB), '\n' and '\r'
*/

std::string
BinaryFile::readWord()
{
  unsigned char* startPos = _currentPosition;

  while (_currentPosition != _bufferEnd && (*_currentPosition) != ' '
                                        && (*_currentPosition) != '\t'
                                        && (*_currentPosition) != '\n'
                                        && (*_currentPosition) != '\r')
  {
    ++_currentPosition;
  }

  std::string word("");
  word.append( (char*)startPos, _currentPosition-startPos );
  seekNextWord();

  return word;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Returns the whole line from current read position
  \remarks Lines are seperated by '\n' and '\r'. If the read position is
           somewhere within a line, only the rest of the line will be returned
*/

std::string
BinaryFile::readLine()
{
  std::string line = "";
  unsigned char* startPos = _currentPosition;

  while (_currentPosition != _bufferEnd && (*_currentPosition) != '\n'
                                        && (*_currentPosition) != '\r')
  {
    ++_currentPosition;
  }

  line.append( (char*)startPos, _currentPosition-startPos );
  seekNextWord();

  return line;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Reads 1 bytes of the buffer and returns it as a char value
  \remarks ...
*/

char
BinaryFile::readChar8()
{
  char value = (char)(*(reinterpret_cast<char*>(_currentPosition)));
  _currentPosition += 1;

  return value;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Reads 1 bytes of the buffer and returns it as a unsigned char value
  \remarks ...
*/

unsigned char
BinaryFile::readUChar8()
{
  unsigned char value = (unsigned char)(*(reinterpret_cast<unsigned char*>(_currentPosition)));
  _currentPosition += 1;

  return value;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   reads 2 bytes of the buffer and returns it as a short value
  \remarks ...
*/

unsigned short
BinaryFile::readUShort16()
{
  unsigned short value = (unsigned short)(*(reinterpret_cast<unsigned short*>(_currentPosition)));
  _currentPosition += 2;
  return value;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Reads 4 bytes of the buffer and returns it as a int value
  \remarks ...
*/

int
BinaryFile::readInt32()
{
  int value = int(*(reinterpret_cast<int*>(_currentPosition)));
  _currentPosition += 4;

  return value;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Reads 4 bytes of the buffer and returns it as a int value
  \remarks ...
*/

unsigned
BinaryFile::readUInt32()
{
  int value = int(*(reinterpret_cast<unsigned*>(_currentPosition)));
  _currentPosition += 4;

  return value;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Reads 4 bytes of the buffer and returns it as a float value
  \remarks ...
*/

float
BinaryFile::readFloat32()
{
  float value = float(*(reinterpret_cast<float*>(_currentPosition)));
  _currentPosition += 4;

  return value;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   reads 8 bytes of the buffer
  \remarks ...
*/

long unsigned
BinaryFile::readLongUnsigned()
{
  long unsigned value = (long unsigned)(*(reinterpret_cast<long unsigned*>(_currentPosition)));
  _currentPosition += 8;

  return value;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   reads 8 bytes of the buffer
  \remarks ...
*/

long int
BinaryFile::readLongInt()
{
  long int value = (long int)(*(reinterpret_cast<long int*>(_currentPosition)));
  _currentPosition += 8;
  return value;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   reads 8 bytes of the buffer
  \remarks ...
*/

double
BinaryFile::readFloat64()
{
  double value = double(*(reinterpret_cast<double*>(_currentPosition)));
  _currentPosition += 8;
  return value;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Reads numBytes bytes (or the rest) of the buffer
  \remarks The buffer will be allocated within this methode. The size of the Buffer
           is either numBytes or the number of bytes left in the file.
           In the second case, the argument numBytes will be changed to the number
           of bytes actually read.<br>
           Is numBytes given with 0, everything behind the current position in the
           file is read.<br>
           If end of file was allready reached, buffer will be set to 0.
*/

void
BinaryFile::readVarLength(unsigned char*& buffer,
                          size_t&      numBytes)
{
  if (numBytes)
  {
    numBytes = math::min(numBytes, (size_t) (_bufferEnd-_currentPosition) );
  }
  else
  {
    numBytes = (size_t) (_bufferEnd-_currentPosition);
  }

  if (numBytes)
  {
    buffer = new unsigned char[numBytes];
    memcpy ( buffer, _currentPosition, numBytes);
    _currentPosition += numBytes;
    return;
  }
  buffer = 0;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Reads numBytes bytes from the buffer
  \remarks
*/

bool
BinaryFile::readBuffer(unsigned char* buffer,
                       size_t      numBytes)
{
  if (numBytes > (size_t) (_bufferEnd-_currentPosition))
  {
    return false;
  }

  memcpy ( buffer, _currentPosition, numBytes);
  _currentPosition += numBytes;

  return true;
}


////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Reads numBytes bytes from the buffer
  \remarks
*/

/*static*/
const std::string
BinaryFile::readFileAsString(const gloost::PathType& filename)
{
  unsigned char* buffer           = 0u;
  size_t         bufferSizeInByte = 0u;

  if (!gloost::BinaryFile::read(filename, buffer, bufferSizeInByte))
  {
#ifndef GLOOST_SYSTEM_DISABLE_OUTPUT_ERRORS
    gloost::log::GlobalLog::postDebug(BOOST_CURRENT_FUNCTION, "Could NOT load source file");
#endif
    delete buffer;
    return std::string("");
  }

  char* source = new char[bufferSizeInByte+1];
  memcpy(source, buffer, bufferSizeInByte);
  source[bufferSizeInByte] = '\0';

  return std::string(source);
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Sets the current read position to the next character != ' ', '\t', '\n', '\r'
  \remarks ...
*/

void
BinaryFile::seekNextWord()
{
  while ((*_currentPosition) == ' ' || (*_currentPosition) == '\t'
                                    || (*_currentPosition) == '\n'
                                    || (*_currentPosition) == '\r')
  {
    ++_currentPosition;

    if (_currentPosition >= _bufferEnd)
    {
      break;
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Increments the read position by numBytes
  \remarks ...
*/

void
BinaryFile::advanceBy(size_t numBytes)
{
  _currentPosition += numBytes;
  if (_currentPosition > _bufferEnd)
  {
    _currentPosition = _bufferEnd;
  }
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   decrement the read position by numBytes
  \remarks ...
*/

void
BinaryFile::rewindBy(size_t numBytes)
{
  if (numBytes > (size_t)(_bufferStart-_currentPosition))
  {
    _currentPosition = 0u;
   return;
  }
  _currentPosition -= numBytes;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns the number of bytes left behind the current reading position
  \remarks ...
*/

size_t
BinaryFile::getBytesLeft() const
{
  return _bufferEnd - _currentPosition;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Returns true if end of file is reached
  \remarks ...
*/

bool
BinaryFile::eof() const
{
  return (_currentPosition >= _bufferEnd);
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns a pointer to the current position in the loaded data
  \remarks ...
*/

const unsigned char*
BinaryFile::getCurrent() const
{
  return _currentPosition;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns a pointer to the current position in the loaded data
  \remarks ...
*/

unsigned char*
BinaryFile::getCurrent()
{
  return _currentPosition;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns the current read position as index within the loaded file
  \remarks ...
*/

size_t
BinaryFile::getCurrentReadPosition() const
{
  return _currentPosition - _bufferStart;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   sets the current read position as index within the loaded file
  \remarks ...
*/

bool
BinaryFile::setCurrentReadPosition(size_t indexInBytes)
{
  if (_bufferStart+indexInBytes > _bufferEnd)
  {
    return false;
  }

  _currentPosition = _bufferStart+indexInBytes;
  return true;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   opens a file to write data piece by piece
  \remarks ...
*/

bool
BinaryFile::openToWrite(const std::string& filename)
{
  return openToWrite(gloost::PathType(filename));
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   opens a file to write data piece by piece
  \remarks ...
*/

bool
BinaryFile::openToWrite(const gloost::PathType& filename)
{
  if (!_fileToWriteTo)
  {
      _fileToWriteTo = OutputFileType(new boost::filesystem::ofstream(filename, std::ios::out | std::ios::binary));
  }

  if (!_fileToWriteTo->is_open())
  {
    std::cerr << std::endl << "ERROR in BinaryFile::openToWrite(const gloost::PathType& filename) ";
    std::cerr << std::endl << "         Could NOT open file \"" << filename << "\" to write to.";
    std::cerr << std::endl;

    _fileToWriteTo = nullptr;

    return false;
  }

  return true;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   opens a file to write data piece by piece
  \remarks ...
*/

bool
BinaryFile::openToAppend(const std::string& filename)
{
  return openToAppend(gloost::PathType(filename));
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   opens a file to write data piece by piece
  \remarks ...
*/

bool
BinaryFile::openToAppend(const gloost::PathType& filename)
{
  if (!_fileToWriteTo)
  {
    _fileToWriteTo = OutputFileType(new boost::filesystem::ofstream(filename, std::ios::out | std::ios::binary | std::ios::app ));
  }

  if (!_fileToWriteTo->is_open())
  {
    std::cerr << std::endl << "ERROR in BinaryFile::openToAppend(const gloost::PathType& filename): ";
    std::cerr << std::endl << "         Could NOT open file \"" << filename << "\" to write to.";
    std::cerr << std::endl;

    _fileToWriteTo = nullptr;

    return false;
  }

  return true;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   closes the file you wrote to
  \remarks You need to call this method only if you opened a file with
           BinaryFile::openToWrite(const std::string& filename)
*/

void
BinaryFile::close()
{
  if (_fileToWriteTo)
  {
    _fileToWriteTo->close();
    _fileToWriteTo = nullptr;
  }
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   closes the file you wrote to
  \remarks
*/


// writes a string to the file (length-1 byte)
void
BinaryFile::writeString(const std::string& string)
{
  _fileToWriteTo->write ((char*)string.c_str(), string.length());
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   writes a char to the file (1 byte)
  \remarks
*/

void
BinaryFile::writeChar8(char c)
{
  _fileToWriteTo->write ((char*)&c, 1);
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   writes a char to the file (1 byte)
  \remarks
*/

void
BinaryFile::writeUChar8(unsigned char c)
{
  _fileToWriteTo->write ((char*)&c, 1);
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   writes an signed integer value to the file (4 byte)
  \remarks
*/

void
BinaryFile::writeInt32(int i)
{
  _fileToWriteTo->write ((char*)&i, 4);
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   writes an unsigned integer value to the file (4 byte)
  \remarks
*/

void
BinaryFile::writeUInt32(unsigned ui)
{
  _fileToWriteTo->write ((char*)&ui, 4);
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   writes an float value to the file (4 byte)
  \remarks
*/

void
BinaryFile::writeFloat32(float f)
{
  _fileToWriteTo->write ((char*)&f, 4);
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   writes a long unsigned value to the file (8 byte)
  \remarks
*/

void
BinaryFile::writeLongUnsigned(long unsigned lu)
{
  _fileToWriteTo->write ((char*)&lu, 8);
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   writes a long int value to the file (8 byte)
  \remarks
*/

void
BinaryFile::writeLongInt(long int li)
{
  _fileToWriteTo->write ((char*)&li, 8);
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   writes a double value to the file (8 byte)
  \remarks
*/

void
BinaryFile::writeFloat64(double d)
{
  _fileToWriteTo->write ((char*)&d, 8);
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   writes a chunk of data to the file
  \remarks
*/

void
BinaryFile::writeBuffer(const unsigned char* buffer,
                        size_t               bufferSizeInByte)
{
  _fileToWriteTo->write ((char*)buffer, bufferSizeInByte);
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   writes a bundle of data to the file
  \remarks
*/

void
BinaryFile::writeBundle(const std::shared_ptr<const BinaryBundle>& bundle) const
{
  _fileToWriteTo->write ((char*)bundle->data(), bundle->getSize());
}

////////////////////////////////////////////////////////////////////////////////

} // namespace gloost
