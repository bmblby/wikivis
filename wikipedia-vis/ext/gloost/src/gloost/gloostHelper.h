
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

#ifndef H_GLOOST_HELPER
#define H_GLOOST_HELPER

#include <gloost/gloostConfig.h>
#include <gloost/log/GlobalLog.h>
#include <gloost/util/Timer.h>
#include <gloost/RandomSequence.h>

// cpp includes
#include <string>
#include <sstream>
#include <cstring>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <assert.h>
#include <locale>
#include <omp.h>
#include <thread>

#include <tinyxml2.h>


#ifdef GLOOST_WINDOWS
  #include <windows.h>
  #include <freeimage/freeimage.h>
#else
  #include <FreeImage.h>
#endif

#include <boost/lexical_cast.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/algorithm/string.hpp>

namespace gloost
{
namespace helper
{

const static std::string emptyString = "";

inline
void
stringExplode(const std::string&        aString,
                          const std::string&        separator,
                          std::vector<std::string>& results);

///////////////////////////////////////////////////////////////////////////////

  /// cuts a string so it is not longer as maxLength

inline
std::string
cutString(const std::string& line, unsigned maxLength)
{
  if (line.length() > maxLength)
  {
    return line.substr( 0, maxLength);
  }
  return line;
}

///////////////////////////////////////////////////////////////////////////////

  /// converts a float into a string

template <class T>
inline std::string
toString(const T& value, unsigned precision = 8u)
{
  std::ostringstream stream;
  stream.precision(precision);
  stream << value;
  return stream.str();
}

///////////////////////////////////////////////////////////////////////////////

  /// converts a float into a string

template <class T>
inline std::string
toFormatedString(const T& value,
                   unsigned precision = 8,
                   const std::ios_base::fmtflags& format = std::ios::fixed)
{
  std::ostringstream stream;
  stream.precision(precision);
  stream << format << value;
  return stream.str();
}

///////////////////////////////////////////////////////////////////////////////

  /// converts a float into a string

inline std::string
floatToString(gloost::mathType value, unsigned precision = 8u)
{
  const std::string withZeros = toString(value, precision);
  return withZeros;
  //removed because of bug when notation is scientifc like 4.8928-e05
/*
  // find last dot
  const auto dotPos = withZeros.rfind('.');

  if (dotPos == std::string::npos)
  {
    return withZeros;
  }

  const auto dezimal = withZeros.substr( dotPos+1, std::string::npos);
  if(dezimal.length() <= precision+1)
  {
    return withZeros;
  }

  const auto result = withZeros.substr( 0, dotPos+precision+1);
  return result;
*/
}

///////////////////////////////////////////////////////////////////////////////

  /// converts something into a string of constant length and fills empty chars with fillChar

template <class T>
inline std::string
toConstLengthString(T value, unsigned length, char fillChar)
{
  std::ostringstream stream;
  stream << std::setw(length) << std::setfill(fillChar) << value;
  return stream.str();
}

///////////////////////////////////////////////////////////////////////////////

  /// converts something into a string of constant length and fills empty chars with fillChar

template <class T>
inline std::string
toConstLengthString(T value, unsigned precision, unsigned length, char fillChar)
{
  std::ostringstream stream;
  stream.precision(precision);
  stream << std::setw(length) << std::setfill(fillChar) << value;
  return stream.str();
}

///////////////////////////////////////////////////////////////////////////////

  /// returns the basename of a file within a path ( "folder/subfolder/someimage.jpg" -> "someimage" )

inline
std::string
pathToBasename(const std::string& filepath)
{
  // find last dot
  const int dotPos   = filepath.rfind('.');
  // find last slash
  const int slashPos = filepath.rfind('/');
  std::string filename = filepath.substr( slashPos+1, dotPos-slashPos-1);
  return filename;
}

///////////////////////////////////////////////////////////////////////////////

  /// returns the basename of a file within a path ( "folder/subfolder/someimage.jpg" -> "someimage" )

inline
PathType
pathToBasename(const PathType& filepath)
{
  return filepath.stem();
}

///////////////////////////////////////////////////////////////////////////////

