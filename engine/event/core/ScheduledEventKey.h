#ifndef _CE_SCHEDULED_EVENT_KEY_H_
#define _CE_SCHEDULED_EVENT_KEY_H_

struct ScheduledEventKey
{
	uint64_t deliveryTicks;
	EventType type;
};

inline bool operator<(const ScheduledEventKey& lhs, const ScheduledEventKey& rhs)
{
	if (lhs.deliveryTicks < rhs.deliveryTicks)
	{
		return true;
	}

	if (lhs.deliveryTicks > rhs.deliveryTicks)
	{
		return false;
	}

	return lhs.type < rhs.type;
}

#endif // _CE_SCHEDULED_EVENT_KEY_H_
