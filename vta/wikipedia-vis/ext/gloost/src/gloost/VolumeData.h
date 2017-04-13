
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

#ifndef H_GLOOST_VOLUME_DATA
#define H_GLOOST_VOLUME_DATA

// gloost system includes
#include <gloost/gloostConfig.h>
#include <gloost/Vector3.h>
#include <gloost/gloostMath.h>
#include <gloost/BinaryFile.h>
#include <gloost/BoundingBox.h>
#include <gloost/util/ValueRange.h>

// cpp includes
#include <string>
#include <vector>
#include <atomic>
#include <omp.h>

namespace gloost
{

/**
  \class   VolumeData

  \brief   template class, stores different kinds of volume data

  \author  Felix Weiszig, dynardo GmbH
  \date    March 2013
  \remarks ...
*/

template <class VoxelType=gloost::mathType>
class VolumeData
{
	public:

    /// a std::shared_ptr of a FeFaceSample instance
    typedef std::shared_ptr< VolumeData<VoxelType> >       shared_ptr;
    typedef std::shared_ptr< const VolumeData<VoxelType> > const_shared_ptr;

    ///////////////////////////////////////////////////////////////////////////

    /// class factory
    static shared_ptr create( unsigned resolutionX ,unsigned resolutionY, unsigned resolutionZ)
    {
      auto volume = new VolumeData(resolutionX, resolutionY, resolutionZ);
      return std::shared_ptr<VolumeData <VoxelType> >(volume);
    }

    ///////////////////////////////////////////////////////////////////////////

    /// class factory
    static shared_ptr create( const gloost::Aabb& aabb, unsigned maxResolution)
    {
      auto instance = std::shared_ptr<VolumeData <VoxelType> >(new VolumeData( aabb , maxResolution));
      return instance;
    }

    ///////////////////////////////////////////////////////////////////////////

    /// class factory
    static shared_ptr create( gloost::Vector3 resolution)
    {
      auto volume = new VolumeData(resolution[0], resolution[1], resolution[2]);
      return std::shared_ptr<VolumeData <VoxelType> >(volume);
    }

    /// class factory
    static shared_ptr create( gloost::Vector3 resolution, const VoxelType& fillValue)
    {
      auto volume = new VolumeData(resolution[0], resolution[1], resolution[2]);
      volume->fill(fillValue);
      return std::shared_ptr<VolumeData <VoxelType> >(volume);
    }

    ///////////////////////////////////////////////////////////////////////////

    /// class factory
    static shared_ptr create(const gloost::PathType& filePath)
    {
      auto volume = new VolumeData(filePath);
      return std::shared_ptr<VolumeData <VoxelType> >(volume);
    }

    ///////////////////////////////////////////////////////////////////////////

    /// class factory
    static shared_ptr create(const gloost::PathType& filePath, unsigned resolutionX ,unsigned resolutionY, unsigned resolutionZ)
    {
      auto volume = new VolumeData(filePath, resolutionX, resolutionY, resolutionZ);
      return std::shared_ptr<VolumeData <VoxelType> >(volume);
    }

    ///////////////////////////////////////////////////////////////////////////

    /// class factory
    static shared_ptr create( unsigned resolutionX ,unsigned resolutionY, unsigned resolutionZ,
                              const VoxelType& fillValue)
    {
      auto volume = new VolumeData(resolutionX, resolutionY, resolutionZ);
      volume->fill(fillValue);
      return std::shared_ptr<VolumeData <VoxelType> >(volume);
    }

    ///////////////////////////////////////////////////////////////////////////

    /// class factory
    static shared_ptr create()
    {
      auto volume = new VolumeData(1, 1, 1);
      return std::shared_ptr<VolumeData <VoxelType> >(volume);
    }

    ///////////////////////////////////////////////////////////////////////////

