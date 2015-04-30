#ifndef __BASH_H
#define __BASH_H

/* the same as struct 'CARpcMsg' in omsacs's code */
typedef struct {
    unsigned int id; /* i don't know what it is */
    unsigned int type; /* rpc type */
    int holdrequests; /* 1 - holdrequests, 0 - none */
    void *body; /* struct of msg, depends on rpc's type */
} CpeRpcMsg;

typedef struct {
    char url[512];
} ACS;

#endif
