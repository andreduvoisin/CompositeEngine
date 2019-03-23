#include "InputFileStream.h"

#ifdef __APPLE__
#include "CoreFoundation/CoreFoundation.h"
#endif

namespace CE
{

	InputFileStream::InputFileStream(const char *file)
	{
		std::string fileString(file);

		std::string directoryName;
		std::string fileName;
		std::string fileTitle;
		std::string fileExtension;

		size_t slashPos = fileString.find_last_of('/');
		if (slashPos == std::string::npos)
		{
			directoryName = std::string();
			fileName = fileString;
		}
		else
		{
			directoryName = fileString.substr(0, slashPos);
			fileName = fileString.substr(slashPos + 1);
		}
		size_t dotPos = fileName.find_last_of('.');
		fileTitle = fileName.substr(0, dotPos);
		fileExtension = fileName.substr(dotPos + 1);

		CFStringRef fileTitleStringRef = CFStringCreateWithCStringNoCopy(NULL, fileTitle.c_str(), kCFStringEncodingASCII, kCFAllocatorNull);
		CFStringRef fileExtensionStringRef = CFStringCreateWithCStringNoCopy(NULL, fileExtension.c_str(), kCFStringEncodingASCII, kCFAllocatorNull);
		CFStringRef directoryNameStringRef = directoryName.empty() ? NULL : CFStringCreateWithCStringNoCopy(NULL, directoryName.c_str(), kCFStringEncodingASCII, kCFAllocatorNull);

		CFBundleRef mainBundle = CFBundleGetMainBundle();
		CFURLRef fileUrl = CFBundleCopyResourceURL(
			mainBundle,
			fileTitleStringRef,
			fileExtensionStringRef,
			directoryNameStringRef);

		UInt8 realFileName[1024];
		CFURLGetFileSystemRepresentation(fileUrl, true, realFileName, 1024);

		stream.open((char *)realFileName, std::ios::in | std::ios::binary);
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
