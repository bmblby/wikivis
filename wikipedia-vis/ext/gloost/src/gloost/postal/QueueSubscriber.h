
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



#ifndef H_GLOOST_MESSAGE_QUEUESUBSCRIBER
#define H_GLOOST_MESSAGE_QUEUESUBSCRIBER


// gloost includes
#include <gloost/postal/Subscriber.h>


// cpp includes
#include <string>
#include <memory>
#include <boost/thread/mutex.hpp>
#include <queue>



namespace gloost
{
namespace postal
{


  //  QueueSubscriber

class QueueSubscriber : public Subscriber
{
	public:

    /// a std::shared_ptr of an QueueSubscriber instance
	  typedef std::shared_ptr<QueueSubscriber>       shared_ptr;
    typedef std::shared_ptr<const QueueSubscriber> const_shared_ptr;


    // creates a QueueSubscriber instance
    static shared_ptr create(size_t queueSize = 0);
    static shared_ptr create(const std::string& name, size_t queueCapacity = 0);


    // class destructor
	  virtual ~QueueSubscriber();


	  // receive a message
	  virtual bool receive(const std::string& path, Message::shared_ptr message);


	  // pops and returns the next message from queue or returns a nullptr
    Message::shared_ptr popMessage();

	  // returns the last message from queue or returns a nullptr
    Message::shared_ptr getLatestMessage();

    // returns the number of messages in the queue
    size_t getQueueSize() const;

    // returns the max number of messages that fit in the queue
    size_t getQueueCapacity() const;

	  // clears the message queue
    void clearQueue();




	protected:

    // class constructor
    QueueSubscriber(size_t queueCapacity);
    QueueSubscriber(const std::string& name, size_t queueCapacity);



	private:

    size_t                          _queueCapacity;
	  std::queue<Message::shared_ptr> _messageQueue;
    boost::mutex                    _mutex_messageQueue;


};


} // namespace postal

} // namespace gloost


#endif // H_GLOOST_MESSAGE_QUEUESUBSCRIBER












