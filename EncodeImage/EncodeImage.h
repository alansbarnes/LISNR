#ifndef ENCODEIMAGE_H
#define ENCODEIMAGE_H

#ifdef __cplusplus
extern "C" {
#endif
    int EncodeImage(const char* URL, char *Base64Image, size_t *Base64ImageSize);
#ifdef __cplusplus
}
#endif

#endif // ENCODEIMAGE_H