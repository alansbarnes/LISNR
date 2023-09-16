The following API is exposed by the static library EncodeImage.lib.

int EncodeImage(const char* URL, char *Base64Image, size_t *Base64ImageSize);

EncodeImage.lib leverages the libcurl v7.83.1 static libraries which have been prebuilt and can be found together with the applicable headers in EncodeImage\curl. All project files were built with Visual Studio 2022 Community Edition. Usage for ArchiveImage.exe is as follows:

ArchiveImage <Image URL> <Encoded archive>
