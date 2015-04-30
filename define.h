#ifndef _CWMPACS_CWMP_DEFINE_H_
#define _CWMPACS_CWMP_DEFINE_H_

#define CWMP_VERSION_1_1    0x00010001
#define CWMP_VERSION_1_2    0x00010002

#define CWMP_MAX_ENVELOPES  1

#define CWMP_CPE_UNIQUE_MODE_OUI_SERIAL         0
#define CWMP_CPE_UNIQUE_MODE_OUI_PRODUCT_SERIAL 1

#define MAX_CPE_OUI_SIZE        32
#define MAX_CPE_PRODCLASS_SIZE  64
#define MAX_CPE_SERIALNUM_SIZE  64
#define MAX_CPE_MANUFACTURER_SIZE  64

typedef struct _CACpe {
    char tag[32];
    int id;

    char oui[MAX_CPE_OUI_SIZE];
    char serial_number[MAX_CPE_SERIALNUM_SIZE];
    char manufacturer[MAX_CPE_MANUFACTURER_SIZE];
    char product_class[MAX_CPE_PRODCLASS_SIZE];

    int unique_mode;
} CACpe;

#define CPE_FMT  "cpe(%s-%s) "
#define CPE_PARAMS(cpe) (cpe)->oui, (cpe)->serial_number

typedef enum {
    CWMP_MSG_NONE = 0,
    CWMP_MSG_FAULT,

    CWMP_MSG_GET_RPCMETHODS_REQ,
    CWMP_MSG_GET_RPCMETHODS_RSP,

    CWMP_MSG_EMPTY_REQ,
    CWMP_MSG_EMPTY_RSP,
    CWMP_MSG_INFROM_REQ,
    CWMP_MSG_INFROM_RSP,
    CWMP_MSG_TRANSFER_COMPLETE_REQ,
    CWMP_MSG_TRANSFER_COMPLETE_RSP,
    CWMP_MSG_AUTONOMOUS_TRANSFER_COMPLETE_REQ,
    CWMP_MSG_AUTONOMOUS_TRANSFER_COMPLETE_RSP,
    CWMP_MSG_DUSTATE_CHANGE_COMPLETE_REQ,
    CWMP_MSG_DUSTATE_CHANGE_COMPLETE_RSP,
    CWMP_MSG_AUTONOMOUS_DUSTATE_CHANGE_COMPLETE_REQ,
    CWMP_MSG_AUTONOMOUS_DUSTATE_CHANGE_COMPLETE_RSP,
    CWMP_MSG_REQUEST_DOWNLOAD_REQ,
    CWMP_MSG_REQUEST_DOWNLOAD_RSP,
    CWMP_MSG_KICKED_REQ,
    CWMP_MSG_KICKED_RSP,

    CWMP_MSG_SET_PARAM_VALUES_REQ,
    CWMP_MSG_SET_PARAM_VALUES_RSP,
    CWMP_MSG_GET_PARAM_VALUES_REQ,
    CWMP_MSG_GET_PARAM_VALUES_RSP,
    CWMP_MSG_GET_PARAM_NAMES_REQ,
    CWMP_MSG_GET_PARAM_NAMES_RSP,
    CWMP_MSG_SET_PARAM_ATTRIBUTES_REQ,
    CWMP_MSG_SET_PARAM_ATTRIBUTES_RSP,
    CWMP_MSG_GET_PARAM_ATTRIBUTES_REQ,
    CWMP_MSG_GET_PARAM_ATTRIBUTES_RSP,
    CWMP_MSG_ADD_OBJECT_REQ,
    CWMP_MSG_ADD_OBJECT_RSP,
    CWMP_MSG_DELETE_OBJECT_REQ,
    CWMP_MSG_DELETE_OBJECT_RSP,
    CWMP_MSG_REBOOT_REQ,
    CWMP_MSG_REBOOT_RSP,
    CWMP_MSG_DOWNLOAD_REQ,
    CWMP_MSG_DOWNLOAD_RSP,
    CWMP_MSG_SCHEDULE_DOWNLOAD_REQ,
    CWMP_MSG_SCHEDULE_DOWNLOAD_RSP,
    CWMP_MSG_UPLOAD_REQ,
    CWMP_MSG_UPLOAD_RSP,
    CWMP_MSG_FACTORY_RESET_REQ,
    CWMP_MSG_FACTORY_RESET_RSP,
    CWMP_MSG_GET_QUEUED_TRANSFERS_REQ,
    CWMP_MSG_GET_QUEUED_TRANSFERS_RSP,
    CWMP_MSG_GET_ALL_QUEUED_TRANSFERS_REQ,
    CWMP_MSG_GET_ALL_QUEUED_TRANSFERS_RSP,
    CWMP_MSG_CANCEL_TRANSFERS_REQ,
    CWMP_MSG_CANCEL_TRANSFERS_RSP,
    CWMP_MSG_SCHEDULE_INFORM_REQ,
    CWMP_MSG_SCHEDULE_INFORM_RSP,
    CWMP_MSG_CHANGE_DUSTATE_REQ,
    CWMP_MSG_CHANGE_DUSTATE_RSP,

    CWMP_MSG_MAX,
} CACwmpMsgType;

