#ifndef _CE_APPLICATION_WIN32_H_
#define _CE_APPLICATION_WIN32_H_

#include "Common\Singleton.h"

typedef struct _iobuf FILE;

namespace CE
{
	class ApplicationWin32 : public Singleton<ApplicationWin32>
	{
	public:
		void Initialize();
		void Destroy();

#if _DEBUG
	private:
		void InitializeConsole();
		void DestroyConsole();
		FILE *m_in, *m_out, *m_err;
#endif
	};

	class Application : public ApplicationWin32 {};
}

#endif // _CE_APPLICATION_WIN32_H_