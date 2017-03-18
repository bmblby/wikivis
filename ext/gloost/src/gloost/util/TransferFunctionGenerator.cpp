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

//
#include <gloost/util/TransferFunctionGenerator.h>
#include <gloost/util/ValueToWavelength.h>


// gloost includes
#include <gloost/gloostMath.h>
#include <gloost/gloostHelper.h>


// cpp includes
#include <string>
#include <iostream>
#include <algorithm>
#include <set>



namespace gloost
{
namespace util
{

/**
  \class   TransferFunctionGenerator

  \brief   generates color grandients and provides a texture or a vector<gloost::vec4>

  \author  Felix Weiszig, dynardo GmbH
  \date    May 2013
  \remarks ...
*/

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   Class constructor
  \remarks ...
*/

TransferFunctionGenerator::TransferFunctionGenerator( const TransferFunctionGenerator::Interpolation& interpolation,
    const TransferFunctionGenerator::Segmentation& segmentation,
    unsigned resolution,
    const std::string& label):
  _interpolation(interpolation),
  _segmentation(segmentation),
  _resolution(resolution),
  _label(label),
  _nodeSet(),
  _nodeVector(),
  _annotations(),
  _upperBoundColor(0.5,0.2,0.2,1.0),
  _lowerBoundColor(0.2,0.5,0.2,1.0)
{
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   Class constructor
  \remarks ...
*/

TransferFunctionGenerator::TransferFunctionGenerator(const std::shared_ptr<const TransferFunctionGenerator>& original):
  _interpolation(original->_interpolation),
  _segmentation(original->_segmentation),
  _resolution(original->_resolution),
  _label(original->_label),
  _nodeSet(original->_nodeSet),
  _nodeVector(original->_nodeVector),
  _annotations(original->_annotations),
  _upperBoundColor(0.5,0.2,0.2,1.0),
  _lowerBoundColor(0.2,0.5,0.2,1.0)
{
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   class factory
  \remarks ...
*/

/*static*/
TransferFunctionGenerator::shared_ptr
TransferFunctionGenerator::create( const TransferFunctionGenerator::Interpolation& interpolation,
    const TransferFunctionGenerator::Segmentation& segmentation,
    unsigned resolution,
    const std::string& label)
{
  auto instance = std::shared_ptr<TransferFunctionGenerator>(new TransferFunctionGenerator(interpolation,
      segmentation,
      resolution,
      label));
  return instance;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   class factory
  \remarks ...
*/

/*static*/
TransferFunctionGenerator::shared_ptr
TransferFunctionGenerator::create(const std::shared_ptr<const gloost::util::TransferFunctionGenerator>& original)
{
  auto instance = std::shared_ptr<TransferFunctionGenerator>(new TransferFunctionGenerator(original));
  return instance;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   class factory, loads an xml file
  \remarks ...
*/

/*static*/
TransferFunctionGenerator::shared_ptr
TransferFunctionGenerator::createFromXmlFile(const gloost::PathType& filePath)
{
  auto doc = gloost::helper::openXmlFile(filePath);
  if(!doc)
  {
    gloost::log::GlobalLog::postDoh(GLOOST_CURRENT_LOCATION,
        "Path not found or file invalid@" + filePath.string());
    return nullptr;
  }
  // read
  tinyxml2::XMLElement* gradientTag = doc->FirstChildElement("transfer_function");
  if(auto instance = createFromXmlElement(gradientTag))
  {
    return instance;
  }
  return nullptr;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   class factory, loads an xml file
  \remarks ...
*/

/*static*/
TransferFunctionGenerator::shared_ptr
TransferFunctionGenerator::createFromXmlElement(tinyxml2::XMLElement* localRoot)
{
  // define default values
  auto interpolation = Interpolation::LINEAR;
  auto segmentation  = Segmentation::MANUAL;
  int  resolution    = 2048;
  std::string label = "";

  shared_ptr instance = nullptr;

  if ( !(localRoot && localRoot->Name() == std::string("transfer_function")))
  {
    return shared_ptr(new TransferFunctionGenerator( interpolation, segmentation, resolution, label));
  }

  int fd_version = 0;
  gloost::helper::interpretFromChar(fd_version, localRoot->Attribute("fd_version"));
  if (fd_version != 0)
  {
    return shared_ptr(new TransferFunctionGenerator( interpolation, segmentation, resolution, label));
  }

  bool everythingIsFine = true;
  if(localRoot->Attribute("label"))
  {
    everythingIsFine &= gloost::helper::interpretFromChar(label, localRoot->Attribute("label"));
    if(label.empty())
    {
      label = "noLabel";
    }
  }

  if (localRoot->Attribute("interpolation"))
  {
    std::string stateString = "linear";
    everythingIsFine &= gloost::helper::interpretFromChar(stateString, localRoot->Attribute("interpolation"));

    if (stateString == "linear")
    {
      interpolation = Interpolation::LINEAR;
    }
    else if (stateString == "discrete")
    {
      interpolation = Interpolation::DISCRETE;
    }
    else if (stateString == "discrete range")
    {
      interpolation = Interpolation::DISCRETE_RANGE;
    }
    else
    {
      std::cerr << std::endl << "D'OH from TransferFunctionGenerator::createFromXmlElement(): ";
      std::cerr << std::endl << "          Interpolation is nether \"linear\" nor \"discrete\"";
      std::cerr << std::endl << "          Using \"linear\" as default ";
      interpolation = Interpolation::LINEAR;
    }
  }


  if (localRoot->Attribute("segmentation"))
  {
    std::string stateString = "uniform";
    everythingIsFine &= gloost::helper::interpretFromChar(stateString, localRoot->Attribute("segmentation"));
    if (stateString == "uniform")
    {
      segmentation = Segmentation::UNIFORM;
    }
    else if (stateString == "manual")
    {
      segmentation = Segmentation::MANUAL;
    }
    else
    {
      std::cerr << std::endl << "D'OH from TransferFunctionGenerator::createFromXmlElement(): ";
      std::cerr << std::endl << "          Segmentation attribute is nether \"manual\" nor \"uniform\"";
      std::cerr << std::endl << "          Using \"manual\" as default ";
      segmentation = Segmentation::MANUAL;
    }
  }

  if (localRoot->Attribute("resolution"))
  {
    everythingIsFine &= gloost::helper::interpretFromChar(resolution, localRoot->Attribute("resolution"));
  }
  instance = shared_ptr(new TransferFunctionGenerator( interpolation, segmentation, resolution, label));

  // parse lower bound
  tinyxml2::XMLElement* lowerBoundNode = localRoot->FirstChildElement("lower_bound");
  if (lowerBoundNode)
  {
    everythingIsFine &= gloost::helper::interpretFromChar(instance->_lowerBoundColor.r, lowerBoundNode->Attribute("r"));
    everythingIsFine &= gloost::helper::interpretFromChar(instance->_lowerBoundColor.g, lowerBoundNode->Attribute("g"));
    everythingIsFine &= gloost::helper::interpretFromChar(instance->_lowerBoundColor.b, lowerBoundNode->Attribute("b"));
    everythingIsFine &= gloost::helper::interpretFromChar(instance->_lowerBoundColor.a, lowerBoundNode->Attribute("a"));
  }
  // parse lower bound
  tinyxml2::XMLElement* upperBoundNode = localRoot->FirstChildElement("upper_bound");
  if (upperBoundNode)
  {
    everythingIsFine &= gloost::helper::interpretFromChar(instance->_upperBoundColor.r, upperBoundNode->Attribute("r"));
    everythingIsFine &= gloost::helper::interpretFromChar(instance->_upperBoundColor.g, upperBoundNode->Attribute("g"));
    everythingIsFine &= gloost::helper::interpretFromChar(instance->_upperBoundColor.b, upperBoundNode->Attribute("b"));
    everythingIsFine &= gloost::helper::interpretFromChar(instance->_upperBoundColor.a, upperBoundNode->Attribute("a"));
  }

  // parse Nodes
  tinyxml2::XMLElement* nodeElement = localRoot->FirstChildElement("node");
//  auto nodeCounter = 0u;
  while (nodeElement)
  {
    double pos = 0.0;
    float color_r = 0.0f;
    float color_g = 0.0f;
    float color_b = 0.0f;
    float color_a = 1.0f;
    int showValue = 1;
    std::string label = "";

    if(nodeElement->Attribute("pos"))
    {
      everythingIsFine &= gloost::helper::interpretFromChar(pos, nodeElement->Attribute("pos"));
//      std::cerr << std::endl << "pos: " << pos;
    }
    if(nodeElement->Attribute("r"))
    {
      everythingIsFine &= gloost::helper::interpretFromChar(color_r, nodeElement->Attribute("r"));
    }
    if(nodeElement->Attribute("g"))
    {
      everythingIsFine &= gloost::helper::interpretFromChar(color_g, nodeElement->Attribute("g"));
    }
    if(nodeElement->Attribute("b"))
    {
      everythingIsFine &= gloost::helper::interpretFromChar(color_b, nodeElement->Attribute("b"));
    }
    if(nodeElement->Attribute("a"))
    {
      everythingIsFine &= gloost::helper::interpretFromChar(color_a, nodeElement->Attribute("a"));
    }
    if(nodeElement->Attribute("showvalue"))
    {
      everythingIsFine &= gloost::helper::interpretFromChar(showValue, nodeElement->Attribute("showvalue"));
    }
    if(nodeElement->Attribute("label"))
    {
      everythingIsFine &= gloost::helper::interpretFromChar(label, nodeElement->Attribute("label"));
    }

    instance->addNode(pos, {color_r, color_g, color_b, color_a}, label, (bool)showValue);
    nodeElement = nodeElement->NextSiblingElement("node");
  }


  // parse Annotations
  tinyxml2::XMLElement* annotationTag = localRoot->FirstChildElement("annotation");
  while (annotationTag)
  {
    float annotationPos = 0.0f;
    std::string label = "";
    int showValue = 1;

    if(annotationTag->Attribute("pos"))
    {
      everythingIsFine &= gloost::helper::interpretFromChar(annotationPos, annotationTag->Attribute("pos"));
    }
    if(annotationTag->Attribute("label"))
    {
      everythingIsFine &= gloost::helper::interpretFromChar(label, annotationTag->Attribute("label"));
    }
    if(annotationTag->Attribute("showvalue"))
    {
      everythingIsFine &= gloost::helper::interpretFromChar(showValue, annotationTag->Attribute("showvalue"));
    }
    instance->addAnnotation(annotationPos, label, (bool)showValue);
    annotationTag = annotationTag->NextSiblingElement("annotation");
  }
  return instance;
}

///////////////////////////////////////////////////////////////////////////////

bool
TransferFunctionGenerator::writeToXml(const gloost::PathType& path)
{
  auto newDoc = std::shared_ptr<tinyxml2::XMLDocument>(new tinyxml2::XMLDocument());
  tinyxml2::XMLDeclaration* decl = newDoc->NewDeclaration("xml version=\"1.0\" standalone=\"yes\"");
  newDoc->InsertEndChild(decl);

  newDoc->InsertEndChild(toXmlElement(newDoc));
  if(gloost::helper::writeXmlFile(path, newDoc))
  {
    return true;
  }
  return false;
}

///////////////////////////////////////////////////////////////////////////////

tinyxml2::XMLElement*
TransferFunctionGenerator::toXmlElement(const std::shared_ptr<tinyxml2::XMLDocument>& xmlDocument)
{
  tinyxml2::XMLElement* element = xmlDocument->NewElement("transfer_function");
  element->SetAttribute("label",  getLabel().c_str());
  // version of the xml notation
  element->SetAttribute("fd_version",  "0");

  std::string interpolationString;
  if(getInterpolation() == gloost::util::TransferFunctionGenerator::Interpolation::LINEAR)
  {
    interpolationString =  "linear";
  }
  else if (getInterpolation() == gloost::util::TransferFunctionGenerator::Interpolation::DISCRETE)
  {
    interpolationString =  "discrete";
  }
  else if (getInterpolation() == gloost::util::TransferFunctionGenerator::Interpolation::DISCRETE_RANGE)
  {
    interpolationString =  "discrete range";
  }
  element->SetAttribute("interpolation", interpolationString.c_str());
  element->SetAttribute("resolution", getResolution());


  std::string segmentationString = "uniform";
  if (_segmentation == Segmentation::UNIFORM)
  {
    segmentationString = "uniform";
  }
  else if (_segmentation == Segmentation::MANUAL)
  {
    segmentationString = "manual";
  }
  element->SetAttribute("segmentation", segmentationString.c_str());

  auto lowerBoundNode =  xmlDocument->NewElement("lower_bound");
  element->InsertEndChild(lowerBoundNode);
  lowerBoundNode->SetAttribute("r", gloost::helper::toString(_lowerBoundColor.r).c_str());
  lowerBoundNode->SetAttribute("g", gloost::helper::toString(_lowerBoundColor.g).c_str());
  lowerBoundNode->SetAttribute("b", gloost::helper::toString(_lowerBoundColor.b).c_str());
  lowerBoundNode->SetAttribute("a", gloost::helper::toString(_lowerBoundColor.a).c_str());

  auto upperBoundNode =  xmlDocument->NewElement("upper_bound");
  element->InsertEndChild(upperBoundNode);
  upperBoundNode->SetAttribute("r", gloost::helper::toString(_upperBoundColor.r).c_str());
  upperBoundNode->SetAttribute("g", gloost::helper::toString(_upperBoundColor.g).c_str());
  upperBoundNode->SetAttribute("b", gloost::helper::toString(_upperBoundColor.b).c_str());
  upperBoundNode->SetAttribute("a", gloost::helper::toString(_upperBoundColor.a).c_str());


  // write color nodes
  for (const auto& colorNode : getNodeVector())
  {
    tinyxml2::XMLElement* node = xmlDocument->NewElement("node");
    node->SetAttribute("pos", gloost::helper::floatToString(colorNode._position).c_str());
    node->SetAttribute("r",   gloost::helper::floatToString(colorNode._color.r).c_str());
    node->SetAttribute("g",   gloost::helper::floatToString(colorNode._color.g).c_str());
    node->SetAttribute("b",   gloost::helper::floatToString(colorNode._color.b).c_str());
    node->SetAttribute("a",   gloost::helper::floatToString(colorNode._color.a).c_str());
    node->SetAttribute("showValue", colorNode._showValue);
    node->SetAttribute("label", colorNode._label.c_str());
    element->InsertEndChild(node);
  }

  // write Annotations
  for (const auto& annotation : getAnnotationVector())
  {
    tinyxml2::XMLElement* annotationElement = xmlDocument->NewElement("annotation");

    annotationElement->SetAttribute("pos", gloost::helper::floatToString(annotation._position).c_str());
    annotationElement->SetAttribute("showValue", annotation._showValue);
    annotationElement->SetAttribute("label", annotation._label.c_str());

    element->InsertEndChild(annotationElement);
  }
  return element;
}

///////////////////////////////////////////////////////////////////////////////

//
///**
//  \brief   creates a ColorGradientGenerator with spectrum colors
//  \param   ...
//  \remarks ...
//*/
//
///*static*/
//TransferFunctionGenerator::shared_ptr
//TransferFunctionGenerator::createFromSpectrum( const TransferFunctionGenerator::Interpolation& interpolation,
//                                               unsigned resolution,
//                                               unsigned numSamples)
//{
//  auto generator = TransferFunctionGenerator::create();
//
//  const float rangeMin = 0.0f;
//  const float rangeMax = 1.0f;
//
//  double stepWidth = abs(rangeMax-rangeMin)/(float)numSamples;
//
//  for (unsigned i=0u; i<=numSamples; ++i)
//  {
//    double wavelengthCoord = math::normalize((double)i, 0.0, (double)numSamples);
//    vec4 color             = ValueToWavelength::toWavelengthColor(i, 0.0, (float)numSamples);
//
//    double gradientCoord = rangeMin + i*stepWidth;
//    generator->addNode(gradientCoord, color);
//  }
//
//  return generator;
//}
//
//
/////////////////////////////////////////////////////////////////////////////////
//
//
///**
//  \brief   creates a ColorGradientGenerator with an HSV rotation in Hue
//  \param   ...
//  \remarks ...
//*/
//
///*static*/
//TransferFunctionGenerator::shared_ptr
//TransferFunctionGenerator::createFromHSV( const TransferFunctionGenerator::Interpolation& interpolation,
//                                          unsigned resolution,
//                                          unsigned numSamples)
//{
//  auto generator = TransferFunctionGenerator::create();
//
//  const float rangeMin = 0.0f;
//  const float rangeMax = 1.0f;
//
//  double stepWidth = abs(rangeMax-rangeMin)/(float)numSamples;
//
//  for (unsigned i=0u; i<=numSamples; ++i)
//  {
//    double wavelengthCoord = math::normalize((double)i, 0.0, (double)numSamples);
//    vec4 color             = math::hsvToRgb(math::normalize((float)i, (float)0, (float)numSamples),
//                                               1.0,1.0,1.0);
//    double gradientCoord   = rangeMin + i*stepWidth;
//    generator->addNode(gradientCoord, color);
//  }
//
//  return generator;
//}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   Class destructor
  \remarks ...
*/

TransferFunctionGenerator::~TransferFunctionGenerator()
{
  // insert your code here
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   adds a node to the gradient
  \param   ...
  \remarks ...
*/

bool
TransferFunctionGenerator::addNode(TransferFunctionGenerator::PositionType position,
    const gloost::vec4& color, const std::string& label, bool showValue)
{
  _nodeSet.erase(ColorNode(position, color, label, showValue));
  _nodeSet.insert(ColorNode(position, color, label, showValue));
  updateNodeVector();
  return true;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   adds a node to the gradient
  \param   ...
  \remarks ...
*/

bool
TransferFunctionGenerator::addNode(const ColorNode& colorNode)
{
  _nodeSet.erase(ColorNode(colorNode._position, colorNode._color, colorNode._label, colorNode._showValue));
  _nodeSet.insert(ColorNode(colorNode._position, colorNode._color, colorNode._label, colorNode._showValue));
  updateNodeVector();
  return true;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   adds a node to the gradient
  \param   ...
  \remarks ...
*/

bool
TransferFunctionGenerator::addAnnotation(TransferFunctionGenerator::PositionType position,
    const std::string& label, bool showValue)
{
  _annotations.erase(Annotation(position, label, showValue));
  _annotations.insert(Annotation(position, label, showValue));
  return true;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns the resolution
  \param   ...
  \remarks ...
*/

unsigned
TransferFunctionGenerator::getResolution() const
{
  return _resolution;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns the resolution
  \param   ...
  \remarks ...
*/

const TransferFunctionGenerator::Interpolation&
TransferFunctionGenerator::getInterpolation() const
{
  return _interpolation;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns the resolution
  \param   ...
  \remarks ...
*/

const TransferFunctionGenerator::Segmentation&
TransferFunctionGenerator::getSegmentation() const
{
  return _segmentation;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns the resolution
  \param   ...
  \remarks ...
*/

const std::string&
TransferFunctionGenerator::getLabel() const
{
  return _label;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   ...
  \param   ...
  \remarks ...
*/

const gloost::vec4&
TransferFunctionGenerator::getUpperBoundColor() const
{
  return _upperBoundColor;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   ...
  \param   ...
  \remarks ...
*/

void
TransferFunctionGenerator::setUpperBoundColor(const gloost::vec4& color)
{
  _upperBoundColor = color;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   ...
  \param   ...
  \remarks ...
*/

const gloost::vec4&
TransferFunctionGenerator::getLowerBoundColor() const
{
  return _lowerBoundColor;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   ...
  \param   ...
  \remarks ...
*/

void
TransferFunctionGenerator::setLowerBoundColor(const gloost::vec4& color)
{
  _lowerBoundColor = color;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   generates a vector with n colors, representing the resolution pixels
  \param   ...
  \remarks ...
*/

std::vector<vec4>
TransferFunctionGenerator::generateGradientVector() const
{
  if (_interpolation == Interpolation::DISCRETE_RANGE)
  {
    return generateGradientDiscretRange();
  }
  else if (_interpolation == Interpolation::DISCRETE)
  {
    return generateGradientDiscret();
  }
  else if (_interpolation == Interpolation::LINEAR)
  {
    return generateGradientLinear();
  }
  else
  {
    std::cerr << std::endl << "D'OH from TransferFunctionGenerator::generate()(): " << this;
    std::cerr << std::endl << "          Interpolation has invalid value...";
    std::cerr << std::endl << "          Using linear interpolation as default";
    return generateGradientLinear();
  }
}
///////////////////////////////////////////////////////////////////////////////

/**
  \brief   generates a color vector but leaves the last node out (which is the upper bound of the last range)
  \param   ...
  \remarks ...
*/

std::vector<vec4>
TransferFunctionGenerator::generateGradientDiscretRange() const
{
  if (!_resolution)
  {
    std::cerr << std::endl << "D'OH from TransferFunctionGenerator::generateDiscret(): " << this;
    std::cerr << std::endl << "          NO pixel _resolution was set";
    return std::vector<vec4>();
  }
  if (!_nodeVector.size())
  {
    std::cerr << std::endl << "D'OH from TransferFunctionGenerator::generateDiscret(): " << this;
    std::cerr << std::endl << "          NO nodes are defined";
    return std::vector<vec4>();
  }

  // serialize
  std::vector<ColorNode> normalizedNodes = getNormalizedNodeVector();

  // allocate
  std::vector<vec4> gradientVector(_resolution, gloost::vec4(1.0f,1.0f,1.0f,1.0f));
  for (int pix=0; pix < (int)_resolution; ++pix)
  {
    const auto currentColorNodeIndex = gloost::math::map((float)pix, 0.f, (float)_resolution, 0.f, (float)normalizedNodes.size()-1);
    gradientVector[pix] = normalizedNodes[currentColorNodeIndex]._color;
  }
  return gradientVector;
}
///////////////////////////////////////////////////////////////////////////////

/**
  \brief   generates a vector with discrete colors
  \param   ...
  \remarks ...
*/

std::vector<vec4>
TransferFunctionGenerator::generateGradientDiscret() const
{
  if (!_resolution)
  {
    std::cerr << std::endl << "D'OH from TransferFunctionGenerator::generateDiscret(): " << this;
    std::cerr << std::endl << "          NO pixel _resolution was set";
    return std::vector<vec4>();
  }
  if (!_nodeVector.size())
  {
    std::cerr << std::endl << "D'OH from TransferFunctionGenerator::generateDiscret(): " << this;
    std::cerr << std::endl << "          NO nodes are defined";
    return std::vector<vec4>();
  }

  // serialize
  std::vector<ColorNode> normalizedNodes = getNormalizedNodeVector();

  // allocate
  std::vector<vec4> gradientVector(_resolution, gloost::vec4(1.0f,1.0f,1.0f,1.0f));
  for (size_t pix=0; pix!=_resolution; ++pix)
  {
    const auto currentColorNodeIndex = gloost::math::map((float)pix, 0.f, (float)_resolution, 0.f, (float)normalizedNodes.size());
    gradientVector[pix] = normalizedNodes[currentColorNodeIndex]._color;
  }
  return gradientVector;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief  generates vector of colors, linear interpolated over all nodes
  \param   ...
  \remarks ...
*/

std::vector<vec4>
TransferFunctionGenerator::generateGradientLinear() const
{
  if (!_resolution)
  {
    std::cerr << std::endl << "D'OH from TransferFunctionGenerator::generateLinear(): " << this;
    std::cerr << std::endl << "          NO pixel _resolution was set";
    return std::vector<vec4>();
  }
  if (!_nodeVector.size())
  {
    std::cerr << std::endl << "D'OH from TransferFunctionGenerator::generateLinear(): " << this;
    std::cerr << std::endl << "          NO nodes are defined";
    return std::vector<vec4>();
  }

  // serialize
  std::vector<ColorNode> normalizedNodes = getNormalizedNodeVector();

  // allocate
  std::vector<vec4> pixelValueVector(_resolution, gloost::vec4(1.0,1.0,1.0,1.0));

  for (size_t i=1; i!=normalizedNodes.size(); ++i)
  {
    int rangeStart = (int)floor(normalizedNodes[i-1]._position*_resolution);
    int rangeStop  = (int)floor(normalizedNodes[i]._position*_resolution);

    rangeStart = math::clamp(rangeStart, 0, (int)_resolution);
    rangeStop  = math::clamp(rangeStop,  0, (int)_resolution);

    float length = (float)(rangeStop-rangeStart);
    float fraction = 1.0f/length;

    if (rangeStop == rangeStart)
    {
      continue;
    }

    const float rangeStep = 1.0/(rangeStop-rangeStart);
    const gloost::vec4& colorA = normalizedNodes[i-1]._color;
    const gloost::vec4& colorB = normalizedNodes[i]._color;

    unsigned step = 0u;
    for (int r=rangeStart; r!=rangeStop; ++r)
    {
      float fractionOffset = (float)step/((float)length-1)*fraction;
      pixelValueVector[r].r = math::interpolateLinear(colorA.r, colorB.r, step*rangeStep+fractionOffset);
      pixelValueVector[r].g = math::interpolateLinear(colorA.g, colorB.g, step*rangeStep+fractionOffset);
      pixelValueVector[r].b = math::interpolateLinear(colorA.b, colorB.b, step*rangeStep+fractionOffset);
      pixelValueVector[r].a = math::interpolateLinear(colorA.a, colorB.a, step*rangeStep+fractionOffset);
      ++step;
    }
  }
  return pixelValueVector;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   generates a serialized version of the transfer function (pos0, r0, g0, b0, pos0, r1, g1, b1, pos2, ...)
  \param   ...
  \remarks ...
*/

std::vector<TransferFunctionGenerator::PositionType>
TransferFunctionGenerator::getAsSerializedPosRGB() const
{
  static const auto packageSize = 4u; // this would be pos, red, gree
  std::vector<TransferFunctionGenerator::PositionType> interleavedValues((_nodeVector.size())*packageSize, 1.0f);
  for (unsigned inIndex=0u; inIndex!=_nodeVector.size(); ++inIndex)
  {
    auto outIndex = inIndex*packageSize;
    interleavedValues[outIndex++] = _nodeVector[inIndex]._position;
    interleavedValues[outIndex++] = _nodeVector[inIndex]._color.r;
    interleavedValues[outIndex++] = _nodeVector[inIndex]._color.g;
    interleavedValues[outIndex++] = _nodeVector[inIndex]._color.b;
  }
  return interleavedValues;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns the interpolated output value for an input value
  \param   ...
  \remarks ...
*/

vec4
TransferFunctionGenerator::getValue(const mathType& input)  const
{
  // find nodes befor and after the input value
  const auto beforIndex = findKeyBeforIndex(input, 0, _nodeVector.size());

  // if the index is out of bound
  if (beforIndex < 0 || beforIndex == (int)_nodeVector.size())
  {
    return vec4(1);
  }

  // if it is the last index
  if (beforIndex == (int)_nodeVector.size()-1)
  {
    return _nodeVector[_nodeVector.size()-1]._color;
  }

  // return the color at beforIndex position
  if (_interpolation == Interpolation::DISCRETE)
  {
    return _nodeVector[beforIndex+1]._color;
  }

  // normalize the input
  const auto t1 = _nodeVector[beforIndex]._position;
  const auto t2 = _nodeVector[beforIndex+1]._position;
  const auto normalizedInput = (input-t1)/(t2-t1);

  const auto v1 = _nodeVector[beforIndex]._color;
  const auto v2 = _nodeVector[beforIndex+1]._color;

  return gloost::math::interpolateLinear(v1, v2, normalizedInput);
}


///////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns the interpolated output value for an input value
  \param   ...
  \remarks This implements a binary search
*/

int
TransferFunctionGenerator::findKeyBeforIndex(PositionType searchValue, int startIndex, int endIndex) const
{
  // recursion end
  auto size = endIndex - startIndex;
  if (size == 0)
  {
    return startIndex-1;
  }

  // found key with key.searchValue == searchValue
  int middle = (int)((startIndex + endIndex)*0.5f);

  if (_nodeVector[middle]._position == searchValue)
  {
    return middle;
  }

  if (searchValue > _nodeVector[middle]._position )
  {
    return findKeyBeforIndex(searchValue, middle+1, endIndex);
  }
  // else
  return findKeyBeforIndex(searchValue, startIndex, middle);
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   generates a vector with n colors, representing the resolution pixels
  \param   ...
  \remarks ...
*/

gloost::util::ValueRange<gloost::mathType>
TransferFunctionGenerator::getKeyRange() const
{
  if (!_nodeVector.size())
  {
    return gloost::util::ValueRange<gloost::mathType>(0.0);
  }
  const auto rangeMin = _nodeVector[0]._position;
  const auto rangeMax = _nodeVector[_nodeVector.size()-1]._position;
  return {rangeMin,rangeMax};
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   generates a vector with n colors, representing the resolution pixels
  \param   ...
  \remarks ...
*/

gloost::mathType
TransferFunctionGenerator::getMinimalKeyDistance() const
{
  float minDist = getKeyRange().getWidth();
  for (unsigned i=0u; i != _nodeVector.size()-1; ++i)
  {
    minDist = std::min(minDist, _nodeVector[i+1]._position-_nodeVector[i]._position);
  }
  return minDist;
}

///////////////////////////////////////////////////////////////////////////////

void
TransferFunctionGenerator::clear()
{
  _nodeSet.clear();
  _nodeVector.clear();
  _annotations.clear();
  updateNodeVector();
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns the vector of nodes
  \param   ...
  \remarks ...
*/

const std::vector<TransferFunctionGenerator::ColorNode>&
TransferFunctionGenerator::getNodeVector() const
{
  return _nodeVector;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns the set of nodes
  \param   ...
  \remarks ...
*/

const std::set<TransferFunctionGenerator::ColorNode>&
TransferFunctionGenerator::getNodeSet() const
{
  return _nodeSet;
}

///////////////////////////////////////////////////////////////////////////////


void
TransferFunctionGenerator::updateNodeVector()
{
  // serialize
  std::vector<ColorNode> nodeVector;
  nodeVector.reserve(_nodeSet.size());
  for (const auto& node : _nodeSet)
  {
    nodeVector.push_back(node);
  }
  _nodeVector.swap(nodeVector);
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns a vector of nodes with normalized node positions
  \param   ...
  \remarks ...
*/

std::vector<TransferFunctionGenerator::ColorNode>
TransferFunctionGenerator::getNormalizedNodeVector() const
{
  if (!_nodeVector.size())
  {
    return std::vector<ColorNode>();
  }
  const auto range = getKeyRange();

  // copy and normalize
  std::vector<ColorNode> normalizedNodes = _nodeVector;
  const auto& lower = range.getLower();
  const auto& upper = range.getUpper();

  if (std::abs(upper-lower) > GLOOST_MATHTYPE_MIN_EPSILON)
  {
    const double maxMinusMinInv = 1.0/(upper-lower);
    for (unsigned i=0u; i!=normalizedNodes.size(); ++i)
    {
      normalizedNodes[i]._position =  math::normalizeFaster( normalizedNodes[i]._position,
          (PositionType)lower,
          (PositionType)maxMinusMinInv);
    }
  }

  return normalizedNodes;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns a vector of annotations
  \param   ...
  \remarks ...
*/

std::vector<TransferFunctionGenerator::Annotation>
TransferFunctionGenerator::getAnnotationVector() const
{
  // serialize
  std::vector<Annotation> annotations;
  annotations.reserve(_annotations.size());
  for (const auto& annotation : _annotations)
  {
    annotations.push_back(annotation);
  }
  return annotations;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   sorts nodes by position and stuff
  \param   ...
  \remarks ...
*/

/*extern*/
std::ostream&
operator<< (std::ostream& out, const TransferFunctionGenerator::ColorNode& node)
{
  out << "ColorNode { pos: " << node._position << ", " << node._color << " }";
  return out;
}

///////////////////////////////////////////////////////////////////////////////

} // namespace util
} // namespace gloost
