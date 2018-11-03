#ifndef _CE_SET_ANIMATION_TIME_MESSAGE_H_
#define _CE_SET_ANIMATION_TIME_MESSAGE_H_

#include "UIMessage.h"
#include "JsonDeserializer.h"

class SetAnimationTimeRequest : public UIMessageRequest
{
public:
	SetAnimationTimeRequest();

	float GetTime() const { return time; }

protected:
	void DeserializeInternal(const JsonDeserializer& deserializer) override;

private:
	float time;
};

#endif // _CE_SET_ANIMATION_TIME_MESSAGE_H_