    /// class factory
    static shared_ptr create(const const_shared_ptr& volumeData)
    {
      auto volume = new VolumeData(volumeData->getResolution()[0],
                                   volumeData->getResolution()[1],
                                   volumeData->getResolution()[2]);

      volume->_voxelSize = volumeData->_voxelSize;
      volume->_data = volumeData->_data;
      volume->_aabb = volumeData->_aabb;

      return shared_ptr(volume);
    }

    ///////////////////////////////////////////////////////////////////////////

    /// class destructor
	  virtual ~VolumeData()
    {
      // insert your code here
    }

    ///////////////////////////////////////////////////////////////////////////

    /**
      \brief   fills the volume with a particular value
    */
    void fill(const VoxelType& value)
    {
      std::vector<VoxelType> huhu(_data.size(), value);
      _data.swap(huhu);
    }

    ///////////////////////////////////////////////////////////////////////////

    /**
      \brief   fills the volume with a particular value
    */
    void fillSliceZ(unsigned sliceZPos, const VoxelType& value)
    {
      for (unsigned yy=0u; yy!=(unsigned)_resolution[1]; ++yy)
      {
        for (unsigned xx=0u; xx!=(unsigned)_resolution[0]; ++xx)
        {
          setAt(xx, yy, sliceZPos, value);
        }
      }
    }

    ///////////////////////////////////////////////////////////////////////////

    /**
      \brief   returns a std::vector containing the volume
    */
    std::vector<VoxelType>& getVector()
    {
      return _data;
    }

    ///////////////////////////////////////////////////////////////////////////

    /**
      \brief   returns a std::vector containing the volume
    */
    const std::vector<VoxelType>& getVector() const
    {
      return _data;
    }

    ///////////////////////////////////////////////////////////////////////////

    /**
      \brief   returns the nth value of the serial volume data
    */
    VoxelType& getAt(unsigned serialPosition)
    {
      return _data[serialPosition];
    }

    ///////////////////////////////////////////////////////////////////////////

    /**
      \brief   returns the nth value of the serial volume data
    */
    const VoxelType& getAt(unsigned serialPosition) const
    {
      return _data[serialPosition];
    }

    ///////////////////////////////////////////////////////////////////////////

    /**
      \brief   returns the nth value of the serial volume data
    */
    VoxelType& getAtSave(unsigned serialPosition, VoxelType& outOfBoundValue)
    {
      return serialPosition < (_resolution[0]*_resolution[1]*_resolution[2]) ? _data[serialPosition] : outOfBoundValue;
    }

    ///////////////////////////////////////////////////////////////////////////

    /**
      \brief   returns the nth value of the serial volume data
    */
    const VoxelType& getAtSave(unsigned serialPosition, VoxelType& outOfBoundValue) const
    {
      return serialPosition < (_resolution[0]*_resolution[1]*_resolution[2]) ? _data[serialPosition] : outOfBoundValue;
    }

    ///////////////////////////////////////////////////////////////////////////

    /**
      \brief   returns the value at x,y,z of the volume data
    */
    VoxelType& getAt(unsigned posX, unsigned posY, unsigned posZ)
    {
      return _data[gloost::math::coord3dToSerialIndex(posX,
                                               posY,
                                               posZ,
                                               (unsigned)_resolution[0],
                                               (unsigned)_resolution[1])];
    }

    ///////////////////////////////////////////////////////////////////////////

    /**
      \brief   returns the value at x,y,z of the volume data
    */
    const VoxelType& getAt(unsigned posX, unsigned posY, unsigned posZ) const
    {
      return _data[gloost::math::coord3dToSerialIndex( (unsigned)posX,
                                                       (unsigned)posY,
                                                       (unsigned)posZ,
                                                       (unsigned)_resolution[0],
                                                       (unsigned)_resolution[1])];
    }

    ///////////////////////////////////////////////////////////////////////////

