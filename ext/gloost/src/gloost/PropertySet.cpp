
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
#include <gloost/PropertySet.h>
#include <gloost/gloostHelper.h>

// cpp includes
#include <tinyxml2.h>

namespace gloost
{

///////////////////////////////////////////////////////////////////////////////

  ///
/*static*/
PropertySet::shared_ptr
PropertySet::create()
{
  return shared_ptr();
}

///////////////////////////////////////////////////////////////////////////////

  ///

/*static*/
PropertySet::shared_ptr
PropertySet::create(const PropertySet& propSet)
{
  return shared_ptr(new PropertySet(propSet));
}

///////////////////////////////////////////////////////////////////////////////

  ///

/*static*/
PropertySet::shared_ptr
PropertySet::create(const std::string& xmlFilePath)
{
  return shared_ptr(new PropertySet(xmlFilePath));
}

///////////////////////////////////////////////////////////////////////////////

  ///

/*static*/
PropertySet::shared_ptr
PropertySet::create(const gloost::PathType& xmlFilePath)
{
  return shared_ptr(new PropertySet(xmlFilePath));
}

///////////////////////////////////////////////////////////////////////////////

  ///

PropertySet::PropertySet():
    _properties(),
    _propertiesString(),
    _propertiesPoint3(),
    _dirty(false)
{

}

///////////////////////////////////////////////////////////////////////////////

  ///

PropertySet::PropertySet(const PropertySet& propSet):
    _properties(),
    _propertiesString(),
    _dirty(true)
{
  _properties       = propSet._properties;
  _propertiesString = propSet._propertiesString;
}

///////////////////////////////////////////////////////////////////////////////

  ///

PropertySet::PropertySet(const std::string& xmlFilePath):
  PropertySet(gloost::PathType(xmlFilePath))
{
}

///////////////////////////////////////////////////////////////////////////////

  ///

PropertySet::PropertySet(const gloost::PathType& xmlFilePath):
    _properties(),
    _propertiesString(),
    _dirty(true)
{
  readXml(xmlFilePath);
}

///////////////////////////////////////////////////////////////////////////////

  /// destructor

/*virtual*/
PropertySet::~PropertySet()
{

  /// enter code here ...

}

///////////////////////////////////////////////////////////////////////////////

  /// find a numeric property and return a iterator to it, 0 if not found

const std::map<std::string, float>::const_iterator
PropertySet::findProp(const std::string& name,
                      bool enableErrorMessage) const
{
  return _properties.find(name);
}

///////////////////////////////////////////////////////////////////////////////

  /// find a string property and return a iterator to it, 0 if not found

const std::map<std::string, std::string>::const_iterator
PropertySet::findPropString(const std::string& name,
                            bool enableErrorMessage) const
{
  return  _propertiesString.find(name);
}

///////////////////////////////////////////////////////////////////////////////

  /// find a Point3 property and return a iterator to it, 0 if not found

const std::map<std::string, Point3>::const_iterator
PropertySet::findPropPoint3(const std::string& name,
                            bool enableErrorMessage) const
{
  return _propertiesPoint3.find(name);
}

///////////////////////////////////////////////////////////////////////////////

// checks if std::string property exists

bool
PropertySet::stringPropExists(const std::string& name)
{
  auto pos = _propertiesString.find(name);
  if(pos != _propertiesString.end())
  {
    return true;
  }
  return false;
}

///////////////////////////////////////////////////////////////////////////////

  /// get a const char* property

const std::string
PropertySet::getPropString(const std::string& name) const
{

  std::map<std::string, std::string>::const_iterator propIt = findPropString(name);

  if (propIt != _propertiesString.end())
  {
    return (*propIt).second;
  }

  return std::string("PROPERTY NOT FOUND: ") + name;
}

///////////////////////////////////////////////////////////////////////////////

  /// get a bool property

bool
PropertySet::getPropBool(const std::string& name) const
{
  std::map<std::string, float>::const_iterator propIt = findProp(name);

  if (propIt != _properties.end())
  {
    return (bool)(*propIt).second;
  }

  return false;
}

///////////////////////////////////////////////////////////////////////////////

