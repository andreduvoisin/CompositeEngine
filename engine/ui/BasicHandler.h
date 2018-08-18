#ifndef _CE_BASIC_HANDLER_H_
#define _CE_BASIC_HANDLER_H_

#include "include/cef_client.h"

class BasicHandler : public CefClient
{
public:
	BasicHandler() { }
	~BasicHandler() { }

	// Note that any of the IMPLEMENT_WHATEVER
	// macros that come with CEF can (and do) set
	// access modifiers, so you'll want them after
	// everything else in your class or you may be
	// in for a surprise when the access of a member
	// isn't what you expect it to be!
	IMPLEMENT_REFCOUNTING(BasicHandler);
};

#endif //_CE_BASIC_HANDLER_H_