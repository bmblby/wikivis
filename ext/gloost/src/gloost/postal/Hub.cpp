
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
#include <gloost/postal/Hub.h>
#include <gloost/postal/PathSystemNode.h>
#include <gloost/gloostMath.h>
#include <gloost/gloostHelper.h>


// cpp includes
#include <string>
#include <iostream>



namespace gloost
{
namespace postal
{


/**
  \class   Hub

  \brief   Distributor of gloost::postal::Message to instances of postal::Node

  \author  Felix Weiszig
  \date    December 2012
  \remarks ...
*/

////////////////////////////////////////////////////////////////////////////////


  /*static*/ boost::mutex Hub::_create_mutex;
  /*static*/ Hub*         Hub::_theInstance = nullptr;


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   creates a Hub instance
  \remarks ...
*/

/*static*/
Hub*
Hub::get()
{
  boost::mutex::scoped_lock lock(_create_mutex);

  if (!_theInstance)
  {
    _theInstance = new Hub();
  }
  return _theInstance;
}

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class constructor
  \remarks ...
*/

Hub::Hub():
  _pathSystemRoot( PathSystemNode::create("root") ),
  _connectedNodes(),
  _mutex_connectedNodes(),
  _knownDomains()
{

#ifndef GLOOST_SYSTEM_DISABLE_OUTPUT_WARNINGS
    std::cerr << std::endl << "Warning from Hub::Hub(): ";
    std::cerr << std::endl << "             gloost::message::... is in prealpha/experimental state, please use";
    std::cerr << std::endl << "             it with caution.";
    std::cerr << std::endl;
#endif

  _knownDomains.insert(".gloost.system.log");
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class destructor
  \remarks ...
*/

/*virtual*/
Hub::~Hub()
{
	// insert your code here
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   connects a Subscriber to a path, returns false if the node was already connected
  \param   ...
  \remarks ...
*/

/*static*/
bool
Hub::connect(Subscriber::shared_ptr subscriber, const std::string& path)
{
  boost::mutex::scoped_lock lock(_mutex_connectedNodes);

//  std::cerr << std::endl << "path: " << path;

  std::vector< std::string > explodedPath = gloost::helper::stringExplode(path, "/");


  // test path
  if (!path.length() || !explodedPath.size() || explodedPath[0].length())
  {
    std::cerr << std::endl <<  "(D'OH):     gloost::postal::Hub::connect() ";
    std::cerr << std::endl <<  "            invalid Path: \"" << path << "\"";
    std::cerr << std::endl;
    return false;
  }

  if (explodedPath.size() > 1 && !explodedPath[explodedPath.size()-1].length())
  {
    explodedPath.pop_back();
  }

  // add subscriber to the path system, building new nodes if necessary
  auto currentNode = _pathSystemRoot;
  for (unsigned i=1u; i!=explodedPath.size(); ++i)
  {
    auto childNode = currentNode->getChild(explodedPath[i]);
//    std::cerr << std::endl << "  cd " << explodedPath[i];

    // build path tree
    if (!childNode)
    {
      childNode = PathSystemNode::create(explodedPath[i]);
      currentNode->addChild(childNode);
    }
    currentNode = childNode;

  }
  return currentNode->addSubscriber(subscriber);
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   disconnect a Subscriber from a path, returns false if node was not connected to that path
  \param   ...
  \remarks ...
*/

/*static*/
bool
Hub::disconnect(Subscriber::shared_ptr subscriber, const std::string& path)
{
 boost::mutex::scoped_lock lock(_mutex_connectedNodes);

  std::cerr << std::endl << "path: " << path;

  std::vector< std::string > explodedPath = gloost::helper::stringExplode(path, "/");


  // test path
  if (!path.length() || !explodedPath.size() || explodedPath[0].length())
  {
    std::cerr << std::endl <<  "(D'OH):     gloost::postal::Hub::connect() ";
    std::cerr << std::endl <<  "            invalid Path: \"" << path << "\"";
    std::cerr << std::endl;
    return false;
  }

  if (explodedPath.size() > 1 && !explodedPath[explodedPath.size()-1].length())
  {
    explodedPath.pop_back();
  }

  // remove subscriber from the path/node, break if path doas not exists
  auto currentNode = _pathSystemRoot;
  for (unsigned i=1u; i!=explodedPath.size(); ++i)
  {
    auto childNode = currentNode->getChild(explodedPath[i]);

    // build path tree
    if (!childNode)
    {
      return false;
    }
    currentNode = childNode;

  }

  // remove subscriber
  currentNode->removeSubscriber(subscriber);


  // test if node is now empty and has no children and remove them recursively
  while (currentNode != currentNode)
  {
    if (!currentNode->getChildren().size() && !currentNode->getSubscribers().size())
    {
      auto parentNode = currentNode->getParent();
      parentNode->removeChild(currentNode);

      currentNode = parentNode;
    }
    else
    {
      currentNode = currentNode;
    }
  }

  return true;

}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   sends a message
  \param   ...
  \remarks ...
*/

/*static*/
unsigned
Hub::send(const std::string& path, Message::shared_ptr message, bool recursive)
{
  boost::mutex::scoped_lock lock(_mutex_connectedNodes);

  std::vector< std::string > explodedPath = gloost::helper::stringExplode(path, "/");

  // test path
  if (!path.length() || !explodedPath.size() || explodedPath[0].length())
  {
    std::cerr << std::endl <<  "(D'OH):     gloost::postal::Hub::connect() ";
    std::cerr << std::endl <<  "            invalid Path: \"" << path << "\"";
    std::cerr << std::endl;
    return false;
  }

  if (explodedPath.size() > 1 && !explodedPath[explodedPath.size()-1].length())
  {
    explodedPath.pop_back();
  }

  // remove subscriber from the path/node, break if path doas not exists
  auto currentNode = _pathSystemRoot;
  for (unsigned i=1u; i!=explodedPath.size(); ++i)
  {
    auto childNode = currentNode->getChild(explodedPath[i]);

    if (!childNode)
    {
      return false;
    }
    currentNode = childNode;
  }


  if (recursive)
  {
    unsigned numReceivers = sendRecursive(path, currentNode, message);
    return numReceivers;
  }


  for (auto subscriber : currentNode->getSubscribers() )
  {
    subscriber->receive(path, message);
  }

  return currentNode->getSubscribers().size();
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   sends a message
  \param   ...
  \remarks ...
*/

/*static*/
unsigned
Hub::sendRecursive(const std::string& path, std::shared_ptr<PathSystemNode> currentNode, Message::shared_ptr message)
{

  if (!currentNode)
  {
    return 0;
  }

  // send to own subscribers
  for (auto subscriber : currentNode->getSubscribers() )
  {
    subscriber->receive(path, message);
  }

  // send to children
  unsigned numReceivers = currentNode->getSubscribers().size();
  for (auto nameAndNode : currentNode->getChildren())
  {
    numReceivers += sendRecursive(path, nameAndNode.second, message);
  }
  return numReceivers;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   creates a unique string to use as a prefix/domain/namespace in a path
  \param   ...
  \remarks ...
*/

/*virtual*/
std::string
Hub::createUniqueDomainString()
{
  // create a random string
  bool notDone = true;
  while (notDone)
  {
    std::string randomPrefix("/domains/");
    for (unsigned i=0u; i!=8u; ++i)
    {
      randomPrefix += (unsigned char)(65 + gloost::math::frand()*26);
    }
    if (registerUniqueDomainString(randomPrefix))
    {
      notDone = false;
    }
    return randomPrefix;
  }
  return "";
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   registers a unique prefix and returns true. returns false if the prefix was already defined
  \param   ...
  \remarks ...
*/

/*virtual*/
bool
Hub::registerUniqueDomainString(const std::string& prefix)
{
  if (!prefix.length())
  {
    return false;
  }

  if (_knownDomains.find(prefix) == _knownDomains.end())
  {
    _knownDomains.insert(prefix);
    return true;
  }
  return false;
}


////////////////////////////////////////////////////////////////////////////////





} // namespace postal
} // namespace gloost

