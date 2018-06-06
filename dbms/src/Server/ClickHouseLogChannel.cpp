#include "ClickHouseLogChannel.h"

#include <Common/CurrentThread.h>
#include <Core/SystemLogsQueue.h>

#include <Poco/Message.h>
#include <Poco/Ext/ThreadNumber.h>

#include <sys/time.h>
#include <iostream>


namespace DB
{


void ClickHouseLogChannel::log(const Poco::Message & msg)
{
    if (auto logs_queue = CurrentThread::getSystemLogsQueue())
    {
        std::cerr << "Logging to client: " << msg.getText() << "\n";
        MutableColumns columns = SystemLogsQueue::getSampleColumns();

        ::timeval tv;
        if (0 != gettimeofday(&tv, nullptr))
            DB::throwFromErrno("Cannot gettimeofday");

        size_t i = 0;
        columns[i++]->insert(static_cast<UInt64>(tv.tv_sec));
        columns[i++]->insert(static_cast<UInt64>(tv.tv_usec));
        columns[i++]->insert(static_cast<UInt64>(Poco::ThreadNumber::get()));
        columns[i++]->insert(static_cast<Int64>(msg.getPriority()));
        columns[i++]->insert(msg.getSource());
        columns[i++]->insert(msg.getText());

        logs_queue->emplace(std::move(columns));
    }
}


}