  /// return the basename of a filename or a path ( "folder/subfolder/someimage.jpg" -> "someimage.jpg" )

inline
std::string
pathToFilename(const std::string& filepath)
{
  // find last slash
  const int slashPos = filepath.rfind('/');
  if ((size_t)slashPos == std::string::npos)
  {
    return filepath;
  }
  return filepath.substr( slashPos+1, std::string::npos);
}

///////////////////////////////////////////////////////////////////////////////

  /// return the basename of a filename or a path ( "folder/subfolder/someimage.jpg" -> "someimage.jpg" )

inline
gloost::PathType
pathToFilename(const gloost::PathType& filepath)
{
  return filepath.filename();
}

///////////////////////////////////////////////////////////////////////////////

  /// return extension of a filename ore path ( "folder/subfolder/someimage.jpg" -> "jpg" )

inline
std::string
pathToExtension(const std::string& filepath)
{
  const std::string filename = pathToFilename(filepath);
  // find last dot
  const auto dotPos = filename.rfind('.');
  if (dotPos == std::string::npos)
  {
    return "";
  }
  std::string extension = filename.substr( dotPos+1, std::string::npos);
  if (!strcmp(extension.c_str(), filename.c_str()))
  {
    extension = "";
  }
  return extension;
}

///////////////////////////////////////////////////////////////////////////////

  /// return extension of a filename ore path ( "folder/subfolder/someimage.jpg" -> "jpg" )

inline
PathType
pathToExtension(const PathType& filepath)
{
  return filepath.extension();
}

///////////////////////////////////////////////////////////////////////////////

  /// return the path of a file without the filename of a path ( "folder/subfolder/someimage.jpg" -> "folder/subfolder" )

inline
std::string
pathToBasePath(const std::string& filepath, const char delimiter = '/')
{
  // find last slash
  const int slashPos = filepath.rfind(delimiter);
  return filepath.substr( 0, slashPos+1);
}

///////////////////////////////////////////////////////////////////////////////

  /// return the path of a file without the filename of a path ( "folder/subfolder/someimage.jpg" -> "folder/subfolder" )

inline
boost::filesystem::path
pathToBasePath(const gloost::PathType& filepath)
{
  return filepath.parent_path();
}


///////////////////////////////////////////////////////////////////////////////

  /// taken from http://stackoverflow.com/questions/10167382/boostfilesystem-get-relative-path
  /// returns a path relative to 'from'

inline
boost::filesystem::path pathRelativeTo(boost::filesystem::path from, boost::filesystem::path to)
{
   // Start at the root path and while they are the same then do nothing then when they first
   // diverge take the remainder of the two path and replace the entire from path with ".."
   // segments.
   boost::filesystem::path::const_iterator fromIter = from.begin();
   boost::filesystem::path::const_iterator toIter = to.begin();

   // Loop through both
   while (fromIter != from.end() && toIter != to.end() && (*toIter) == (*fromIter))
   {
      ++toIter;
      ++fromIter;
   }

   boost::filesystem::path finalPath;
   while (fromIter != from.end())
   {
      finalPath /= "..";
      ++fromIter;
   }

   while (toIter != to.end())
   {
      finalPath /= *toIter;
      ++toIter;
   }

   return finalPath;
}

///////////////////////////////////////////////////////////////////////////////

  /// returns the basename of a file within a path ( "folder/subfolder/someimage.jpg" -> "someimage" )

inline
std::string
pathToLastPart(const std::string& filepath)
{
  std::vector<std::string> fragments;
  stringExplode(filepath, "/", fragments);
  if (fragments.size() > 0 && fragments[fragments.size()-1].length())
  {
    return fragments[fragments.size()-1];
  }
  else if (fragments.size() > 1 && fragments[fragments.size()-2].length())
  {
    return fragments[fragments.size()-2];
  }
  return "";
}

///////////////////////////////////////////////////////////////////////////////