    /**
      \brief returns the value at x,y,z of the volume data, xy and z will be clamped against volume boundaries
    */
    VoxelType& getAtClamped(int posX, int posY, int posZ)
    {
      posX = math::clamp(posX, 0, (int)_resolution[0]);
      posY = math::clamp(posY, 0, (int)_resolution[1]);
      posZ = math::clamp(posZ, 0, (int)_resolution[2]);

      return _data[gloost::math::coord3dToSerialIndex( (unsigned)posX,
                                                       (unsigned)posY,
                                                       (unsigned)posZ,
                                                       (unsigned)_resolution[0],
                                                       (unsigned)_resolution[1])];
    }

    ///////////////////////////////////////////////////////////////////////////

    /**
      \brief returns the value at x,y,z of the volume data, xy and z will be clamped against volume boundaries
    */
    const VoxelType& getAtClamped(int posX, int posY, int posZ) const
    {
      posX = math::clamp(posX, 0, (int)_resolution[0]);
      posY = math::clamp(posY, 0, (int)_resolution[1]);
      posZ = math::clamp(posZ, 0, (int)_resolution[2]);

      return _data[gloost::math::coord3dToSerialIndex( (unsigned)posX,
                                                       (unsigned)posY,
                                                       (unsigned)posZ,
                                                       (unsigned)_resolution[0],
                                                       (unsigned)_resolution[1])];
    }

    ///////////////////////////////////////////////////////////////////////////

    /**
      \brief returns the value at x,y,z of the volume data, if x, y or z is outside of the volume boundaries, outOfBoundValue will be returned
    */
    VoxelType& getAtSave(int posX, int posY, int posZ, VoxelType& outOfBoundValue)
    {
      if (   posX < 0 || !(posX < _resolution[0])
          || posY < 0 || !(posY < _resolution[1])
          || posZ < 0 || !(posZ < _resolution[2]))
      {
        return outOfBoundValue;
      }
      return _data[gloost::math::coord3dToSerialIndex( (unsigned)posX,
                                                       (unsigned)posY,
                                                       (unsigned)posZ,
                                                       (unsigned)_resolution[0],
                                                       (unsigned)_resolution[1])];
    }

    ///////////////////////////////////////////////////////////////////////////

    /**
      \brief returns the value at x,y,z of the volume data, if x, y or z is outside of the volume boundaries, outOfBoundValue will be returned
    */
    const VoxelType& getAtSave(int posX, int posY, int posZ, const VoxelType& outOfBoundValue) const
    {
      if (   posX < 0 || !(posX < _resolution[0])
          || posY < 0 || !(posY < _resolution[1])
          || posZ < 0 || !(posZ < _resolution[2]))
      {
        return outOfBoundValue;
      }
      return _data[gloost::math::coord3dToSerialIndex( (unsigned)posX,
                                                       (unsigned)posY,
                                                       (unsigned)posZ,
                                                       (unsigned)_resolution[0],
                                                       (unsigned)_resolution[1])];
    }

    ///////////////////////////////////////////////////////////////////////////

    /**
      \brief   sets the value at x,y,z of the volume data
    */
    void setAt(unsigned posX, unsigned posY, unsigned posZ, const VoxelType& value)
    {
      _data[gloost::math::coord3dToSerialIndex( posX,
                                                posY,
                                                posZ,
                                                (unsigned)_resolution[0],
                                                (unsigned)_resolution[1])] = value;
    }

    ///////////////////////////////////////////////////////////////////////////

    /**
      \brief returns the value at x,y,z of the volume data, xy and z will be clamped against volume boundaries
    */
    void setAtClamped(unsigned posX, unsigned posY, unsigned posZ, const VoxelType& value)
    {
      posX = math::clamp(posX, 0u, (unsigned)_resolution[0]);
      posY = math::clamp(posY, 0u, (unsigned)_resolution[1]);
      posZ = math::clamp(posZ, 0u, (unsigned)_resolution[2]);

      _data[gloost::math::coord3dToSerialIndex(posX,
                                               posY,
                                               posZ,
                                               (unsigned)_resolution[0],
                                               (unsigned)_resolution[1])] = value;
    }

