
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



#ifndef H_GLOOST_BINARYFILE
#define H_GLOOST_BINARYFILE


// gloost includes
#include <gloost/gloostConfig.h>
#include <gloost/BinaryBundle.h>

// boost
#include <boost/filesystem/fstream.hpp>

// cpp includes
#include <string>
#include <sys/stat.h>




namespace gloost
{


  //  Reads and writes binary files

class BinaryFile
{
	public:
    /// a std::shared_ptr of a BinaryFile instance
    typedef std::shared_ptr<BinaryFile>       shared_ptr;
    typedef std::shared_ptr<const BinaryFile> const_shared_ptr;

    // creates a BinaryFile instance
    static shared_ptr create();

    typedef std::shared_ptr<boost::filesystem::ofstream>  OutputFileType;


    // class constructor
    BinaryFile();

    // class destructor
	  ~BinaryFile();

    // returns the size of a file in bytes if the file exists
	  static bool pathExists  (const std::string& filename);
	  static bool pathExists  (const gloost::PathType& filename);
	  static bool fileExists  (const gloost::PathType& filename);
	  static bool folderExists(const gloost::PathType& filename);

    static size_t getFileSize(const std::string& filename);
	  static size_t getFileSize(const gloost::PathType& filename);


    // reads a file from file
	  static bool read(const std::string& filename,
                     unsigned char*& buffer,
                     size_t& bufferSizeInBytes);

    // reads a file from file
	  static bool read(const gloost::PathType& filename,
                     unsigned char*& buffer,
                     size_t& bufferSizeInBytes);

    // reads a file and returns a BinaryBundle
	  static BinaryBundle::shared_ptr read(const std::string& filename);


    // reads a file and returns a BinaryBundle
	  static BinaryBundle::shared_ptr read(const gloost::PathType& filename);


    // writes a whole buffer to a file
	  static bool write(const std::string& filename,
                      unsigned char* buffer,
                      size_t bufferSizeInBytes);

    // writes a whole buffer to a file
	  static bool write(const gloost::PathType& filename,
                      unsigned char* buffer,
                      size_t bufferSizeInBytes);

    // writes a BinaryBundle to a file
	  static bool write(const std::string& filename,
                      const BinaryBundle& bundle);

    // writes a BinaryBundle to a file
	  static bool write(const gloost::PathType& filename,
                      const BinaryBundle& bundle);


    // READ METHODES

    // opens and loads a binary file for reading
    bool openAndLoad(const std::string& filename);

    // opens and loads a binary file for reading
    bool openAndLoad(const gloost::PathType& filename);

    // "opens" a BinaryBundle to read from
    bool openAndLoad(BinaryBundle::shared_ptr bundle);
    bool openAndLoad(BinaryBundle::const_shared_ptr bundle);

    // "opens" custom binary data to read from
    bool openAndLoad(unsigned char* data, size_t dataSizeInBytes);

    // unloads the file from RAM
    void unload();

    // opens a file to write data piece by piece
    bool openToWrite(const std::string& filename);

    // WRITE METHODES
    // opens a file to write data piece by piece
    bool openToWrite(const gloost::PathType& filename);

    // opens a file to append data piece by piece
    bool openToAppend(const std::string& filename);

    // opens a file to append data piece by piece
    bool openToAppend(const gloost::PathType& filename);

    // closes the file you wrote to
    void close();



    // returns the next word as string (words are seperated by ' ', '\t', '\n' and '\r')
    std::string readWord();

    // returns the next word as string (words are seperated by seperator, '\n' and '\r')
    std::string readWord(char seperator);

    // returns the whole line from current read position
    std::string readLine();

    // reads 1 bytes of the buffer and returns it as a unsigned char value
    char readChar8();

    // reads 1 bytes of the buffer and returns it as a unsigned char value
    unsigned char readUChar8();

    // reads 1 bytes of the buffer and returns it as a unsigned char value
    unsigned short readUShort16();

    // reads 4 bytes of the buffer and returns it as a int value
    int readInt32();

    // reads 4 bytes of the buffer and returns it as a int value
    unsigned readUInt32();

    // reads 4 bytes of the buffer and returns it as a float value
    float readFloat32();

    // reads 8 bytes of the buffer
    long unsigned readLongUnsigned();

    // reads 8 bytes of the buffer
    long int readLongInt();

    // reads 8 bytes of the buffer
    double readFloat64();


    // reads numBytes bytes of the file to a buffer
    void readVarLength(unsigned char*& buffer,
                       size_t&         numBytes);

    // reads numBytes bytes of the file to a buffer
    bool readBuffer(unsigned char*  buffer,
                    size_t          numBytes);

    static const std::string readFileAsString(const gloost::PathType& filename);


    // sets the current read position to the next character != ' ', '\t', '\n' or '\r'
    void seekNextWord();

    // increments the read position by numBytes
    void advanceBy(size_t numBytes);

    // decrement the read position by numBytes
    void rewindBy(size_t numBytes);

    // returns the number of bytes left behind the current reading position
    size_t getBytesLeft() const;

    // returns true if end of file is reached
    bool eof() const;

    // returns a pointer to the current position in the loaded data
    const unsigned char* getCurrent() const;

    // returns a pointer to the current position in the loaded data
    unsigned char* getCurrent();

    // returns the current read position within the loaded file
    size_t getCurrentReadPosition() const;

    // sets the current read position within the loaded file
    bool setCurrentReadPosition(size_t indexInBytes);




    // WRITE METHODES

    // writes a string to the file (length-1 byte)
    void writeString(const std::string& s);

    // writes a char to the file (1 byte)
    void writeChar8(char c);

    // writes a char to the file (1 byte)
    void writeUChar8(unsigned char c);

    // writes an signed integer value to the file (4 byte)
    void writeUInt32(unsigned i);

    // writes an integer value to the file (4 byte)
    void writeInt32(int i);

    // writes an float value to the file (4 byte)
    void writeFloat32(float f);

    // writes a long unsigned value to the file (8 byte)
    void writeLongUnsigned(long unsigned lu);

    // writes a long int value to the file (8 byte)
    void writeLongInt(long int li);

    // writes a double value to the file (8 byte)
    void writeFloat64(double d);

    // writes a chunk of data to the file
    void writeBuffer(const unsigned char* buffer,
                     size_t               bufferSizeInByte);

    // writes a bundle of data to the file
    void writeBundle(const std::shared_ptr<const BinaryBundle>& bundle) const;


	private:


    gloost::PathType    _filePath;
    unsigned char*      _fileBuffer;
    size_t              _fileSize;

    unsigned char*      _bufferStart;
    unsigned char*      _currentPosition;
    unsigned char*      _bufferEnd;

    bool                _deleteBufferInDestructor;

    OutputFileType      _fileToWriteTo;


};


} // namespace gloost


#endif // H_GLOOST_BINARYFILE