  /// remarks: taken from http://www.infernodevelopment.com/perfect-c-string-explode-split

inline
void
stringExplode(const std::string&        aString,
              const std::string&        separator,
              std::vector<std::string>& results)
{
  if (!separator.length())
  {
    return;
  }

  std::string tmpString = aString;
  int found = tmpString.find_first_of(separator);

  while(found != (int)std::string::npos)
  {
    if(found > 0)
    {
      results.push_back(tmpString.substr(0,found));
    }
    else
    {
      results.push_back("");
    }
    tmpString = tmpString.substr(found+separator.length());
    found = tmpString.find_first_of(separator);
  }

//  if(tmpString.length() > 0)
  {
    results.push_back(tmpString);
  }

}

///////////////////////////////////////////////////////////////////////////////

  /// remarks: taken from http://www.infernodevelopment.com/perfect-c-string-explode-split

inline
std::vector<std::string>
stringExplode(const std::string& aString,
              const std::string& separator)
{
  std::vector<std::string> results;
  stringExplode(aString,
                separator,
                results);

  return results;
}

///////////////////////////////////////////////////////////////////////////////

  /// creates a string by concatinating the strings from a vecor using a seperator

std::string inline
stringImplode(const std::vector<std::string>& stringVector, const std::string& seperator)
{
  std::string tmpString("");
  if (!stringVector.empty())
  {
    tmpString = stringVector[0];

    for (unsigned int i=1; i!=stringVector.size(); ++i)
    {
      tmpString = tmpString + seperator + stringVector[i];
    }
  }
  return tmpString;
}

///////////////////////////////////////////////////////////////////////////////

