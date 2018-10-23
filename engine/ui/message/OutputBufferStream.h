#ifndef _CE_OUTPUT_BUFFER_STREAM_H_
#define _CE_OUTPUT_BUFFER_STREAM_H_

#include <sstream>

class OutputBufferStream
{
public:
	template<typename T>
	void Write(const T& data);

	template<typename T>
	void Write(const T* data, size_t count);

	std::string ToString();

private:
	std::ostringstream stream;
};

template<typename T>
void OutputBufferStream::Write(const T& data)
{
	stream.write(reinterpret_cast<const char*>(&data), sizeof(T));
}

template<typename T>
void OutputBufferStream::Write(const T* data, size_t count)
{
	stream.write(reinterpret_cast<const char*>(data), sizeof(T) * count);
}

template<typename T>
OutputBufferStream& operator<<(OutputBufferStream& stream, const T& data)
{
	stream.Write(data);
	return stream;
}

#endif // _CE_OUTPUT_BUFFER_STREAM_H_
