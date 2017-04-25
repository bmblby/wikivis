
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



#ifndef GLOOST_PROPERTY_SET_H
#define GLOOST_PROPERTY_SET_H



// gloost includes
#include <gloost/Point3.h>



// cpp includes
#include <string>
#include <map>
#include <iostream>
#include <string>
#include <memory>

namespace tinyxml2
{
  class XMLElement;
}





namespace gloost
{

  ///  Holds strings and numeric (float, bool, int) properties with names as strings

class PropertySet
{
	public:

    /// a std::shared_ptr of an PropertySet instance
    typedef std::shared_ptr<PropertySet>       shared_ptr;
    typedef std::shared_ptr<const PropertySet> const_shared_ptr;

    // factory
    static shared_ptr create();
    static shared_ptr create(const PropertySet& propSet);
    static shared_ptr create(const std::string& xmlFilePath);
    static shared_ptr create(const gloost::PathType& xmlFilePath);


    // constructor
    PropertySet();
    PropertySet(const PropertySet& propSet);
    PropertySet(const std::string& xmlFilePath);
    PropertySet(const gloost::PathType& xmlFilePath);

    virtual ~PropertySet();


    /// find a numeric property and return a iterator to it
    const std::map<std::string, float>::const_iterator findProp(const std::string& name,
                                                                bool enableErrorMessage = 1) const;

    /// find a string property and return a iterator to it
    const std::map<std::string, std::string>::const_iterator findPropString(const std::string& name,
                                                                            bool enableErrorMessage = 1) const;

    /// find a Point3 property and return a iterator to it
    const std::map<std::string, Point3>::const_iterator findPropPoint3(const std::string& name,
                                                                            bool enableErrorMessage = 1) const;

    // checks if a property exists
    bool stringPropExists(const std::string& name);

    /// returns a property as const std::string
    const std::string getPropString(const std::string& name) const;


    /// returns a property of this node as  bool
    bool getPropBool(const std::string& name) const;
    /// returns a property of this node as  bool
    std::string getPropBoolAsString(const std::string& name) const;

    /// returns a numeric property int
    int getPropInt(const std::string& name) const;
    /// returns a int property as string
    std::string getPropIntAsString(const std::string& name) const;

    /// returns a numeric property as float
    float getPropFloat(const std::string& name) const;
    /// returns a float property as string
    std::string getPropFloatAsString(const std::string& name) const;

    /// returns a Point3 property
    const Point3& getPropPoint3(const std::string& name) const;
    /// returns a Point3 property as string (three components like this: "5.5 2.56 123.1")
    std::string getPropPoint3AsString(const std::string& name) const;



    /// sets a string property
    void setProp(const std::string& name, const std::string& value);
    /// sets a numeric property
    void setProp(const std::string& name, float value);
    /// sets a Point3 property
    void setProp(const std::string& name, const Point3& value);


    const std::map<std::string, float>&       getProps() const;
    const std::map<std::string, std::string>& getPropsString() const;
    const std::map<std::string, Point3>& getPropsPoint3() const;



	  /// loads a PropertySet from xml an file. return value indicates success
	  bool readXml(const std::string& xmlFilePath);

	  /// loads a PropertySet from xml an file. return value indicates success
	  bool readXml(const gloost::PathType& xmlFilePath);

	  /// traverses a XML PropertySet Node
	  bool traversXml(tinyxml2::XMLElement* element);

	  /// writes the PropertySet to a xml file
	  bool writeXml(const std::string& xmlFilePath);

	  /// writes the PropertySet to a xml file
	  bool writeXml(const gloost::PathType& xmlFilePath);



    /// assignment
    void operator= (const PropertySet&);



		/// sets the dirty flag
		void setDirty(const bool dirty = true);
		/// unsets the dirty flag
		void setClean();
		/// checks if the PropertySet is dirty
		bool isDirty() const;


	private:


    /// map with all numeric values
    std::map<std::string, float>       _properties;

    /// map with all string values
    std::map<std::string, std::string> _propertiesString;

    /// map with all Point3 values
    std::map<std::string, Point3>      _propertiesPoint3;


    bool _dirty;
};


extern std::ostream& operator<< (std::ostream& os, PropertySet& src);


} // namespace gloost


#endif // GLOOST_PROPERTY_SET_H



