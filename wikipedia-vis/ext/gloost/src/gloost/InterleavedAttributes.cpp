
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
#include <gloost/InterleavedAttributes.h>
#include <gloost/Mesh.h>
#include <gloost/BinaryBundle.h>
#include <gloost/BinaryFile.h>
#include <gloost/gloostSerial.h>

// cpp includes
#include <string>
#include <iostream>

namespace gloost
{

/**
  \class   InterleavedAttributes

  \brief   A vector of interleaved float attributes with a layout description to be used as input for a VBO or vertex array

  \author  Felix Weiszig, Henning Gruendl
  \date    November 2011
  \remarks ...
*/

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   creates an InterleavedAttributes instance
  \param   ...
  \remarks ...
*/

/*static*/
InterleavedAttributes::shared_ptr
InterleavedAttributes::create()
{
  return shared_ptr(new InterleavedAttributes());
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   creates an InterleavedAttributes instance
  \param   ...
  \remarks ...
*/

/*static*/
InterleavedAttributes::shared_ptr
InterleavedAttributes::create(const const_shared_ptr& original)
{
  if (original)
  {
    auto instanceOnAStick = shared_ptr(new InterleavedAttributes());
    instanceOnAStick->_vector             = original->_vector;
    instanceOnAStick->_layout             = original->_layout;
    instanceOnAStick->_packageStride      = original->_packageStride;
    instanceOnAStick->_elementsPerPackage = original->_elementsPerPackage;
    return instanceOnAStick;
  }
  return nullptr;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   creates an InterleavedAttributes instance
  \param   ...
  \remarks ...
*/

/*static*/
InterleavedAttributes::shared_ptr
InterleavedAttributes::create(size_t numValues)
{
  return shared_ptr(new InterleavedAttributes(numValues));
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   creates an InterleavedAttributes instance
  \param   ...
  \remarks ...
*/

/*static*/
InterleavedAttributes::shared_ptr
InterleavedAttributes::create(const LayoutVector& attribLayout, size_t numPackages)
{
  return shared_ptr(new InterleavedAttributes(attribLayout, numPackages));
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   creates an InterleavedAttributes instance
  \param   ...
  \remarks ...
*/

/*static*/
InterleavedAttributes::shared_ptr
InterleavedAttributes::create(const std::shared_ptr<const Mesh>& mesh)
{
  return shared_ptr(new InterleavedAttributes((mesh)));
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   creates an InterleavedAttributes instance
  \param   ...
  \remarks ...
*/

/*static*/
InterleavedAttributes::shared_ptr
InterleavedAttributes::create(const BinaryBundle& bundle)
{
  return shared_ptr(new InterleavedAttributes(bundle));
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   creates an InterleavedAttributes instance
  \param   ...
  \remarks ...
*/

/*static*/
InterleavedAttributes::shared_ptr
InterleavedAttributes::create(const gloost::PathType& filePath)
{
  return shared_ptr(new InterleavedAttributes(filePath));
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Class constructor
  \remarks ...
*/

InterleavedAttributes::InterleavedAttributes():
    _vector(),
    _layout(),
    _packageStride(0u),
    _elementsPerPackage(0u)
{
	// insert your code here
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Class constructor
  \remarks ...
*/

InterleavedAttributes::InterleavedAttributes(size_t numValues):
    _vector(numValues, 0.0f),
    _layout(),
    _packageStride(0u),
    _elementsPerPackage(0u)
{
	// insert your code here
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Class constructor
  \remarks ...
*/

InterleavedAttributes::InterleavedAttributes(const LayoutVector& attribLayout, size_t numPackages):
    _vector(),
    _layout(attribLayout),
    _packageStride(0u),
    _elementsPerPackage(0u)
{
  for (unsigned i=0; i!=_layout.size(); ++i)
  {
    _packageStride      += _layout[i]._stride;
    _elementsPerPackage += _layout[i]._numElements;
  }
  if (numPackages)
  {
    resize(numPackages);
  }
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Class constructor
  \remarks ...
*/

InterleavedAttributes::InterleavedAttributes(const std::shared_ptr<const Mesh>& mesh):
    _vector(),
    _layout(),
    _packageStride(0u),
    _elementsPerPackage(0u)
{
  std::shared_ptr<const Mesh> interleavedMesh = nullptr;

  if (mesh->getInterleavedAttributes().size())
  {
    interleavedMesh = mesh;
  }
  else
  {
    interleavedMesh = gloost::Mesh::create(mesh, true);
  }

  if (interleavedMesh->getInterleavedAttributes().size())
  {
    if (interleavedMesh->isInterleavedAttributeSupported(GLOOST_MESH_POSITIONS))
    {
      addAttribute(3, 12, "in_position");
    }
    if (interleavedMesh->isInterleavedAttributeSupported(GLOOST_MESH_NORMALS))
    {
      addAttribute(3, 12, "in_normal");
    }
    if (interleavedMesh->isInterleavedAttributeSupported(GLOOST_MESH_COLORS))
    {
      addAttribute(4, 16, "in_color");
    }
    if (interleavedMesh->isInterleavedAttributeSupported(GLOOST_MESH_TEXCOORDS))
    {
      addAttribute(2, 8, "in_texcoord");
    }
    _vector = interleavedMesh->getInterleavedAttributes();
  }
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Class constructor
  \remarks ...
*/

InterleavedAttributes::InterleavedAttributes(const BinaryBundle& bundle):
    _vector(bundle.getSize()/sizeof(float)),
    _layout(),
    _packageStride(0u),
    _elementsPerPackage(0u)
{
  memcpy(&_vector.front(), bundle.data(), bundle.getSize());
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Class constructor
  \remarks ...
*/

InterleavedAttributes::InterleavedAttributes(const gloost::PathType& filePath):
    _vector(),
    _layout(),
    _packageStride(0u),
    _elementsPerPackage(0u)
{
  loadFromFile(filePath);
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   Class destructor
  \remarks ...
*/

InterleavedAttributes::~InterleavedAttributes()
{

//	std::cerr << std::endl << "InterleavedAttributes::~InterleavedAttributes(): " << this;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   adds an interleaved attribute to the layout
  \param   ...
  \remarks ...
*/

void
InterleavedAttributes::addAttribute(unsigned numElements,
                                    unsigned stride,
                                    const    std::string& name)
{
	_layout.push_back(AttributeInfo(numElements, stride, name));
  _packageStride      += stride;
  _elementsPerPackage += numElements;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   adds an interleaved attribute to the layout
  \param   ...
  \remarks ...
*/

void
InterleavedAttributes::addAttribute(unsigned numElements,
                                    const    std::string& name)
{
	_layout.push_back(AttributeInfo(numElements, sizeof(float)*numElements, name));
  _packageStride      += sizeof(float)*numElements;
  _elementsPerPackage += numElements;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   resizes the internal vector to fit numPackages packages
  \param   numPackages number of packages one wish to store within this container.
  \remarks The actual size of the internal vector will be numPackages*getNumElementsPerPackage().
           Don't forget to define a layout first by calling addAttribute(numElements, stride, name)
*/

void
InterleavedAttributes::resize(size_t numPackages)
{
	_vector.resize(numPackages*getNumElementsPerPackage());
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   fills all elements of all packages with value
  \param   ...
  \remarks ...
*/

void
InterleavedAttributes::fill(float value)
{
	_vector = std::vector<float>(_vector.size(), value);
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief   deletes all interleaved data and frees the memory but keeps the layout
  \param   ...
  \remarks ...
*/

void
InterleavedAttributes::clear()
{
	std::vector<float>().swap(_vector);
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief returns the vector of interleaved attributes
  \remarks ...
*/

std::vector<float>&
InterleavedAttributes::getVector()
{
  return _vector;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief returns the vector of interleaved attributes
  \remarks ...
*/

const std::vector<float>&
InterleavedAttributes::getVector() const
{
  return _vector;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   adds a float value on the end of the interleaved attributes
  \remarks ...
*/

void
InterleavedAttributes::push_back(float value)
{
  _vector.push_back(value);
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   adds values on the end of the interleaved attributes
  \remarks ...
*/

void
InterleavedAttributes::push_back(const std::vector<float>& values)
{
  _vector.insert(_vector.end(), values.begin(), values.end());
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief returns the vector of information for each attrib within the interleaved structure
  \remarks ...
*/

const InterleavedAttributes::LayoutVector&
InterleavedAttributes::getLayout() const
{
  return _layout;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief returns the layout of this Attributes to be used in the vertex shader
  \remarks ...
*/

std::string
InterleavedAttributes::getLayoutString() const
{
  std::string layoutString = "\n// vertex attribute layout \n";

  for (unsigned int i=0; i!=_layout.size(); ++i)
  {
    std::string typeString = "unknowType";

    switch (_layout[i]._numElements)
    {
      case 1:
        typeString = "float";
        break;
      case 2:
        typeString = "vec2";
        break;
      case 3:
        typeString = "vec3";
        break;
      case 4:
        typeString = "vec4";
        break;
      case 6:
        typeString = "mat2";
        break;
      case 12:
        typeString = "mat3";
        break;
      case 16:
        typeString = "mat4";
        break;
    }

    layoutString += "layout(location=" + gloost::helper::toString(i) + ") in " + typeString + " " +  _layout[i]._name + ";\n";
  }

  layoutString += "\n";

  return layoutString;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns the number of packages
  \remarks This will return the number of single values if no layout was defined
*/

unsigned
InterleavedAttributes::getNumPackages() const
{
  if (_elementsPerPackage)
  {
    return _vector.size()/(float)_elementsPerPackage;
  }
  return _vector.size();
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns the number of Elements per Package
  \remarks This will return 1 if no layout was defined
*/

unsigned
InterleavedAttributes::getNumElementsPerPackage() const
{
  if (!_elementsPerPackage)
  {
    return 1;
  }

  return _elementsPerPackage;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief returns the size of one package in bytes
  \remarks ...
*/

unsigned
InterleavedAttributes::getPackageStride() const
{
  return _packageStride;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns the index of the first element of a package
  \param   packageId number of the package
  \remarks This will return the nth element if no layout was defined
*/

unsigned
InterleavedAttributes::getPackageIndex(unsigned packageId) const
{
  return packageId*getNumElementsPerPackage();
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   writes attribs to a file
  \remarks Use the file extension ".ia" to avoid confusion
*/

bool
InterleavedAttributes::writeToFile(const gloost::PathType& filePath) const
{
  BinaryFile outFile;
  if (!outFile.openToWrite(filePath))
  {
    std::cerr << std::endl;
    std::cerr << std::endl << "Warning from InterleavedAttributes::writeToFile(): ";
    std::cerr << std::endl << "             Could NOT write to filePath" << filePath;
    return false;
  }

  writeToFile(outFile);

  outFile.close();

  return true;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   writes attribs to a file
  \remarks Use the file extension ".ia" to avoid confusion
*/

bool
InterleavedAttributes::writeToFile(gloost::BinaryFile& outFile) const
{

  outFile.writeString("gloost_interleaved_attributes");
  outFile.writeString(" version1 ");
  outFile.writeString(gloost::helper::toString(_layout.size()) + " ");


  for (unsigned l=0; l!=_layout.size(); ++l)
  {
    outFile.writeString(_layout[l]._name + " ");
    outFile.writeString(gloost::helper::toString(_layout[l]._numElements) + " ");
    outFile.writeString(gloost::helper::toString(_layout[l]._stride) + " ");
  }

  outFile.writeString(gloost::helper::toString(_vector.size()) + " ");


  outFile.writeBuffer((unsigned char*)&_vector.front(),
                      _vector.size()*sizeof(float));

  return true;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief load from file
  \remarks ...
*/

bool
InterleavedAttributes::loadFromFile(const gloost::PathType& filePath)
{

  BinaryFile inFile;
  if (!inFile.openAndLoad(filePath))
  {
    std::cerr << std::endl << "Warning from InterleavedAttributes::loadFromFile(): ";
    std::cerr << std::endl << "             Could NOT open filePath" << filePath;
    return false;
  }

  loadFromFile(inFile);
  inFile.unload();
  return true;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief load from file
  \remarks ...
*/

bool
InterleavedAttributes::loadFromFile(gloost::BinaryFile& inFile)
{
  _vector.clear();
  _layout.clear();
  _packageStride = 0;


  if (inFile.readWord() != "gloost_interleaved_attributes")
  {
    std::cerr << std::endl << "Warning from InterleavedAttributes::loadFromFile(): ";
    std::cerr << std::endl << "             File does NOT begin with string \"gloost_interleaved_attributes\"";
    return false;
  }

  if (inFile.readWord() != "version1")
  {
    std::cerr << std::endl << "Warning from InterleavedAttributes::loadFromFile(): ";
    std::cerr << std::endl << "             Unsupported version string";
    return false;
  }

  // reading layout
  unsigned numAttributes = atoi(inFile.readWord().c_str());

  _layout.resize(numAttributes, AttributeInfo(0, 0, "empty") );


  for (unsigned l=0; l!=numAttributes; ++l)
  {
    _layout[l]._name        = inFile.readWord();
    _layout[l]._numElements = atoi(inFile.readWord().c_str());
    _layout[l]._stride      = atoi(inFile.readWord().c_str());
    _packageStride         += _layout[l]._stride;
    _elementsPerPackage    += _layout[l]._numElements;
  }

  // reading data
  unsigned numValues = atoi(inFile.readWord().c_str());

  _vector.resize(numValues);
  gloost::serial::unserialize(&_vector.front(), inFile, numValues*sizeof(float));

  return true;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   adds the conten of another InterleavedAttributes container to this one
  \remarks ...
*/

void
InterleavedAttributes::add(const_shared_ptr moreAttributes)
{
  if (!moreAttributes || !moreAttributes->getVector().size())
  {
    return;
  }

  size_t index = _vector.size();
  _vector.resize(index + moreAttributes->getVector().size());

  memcpy((unsigned char*)  &_vector[index],
          moreAttributes->getData(),
          moreAttributes->getVector().size()*sizeof(float));
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns a unsigned char* to the beginning auf the attribute data
  \remarks ...
*/

unsigned char*
InterleavedAttributes::getData()
{
 return (unsigned char*)&_vector.front();
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns a unsigned char* to the beginning auf the attribute data
  \remarks ...
*/

const unsigned char*
InterleavedAttributes::getData() const
{
 return (unsigned char*)&_vector.front();
}

////////////////////////////////////////////////////////////////////////////////

float&
InterleavedAttributes::operator[](unsigned a)
{
  return _vector[a];
}

////////////////////////////////////////////////////////////////////////////////

const float&
InterleavedAttributes::operator[](unsigned a) const
{
  return _vector[a];
}

////////////////////////////////////////////////////////////////////////////////

} // namespace gloost
