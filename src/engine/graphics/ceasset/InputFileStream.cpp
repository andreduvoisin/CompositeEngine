#include "InputFileStream.h"

namespace CE
{
	InputFileStream::InputFileStream(const char* fileName)
	{
		stream.open(fileName, std::ios::in | std::ios::binary);
	}

	InputFileStream::~InputFileStream()
	{
		stream.close();
	}

	bool InputFileStream::IsValid()
	{
		return stream.is_open() && !stream.fail();
	}

	bool InputFileStream::HasData()
	{
		return stream.peek() != EOF;
	}
}
