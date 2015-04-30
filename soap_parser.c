
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "soap/soapH.h"
#include "soap/ns1.nsmap"
#include "cwmp/define.h"

#include "log/uulog.h"
#define PARSER_TAG "soap-parser"

struct soap_parser {
    char tag[LOG_TAG_SIZE];
    struct soap *soap;

    struct SOAP_ENV__Header *soap_header;
    struct SOAP_ENV__Fault *soap_fault;
};

static void _soap_header_destroy(void *header)
{
    struct SOAP_ENV__Header *hdr = (struct SOAP_ENV__Header *)header;
    if (hdr == NULL) {
        return;
    }
    free(hdr);
}

static void *_soap_header_create(int id, int hold_requests)
{
    int size, offset;
    char *buf;
    struct SOAP_ENV__Header *hdr = NULL;

    size = sizeof(struct SOAP_ENV__Header)
           + sizeof(struct _ns1__ID) + 128
           + sizeof(struct _ns1__HoldRequests);
    buf = malloc(size);
    if (buf == NULL) {
        LOGE(PARSER_TAG, "out of memory");
        return NULL;
    }
    memset(buf, 0, size);

    offset = 0;
    hdr = (struct SOAP_ENV__Header *)(buf + offset);
    offset += sizeof(struct SOAP_ENV__Header);
    hdr->__ID = (struct _ns1__ID *)(buf + offset);
    offset += sizeof(struct _ns1__ID);
    hdr->__ID->__item = buf + offset;
    offset += 128;
    hdr->__HoldRequests = (struct _ns1__HoldRequests *)(buf + offset);
    offset += sizeof(struct _ns1__HoldRequests);

    if (id != 0) {
        snprintf(hdr->__ID->__item, 128, "%d", id);
        hdr->__ID->SOAP_ENV__mustUnderstand = "1";
    } else {
        hdr->__ID = NULL;
    }

    if (hold_requests != 0) {
        hdr->__HoldRequests->__item = 1;
        hdr->__HoldRequests->SOAP_ENV__mustUnderstand = "1";
    } else {
        hdr->__HoldRequests->__item = 0;
        hdr->__HoldRequests->SOAP_ENV__mustUnderstand = "1";
    }

    return (void *)hdr;
}

static void _soap_fault_free(struct SOAP_ENV__Fault *soap_fault)
{
    if (soap_fault == NULL) {
        return;
    }

    free(soap_fault);
}

static void *_soap_fault_create(struct _ns1__Fault *detail)
{
    struct SOAP_ENV__Fault *soap_fault = NULL;

    char *buf;
    int size;
    int offset;

    size = sizeof(struct SOAP_ENV__Fault)
           + sizeof(struct SOAP_ENV__Detail);
    buf = malloc(size);
    if (buf == NULL) {
        LOGE(PARSER_TAG, "out of memory");
        return NULL;
    }
    memset(buf, 0, size);

    offset = 0;
    soap_fault = (struct SOAP_ENV__Fault *)(buf + offset);
    offset += sizeof(struct SOAP_ENV__Fault);
    soap_fault->detail = (struct SOAP_ENV__Detail *)(buf + offset);
    offset += sizeof(struct SOAP_ENV__Detail);

    soap_fault->detail->__any = (char *)detail;
    soap_fault->faultcode = SOAP_FAULT_CODE_SERVER;
    soap_fault->faultstring = SOAP_FAULT_STRING_CWMP;

    return (void *)soap_fault;
}

static char *slurpfd(int fd)
{
    const int bytes_at_a_time = 2;
    char *read_buffer = NULL;
    int buffer_size = 0;
    int buffer_offset = 0;
    int chars_io;

    while (1) {
        if (buffer_offset + bytes_at_a_time > buffer_size) {
            buffer_size = bytes_at_a_time + buffer_size * 2;
            read_buffer = (char *) realloc(read_buffer, buffer_size);
            if (!read_buffer) {
                perror("memory");
                exit(EXIT_FAILURE);
            }
        }

        chars_io = read(fd, read_buffer + buffer_offset, bytes_at_a_time);
        if (chars_io <= 0) {
            break;
        }

        buffer_offset += chars_io;
    }
    read_buffer[buffer_offset] = '\0';

    if (chars_io < 0) {
        perror("read");
        exit(EXIT_FAILURE);
    }

    close(fd);
    /*LOGD(PARSER_TAG, read_buffer);*/
    return read_buffer; /* caller gets to free it */
}

