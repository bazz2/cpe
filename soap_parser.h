
#ifndef _CWMPACS_SOAP_PARSER_H
#define _CWMPACS_SOAP_PARSER_H

#include "soap/soapH.h"

void *soap_parser_create();
void soap_parser_destroy(void *handle);
int soap_parser_decode(void *handle, const char *xml,
                       int *id, int *hold_requests,
                       int *type, void **body);
char *soap_parser_encode(void *handle,
                         int id, int hold_requests,
                         int type, void *body);

#endif
