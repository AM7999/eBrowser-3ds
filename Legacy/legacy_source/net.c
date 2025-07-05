#include "../include/includes.h"

bool downloadFile(char *url, char *filename) {
    CURL *curl_handle;
    char *fileOut = filename;
    FILE *pagefile;

    curl_global_init(CURL_GLOBAL_ALL);

    curl_handle = curl_easy_init();
 
    curl_easy_setopt(curl_handle, CURLOPT_URL, url);
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "eBrowser for 3ds, By AM7999");
    curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);

    pagefile = fopen(fileOut, "wb");
    if(pagefile) {
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, pagefile);
        curl_easy_perform(curl_handle);
        fclose(pagefile);
    } else if(!pagefile) {
        return false;
    }
 
    curl_easy_cleanup(curl_handle);
 
    curl_global_cleanup();

    return true;
}