/**
 * xml -> body
 * @param[in] handle: parser
 * @param[in] xml: the xml to be decoded
 * @param[out] id: cpe's id? I don't know
 * @param[out] hold_request: hold
 * @param[out] type: SOAP/RPC type
 * @param[out] body: the result get from xml
 */
int soap_parser_decode(void *handle, const char *xml,
                       int *id, int *hold_requests, int *type, void **body)
{
    struct soap_parser *parser = (struct soap_parser *)handle;
    struct soap *soap = parser->soap;
    int fd[2];

    if (xml == NULL) {
        return -1;
    }

    /*LOGD(parser->tag, xml);*/

    pipe(fd);

    *id = 0;
    *hold_requests = 0;
    *type = 0;
    *body = NULL;

    soap_init(soap);
    soap->socket = -1;
    soap->recvfd = fd[0];
    write(fd[1], xml, strlen(xml));
    close(fd[1]);

    if (soap_begin_recv(soap)
        || soap_envelope_begin_in(soap)
        || soap_recv_header(soap)
        || soap_body_begin_in(soap)) {
        return -1;
    }

    if (soap->header != NULL) {
        if (soap->header->__HoldRequests != NULL) {
            *hold_requests = soap->header->__HoldRequests->__item;
        }
        if (soap->header->__ID != NULL) {
            if (soap->header->__ID->__item != NULL) {
                *id = atoi(soap->header->__ID->__item);
            }
        }
    }

    if (soap_recv_fault(soap, 1)) {
        LOGE(parser->tag, "FaultCode=%s, FaultString=%s",
             soap->fault->faultcode, soap->fault->faultstring);
        *type = SOAP_TYPE__ns1__Fault;
        if (soap->fault != NULL && soap->fault->detail != NULL) {
            *body = soap->fault->detail->__any;
        }
        return 0;
    }

    *body = soap_getelement(soap, type);
    if (soap->error) {
        return -1;
    }

    if (soap_body_end_in(soap)
        || soap_envelope_end_in(soap)
        || soap_end_recv(soap)) {
        return -1;
    }
    ////soap_end(soap);
    //soap_free(soap);
    close(fd[0]);

    if (soap->header != NULL) {
        if (soap->header->__ID != NULL) {
            LOGV(parser->tag, "soap header: ID=%s(%s)",
                 soap->header->__ID->__item,
                 soap->header->__ID->SOAP_ENV__mustUnderstand);
        }
        if (soap->header->__HoldRequests != NULL) {
            LOGV(parser->tag, "soap header: HoldRequests=%s(%s)",
                 soap->header->__HoldRequests->__item,
                 soap->header->__HoldRequests->SOAP_ENV__mustUnderstand);
        }
    }
    return 0;
}

/**
 * body -> xml
 * @param[in] handle: soap_parser
 * @param[in] id: cpe id?
 * @param[in] hold_request: hold request
 * @param[in] type: SOAP/RPC type
 * @param[in] body: a kind of struct
 * @return: xml, the string get from body
 */ 
