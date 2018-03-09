#ifndef _CE_OUTPUT_FILE_STREAM_H_
#define _CE_OUTPUT_FILE_STREAM_H_

#include <fstream>

namespace CE
{
	class OutputFileStream
	{
	public:
		OutputFileStream(const char* fileName);
		~OutputFileStream();

		bool IsValid();

		template<typename T>
		void Write(T data);

		template<typename T>
		void Write(T* data, size_t count);

	private:
		std::ofstream stream;
	};

	template <typename T>
	void OutputFileStream::Write(T data)
	{
		stream.write(reinterpret_cast<const char*>(&data), sizeof(T));
	}

	template <typename T>
	void OutputFileStream::Write(T* data, size_t count)
	{
		stream.write(reinterpret_cast<const char*>(data), sizeof(T) * count);
	}

	template <typename T>
	OutputFileStream& operator<<(OutputFileStream& stream, T data)
	{
		stream.Write(data);
		return stream;
	}
}

#endif // _CE_OUTPUT_FILE_STREAM_H_