
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



#ifndef H_GLOOST_ZEROMQNODE
#define H_GLOOST_ZEROMQNODE

// gloost system includes
#include <gloost/gloostConfig.h>
#include <gloost/gloostMath.h>
#include <gloost/BinaryBundle.h>


// cpp includes
#include <string>
#include <zmq.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>


namespace gloost
{


  //  ZeroMqNode

class ZeroMqNode
{
	public:

    // class constructor
    ZeroMqNode(unsigned contextId = 1,
               unsigned mode      = ZMQ_REQ);

    // class destructor
	  virtual ~ZeroMqNode();


	  // connects the client to a server
	  virtual void connect(const std::string& address, const std::string& port) = 0;


	  // starts the thread
	  virtual void start();

	  // stops the thread
	  virtual void stop();


	  // returns true if client is running
	  bool isRunning() const;



	  // queues a BinaryBundle for sending
	  virtual void queue(BinaryBundle::shared_ptr bundle);

	  // returns a BinaryBundle from the recieved bundles and removes it from the buffer
	  virtual BinaryBundle::shared_ptr getRecievedBundle();

	  // returns the last BinaryBundle received  and removes it from the buffer
	  virtual BinaryBundle::shared_ptr getLastRecievedBundle();


	  // returns the number of BinaryBundles currently in the in queue
	  virtual unsigned getInQueueSize();

	  // returns the number of BinaryBundles currently in the out queue
	  virtual unsigned getOutQueueSize();


	  // cleares the queue with the received Messages
	  virtual void clearReceivedMessges();


	protected:

	  virtual void receiveMessage();
	  virtual void sendMessage();



    // threaded loop
	  virtual void run () = 0;

	  zmq::context_t* _context;
	  unsigned        _contextId;
    zmq::socket_t*  _socket;
    unsigned        _mode;

    bool            _running;

    boost::mutex                        _outQueueMutex;
    std::list<BinaryBundle::shared_ptr> _outQueue;

    boost::mutex                        _inQueueMutex;
    std::list<BinaryBundle::shared_ptr> _inQueue;



};


} // namespace gloost


#endif // H_GLOOST_ZEROMQNODE