  /// get a bool property as string

std::string
PropertySet::getPropBoolAsString(const std::string& name) const
{
  std::map<std::string, float>::const_iterator propIt = findProp(name);

  if (propIt != _properties.end())
  {
    return gloost::helper::toString((bool)(*propIt).second);
  }

  return std::string("PROPERTY NOT FOUND: ") + name;
}

///////////////////////////////////////////////////////////////////////////////

  /// get a property as int

int
PropertySet::getPropInt(const std::string& name) const
{
  std::map<std::string, float>::const_iterator propIt = findProp(name);

  if (propIt != _properties.end())
  {
    return (int)(*propIt).second;
  }
  return 0;
}

///////////////////////////////////////////////////////////////////////////////

  /// get a int property as string

std::string
PropertySet::getPropIntAsString(const std::string& name) const
{
  std::map<std::string, float>::const_iterator propIt = findProp(name);

  if (propIt != _properties.end())
  {
    return gloost::helper::toString((int)(*propIt).second);
  }

  return std::string("INT PROPERTY NOT FOUND: ") + name;
}

///////////////////////////////////////////////////////////////////////////////

  /// get a property as float

float
PropertySet::getPropFloat(const std::string& name) const
{
  std::map<std::string, float>::const_iterator propIt = findProp(name);

  if (propIt != _properties.end())
  {
    return (float)(*propIt).second;
  }

  return 0;
}

///////////////////////////////////////////////////////////////////////////////

  /// get a float property as string

std::string
PropertySet::getPropFloatAsString(const std::string& name) const
{
  std::map<std::string, float>::const_iterator propIt = findProp(name);

  if (propIt != _properties.end())
  {
    return gloost::helper::toString((float)(*propIt).second);
  }

  return std::string("FLOAT PROPERTY NOT FOUND: ") + name;
}

///////////////////////////////////////////////////////////////////////////////

  /// get a Point3 property

const Point3&
PropertySet::getPropPoint3(const std::string& name) const
{
  std::map<std::string, Point3>::const_iterator propIt = findPropPoint3(name);

  if (propIt != _propertiesPoint3.end())
  {
    return (*propIt).second;
  }

  return Point3::origin;
}

///////////////////////////////////////////////////////////////////////////////

  /// get a float property as string

std::string
PropertySet::getPropPoint3AsString(const std::string& name) const
{
  std::map<std::string, Point3>::const_iterator propIt = findPropPoint3(name);

  if (propIt != _propertiesPoint3.end())
  {
    return gloost::helper::toString((*propIt).second[0]) +
           std::string(" ") +
           gloost::helper::toString((*propIt).second[1]) +
           std::string(" ") +
           gloost::helper::toString((*propIt).second[2]);
  }

  return std::string("POINT3 PROPERTY NOT FOUND: ") + name;
}

///////////////////////////////////////////////////////////////////////////////

  /// set a string property

void
PropertySet::setProp(const std::string& name, const std::string& value)
{
  _propertiesString[name] = value;
  setDirty(true);
}

///////////////////////////////////////////////////////////////////////////////

  /// set a numeric property

void
PropertySet::setProp(const std::string& name, float value)
{
  _properties[name] = value;
  setDirty(true);
}

///////////////////////////////////////////////////////////////////////////////

  /// set a numeric property

void
PropertySet::setProp(const std::string& name, const Point3& point)
{
  _propertiesPoint3[name] = point;
  setDirty(true);
}

///////////////////////////////////////////////////////////////////////////////

  /// get the map with all numeric props

const std::map<std::string, float>&
PropertySet::getProps() const
{
  return _properties;
}

///////////////////////////////////////////////////////////////////////////////

 /// get the map with all string props

const std::map<std::string, std::string>&
PropertySet::getPropsString() const
{
  return _propertiesString;
}

///////////////////////////////////////////////////////////////////////////////

 /// get the map with all string props

const std::map<std::string, Point3>&
PropertySet::getPropsPoint3() const
{
  return _propertiesPoint3;
}

///////////////////////////////////////////////////////////////////////////////

  /// compatibility call


bool
PropertySet::readXml(const std::string& xmlFilePath)
{
  return readXml(gloost::PathType(xmlFilePath));
}

///////////////////////////////////////////////////////////////////////////////

