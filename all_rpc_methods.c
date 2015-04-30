#include <stdio.h>

#include "log/uulog.h"
#include "base.h"

/**
 * step:
 * 1. encode into xml from struct(many ways);
 * 2. send xml;
 *  2.1. establish connection;
 * 3. recv xml;
 *  3.1. close connection;
 * 4. decode into struct from xml(by using gsoap API).
 * @param[in] acs_url: such as 'http://ip<:port>/cgi'
 * @param[in] send: the struct that send to asc
 * @param[out] recv: the struct that recv from acs
 * @return: 0 - seccess; -1 - failed.
 */
static int _struct_comm_with_acs(char *acs_url, CpeRpcMsg* send, CpeRpcMsg **recv)
{
    char *xml_send = NULL, *xml_recv = NULL;
    handle = soap_create();

    /* step 1. encode into xml */
    xml_send = soap_parser_encode(handle, send);
    /* step 2&3. send & recv xml */
    http_comm_with_acs(acs_url, xml_send, &xml_recv);
    /* step 4. decode string 'xml_recv' into struct 'recv' */
    soap_parser_decode(handle, xml_recv, recv);

    free(xml_send);
    free(xml_recv);
}

/**
 * launch a inform request to acs
 * @param[in] url: acs's url
 * @return: struct that received from acs
 */
CpeRpcMsg *rpc_inform_req(char *url)
{
    CpeRpcMsg *send, *recv;

    /* TODO: create_msg() - get device's events */
    send = create_msg();
    _struct_comm_with_acs(url, send, &recv);
    return recv;
}

int rpc_set_param_values_rsp(ACS *acs, CARpcMsg* send, CARpcMsg **recv)
{
    
}
