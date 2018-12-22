#ifndef _CE_INPUT_FILE_STREAM_H_
#define _CE_INPUT_FILE_STREAM_H_

#include <fstream>
#include <string>

namespace CE
{
	class InputFileStream
	{
	public:
		InputFileStream(const char* fileName);
		~InputFileStream();

		bool IsValid();
		bool HasData();

		template<typename T>
		T Read();

		template<typename T>
		void Read(T& data);

		template<typename T>
		void Read(T* data, size_t count);

	private:
		std::ifstream stream;
	};

	template<typename T>
	T InputFileStream::Read()
	{
		T data;
		Read(data);
		return data;
	}

	template<typename T>
	void InputFileStream::Read(T& data)
	{
		stream.read(reinterpret_cast<char*>(&data), sizeof(T));
	}

	template<>
	inline void InputFileStream::Read(std::string& data)
	{
		std::getline(stream, data, '\0');
	}

	template<typename T>
	void InputFileStream::Read(T* data, size_t count)
	{
		stream.read(reinterpret_cast<char*>(data), sizeof(T) * count);
	}

	template<typename T>
	InputFileStream& operator>>(InputFileStream& stream, T& data)
	{
		stream.Read(data);
		return stream;
	}
}

#endif // _CE_INPUT_FILE_STREAM_H_