  /// load a PropertySet from xml an file. return value indicates success


bool
PropertySet::readXml(const gloost::PathType& xmlFilePath)
{
  ///
  auto propFile = gloost::helper::openXmlFile(xmlFilePath);

	if(!propFile)
	{
    gloost::log::GlobalLog::postDebug(GLOOST_CURRENT_LOCATION,
                                     "Could not open XML file \"" + xmlFilePath.string() + "\" ");
		return false;
	}
//  tinyxml2::XMLNode* root = propFile->FirstChild();
  tinyxml2::XMLElement* rootElement = propFile->FirstChildElement("gloostPropertySet");

  if(!rootElement)
  {
    std::cerr << std::endl;
    std::cerr << std::endl << "ERROR in PropertySet::loadXml():";
    std::cerr << std::endl << "         Error within traversXml() while loading "<< xmlFilePath;
    std::cerr << std::endl;
    return false;
  }

  // check for "gloost::PropertySet" tag
  if(!strcmp(rootElement->Name(), "gloostPropertySet"))
  {

    // Travers the xml file
    if ( traversXml(rootElement))
    {
//        std::cerr << std::endl;
//        std::cerr << std::endl << "Message from PropertySet::loadXml():";
//				std::cerr << std::endl << "             Properties loaded from \"" << xmlFilePath << "\"";
//        std::cerr << std::endl;
      return true;
    }
    else
    {
      std::cerr << std::endl;
      std::cerr << std::endl << "ERROR in PropertySet::loadXml():";
      std::cerr << std::endl << "         Error within traversXml() while loading "<< xmlFilePath;
      std::cerr << std::endl << "          TiXml ERROR : " << propFile->GetErrorStr1();
      return false;
    }
  }
  else
  {
    std::cerr << std::endl;
    std::cerr << std::endl << "ERROR in PropertySet::loadXml():";
    std::cerr << std::endl << "         The name of the root tag must be <gloostPropertySet>";
    std::cerr << std::endl;

    return false;
  }
	return true;
}

///////////////////////////////////////////////////////////////////////////////

  /// traverse a XML PropertySet Node

bool
PropertySet::traversXml(tinyxml2::XMLElement* element)
{
  // reading resources
  tinyxml2::XMLElement* subTag  = element->FirstChildElement("property");

  for( ; subTag; subTag = subTag->NextSiblingElement("property"))
  {
    std::string type = subTag->Attribute("type");
    std::string name = subTag->Attribute("name");

    if (type == std::string("string"))
    {
      std::string value = subTag->Attribute("value");
      setProp(name, value);
    }
    else if (type == std::string("number"))
    {
      float value = atof(subTag->Attribute("value"));
      setProp(name, value);
    }
    else if (type == std::string("point3"))
    {
      float x = atof(subTag->Attribute("x"));
      float y = atof(subTag->Attribute("y"));
      float z = atof(subTag->Attribute("z"));
      setProp(name, Point3(x,y,z));
    }
    else
    {
      std::cerr << std::endl;
      std::cerr << std::endl << "WARNING from PropertySet::loadXml():";
      std::cerr << std::endl << "             Property type \"" << type << "\" is not supported!";
      std::cerr << std::endl << "             Registered as string property instead ...";
      std::cerr << std::endl;

//      std::string value = subTag->Attribute("value");
//      setProp(name, value);
    }
  }

	return true;
}

///////////////////////////////////////////////////////////////////////////////

  /// compatibility call

bool
PropertySet::writeXml(const std::string& xmlFilePath)
{
  return writeXml(gloost::PathType(xmlFilePath));
}


///////////////////////////////////////////////////////////////////////////////

