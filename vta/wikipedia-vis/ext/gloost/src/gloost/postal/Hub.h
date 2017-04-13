
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



#ifndef H_GLOOST_MESSAGE_HUB
#define H_GLOOST_MESSAGE_HUB


// gloost includes
#include <gloost/postal/Subscriber.h>
#include <gloost/postal/Message.h>


// cpp includes
#include <string>
#include <memory>
#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>
#include <boost/thread/mutex.hpp>


namespace gloost
{
namespace postal
{

  class PathSystemNode;



  //  Distributor of gloost::postal::Message to instances of postal::Subscriber

class Hub
{
	public:

    // returns the Hub instance
    static Hub* get();


    // connects a Subscriber to a path, returns false if the subscriber was already connected
    bool connect(Subscriber::shared_ptr subscriber, const std::string& path);

    // disconnect a Node from a path, returns false if the subscriber was already connected
    bool disconnect(Subscriber::shared_ptr subscriber, const std::string& path);


    // sends a message
    unsigned send(const std::string& path, Message::shared_ptr message, bool recursive = false);

    // creates a unique string to use as a prefix/domain/namespace in a path
    virtual std::string createUniqueDomainString();


	protected:

    // class constructor
    Hub();

    // class destructor
	  virtual ~Hub();


    // registers a unique prefix and returns true. returns false if the prefix was already defined
    virtual bool registerUniqueDomainString(const std::string& prefix);


    unsigned sendRecursive(const std::string& path,
                           std::shared_ptr<PathSystemNode> currentNode,
                           Message::shared_ptr             message);


	private:

    //
	  static Hub*         _theInstance;
    static boost::mutex _create_mutex;  // only needed in if c++ version < C++11

    /// root of the path system
    std::shared_ptr<PathSystemNode> _pathSystemRoot;

    typedef boost::unordered_multimap<std::string, Subscriber::shared_ptr> ContainerType;
    ContainerType _connectedNodes;
    boost::mutex  _mutex_connectedNodes;

    //
    boost::unordered_set<std::string> _knownDomains;

};


} // namespace postal

} // namespace gloost


#endif // H_GLOOST_MESSAGE_HUB












