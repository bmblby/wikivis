
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



#ifndef H_GLOOST_INTERLEAVEDATTRIBUTES
#define H_GLOOST_INTERLEAVEDATTRIBUTES



// gloost system includes
#include <gloost/gloostConfig.h>
#include <gloost/gloostMath.h>
#include <gloost/BinaryFile.h>


// cpp includes
#include <string>



namespace gloost
{

  class Mesh;
  class BinaryBundle;


  //  A vector of interleaved float attributes with a layout description to be used as input for a VBO or vertex array

class InterleavedAttributes
{
	public:

    /// a std::shared_ptr of an InterleavedAttributes instance
    typedef std::shared_ptr<InterleavedAttributes>       shared_ptr;
    typedef std::shared_ptr<const InterleavedAttributes> const_shared_ptr;

    // specifies one attribute and its components of a InterleavedAttributes class
	  struct AttributeInfo
	  {
	    AttributeInfo(unsigned numElements, unsigned stride, const std::string& name)
	    {
        _numElements   = numElements;
        _stride        = stride;
        _name          = name;
	    }

      // number of elements
	    unsigned _numElements;

	    // length in bytes for this attribute
	    unsigned _stride;

	    // name of the attrib location within the shader
	    std::string _name;
	  };

    // container of attribute properties
	  typedef std::vector<AttributeInfo> LayoutVector;

    // creates a InterleavedAttributes instance
    static shared_ptr create();
    static shared_ptr create(const const_shared_ptr& original);
    static shared_ptr create(size_t numValues);
    static shared_ptr create(const LayoutVector& attribLayout, size_t numPackages = 0u);
    static shared_ptr create(const std::shared_ptr<const Mesh>& mesh);
    static shared_ptr create(const BinaryBundle& bundle);
    static shared_ptr create(const gloost::PathType& filePath);

    // class destructor
	  virtual ~InterleavedAttributes();


	  // adds an interleaved attrib to the layout
	  void addAttribute(unsigned numElements,
                      unsigned stride,
                      const    std::string& name);

	  // adds an interleaved attrib to the layout
	  void addAttribute(unsigned numElements,
                      const    std::string& name);

    // resizes the internal vector to fit numPackages packages
    void resize(size_t numPackages);

    // fills all elements of all packages with value
    void fill (float value);

    // deletes all interleaved data and frees the memory but keeps the layout
    void clear();

	  // returns the vector of interleaved attributes
	  std::vector<float>&       getVector();
	  const std::vector<float>& getVector() const;

    // adds a float value on the end of the interleaved attributes
	  void push_back(float value);

    // adds values on the end of the interleaved attributes
	  void push_back(const std::vector<float>& values);

	  // returns the vector of information for each attrib within the interleaved structure
	  const LayoutVector& getLayout() const;

	  // returns the layout of this Attributes to be used in the vertex shader
	  std::string getLayoutString() const;

	  // returns the number of packages
	  unsigned getNumPackages() const;

	  // returns the number of Elements per Package
	  unsigned getNumElementsPerPackage() const;

	  // returns the size of one package in bytes
	  unsigned getPackageStride() const;


	  // returns the index of the first element of a package
	  unsigned getPackageIndex(unsigned packageId) const;


	  // adds the conten of another InterleavedAttributes container to this one
	  void add(const_shared_ptr moreAttributes);


	  bool writeToFile(const gloost::PathType& filePath) const;
	  bool writeToFile(gloost::BinaryFile& outFile) const;

	  bool loadFromFile(const gloost::PathType& filePath);
	  bool loadFromFile(gloost::BinaryFile& inFile);



    // returns a unsigned char* to the beginning auf the attribute data
    unsigned char* getData();
    const unsigned char* getData() const;


	  // indexing
    float& operator[](unsigned int);
    const float& operator[](unsigned int) const;


	protected:

    // class constructors
    InterleavedAttributes();
    InterleavedAttributes(size_t size);
    InterleavedAttributes(const LayoutVector& attribLayout, size_t size);
    InterleavedAttributes(const std::shared_ptr<const Mesh>& mesh);
    InterleavedAttributes(const BinaryBundle& bundle);
    InterleavedAttributes(const gloost::PathType& filePath);

    /// vector of interleaved attributes
    std::vector<float>      _vector;

    /// stores information about the interleaved structure
    LayoutVector _layout;

    // package size
    unsigned                _packageStride;
    unsigned                _elementsPerPackage;
};


} // namespace gloost


#endif // H_GLOOST_INTERLEAVEDATTRIBUTES


