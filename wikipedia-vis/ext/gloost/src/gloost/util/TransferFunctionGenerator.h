
#ifndef H_GLOOST_UTIL_TRANSFERFUNCTIONGENERATOR
#define H_GLOOST_UTIL_TRANSFERFUNCTIONGENERATOR



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
#include <gloost/gloostConfig.h>
#include <gloost/UniformTypes.h>
#include <gloost/util/ValueRange.h>


// cpp includes
#include <string>
#include <memory>
#include <set>
#include <tinyxml2.h>


namespace gloost
{
namespace util
{


  //  generates color grandients and provides a texture or a vector<gloost::vec4>

class TransferFunctionGenerator
{
	public:

    /// a std::shared_ptr of an TransferFunctionGenerator instance
    typedef std::shared_ptr<TransferFunctionGenerator>       shared_ptr;
    typedef std::shared_ptr<const TransferFunctionGenerator> const_shared_ptr;

    typedef float PositionType;

    enum class Interpolation : char { DISCRETE, DISCRETE_RANGE, LINEAR };
    enum class Segmentation  : char { MANUAL = 0, UNIFORM };


    /// a node within a gradient definition
    struct ColorNode
    {
      ColorNode():
          _position(0.0),
          _color(gloost::vec4(0.0f,0.0f,0.0f,1.0f)),
          _label(""),
          _showValue(true)
      {}

      ColorNode(PositionType position, const gloost::vec4& color, const std::string& label, bool showValue):
          _position(position),
          _color(color),
          _label(label),
          _showValue(showValue)
      {}

      /// operator< to sort ColorNodes by position
      bool operator<(const ColorNode& node) const
      {
        return (_position < node._position);
      }
#if 0
      /// operator==
      bool operator==(const ColorNode& node) const
      {
        return (memcmp((unsigned char*)&node._position,  \
                       (unsigned char*)&_position,       - WHAT????
                        sizeof(gloost::mathType)));      /
      }
#endif
      PositionType _position;
      gloost::vec4 _color;
      std::string  _label;
      bool         _showValue;

    };

    struct Annotation
    {
      Annotation():
        _position(0.0),
        _label(""),
        _showValue(true)
        {}

      Annotation(PositionType position, const std::string& label, bool showValue):
        _position(position),
        _label(label),
        _showValue(showValue)
        {}

      /// operator< to sort Annotations by position
      bool operator<(const Annotation& annotation) const
      {
        return (_position < annotation._position);
      }

      PositionType _position;
      std::string  _label;
      bool         _showValue;
    };

    // class factory
    static shared_ptr create( const Interpolation& interpolation = Interpolation::LINEAR,
                              const Segmentation& segmentation   = Segmentation::MANUAL,
                              unsigned resolution = 1024u,
                              const std::string& label = "transferFunction");

    // class factory
    static shared_ptr create( const std::shared_ptr<const gloost::util::TransferFunctionGenerator>& );

    // class factory, loads an xml file
    static shared_ptr createFromXmlFile(const gloost::PathType& filePath);

    // class factory, loads an xml file
    static shared_ptr createFromXmlElement(tinyxml2::XMLElement* transferFunctionTag);

//    // creates a ColorGradientGenerator with spectrum colors
//    static shared_ptr createFromSpectrum(const Interpolation& interpolation = Interpolation::LINEAR,
//                                         const Segmentation& segmentation = Segmentation::MANUAL
//                                         unsigned resolution = 1024,
//                                         unsigned numSamples = 256u);
//
//    // creates a ColorGradientGenerator with spectrum colors
//    static shared_ptr createFromHSV( const Interpolation& interpolation = Interpolation::LINEAR,
//                                     const Segmentation& segmentation = Segmentation::MANUAL
//                                     unsigned resolution = 1024,
//                                     unsigned numSamples = 256u);

    bool writeToXml(const gloost::PathType& filePath);
    tinyxml2::XMLElement* toXmlElement(const std::shared_ptr<tinyxml2::XMLDocument>& xmlDocument);

    // class destructor
    virtual ~TransferFunctionGenerator();



    // adds a node to the gradient
    bool addNode(PositionType position, const gloost::vec4& color, const std::string& label = "", bool showValue = true);

    // adds a node to the gradient
    bool addNode(const ColorNode& colorNode);

    // adds a node to the gradient
    bool addAnnotation(PositionType position, const std::string& label = "", bool showValue = true);

    // returns the horizontal resolution
    unsigned getResolution() const;

    const Interpolation& getInterpolation() const;
    const Segmentation&  getSegmentation() const;
    const std::string&   getLabel() const;

    const gloost::vec4& getUpperBoundColor() const;
    void                setUpperBoundColor(const gloost::vec4& color);
    const gloost::vec4& getLowerBoundColor() const;
    void                setLowerBoundColor(const gloost::vec4& color);


    // returns a vector of nodes
    const std::vector<ColorNode>& getNodeVector() const;
    // returns the set of nodes
    const std::set<ColorNode>& getNodeSet() const;

    // returns a vector of nodes with normalized node position
    std::vector<ColorNode> getNormalizedNodeVector() const;

    // returns a vector of nodes
    std::vector<Annotation> getAnnotationVector() const;

    // generates a vector with n colors, representing the resolution pixels
    std::vector<vec4> generateGradientVector() const;

    // generates a serialized version of the transfer function (pos0, r0, g0, b0, pos0, r1, g1, b1, pos2, ...)
    std::vector<PositionType> getAsSerializedPosRGB() const;


    // returns the interpolated output value for an input value
    vec4 getValue(const mathType& input)  const;

    gloost::util::ValueRange<gloost::mathType> getKeyRange() const;
    gloost::mathType getMinimalKeyDistance() const;

    // clears all nodes and annotations
    void clear();

	protected:

    // returns the colors of the nodes for discrete ranges
    std::vector<vec4> generateGradientDiscretRange() const;

    // generates a texture with discrete color ranges by using the color of the end node of a range
    std::vector<vec4> generateGradientDiscret() const;

    // generates a gradient texture interpolating linear betwen nodes
    std::vector<vec4> generateGradientLinear() const;

    // finds the discrete key befor a given continous key in the vector, returns -1 for a position befor the first index
    int findKeyBeforIndex(PositionType searchPosition, int startIndex, int endIndex) const;

    // class constructor
    TransferFunctionGenerator(const Interpolation& interpolation,
                              const Segmentation& segmentation,
                              unsigned resolution,
                              const std::string& label);

    TransferFunctionGenerator(const std::shared_ptr<const gloost::util::TransferFunctionGenerator>&);

	private:

    //
    void updateNodeVector();

    Interpolation _interpolation;
    Segmentation  _segmentation;
    unsigned      _resolution;
    std::string   _label;

    std::set<ColorNode>    _nodeSet;
    std::vector<ColorNode> _nodeVector;
    std::set<Annotation>   _annotations;

    gloost::vec4 _upperBoundColor;
    gloost::vec4 _lowerBoundColor;

};

/// ostream operator
extern std::ostream& operator<< (std::ostream& out, const TransferFunctionGenerator::ColorNode& node);

} // namespace util
} // namespace gloost


#endif // H_GLOOST_UTIL_TRANSFERFUNCTIONGENERATOR