  /// write the PropertySet to a xml file

bool
PropertySet::writeXml(const gloost::PathType& xmlFilePath)
{

	auto doc = std::shared_ptr<tinyxml2::XMLDocument>(new tinyxml2::XMLDocument());
	tinyxml2::XMLDeclaration* decl = doc->NewDeclaration("xml version=\"1.0\" standalone=\"yes\"");
  doc->InsertEndChild(decl);

	/// map
	tinyxml2::XMLElement* properties = doc->NewElement( "gloostPropertySet" );

	/// save string properties
	std::map<std::string, std::string>::iterator stringPropIt    = _propertiesString.begin();
	std::map<std::string, std::string>::iterator stringPropEndIt = _propertiesString.end();


	for(; stringPropIt != stringPropEndIt; ++stringPropIt)
	{
	  tinyxml2::XMLElement * property = doc->NewElement( "property" );

	  property->SetAttribute("type", "string");
	  property->SetAttribute("name", (*stringPropIt).first.c_str());
	  property->SetAttribute("value",(*stringPropIt).second.c_str());

	  properties->InsertEndChild( property );	  //

	}


	/// save numeric properties
	std::map<std::string, float>::iterator numPropIt    = _properties.begin();
	std::map<std::string, float>::iterator numPropEndIt = _properties.end();

	for(; numPropIt != numPropEndIt; ++numPropIt)
	{
	  tinyxml2::XMLElement * property = doc->NewElement( "property" );

	  property->SetAttribute("type", "number");
	  property->SetAttribute("name", (*numPropIt).first.c_str());
	  property->SetAttribute("value",(*numPropIt).second);

	  properties->InsertEndChild( property );

	}

	/// save point3 properties
	std::map<std::string, Point3>::iterator point3PropIt    = _propertiesPoint3.begin();
	std::map<std::string, Point3>::iterator point3PropEndIt = _propertiesPoint3.end();

	for(; point3PropIt != point3PropEndIt; ++point3PropIt)
	{
	  tinyxml2::XMLElement * property = doc->NewElement( "property" );

	  property->SetAttribute("type", "point3");
	  property->SetAttribute("name", (*point3PropIt).first.c_str());
	  property->SetAttribute("x",(*point3PropIt).second[0]);
	  property->SetAttribute("y",(*point3PropIt).second[1]);
	  property->SetAttribute("z",(*point3PropIt).second[2]);

	  properties->InsertEndChild( property );

	}
	doc->InsertEndChild(properties);

	gloost::helper::writeXmlFile(xmlFilePath, doc);


//  std::cerr << std::endl;
//  std::cerr << std::endl << "PropertySet::writeXml(const std::string& xmlFilePath):";
//  std::cerr << std::endl << "             Properties successfully writen to \"" << xmlFilePath << "\"";
//  std::cerr << std::endl;
//
//

	return true;
}

////////////////////////////////////////////////////////////////////////////////

  ///

void
PropertySet::operator= (const PropertySet& src)
{
  _properties.clear();
  _propertiesString.clear();

  _properties       = src._properties;
  _propertiesString = src._propertiesString;
}

////////////////////////////////////////////////////////////////////////////////

  /// set the dirty flag

void
PropertySet::setDirty(const bool dirty)
{
  _dirty = dirty;
}

////////////////////////////////////////////////////////////////////////////////

  /// unset the dirty flag

void
PropertySet::setClean()
{
  _dirty = false;
}

////////////////////////////////////////////////////////////////////////////////

  /// check if the PropertySet is dirty

bool
PropertySet::isDirty() const
{
  return _dirty;
}

////////////////////////////////////////////////////////////////////////////////

  ///

/* extern */
std::ostream&
operator<< (std::ostream& os, PropertySet& src)
{

  os << std::endl << "PropertySet";
  os << std::endl << "{";
  os << std::endl << "  Numeric properties (" << src.getProps().size() << ")";
  os << std::endl;

  /// cout numeric values
  std::map<std::string, float>::const_iterator numIndex = src.getProps().begin();
  std::map<std::string, float>::const_iterator numEnd   = src.getProps().end();

  while(numIndex != numEnd)
  {
    os << std::endl << "    " << (*numIndex).first << " => " << (*numIndex).second;
    numIndex++;
  }

  // cout string values
  os << std::endl;
  os << std::endl << "  String properties (" << src.getPropsString().size() << ")";
  os << std::endl;

  std::map<std::string, std::string>::const_iterator stringIndex = src.getPropsString().begin();
  std::map<std::string, std::string>::const_iterator stringEnd   = src.getPropsString().end();

  while(stringIndex != stringEnd)
  {
    os << std::endl << "    " << (*stringIndex).first << " => \"" << (*stringIndex).second << "\"";
    stringIndex++;
  }

  os << std::endl << "}  // PropertySet";

  return os;
}

} // namespace ggui

////////////////////////////////////////////////////////////////////////////////