    ///////////////////////////////////////////////////////////////////////////

    /**
      \brief returns the value at x,y,z of the volume data, xy and z will be clamped against volume boundaries
    */
    void setAtSave(unsigned posX, unsigned posY, unsigned posZ, const VoxelType& value)
    {
      if (   posX < 0 || !(posX < _resolution[0])
          || posY < 0 || !(posY < _resolution[1])
          || posZ < 0 || !(posZ < _resolution[2]))
      {
        return;
      }
      _data[gloost::math::coord3dToSerialIndex(posX,
                                               posY,
                                               posZ,
                                               (unsigned)_resolution[0],
                                               (unsigned)_resolution[1])] = value;
    }

    ///////////////////////////////////////////////////////////////////////////

    /**
      \brief   returns a pointer to the first voxel
    */
    const unsigned char* getData() const
    {
      return (unsigned char*)&_data.front();
    }

    ///////////////////////////////////////////////////////////////////////////

    /**
      \brief   returns a pointer to the first voxel
    */
    unsigned char* getData()
    {
      return (unsigned char*)&_data.front();
    }

    ///////////////////////////////////////////////////////////////////////////

    /**
      \brief   returns the number voxels with value
    */
    size_t count(const VoxelType& value) const
    {
      std::atomic_size_t count(0u);
      #pragma omp parallel for num_threads(gloost::helper::getNumSystemThreads())
      for (unsigned i=0u; i < _data.size(); ++i)
      {
        if (_data[i] == value)
        {
          ++count;
        }
      }
      return (size_t)count;
    }

    ///////////////////////////////////////////////////////////////////////////

    /**
      \brief   returns the resolution of the volume in each axis
    */
    const gloost::Vector3& getResolution() const
    {
      return _resolution;
    }

    ///////////////////////////////////////////////////////////////////////////


    /**
      \brief   returns the number of voxels
    */
    size_t getNumVoxels() const
    {
      return _data.size();
    }

    ///////////////////////////////////////////////////////////////////////////


    /**
      \brief returns the physical volume over all voxels returning resolution*_voxelSize
    */
    size_t getVolume() const
    {
      return getNumVoxels()*_voxelSize[0]*_voxelSize[1]*_voxelSize[2];
    }

    ///////////////////////////////////////////////////////////////////////////


    /**
      \brief returns the physical volume of one voxel
    */
    size_t getVoxelVolume() const
    {
      return _voxelSize[0]*_voxelSize[1]*_voxelSize[2];
    }

    ///////////////////////////////////////////////////////////////////////////

    /**
      \brief   returns a const std::vector<VoxelType>&
    */
    const std::vector<VoxelType>& getContainer() const
    {
      return _data;
    }

    ///////////////////////////////////////////////////////////////////////////

    /**
      \brief   returns a const std::vector<VoxelType>&
    */
    std::vector<VoxelType>& getContainer()
    {
      return _data;
    }

    ///////////////////////////////////////////////////////////////////////////

    /**
      \brief   returns the axis aligned bounding box which was used to create this sampler
    */
    const gloost::BoundingBox& getAabb() const
    {
      return _aabb;
    }

    ///////////////////////////////////////////////////////////////////////////

    /**
      \brief   returns the axis aligned bounding box which was used to create this sampler
    */
    void setAabb(const gloost::BoundingBox& aabb)
    {
      _aabb = aabb;
      _voxelSize = _aabb.getSize()/_resolution;
    }

    ///////////////////////////////////////////////////////////////////////////

    /**
      \brief   returns the distance between the centers of two voxel in x,y,z
    */
    const gloost::Vector3& getVoxelSize() const
    {
      return _voxelSize;
    }

