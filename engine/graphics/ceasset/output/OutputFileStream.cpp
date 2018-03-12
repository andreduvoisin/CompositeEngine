#include "OutputFileStream.h"

namespace CE
{
	OutputFileStream::OutputFileStream(const char* fileName)
	{
		stream.open(fileName, std::ios::out | std::ios::binary);
	}

	OutputFileStream::~OutputFileStream()
	{
		stream.close();
	}

	bool OutputFileStream::IsValid()
	{
		return stream.is_open() && !stream.fail();
	}
}
