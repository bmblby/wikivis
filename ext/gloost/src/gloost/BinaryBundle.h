
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



#ifndef H_GLOOST_BINARYBUNDLE
#define H_GLOOST_BINARYBUNDLE



// gloost system includes


// cpp includes
#include <string>
#include <vector>
#include <memory>


namespace gloost
{

  class BinaryFile;


  //  Bundle of binary data aka serialized objects

class BinaryBundle : public std::enable_shared_from_this<BinaryBundle>
{
	public:

    /// a std::shared_ptr of a BinaryBundle instance
    typedef std::shared_ptr<BinaryBundle>       shared_ptr;
    typedef std::shared_ptr<const BinaryBundle> const_shared_ptr;

    // creates a BinaryBundle instance
    static shared_ptr create(size_t capacityInByte);
    static shared_ptr create(const unsigned char* data, size_t size);

    // copies a BinaryBundle instance
    static shared_ptr create(const_shared_ptr bundle);

    // joins a Binaryundles of a vector
    static shared_ptr create(std::vector<const_shared_ptr> bundleVector);


    // class destructor
	  virtual ~BinaryBundle();

    // returns the capacity (in bytes)
	  size_t getCapacity() const;

    // returns the size from start to current put position (in bytes)
	  size_t getSize() const;

    // returns the free elements left in the bundle
	  size_t getFreeSpace() const;


	  // inserts a BinaryBundle at the current position and increments the position
	  bool put(std::shared_ptr<const BinaryBundle> bundle);

	  // inserts sizeInBytes bytes of a BinaryFiles current read position into the bundle
	  bool put(BinaryFile& sourceFile, size_t sizeInBytes);

	  // inserts sizeInBytes bytes of a BinaryFiles current read position into the bundle
	  void putUnsave(BinaryFile& sourceFile, size_t sizeInBytes);

	  // inserts a generic buffer at the current position and increments the position
	  bool put(const unsigned char* buffer, size_t sizeInBytes);

	  // inserts a generic buffer at the current position and increments the position
	  void putUnsave(const unsigned char* buffer, size_t sizeInBytes);

	  // inserts single character at the current position and increments the position
	  bool putChar(unsigned char character);

	  // inserts a unsigned int at the current position and increments the position
	  bool putUnsigned(unsigned number);

	  // inserts a float at the current position and increments the position
	  bool putFloat(float number);

	  // inserts a double at the current position and increments the position
	  bool putDouble(double number);


	  /// inserts a generic type into the BinaryBundle
	  template <class genericType>
	  bool putGeneric(genericType value)
	  {
      return put((unsigned char*)&value, sizeof(genericType));
	  }

	  /// inserts a generic type into the BinaryBundle without checking available space in the BinaryBundle
	  template <class genericType>
	  void putGenericUnsave(genericType value)
	  {
      return putUnsave((unsigned char*)&value, sizeof(genericType));
	  }


	  // sets the current put position lengthInBytes back
	  void rewind(size_t lengthInBytes = 0);

	  // sets the current put position lengthInBytes forward
	  void forward(size_t lengthInBytes = 0);

	  // sets the current put position relative to the begining
	  void setPutPosition(size_t relativePositionInBytes);



	  // returns a pointer to the first buffer element
	  unsigned char*       data();
	  const unsigned char* data() const;

    // returns a pointer to the current put position within the BinaryBundle
    unsigned char*       getCurrentPtr();
    const unsigned char* getCurrentPtr() const;



	  // Factory methode creates and returns a BinaryFile object which was openend with this BinaryBundle to read from it
	  std::shared_ptr<BinaryFile> createReader() const;


    /// assignment
    BinaryBundle& operator= (const BinaryBundle&);


  protected:

    // class constructor
    BinaryBundle(size_t capacity);

    // class constructor
    BinaryBundle(const unsigned char* data, size_t size);


	private:

   size_t         _capacity;

   unsigned char* _buffer;
   unsigned char* _startPtr;
   unsigned char* _endPtr;
   unsigned char* _currentPtr;


};




/// join two bundles to create a third
extern BinaryBundle::shared_ptr operator+(BinaryBundle::const_shared_ptr bundle1, BinaryBundle::const_shared_ptr bundle2);

/// vector addition
extern BinaryBundle::shared_ptr operator+(BinaryBundle::const_shared_ptr, BinaryBundle::const_shared_ptr);


} // namespace gloost


#endif // H_GLOOST_BINARYBUNDLE