char *soap_parser_encode(void *handle, CpeRpcMsg* msg)
{
    struct soap_parser *parser = (struct soap_parser *)handle;
    struct soap *soap = parser->soap;
    int fd[2];
    char *xml = NULL;
    int type = msg->type;

    pipe(fd);

    soap_init(soap);
    soap->socket = -1;
    soap->sendfd = fd[1];

    parser->soap_header = (struct SOAP_ENV__Header *)_soap_header_create(id, hold_requests);
    soap->header = parser->soap_header;
    if (soap->header != NULL) {
        if (soap->header->__ID != NULL) {
            LOGV(parser->tag, "soap header: ID=%s(%s)",
                 soap->header->__ID->__item,
                 soap->header->__ID->SOAP_ENV__mustUnderstand);
        }
        if (soap->header->__HoldRequests != NULL) {
            LOGV(parser->tag, "soap header: HoldRequests=%d(%s)",
                 soap->header->__HoldRequests->__item,
                 soap->header->__HoldRequests->SOAP_ENV__mustUnderstand);
        }
    }

    soap_begin_send(soap);
    soap_envelope_begin_out(soap);
    soap_putheader(soap);
    soap_body_begin_out(soap);

    switch (type) {
        case SOAP_TYPE__ns1__Fault: {
            if (parser->soap_fault != NULL) {
                _soap_fault_free(parser->soap_fault);
                parser->soap_fault = NULL;
            }
            parser->soap_fault = _soap_fault_create((struct _ns1__Fault *)body);
            if (parser->soap_fault != NULL) {
                soap_put_SOAP_ENV__Fault(soap, parser->soap_fault, NULL, NULL);
            }
            break;
        }
        case SOAP_TYPE__ns1__GetRPCMethods:
            soap_put__ns1__GetRPCMethods(soap,
                                         (struct _ns1__GetRPCMethods *)body,
                                         "ns1:GetRPCMethods", NULL);
            break;
        case SOAP_TYPE__ns1__SetParameterValues:
            soap_put__ns1__SetParameterValues(soap,
                                              (struct _ns1__SetParameterValues *)body,
                                              "ns1:SetParameterValues", NULL);
            break;
        case SOAP_TYPE__ns1__GetParameterValues:
            soap_put__ns1__GetParameterValues(soap,
                                              (struct _ns1__GetParameterValues *)body,
                                              "ns1:GetParameterValues", NULL);
            break;
        case SOAP_TYPE__ns1__GetParameterNames:
            soap_put__ns1__GetParameterNames(soap,
                                             (struct _ns1__GetParameterNames *)body,
                                             "ns1:GetParameterNames", NULL);
            break;
        case SOAP_TYPE__ns1__SetParameterAttributes:
            soap_put__ns1__SetParameterAttributes(soap,
                                                  (struct _ns1__SetParameterAttributes *)body,
                                                  "ns1:SetParameterAttributes", NULL);
            break;
        case SOAP_TYPE__ns1__GetParameterAttributes:
            soap_put__ns1__GetParameterAttributes(soap,
                                                  (struct _ns1__GetParameterAttributes *)body,
                                                  "ns1:GetParameterAttributes", NULL);
            break;
        case SOAP_TYPE__ns1__AddObject:
            soap_put__ns1__AddObject(soap,
                                     (struct _ns1__AddObject *)body,
                                     "ns1:AddObject", NULL);
            break;
        case SOAP_TYPE__ns1__DeleteObject:
            soap_put__ns1__DeleteObject(soap,
                                        (struct _ns1__DeleteObject *)body,
                                        "ns1:DeleteObject", NULL);
            break;
        case SOAP_TYPE__ns1__Reboot:
            soap_put__ns1__Reboot(soap,
                                  (struct _ns1__Reboot *)body,
                                  "ns1:Reboot", NULL);
            break;
        case SOAP_TYPE__ns1__Download:
            soap_put__ns1__Download(soap,
                                    (struct _ns1__Download *)body,
                                    "ns1:Download", NULL);
            break;
        case SOAP_TYPE__ns1__ScheduleDownload:
            soap_put__ns1__ScheduleDownload(soap,
                                            (struct _ns1__ScheduleDownload *)body,
                                            "ns1:ScheduleDownload", NULL);
            break;
        case SOAP_TYPE__ns1__Upload:
            soap_put__ns1__Upload(soap,
                                  (struct _ns1__Upload *)body,
                                  "ns1:Upload", NULL);
            break;
        case SOAP_TYPE__ns1__FactoryReset:
            soap_put__ns1__FactoryReset(soap,
                                        (struct _ns1__FactoryReset *)body,
                                        "ns1:FactoryReset", NULL);
            break;
        case SOAP_TYPE__ns1__GetQueuedTransfers:
            soap_put__ns1__GetQueuedTransfers(soap,
                                              (struct _ns1__GetQueuedTransfers *)body,
                                              "ns1:GetQueuedTransfers", NULL);
            break;
        case SOAP_TYPE__ns1__GetAllQueuedTransfers:
            soap_put__ns1__GetAllQueuedTransfers(soap,
                                                 (struct _ns1__GetAllQueuedTransfers *)body,
                                                 "ns1:GetAllQueuedTransfers", NULL);
            break;
        case SOAP_TYPE__ns1__CancelTransfer:
            soap_put__ns1__CancelTransfer(soap,
                                          (struct _ns1__CancelTransfer *)body,
                                          "ns1:CancelTransfer", NULL);
            break;
        case SOAP_TYPE__ns1__ScheduleInform:
            soap_put__ns1__ScheduleInform(soap,
                                          (struct _ns1__ScheduleInform *)body,
                                          "ns1:ScheduleInform", NULL);
            break;
        case SOAP_TYPE__ns1__ChangeDUState:
            soap_put__ns1__ChangeDUState(soap,
                                         (struct _ns1__ChangeDUState *)body,
                                         "ns1:ChangeDUState", NULL);
            break;
        case SOAP_TYPE__ns1__GetRPCMethodsResponse:
            soap_put__ns1__GetRPCMethodsResponse(soap,
                                                 (struct _ns1__GetRPCMethodsResponse *)body,
                                                 "ns1:GetRPCMethodsResponse", NULL);
            break;
        case SOAP_TYPE__ns1__InformResponse:
            soap_put__ns1__InformResponse(soap,
                                          (struct _ns1__InformResponse *)body,
                                          "ns1:InformResponse", NULL);
            break;
        case SOAP_TYPE__ns1__TransferCompleteResponse:
            soap_put__ns1__TransferCompleteResponse(soap,
                                                    (struct _ns1__TransferCompleteResponse *)body,
                                                    "ns1:TransferCompleteResponse", NULL);
            break;
        case SOAP_TYPE__ns1__AutonomousTransferCompleteResponse:
            soap_put__ns1__AutonomousTransferCompleteResponse(soap,
                                                              (struct _ns1__AutonomousTransferCompleteResponse *)body,
                                                              "ns1:AutonomousTransferCompleteResponse", NULL);
            break;
        case SOAP_TYPE__ns1__DUStateChangeCompleteResponse:
            soap_put__ns1__DUStateChangeCompleteResponse(soap,
                                                         (struct _ns1__DUStateChangeCompleteResponse *)body,
                                                         "ns1:DUStateChangeCompleteResponse", NULL);
            break;
        case SOAP_TYPE__ns1__AutonomousDUStateChangeCompleteResponse:
            soap_put__ns1__AutonomousDUStateChangeCompleteResponse(soap,
                                                                   (struct _ns1__AutonomousDUStateChangeCompleteResponse *)body,
                                                                   "ns1:AutonomousDUStateChangeCompleteResponse", NULL);
            break;
        case SOAP_TYPE__ns1__RequestDownloadResponse:
            soap_put__ns1__RequestDownloadResponse(soap,
                                                   (struct _ns1__RequestDownloadResponse *)body,
                                                   "ns1:RequestDownloadResponse", NULL);
            break;
        default :
            LOGE(PARSER_TAG, "unknow SOAP_TYPE, %d", type);
            break;
    }

    soap_body_end_out(soap);
    soap_envelope_end_out(soap);
    soap_end_send(soap);
    /*soap_end(soap);*/
    close(fd[1]);

    xml = slurpfd(fd[0]);
    //soap_free(soap);
    return xml;
}

void soap_parser_destroy(void *handle)
{
    struct soap_parser *parser = (struct soap_parser *)handle;
    soap_end(parser->soap);
    soap_free(parser->soap);
    LOGI(parser->tag, "destroy");
    if (parser->soap_header != NULL) {
        _soap_header_destroy(parser->soap_header);
        parser->soap_header = NULL;
    }
    if (parser->soap_fault != NULL) {
        _soap_fault_free(parser->soap_fault);
        parser->soap_fault = NULL;
    }
    free(parser);
}

void *soap_parser_create()
{
    struct soap_parser *parser;

    parser = (struct soap_parser *)malloc(sizeof(struct soap_parser));
    if (NULL == parser) {
        return NULL;
    }
    memset(parser, 0, sizeof(struct soap_parser));

    snprintf(parser->tag, sizeof(parser->tag), "%s", PARSER_TAG);
    LOGI(parser->tag, "create");

    parser->soap = soap_new();
    if (NULL == parser->soap) {
        free(parser);
        return NULL;
    }

    return (void *)(parser);
}