    ///////////////////////////////////////////////////////////////////////////

    /**
      \brief   returns the position of the voxel with the smallest x,y,z
    */
    const gloost::Vector3 getFirstVoxelCenter() const
    {
      return  _aabb.getPMin() + _voxelSize*0.5;
    }

    ///////////////////////////////////////////////////////////////////////////

    /**
      \brief   returns the position of the voxel with the smallest x,y,z
    */
    const gloost::util::ValueRange<VoxelType> getValueRange() const
    {
      if (!_data.size())
      {
        return gloost::util::ValueRange<VoxelType>(0.0);
      }

      const auto numThreads = gloost::helper::getNumSystemThreads();
      std::vector<gloost::util::ValueRange<gloost::mathType> > ranges(numThreads, gloost::util::ValueRange<gloost::mathType>(_data[0]));

      #pragma omp parallel for num_threads(numThreads)
      for (unsigned ii=0u; ii<_data.size(); ++ii)
      {
        ranges[omp_get_thread_num()].expand(_data[ii]);
      }

      for (unsigned ii=1u; ii!=ranges.size(); ++ii)
      {
        ranges[0].expand(ranges[ii]);
      }

      return ranges[0];
    }

    ///////////////////////////////////////////////////////////////////////////

    /**
      \brief   returns the number of voxels within this volume
    */
    size_t getMemSize() const
    {
      return _data.size()*sizeof(VoxelType);
    }

    ///////////////////////////////////////////////////////////////////////////

    /**
      \brief   returns the size of a voxel in bytes
    */
    size_t getVoxelMemSize() const
    {
      return sizeof(VoxelType);
    }

    ///////////////////////////////////////////////////////////////////////////

    /**
      \brief   indexing
    */
    VoxelType& operator[](unsigned index)
    {
        return (_data.size() < index) ? _data[0] : _data[index];
    }

    ///////////////////////////////////////////////////////////////////////////

    /**
      \brief   indexing const
    */
    const VoxelType& operator[](unsigned index) const
    {
      return (_data.size() < index) ? _data[0] : _data[index];
    }

    ///////////////////////////////////////////////////////////////////////////

    /**
      \brief  adapts the resolution and the aabb of the volume to make the voxel size uniform
    */
    void alignAabbToVoxelRaster(unsigned maxResolution)
    {
      auto volumeResolution = _aabb.getSize();
      unsigned mainAxis = volumeResolution.getMainAxis();
      volumeResolution = (volumeResolution / volumeResolution[mainAxis]) * (gloost::mathType)maxResolution;

      _resolution[0] =  std::ceil(volumeResolution[0]);
      _resolution[1] =  std::ceil(volumeResolution[1]);
      _resolution[2] =  std::ceil(volumeResolution[2]);

      double voxelLength = _aabb.getSize().getMainComponent() / (gloost::mathType)_resolution.getMainComponent();
      auto pMax = _aabb.getPMin() + (_resolution * voxelLength);
      _aabb.setPMax(pMax);

      _voxelSize = _aabb.getSize() / _resolution;
    }

    ///////////////////////////////////////////////////////////////////////////

    #define GLOOST_VOLUMEDATA_MAGICNUMBER 97910101u

    /**
      \brief   writes the volume to a binary file
    */
    bool writeToFile(const gloost::PathType& filePath) const
    {

      if (!getMemSize())
      {
        return false;
      }

      BinaryFile outFile;
      if (!outFile.openToWrite(filePath))
      {
        return false;
      }

      // write a magic number
      outFile.writeUInt32(GLOOST_VOLUMEDATA_MAGICNUMBER);

      // write the current version
      outFile.writeUInt32(2u);

      // write the size of one voxel in memory
      outFile.writeUInt32((unsigned)getVoxelMemSize());

      // write volume resolution in x,y,z
      outFile.writeUInt32((unsigned)_resolution[0]);
      outFile.writeUInt32((unsigned)_resolution[1]);
      outFile.writeUInt32((unsigned)_resolution[2]);

      // write aabb (since file version 2)
      outFile.writeFloat64(_aabb.getPMin()[0]);
      outFile.writeFloat64(_aabb.getPMin()[1]);
      outFile.writeFloat64(_aabb.getPMin()[2]);

      outFile.writeFloat64(_aabb.getPMax()[0]);
      outFile.writeFloat64(_aabb.getPMax()[1]);
      outFile.writeFloat64(_aabb.getPMax()[2]);

      // write volume data
      outFile.writeBuffer((unsigned char*)&_data.front(), getMemSize());

      outFile.close();

      return true;
    }

