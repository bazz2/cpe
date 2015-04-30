#ifndef _CWMPACS_UTILS_CURL_EASY_H_
#define _CWMPACS_UTILS_CURL_EASY_H_

char *curl_easy_download_file(char *url, char *userpwd);
double curl_easy_get_file_size(char *url, char *userpwd);

#endif
