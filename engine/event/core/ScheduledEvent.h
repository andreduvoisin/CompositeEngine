#ifndef _CE_SCHEDULED_EVENT_H_
#define _CE_SCHEDULED_EVENT_H_

struct Event;

struct ScheduledEvent
{
	Event* event;
	uint64_t deliveryTicks;
};

inline bool operator>(const ScheduledEvent& lhs, const ScheduledEvent& rhs)
{
	return lhs.deliveryTicks > rhs.deliveryTicks;
}

#endif // _CE_SCHEDULED_EVENT_H_