    ///////////////////////////////////////////////////////////////////////////

    /**
      \brief   reads a volume from a binary file
    */
    bool readFromFile(const gloost::PathType& filePath)
    {
      BinaryFile inFile;
      if (!inFile.openAndLoad(filePath))
      {
        return false;
      }

      // magic number
      auto uIntValue = inFile.readUInt32();
      if (uIntValue != GLOOST_VOLUMEDATA_MAGICNUMBER)
      {
        std::cerr << std::endl << "From readFromFile(const std::string& filePath): " << this;
        std::cerr << std::endl << "     wrong file format: \"" << filePath.string() << "\"" ;
        std::cerr << std::endl << "     wrong magic number : " << uIntValue;
      }

      // file format version
      uIntValue = inFile.readUInt32();
      const unsigned fileVersion = uIntValue;
      if (!(fileVersion == 1u || fileVersion == 2u))
      {
        std::cerr << std::endl << "From readFromFile(const std::string& filePath): " << this;
        std::cerr << std::endl << "     Unsupported file format version : " << uIntValue;
      }

      // read the size of one voxel in memory
      unsigned voxelMemSize = inFile.readUInt32();
      if (voxelMemSize != getVoxelMemSize())
      {
        std::cerr << std::endl << "D'OH from gloost::VolumeData::readFromFile(): " << this;
        std::cerr << std::endl << "          VoxelType of the file has a different size as this volumes voxel type.";
        std::cerr << std::endl << "          While loading \"" << filePath.string() << "\" with voxelSize " << voxelMemSize << " byte.";
        std::cerr << std::endl;
        return false;
      }

      // read volume resolution in x,y,z
      _resolution[0] = (mathType)inFile.readUInt32();
      _resolution[1] = (mathType)inFile.readUInt32();
      _resolution[2] = (mathType)inFile.readUInt32();
      const auto numVoxels = _resolution[0]*_resolution[1]*_resolution[2];

      Point3 pMin(0.0,0.0,0.0);
      Point3 pMax(1.0,1.0,1.0);
      if (fileVersion > 1u)
      {
        pMin[0] = (mathType)inFile.readFloat64();
        pMin[1] = (mathType)inFile.readFloat64();
        pMin[2] = (mathType)inFile.readFloat64();
        pMax[0] = (mathType)inFile.readFloat64();
        pMax[1] = (mathType)inFile.readFloat64();
        pMax[2] = (mathType)inFile.readFloat64();
        setAabb(Aabb(pMin, pMax));
      }
      else
      {
        setAabb(Aabb(pMin, pMax));
      }

      // read volume data
      _data.resize(numVoxels);
      if (!inFile.readBuffer(getData(), getMemSize()))
      {
        inFile.unload();
        return false;
      }

      inFile.unload();

      return true;
    }

    ///////////////////////////////////////////////////////////////////////////

    /**
      \brief   returns a vector filled with the serial neighbour indices of given 'index'
    */