static char _CWMP_MSG_TYPE_STRING[CWMP_MSG_MAX][100] = {
    "none",
    "Fault",

    "GetRpcMethods",
    "GetRpcMethodsResponse",

    // CPE >> ACS
    "Empty",
    "EmptyResponse",
    "Inform",
    "InformResponse",
    "TransferComplete",
    "TransferCompleteResponse",
    "AutonomousTransferComplete",
    "AutonomousTransferCompleteResponse",
    "DUStateChangeComplete",
    "DUStateChangeCompleteResponse",
    "AutonomousDUStateChangeComplete",
    "AutonomousDUStateChangeCompleteResponse",
    "RequestDownload",
    "RequestDownloadResponse",
    "Kicked",
    "KickedResponse",

    // ACS >> CPE
    "SetParameterValues",
    "SetParameterValuesResponse",
    "GetParameterValues",
    "GetParameterValuesResponse",
    "GetParameterNames",
    "GetParameterNamesResponse",
    "SetParameterAttributes",
    "SetParameterAttributesResponse",
    "GetParameterAttributes",
    "GetParameterAttributesResponse",
    "AddObject",
    "AddObjectResponse",
    "DeleteObject",
    "DeleteObjectResponse",
    "Reboot",
    "RebootResponse",
    "Download",
    "DownloadResponse",
    "ScheduleDownload",
    "ScheduleDownloadResponse",
    "Upload",
    "UploadResponse",
    "FactoryReset",
    "FactoryResetResponse",
    "GetQueuedTransfers",
    "GetQueuedTransfersResponse",
    "GetAllQueuedTransfers",
    "GetAllQueuedTransfersResponse",
    "CancelTransfer",
    "CancelTransferResponse",
    "ScheduleInform",
    "ScheduleInformResponse",
    "ChangeDUState",
    "ChangeDUStateResponse",
};
#define CWMP_MSG_TYPE_STRING(x)  _CWMP_MSG_TYPE_STRING[x]

#define MAX_PARAM_NAME_SIZE     256
#define MAX_PARAM_VALUE_SIZE    1024

#define SOAP_FAULT_CODE_SERVER "Server"
#define SOAP_FAULT_STRING_CWMP "CWMP fault"

#define CWMP_RPC_METHOD_SUPPORT_NUM 8
static char *CWMP_RPC_METHOD_SUPPORT_LIST[CWMP_RPC_METHOD_SUPPORT_NUM] = {
    "GetRPCMethods",
    "Inform",
    "TransferComplete",
    "AutonomousTransferComplete",
    "DuStateChangeComplete",
    "AutonomousDuStateChangeComplete",
    "RequestDownload",
    "Kicked",
};

