#ifndef _CE_INPUT_BUFFER_STREAM_H_
#define _CE_INPUT_BUFFER_STREAM_H_

#include <sstream>

class InputBufferStream
{
public:
	InputBufferStream(const std::string& buffer);

	template<typename T>
	T Read();

	template<typename T>
	void Read(T& data);
	template<>
	void Read(std::string& data);

	template<typename T>
	void Read(T* data, size_t count);

private:
	std::istringstream stream;
};

template<typename T>
T InputBufferStream::Read()
{
	T data;
	Read(data);
	return data;
}

template<typename T>
void InputBufferStream::Read(T& data)
{
	stream.read(reinterpret_cast<char*>(&data), sizeof(T));
}

template<>
void InputBufferStream::Read(std::string& data)
{
	std::getline(stream, data, '\0');
}

template<typename T>
void InputBufferStream::Read(T* data, size_t count)
{
	stream.read(reinterpret_cast<char*>(data), sizeof(T) * count);
}

template<typename T>
InputBufferStream& operator>>(InputBufferStream& stream, T& data)
{
	stream.Read(data);
	return stream;
}

#endif // _CE_INPUT_BUFFER_STREAM_H_