    std::vector<unsigned> getNeighbourIndices(unsigned index)
    {
      std::vector<unsigned> neighbourIndexVector(6);
      auto coord3d = gloost::math::serialIndexToCoord3d(index, _resolution[0], _resolution[1]);
      neighbourIndexVector[0] =  gloost::math::coord3dToSerialIndex(coord3d[0]+1, coord3d[1],   coord3d[2]  , _resolution[0], _resolution[1]);
      neighbourIndexVector[1] =  gloost::math::coord3dToSerialIndex(coord3d[0]-1, coord3d[1],   coord3d[2]  , _resolution[0], _resolution[1]);
      neighbourIndexVector[2] =  gloost::math::coord3dToSerialIndex(coord3d[0],   coord3d[1]+1, coord3d[2]  , _resolution[0], _resolution[1]);
      neighbourIndexVector[3] =  gloost::math::coord3dToSerialIndex(coord3d[0],   coord3d[1]-1, coord3d[2]  , _resolution[0], _resolution[1]);
      neighbourIndexVector[4] =  gloost::math::coord3dToSerialIndex(coord3d[0],   coord3d[1],   coord3d[2]+1, _resolution[0], _resolution[1]);
      neighbourIndexVector[5] =  gloost::math::coord3dToSerialIndex(coord3d[0],   coord3d[1],   coord3d[2]-1, _resolution[0], _resolution[1]);
      return neighbourIndexVector;
    }

    ///////////////////////////////////////////////////////////////////////////


	protected:

    /// class constructor
    VolumeData( const gloost::Aabb& aabb, unsigned maxResolution):
      _resolution(),
      _aabb(aabb),
	    _voxelSize(),
      _data()
    {
      alignAabbToVoxelRaster(maxResolution);
      _data.resize(_resolution[0]*_resolution[1]*_resolution[2]);
    }

    /// class constructor
    VolumeData( unsigned resolutionX ,unsigned resolutionY, unsigned resolutionZ):
      _resolution(resolutionX, resolutionY, resolutionZ),
      _aabb(0.0,0.0,0.0,1.0,1.0,1.0),
	    _voxelSize( (_aabb.getSize()/_resolution) ),
      _data()
    {
      _data.resize(resolutionX*resolutionY*resolutionZ);
    }


    /// class constructor
    VolumeData(const gloost::PathType& filePath):
      _resolution(0.0,0.0,0.0),
      _aabb(0.0,0.0,0.0, 1.0,1.0,1.0),
	    _voxelSize(0.0, 0.0, 0.0),
      _data()
    {
      readFromFile(filePath);
    }

    /// class constructor
    VolumeData(const gloost::PathType& filePath, unsigned resolutionX ,unsigned resolutionY, unsigned resolutionZ):
      _resolution(resolutionX, resolutionY, resolutionZ),
      _aabb(0.0,0.0,0.0,1.0,1.0,1.0),
	    _voxelSize( (_aabb.getSize()/_resolution) ),
      _data()
    {
      _data.resize(resolutionX*resolutionY*resolutionZ);

      BinaryFile inFile;
      if (!inFile.openAndLoad(filePath))
      {
        return;
      }

      if (!inFile.readBuffer(getData(), getMemSize()))
      {
        inFile.unload();
        return;
      }
      inFile.unload();
    }

    ///////////////////////////////////////////////////////////////////////////

	private:

    gloost::Vector3        _resolution;
    BoundingBox            _aabb;
    gloost::Vector3        _voxelSize;
    std::vector<VoxelType> _data;
};

///////////////////////////////////////////////////////////////////////////

template <typename SourceType, typename TargetType>
extern void convert(const std::vector<SourceType>& source,
                    std::vector<TargetType>& target,
                    size_t numThreads = 8)
{
  if(source.size() != target.size())
  {
    target.resize(source.size());
  }
  #pragma omp parallel for num_threads(numThreads)
  for (unsigned i=0u; i < source.size(); ++i)
  {
    target[i] = (TargetType)source[i];
  }
};

///////////////////////////////////////////////////////////////////////////


} // namespace gloost


#endif // H_GLOOST_VOLUME_DATA