  /// creates a string by cloning n times the input string

std::string inline
stringRepeat(const std::string& str, unsigned n)
{
  std::string tmpString = "";
  for (unsigned int i=0; i!=n; ++i)
  {
    tmpString = tmpString + str;
  }

  return tmpString;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   replaces substrings of a string
  \remarks original code taken from http://stackoverflow.com/questions/1452501/string-replace-in-c
*/
inline
std::string
stringSearchAndReplace(const std::string& original, const std::string& search, const std::string& replace)
{
  std::string newString = original;
  std::string::size_type  next;

  for(next = newString.find(search);        // Try and find the first match
      next != std::string::npos;            // next is npos if nothing was found
      next = newString.find(search,next)    // search for the next match starting after
                                            // the last match that was found.
     )
  {
      // Inside the loop. So we found a match.
      newString.replace(next,search.length(),replace);   // Do the replacement.
      next += replace.length();                          // Move to just after the replace
                                                         // This is the point were we start
                                                         // the next search from.
  }
  return newString;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   replaces substrings of a gloost::PathType
  \remarks uses wstring in windows else string (see stringSearchAndReplace)
*/
inline
gloost::PathType
pathSearchAndReplace(const gloost::PathType original, const std::string& search, const std::string& replace)
{
  #ifdef GLOOST_WINDOWS
    std::wstring newString = original.wstring();

    std::wstring newSearch = gloost::PathType(search).wstring();
    std::wstring newReplace = gloost::PathType(replace).wstring();
  #else
    std::string newString = original.string();

    std::string newSearch  = search;
    std::string newReplace = replace;
  #endif
    std::string::size_type  next;

  for(next = newString.find(newSearch);     // Try and find the first match
      next != std::string::npos;            // next is npos if nothing was found
      next = newString.find(newSearch,next) // search for the next match starting after
                                            // the last match that was found.
     )
  {
      // Inside the loop. So we found a match.
      newString.replace(next,newSearch.length(),newReplace); // Do the replacement.
      next += newReplace.length();                           // Move to just after the replace
                                                             // This is the point were we start
                                                             // the next search from.
  }
  return newString;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief returns the next word of an ifstream as int
*/

inline int
readNextWordAsInt(std::ifstream& inStream)
{
  std::string word;
  inStream >> word;
  return atoi(word.c_str());
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief returns the next word of an instream as float
*/

inline float
readNextWordAsFloat(std::ifstream& inStream)
{
  std::string word;
  inStream >> word;
  return atof(word.c_str());
}

///////////////////////////////////////////////////////////////////////////////

  /// converts an unsigned to its binary representation and returns a string

inline
std::string
unsignedToBinaryString(unsigned number)
{
  static char binstr[32];
  binstr[31] = '\0' ;
  for (int i=0; i<32; i++)
  {
    binstr[31-i] = number & 1 ? '1' : '0' ;
    number = number / 2 ;
  }
  std::string returnString(binstr);
  return returnString;
}

///////////////////////////////////////////////////////////////////////////////

  /// reinterpretes the bits of a float as unsigned int

inline
unsigned
float_as_unsigned(float fValue)
{
//  return *(reinterpret_cast<unsigned*>((&floatValue))); // yes I do break aliasing rules, so what
  assert(sizeof(unsigned) == sizeof(float));
  unsigned uValue;
  memcpy(&uValue, &fValue, sizeof fValue);
  return uValue;
}

///////////////////////////////////////////////////////////////////////////////

  /// reinterpretes the bits of an unsigned int as float

inline
float
unsigned_as_float(unsigned uValue)
{
//  return *(reinterpret_cast<float*>((&intValue))); // and again ...
  assert(sizeof(unsigned) == sizeof(float));
  float fValue;
  memcpy(&fValue, &uValue, sizeof fValue);
  return fValue;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   packs four 8bit values into one 32bit unsigned
  \remarks
*/

inline
void
packRgbaToUnsigned(unsigned& value32,
                   unsigned char r, unsigned char g,
                   unsigned char b, unsigned char a)
{
  value32 = (unsigned)r;
  value32 <<= 8;
  value32 = value32 | (unsigned)g;
  value32 <<= 8;
  value32 = value32 | (unsigned)b;
  value32 <<= 8;
  value32 = value32 | (unsigned)a;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   unpacks four 8bit values from one 32bit unsigned
  \remarks
*/

inline
void
writeRGBAToPNG(unsigned char* buffer,
               unsigned int width,
               unsigned int height,
               bool         rotateImage,
               const        std::string& path)
{

  FreeImage_Initialise();
  FIBITMAP* bitmap = FreeImage_Allocate(width, height, 32);
  RGBQUAD color;

  for (size_t y = 0; y != height; ++y)
  {
    for (size_t x = 0; x !=width; ++x)
    {
      unsigned int realX = x;
      unsigned int realY = y;

      if (rotateImage)
      {
        realX = width - x - 1;
        realY = height- y - 1;
      }
      unsigned i = (realX + realY * width )*4;
#if 0
      float value_r = std::min(1.0f, buffer[i])   * 255.0f;
      float value_g = std::min(1.0f, buffer[i+1]) * 255.0f;
      float value_b = std::min(1.0f, buffer[i+2]) * 255.0f;
      float value_a = std::min(1.0f, buffer[i+3]) * 255.0f;

      color.rgbRed =  value_r;
      color.rgbGreen =  value_g;
      color.rgbBlue =  value_b;
      color.rgbReserved =  value_a;
#endif // 0

      color.rgbRed =  buffer[i];
      color.rgbGreen =  buffer[i+1];
      color.rgbBlue =  buffer[i+2];
      color.rgbReserved =  buffer[i+3];

      FreeImage_SetPixelColor(bitmap,x,y, &color);
    }
  };

  std::cerr << std::endl;
  std::cerr << std::endl << "path: " << path;
  std::cerr << std::endl;
  FreeImage_Save(FIF_PNG,bitmap,(char*)path.c_str(), 0);
  FreeImage_Unload(bitmap);
  FreeImage_DeInitialise();
}


///////////////////////////////////////////////////////////////////////////////

/**
  \brief   unpacks four 8bit values from one 32bit unsigned
  \remarks
*/

inline
void
writeRGBAToJPG(unsigned char* buffer,
               unsigned int width,
               unsigned int height,
               bool         rotateImage,
               const        std::string& path,
               unsigned     quality = 10)
{

  FreeImage_Initialise();
  FIBITMAP* bitmap = FreeImage_Allocate(width, height, 24);
  RGBQUAD color;

  for (size_t y = 0; y != height; ++y)
  {
    for (size_t x = 0; x !=width; ++x)
    {
      unsigned int realX = x;
      unsigned int realY = y;

      if (rotateImage)
      {
        realX = width - x - 1;
        realY = height- y - 1;
      }
      unsigned i = (realX + realY * width )*4;
#if 0
      float value_r = std::min(1.0f, buffer[i])   * 255.0f;
      float value_g = std::min(1.0f, buffer[i+1]) * 255.0f;
      float value_b = std::min(1.0f, buffer[i+2]) * 255.0f;
      float value_a = std::min(1.0f, buffer[i+3]) * 255.0f;

      color.rgbRed =  value_r;
      color.rgbGreen =  value_g;
      color.rgbBlue =  value_b;
      color.rgbReserved =  value_a;
#endif // 0

      color.rgbRed =  buffer[i];
      color.rgbGreen =  buffer[i+1];
      color.rgbBlue =  buffer[i+2];
      color.rgbReserved =  buffer[i+3];

      FreeImage_SetPixelColor(bitmap,x,y, &color);
    }
  };
  FreeImage_Save(FIF_JPEG,bitmap,(char*)path.c_str(), quality);
  FreeImage_Unload(bitmap);
  FreeImage_DeInitialise();
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   unpacks four 8bit values from one 32bit unsigned
  \remarks
*/

inline
void
unpackRgbaFromUnsigned(unsigned value32,
                       unsigned char& r, unsigned char& g,
                       unsigned char& b, unsigned char& a)
{
  const unsigned mask = 255;
  a = (unsigned char)(value32 & mask);
  value32 >>= 8;
  b = (unsigned char)(value32 & mask);
  value32 >>= 8;
  g = (unsigned char)(value32 & mask);
  value32 >>= 8;
  r = (unsigned char)(value32 & mask);
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   tries to cast a char* to a mathtype
  \remarks
*/

inline
bool
interpretFromChar(int& output, const char* input)
{
  if(!input)
  {
    return false;
  }
  try
  {
    output = boost::lexical_cast<int>(input);
  }
  catch(const boost::bad_lexical_cast&)
  {
    return false;
  }
  return true;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   tries to cast a char* to a mathtype
  \remarks
*/

inline
bool
interpretFromChar(unsigned& output, const char* input)
{
  if(!input)
  {
    return false;
  }
  try
  {
    output = boost::lexical_cast<unsigned>(input);
  }
  catch(const boost::bad_lexical_cast&)
  {
    return false;
  }
  return true;
}


///////////////////////////////////////////////////////////////////////////////

/**
  \brief   tries to cast a char* to a mathtype
  \remarks
*/

inline
bool
interpretFromChar(double& output, const char* input)
{
  if(!input)
  {
    return false;
  }
  try
  {
    output = boost::lexical_cast<double>(input);
  }
  catch(const boost::bad_lexical_cast&)
  {
    return false;
  }
  return true;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   tries to cast a char* to a mathtype
  \remarks
*/

inline
bool
interpretFromChar(float& output, const char* input)
{
  if(!input)
  {
    return false;
  }
  try
  {
    output = boost::lexical_cast<float>(input);
  }
  catch(const boost::bad_lexical_cast&)
  {
    return false;
  }
  return true;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   tries to cast a char* to a mathtype
  \remarks
*/

inline
bool
interpretFromChar(std::string& output, const char* input)
{
  if(!input)
  {
    return false;
  }
  output = input;
  return true;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   tries to cast a char* to a mathtype
  \remarks
*/

inline
bool
interpretFromChar(PathType& output, const char* input)
{
  if(!input)
  {
    return false;
  }
  output = input;
  return true;
}

//////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns true if a string end with a given substring
  \remarks ...
*/

inline
bool
string_ends_with(const std::string& s, const std::string& e)
{
  int index_s(s.size()-1);
  int index_e(e.size()-1);
  if (index_s < index_e)
  {
    return false;
  }
  while (index_s >= 0 && index_e >= 0)
  {

    if (s[index_s] != e[index_e])
    {
      return false;
    }
    --index_s;
    --index_e;
  }
  return true;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   formats a string using printf format
  \remarks find all parameters here: http://de.cppreference.com/w/cpp/io/c/fprintf
*/

inline
std::string
formatString(const std::string& string, std::string& printfFormatString)
{
  if (!string.length() || !printfFormatString.length())
  {
    return string;
  }
  const auto length = string.length()*2u;
  char* formatedString = new char[length];
  /*auto check = */snprintf ( formatedString, length, printfFormatString.c_str(), string.c_str() );
  const std::string result = formatedString;
  delete[] formatedString;
  return result;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   formats a numeric value using printf format
  \remarks find all parameters here: http://de.cppreference.com/w/cpp/io/c/fprintf
*/

inline
std::string
formatString(double value, const std::string& printfFormatString)
{
  if (!printfFormatString.length())
  {
    return toString(value);
  }
  const auto length =500;
  char* formatedString = new char[length];
  /*auto check = */snprintf (formatedString, length, printfFormatString.c_str(), value );
  const std::string result = formatedString;
  delete[] formatedString;
  return result;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   returns a upper case version of a given string
  \remarks ...
*/

inline
std::string
stringToUpperCase(const std::string& string)
{
  return boost::to_upper_copy<std::string>(string);
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief   wraps writing a tinyxml file to disc
  \remarks for windows _wfopen is used to create a FILE*
*/

inline
bool
writeXmlFile(boost::filesystem::path filePath, const std::shared_ptr<tinyxml2::XMLDocument>& newDoc)
{
  if(!filePath.empty())
  {
    #ifdef GLOOST_WINDOWS
    FILE* fp = nullptr;
    errno_t err = _wfopen_s(&fp, filePath.wstring().c_str(), L"wb" );
    if( fp && !err )
    {
      newDoc->SaveFile(fp);
      fclose( fp );
      return true;
    }
    else
    {
      return false;
    }
    #else
      newDoc->SaveFile(filePath.string().c_str());
      return true;
    #endif
  }
  return false;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief    wraps opening a tinyxml file from disc
  \remarks  since tinyxml2 has no interface for opening files by wstring, _wfopen is used in windows to create a FILE*
*/

inline
std::shared_ptr<tinyxml2::XMLDocument>
openXmlFile(boost::filesystem::path filePath)
{
  auto doc = std::shared_ptr<tinyxml2::XMLDocument>(new tinyxml2::XMLDocument());
  if(!filePath.empty())
  {
#ifdef GLOOST_WINDOWS
    FILE* w_fp = nullptr;
    errno_t err = _wfopen_s(&w_fp, filePath.wstring().c_str(), L"rb" );
    if( !w_fp || err)
    {
        gloost::log::GlobalLog::postDebug(BOOST_CURRENT_FUNCTION,
                                          "Could not load file from \n" + toString(filePath));
        return nullptr;
    }

    auto tinyxmlError = doc->LoadFile(w_fp);
    if(tinyxmlError != 0)
    {
        gloost::log::GlobalLog::postDebug(BOOST_CURRENT_FUNCTION,
                                          "Problem with xml file from \n" + toString(filePath));
//        doc->PrintError();
        return nullptr;
    }
    fclose( w_fp );
#else
      if(doc->LoadFile(filePath.string().c_str()) != 0)
      {
        gloost::log::GlobalLog::postDebug(BOOST_CURRENT_FUNCTION,
                                          "Problem with xml file from \n" + toString(filePath));
        return nullptr;
      }
#endif
  }
  return doc;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief creates a deep copy of a node including all its children
*/

inline
tinyxml2::XMLNode*
xmlDeepCopy(const tinyxml2::XMLNode *src, tinyxml2::XMLDocument *destDoc )
{
  tinyxml2::XMLNode *current = src->ShallowClone( destDoc );
  for(const tinyxml2::XMLNode* child= src->FirstChild(); child; child=child->NextSibling() )
  {
      current->InsertEndChild( xmlDeepCopy( child, destDoc ) );
  }
  return current;
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief returns a std::string containing the a tinyxml2::XMLElement
*/


inline
std::string
xmlElementAsString(tinyxml2::XMLElement* element)
{
  if(element)
  {
    tinyxml2::XMLPrinter p;
    element->Accept(&p);
    return p.CStr();
  }
  gloost::log::GlobalLog::postDoh(BOOST_CURRENT_FUNCTION,
                                   "Element is nullptr.");
  return "";
}

///////////////////////////////////////////////////////////////////////////////

/**
  \brief returns the number of supported native threads
*/

inline
size_t
getNumSystemThreads()
{
//  return omp_get_max_threads();
  return std::thread::hardware_concurrency();
}

///////////////////////////////////////////////////////////////////////////////

template <class ValueType>
class RleTuple
{
  public:
    RleTuple(unsigned count, const ValueType& value):_count(count), _value(value) {}
    unsigned  _count;
    ValueType _value;
};

///////////////////////////////////////////////////////////////////////////////

template <class ValueType>
inline
std::ostream&
operator<< (std::ostream& os, const RleTuple<ValueType>& rlet)
{
  if   (rlet._count == 1u) os << floatToString(rlet._value);
  else                     os << toString(rlet._count) << '*' << floatToString(rlet._value);
  return os;
}

///////////////////////////////////////////////////////////////////////////////


template <class ValueType>
inline
std::vector<RleTuple<ValueType> >
rleCompress(const std::vector<ValueType>& input, size_t maxTupleSize, ValueType maxEpsilon)
{
  std::vector<RleTuple<ValueType> > output;
  if (!input.size())
  {
    return output;
  }

  auto currentValue = input[0];
  auto currentCount = 1u;

  for (unsigned i=1u; i!=input.size(); ++i)
  {
    if (std::abs(input[i] - currentValue) > GLOOST_MATHTYPE_MIN_EPSILON || currentCount == maxTupleSize)
    {
      output.push_back( {currentCount, currentValue} );
      currentValue = input[i];
      currentCount = 1;
    }
    else
    {
      ++currentCount;
    }
  }

  // last tuple
  output.push_back( {currentCount, currentValue} );
  return output;
}

///////////////////////////////////////////////////////////////////////////////


/**
  \brief returns a random string (random numbers with the base of 64)
  \remarks ...
*/

inline
std::string
getRandomString(size_t numDigits, unsigned seed = gloost::util::Timer::getNow()*10000u)
{
  static std::vector<unsigned char> lookup = {'-', '_',
                                              '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                                              'a', 'b', 'c', 'd', 'e', 'f', 'g', 'i', 'h', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
                                              'A', 'B', 'C', 'D', 'E', 'F', 'G', 'I', 'H', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

  static auto randomSequence = gloost::RandomSequence(seed);
  std::string string = "";
  for (unsigned i=0u; i!=numDigits; ++i)
  {
    unsigned digit = (unsigned)(randomSequence.frand()*lookup.size());
    string += lookup[digit];
  }
  return string;
}

} // namespace helper


///////////////////////////////////////////////////////////////////////////////

}  // namespace gloost

/// + operator for string+int

inline const std::string operator+ (const std::string& s, const int ui)
{
  return s + gloost::helper::toString(ui);
}
/// + operator for int+string
inline const std::string operator+ (const int ui, const std::string& s)
{
  return gloost::helper::toString(ui) + s;
}

/// + operator for string+unsigned int
inline const std::string operator+ (const std::string& s, const unsigned int ui)
{
  return s + gloost::helper::toString(ui);
}
/// + operator for unsigned int+string
inline const std::string operator+ (const unsigned int ui, const std::string& s)
{
  return gloost::helper::toString(ui) + s;
}

/// + operator for string+float
inline const std::string operator+ (const std::string& s, const float f)
{
  return s + gloost::helper::toString(f);
}
/// + operator for float+string
inline const std::string operator+ (const float f, const std::string& s)
{
  return gloost::helper::toString(f) + s;
}

#define GLOOST_CURRENT_LOCATION __PRETTY_FUNCTION__  + std::string(" @ Line: ") +  gloost::helper::toString(__LINE__)

//bool stringcmp(const std::string& s1, const std::string& s2)
//{
//  return !(strcmp(s1.c_str(), s2.c_str()));
//}

#endif // #ifndef GLOOST_HELPER_H
