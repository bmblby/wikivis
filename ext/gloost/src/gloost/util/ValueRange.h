
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


#ifndef H_GLOOST_VALUERANGE
#define H_GLOOST_VALUERANGE




// gloost includes
#include <gloost/gloostConfig.h>
#include <gloost/gloostMath.h>


// cpp includes
#include <string>
#include <memory>



namespace gloost
{
namespace util
{

/**
  \class   ValueRange

  \brief   stores a generic value range with minValue and maxValue

  \author  Felix Weiszig, dynardo GmbH
  \date    May 2013
  \remarks ...
*/

template <class ValueType=gloost::mathType>
class ValueRange
{
	public:

    /// a std::shared_ptr of an ValueRange instance
    typedef std::shared_ptr<ValueRange<ValueType> >       shared_ptr;
    typedef std::shared_ptr<const ValueRange<ValueType> > const_shared_ptr;


    ////////////////////////////////////////////////////////////////////////////

    static const std::function<bool(ValueRange<ValueType>,ValueType)> relation_within;
    static const std::function<bool(ValueRange<ValueType>,ValueType)> relation_without;

    ////////////////////////////////////////////////////////////////////////////


    /// class factory
    static shared_ptr create(ValueType initValue)
    {
      return shared_ptr(new ValueRange<ValueType>(initValue));
    }


    /// class factory
    static shared_ptr create(ValueType lower, ValueType upper)
    {
      return shared_ptr(new ValueRange<ValueType>(lower, upper));
    }


    ////////////////////////////////////////////////////////////////////////////


    /// class constructor
    ValueRange(ValueType initValue):
      _lower(initValue),
      _upper(initValue)
    {
      // insert your code here
    }


    /// class constructor
    ValueRange(ValueType lower, ValueType upper):
      _lower(lower),
      _upper(upper)
    {
      // insert your code here
    }

    /// class destructor
	  virtual ~ValueRange(){};

    ////////////////////////////////////////////////////////////////////////////

    /**
      \brief  returns the lower boundary
    */
	  ValueType getLower() const
    {
      return _lower;
    }

    ////////////////////////////////////////////////////////////////////////////

    /**
      \brief  returns the upper boundary
    */
    ValueType getUpper() const
    {
      return _upper;
    }

    ////////////////////////////////////////////////////////////////////////////

    /**
      \brief  set the upper boundary to value
    */
    void setUpper(const ValueType& value)
    {
      _upper = value;
    }

    ////////////////////////////////////////////////////////////////////////////

    /**
      \brief  set the lower boundary to value
    */
    void setLower(const ValueType& value)
    {
      _lower = value;
    }

    ////////////////////////////////////////////////////////////////////////////

    /**
      \brief  set the upper and lower boundary to upper and lower
    */
    void setRange(const ValueType& lower, const ValueType& upper)
    {
       _lower = lower;
       _upper = upper;
    }

    ////////////////////////////////////////////////////////////////////////////

    /**
      \brief  returns the width of the range
    */
	  ValueType getWidth() const
    {
      return _upper - _lower;
    }

    ////////////////////////////////////////////////////////////////////////////

    /**
      \brief  resets the range by setting lower and upper to value
    */
	  void reset(ValueType value)
    {
      _lower = value;
      _upper = value;
    }

    ////////////////////////////////////////////////////////////////////////////

    /**
      \brief expands the range if value is smaller as min or bigger as max, returns true if range was actually expanded
    */
	  bool expand(ValueType value)
    {
      if (value < _lower)
      {
        _lower = value;
        return true;
      }

      if (value > _upper)
      {
        _upper = value;
        return true;
      }
      return false;
    }

    ////////////////////////////////////////////////////////////////////////////

    /**
      \brief expands the rangeby another range, returns true if range was actually modified in the process
    */
	  bool expand(const ValueRange<ValueType>& range)
    {
      auto modified = false;

      if (range._lower < _lower)
      {
        _lower = range._lower;
        modified = true;
      }

      if (range._upper > _upper)
      {
        _upper = range._upper;
        modified = true;
      }
      return modified;
    }

    ////////////////////////////////////////////////////////////////////////////

    /**
      \brief checks if two ranges intersect
    */
	  bool intersects(const ValueRange& range) const
    {
      if(gloost::math::betweenOrEqual(range.getLower(),_lower,_upper) ||
         gloost::math::betweenOrEqual(range.getUpper(),_lower,_upper) ||
         gloost::math::betweenOrEqual(_lower,range.getLower(),range.getUpper()) ||
         gloost::math::betweenOrEqual(_upper,range.getLower(),range.getUpper()) )
      {
        return true;
      }
      else
      {
        return false;
      }
    }

    ////////////////////////////////////////////////////////////////////////////

    /**
      \brief checks if range contains value
    */
	  bool contains(const ValueType& value) const
    {
      return !(value < _lower || value > _upper);
    }

    ////////////////////////////////////////////////////////////////////////////

    /**
      \ =operator
    */
	  ValueRange& operator=(const ValueRange& rhs)
    {
      _lower = rhs.getLower();
      _upper = rhs.getUpper();

      return *this;
    }

    ////////////////////////////////////////////////////////////////////////////

	protected:

   // ...


	private:

    ValueType _lower;
    ValueType _upper;

};

////////////////////////////////////////////////////////////////////////////

// as seen here: http://stackoverflow.com/questions/3229883/static-member-initialization-in-a-class-template

/*static*/
template <class ValueType>
const std::function<bool(ValueRange<ValueType>,ValueType)>
ValueRange<ValueType>::relation_within = [](const ValueRange<ValueType>& range, const ValueType& value)->bool
{
  return ((value > range.getLower()) && (value < range.getUpper()));
};

////////////////////////////////////////////////////////////////////////////

/*static*/
template <class ValueType>
const std::function<bool(ValueRange<ValueType>,ValueType)>
ValueRange<ValueType>::relation_without = [](const ValueRange<ValueType>& range, const ValueType& value)->bool
{
  return ((value < range.getLower()) || (value > range.getUpper()));
};

////////////////////////////////////////////////////////////////////////////

/* extern */
template <class ValueType>
std::ostream&
operator<< (std::ostream& os, const ValueRange<ValueType>& range)
{
  os << "Range (" << std::fixed << range.getLower() << " ... " << range.getUpper() << ")";
  return os;
}

} // namespace util
} // namespace gloost


#endif // H_GLOOST_VALUERANGE


