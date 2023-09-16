// ArchiveImage.c : Defines the functions for the static library.
//

#pragma comment(lib, "normaliz.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "wldap32.lib")
#pragma comment(lib, "crypt32.lib")
#pragma comment(lib, "advapi32.lib")

#include <math.h>

#define CURL_STATICLIB
#include "curl/curl.h"

#include "EncodeImage.h"

struct memory
{
    char* data;
    size_t size;
};

static size_t cb(void* data, size_t size, size_t nmemb, void* clientp)
{
    size_t realsize = size * nmemb;
    struct memory* mem = (struct memory*)clientp;

    char* ptr = realloc(mem->data, mem->size + realsize + 1);
    
    if (ptr == NULL)
    {
        return 0;  /* out of memory! */
    }

    mem->data = ptr;
    memcpy(&(mem->data[mem->size]), data, realsize);
    mem->size += realsize;
    mem->data[mem->size] = 0;

    return realsize;
}

void Base64Encode(const char* source, size_t srclen, char** enc)
{
    const char Base64Table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    unsigned char* src = (unsigned char*)source;
    unsigned char* dst = (unsigned char*)*enc;

    size_t i;
    
    for (i = 0; i < srclen - 2; i += 3)
    {
        unsigned index0 = src[0] >> 2;
        unsigned index1 = ((src[0] & 0b00000011) << 4) | (src[1] >> 4);
        unsigned index2 = ((src[1] & 0b00001111) << 2) | (src[2] >> 6);
        unsigned index3 = src[2] & 0b00111111;

        dst[0] = Base64Table[index0];
        dst[1] = Base64Table[index1];
        dst[2] = Base64Table[index2];
        dst[3] = Base64Table[index3];
        src += 3;
        dst += 4;
    }

    // Handle the case when the input length is not a multiple of 3.
    switch(srclen % 3)
    {
        case 2:
        {
            unsigned index0 = src[0] >> 2;
            unsigned index1 = ((src[0] & 0b00000011) << 4) | (src[1] >> 4);
            unsigned index2 = ((src[1] & 0b00001111) << 2);
            dst[0] = Base64Table[index0];
            dst[1] = Base64Table[index1];
            dst[2] = Base64Table[index2];
            dst[3] = '=';
            break;
        }
        case 1:
        {
            unsigned index0 = src[0] >> 2;
            unsigned index1 = ((src[0] & 0b00000011) << 4);
            dst[0] = Base64Table[index0];
            dst[1] = Base64Table[index1];
            dst[2] = '=';
            dst[3] = '=';
            break;
        }
        default:
        {
            break;
        }
    }
}

int EncodeImage(const char* URL, char* Base64Image, size_t* Base64ImageSize)
{
    curl_global_init(CURL_GLOBAL_ALL);

    struct memory chunk = { 0 };
    CURLcode res;
    CURL* curl_handle = curl_easy_init();

    if (curl_handle)
    {
        curl_easy_setopt(curl_handle, CURLOPT_URL, URL);

        /* send all data to this function  */
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, cb);

        /* we pass our 'chunk' struct to the callback function */
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void*)&chunk);

        /* send a request */
        res = curl_easy_perform(curl_handle);

        /* if the buffer is NULL ... */
        if (Base64Image == NULL)
        {
            /* return the required size */
            *Base64ImageSize = (size_t)(ceil((double)chunk.size / 3)) * 4;
        }
        else
        {
            Base64Encode(chunk.data, chunk.size, &Base64Image);
        }

        free(chunk.data);

        curl_easy_cleanup(curl_handle);
    }

    return 0;
}
