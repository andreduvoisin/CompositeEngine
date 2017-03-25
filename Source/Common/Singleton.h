#ifndef _CE_SINGLETON_H_
#define _CE_SINGLETON_H_

namespace CE
{
	template <typename T>
	class Singleton
	{
	public:
		static T& Get()
		{
			static T instance;
			return instance;
		}

	protected:
		Singleton() { }

	private:
		Singleton(const Singleton<T>&) = delete;
		Singleton(Singleton<T>&&) = delete;
		Singleton<T>& operator=(const Singleton<T>&) = delete;
		Singleton<T>& operator=(Singleton<T>&&) = delete;
	};
}

#endif // _CE_SINGLETON_H_