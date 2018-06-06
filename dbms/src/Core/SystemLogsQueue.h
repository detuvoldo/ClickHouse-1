#pragma once
#include <Common/ConcurrentBoundedQueue.h>
#include <Core/Block.h>


namespace DB
{

class SystemLogsQueue : public ConcurrentBoundedQueue<MutableColumns>
{
public:
    SystemLogsQueue();

    static Block getSampleBlock();
    static MutableColumns getSampleColumns();

    /// Converts priority from Poco::Message::Priority to a string
    static const char * getProrityName(int priority);
};

using SystemLogsQueuePtr = std::shared_ptr<SystemLogsQueue>;

}



