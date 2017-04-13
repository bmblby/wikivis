
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



#ifndef H_GLOOST_ZEROMQSTATESERVER
#define H_GLOOST_ZEROMQSTATESERVER

// gloost system includes
#include <gloost/gloostConfig.h>
#include <gloost/gloostMath.h>
#include <gloost/network/ZeroMqNode.h>


// cpp includes
#include <string>
#include <zmq.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>


namespace gloost
{


  class BinaryBundle;



  //  A server receiving custom data and answers with the current state data

class ZeroMqStateServer: public ZeroMqNode
{
	public:

    // class constructor
    ZeroMqStateServer(unsigned contextId = 1,
                      unsigned mode      = ZMQ_REP);

    // class destructor
	  ~ZeroMqStateServer();


	  // connects the client to a server
	  virtual void connect(const std::string& address, const std::string& port);



	  // queues a BinaryBundle for sending
	  void queue(BinaryBundle* bundle);

	  // returns the number of BinaryBundles currently in the out queue
	  virtual unsigned getOutQueueSize();


	protected:

	  virtual void sendMessage();

    // threaded loop
	  virtual void run ();

    boost::mutex  _currentStateDataMutex;
	  BinaryBundle* _currentStateData;

};


} // namespace gloost


#endif // H_GLOOST_ZEROMQSTATESERVER