#define CWMP_CPE_FAULT_9000_CODE    9000 // "Method not supported"
#define CWMP_CPE_FAULT_9001_CODE    9001 // "Request denied (no reason specified)"
#define CWMP_CPE_FAULT_9002_CODE    9002 // "Internal error"
#define CWMP_CPE_FAULT_9003_CODE    9003 // "Invalid arguments"
#define CWMP_CPE_FAULT_9004_CODE    9004 // "Resources exceeded (when used in association with SetParameterValues, this MUST not be used to indicate parameters in error)"
#define CWMP_CPE_FAULT_9005_CODE    9005 // "Invalid parameter name (associated with Set/GetParameterValues, GetParameterNames, Set/GetParameterAttributes)"
#define CWMP_CPE_FAULT_9006_CODE    9006 // "Invalid parameter type (associated with SetParameterValues)"
#define CWMP_CPE_FAULT_9007_CODE    9007 // "Invalid parameter value (associated with SetParameterValues)"
#define CWMP_CPE_FAULT_9008_CODE    9008 // "Attempt to set a non-writable parameter (associated with SetParameterValues)"
#define CWMP_CPE_FAULT_9009_CODE    9009 // "Notification request rejected (associated with SetParameterAttributes method)."
#define CWMP_CPE_FAULT_9010_CODE    9010 // "Download failure (associated with Download or TransferComplete methods)."
#define CWMP_CPE_FAULT_9011_CODE    9011 // "Upload failure (associated with Upload or TransferComplete methods)."
#define CWMP_CPE_FAULT_9012_CODE    9012 // "File transfer server authentication failure (associated with Upload, Download, or TransferComplete methods)."
#define CWMP_CPE_FAULT_9013_CODE    9013 // "Unsupported protocol for file transfer (associated with Upload and Download methods)."
#define CWMP_CPE_FAULT_9014_CODE    9014 // "Download failure: unable to join multicast group"
#define CWMP_CPE_FAULT_9015_CODE    9015 // "Download failure: unable to contact file"
#define CWMP_CPE_FAULT_9016_CODE    9016 // "Download failure: unable to access file"
#define CWMP_CPE_FAULT_9017_CODE    9017 // "Download failure: unable to complete download"
#define CWMP_CPE_FAULT_9018_CODE    9018 // "Download failure: file corrupted"
#define CWMP_CPE_FAULT_9019_CODE    9019 // "Download failure: file authentication failure"
#define CWMP_CPE_FAULT_98XX_CODE(x) (9800+x) // "Vendor defined fault codes"

static char _CWMP_CPE_FAULT_STRING[20][128] = {
    "Method not supported",
    "Request denied (no reason specified)",
    "Internal error",
    "Invalid arguments",
    "Resources exceeded (when used in association with SetParameterValues, this MUST not be used to indicate parameters in error)",
    "Invalid parameter name (associated with Set/GetParameterValues, GetParameterNames, Set/GetParameterAttributes)",
    "Invalid parameter type (associated with SetParameterValues)",
    "Invalid parameter value (associated with SetParameterValues)",
    "Attempt to set a non-writable parameter (associated with SetParameterValues)",
    "Notification request rejected (associated with SetParameterAttributes method).",
    "Download failure (associated with Download or TransferComplete methods).",
    "Upload failure (associated with Upload or TransferComplete methods).",
    "File transfer server authentication failure (associated with Upload, Download, or TransferComplete methods).",
    "Unsupported protocol for file transfer (associated with Upload and Download methods).",
    "Download failure: unable to join multicast group",
    "Download failure: unable to contact file",
    "Download failure: unable to access file",
    "Download failure: unable to complete download",
    "Download failure: file corrupted",
    "Download failure: file authentication failure",
};

#define CWMP_CPE_FAULT_STRING(x) (_CWMP_CPE_FAULT_STRING[x-9000])

#define CWMP_ACS_FAULT_8000_CODE    8000
#define CWMP_ACS_FAULT_8001_CODE    8001
#define CWMP_ACS_FAULT_8002_CODE    8002
#define CWMP_ACS_FAULT_8003_CODE    8003
#define CWMP_ACS_FAULT_8004_CODE    8004
#define CWMP_ACS_FAULT_8005_CODE    8005
#define CWMP_ACS_FAULT_88XX_CODE(x) (8800+x)

static char _CWMP_ACS_FAULT_STRING[20][128] = {
    "Method not supported",
    "Request denied (no reason specified)",
    "Internal error",
    "Invalid arguments",
    "Resources exceeded",
    "Retry request",
};
#define CWMP_ACS_FAULT_STRING(x) (_CWMP_ACS_FAULT_STRING[x-8000])

typedef enum {
    CWMP_INFORM_EVENT_NONE = 0,
    CWMP_INFORM_EVENT_0_BOOTSTRAP,
    CWMP_INFORM_EVENT_1_BOOT,
    CWMP_INFORM_EVENT_2_PERIODIC,
    CWMP_INFORM_EVENT_3_SCHEDULED,
    CWMP_INFORM_EVENT_4_VALUE_CHANGE,
    CWMP_INFORM_EVENT_5_KICKED,
    CWMP_INFORM_EVENT_6_CONNECTION_REQUEST,
    CWMP_INFORM_EVENT_7_TRANSFER_COMPLETE,
    CWMP_INFORM_EVENT_8_DIAGNOSTICS_COMPLETE,
    CWMP_INFORM_EVENT_9_REQUEST_DOWNLOAD,
    CWMP_INFORM_EVENT_10_AUTONOMOUS_TRANSFER_COMPLETE,
    CWMP_INFORM_EVENT_11_DU_STATE_CHANGE_COMPLETE,
    CWMP_INFORM_EVENT_12_AUTONOMOUS_DU_STATE_CHANGE_COMPLETE,
    CWMP_INFORM_EVENT_MAX,
} CAInformEventType;

static char _CWMP_INFORM_EVENT_STRING[CWMP_INFORM_EVENT_MAX][64] = {
    "NONE",
    "0 BOOTSTRAP",
    "1 BOOT",
    "2 PERIODIC",
    "3 SCHEDULE",
    "4 VALUE CHANGE",
    "5 KICKED",
    "6 CONNECTION REQUEST",
    "7 TRANSFER COMPLETE",
    "8 DIAGNOSTICS COMPLETE",
    "9 REQUEST DOWNLOAD",
    "10 AUTONOMOUS TRANSFER COMPLETE",
    "11 DU STATE CHANGE COMPLETE",
    "12 AUTONOMOUS DU STATE CHANGE COMPLETE",
};
#define CWMP_INFORM_EVENT_STRING(x) _CWMP_INFORM_EVENT_STRING[x]

typedef enum {
    CWMP_INFORM_MEVENT_NONE = 0,
    CWMP_INFORM_MEVENT_REBOOT,
    CWMP_INFORM_MEVENT_SCHEDULEINFORM,
    CWMP_INFORM_MEVENT_DOWNLOAD,
    CWMP_INFORM_MEVENT_UPLOAD,
    CWMP_INFORM_MEVENT_CHANGE_DU_STATE,
    CWMP_INFORM_MEVENT_MAX,
} CAInformEventMType;

static char CWMP_INFORM_MEVENT_STRING[CWMP_INFORM_MEVENT_MAX][32] = {
    "M NONE",
    "M Reboot",
    "M ScheduleInform",
    "M Download",
    "M Upload",
    "M ChangeDUState",
};
#define CWMP_INFORM_MEVENT_STRING(x) _CWMP_INFORM_MEVENT_STRING[x]

#endif
