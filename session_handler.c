#include <pthread.h>

#include "log/uulog.h"
#include "base.h"

#define TAG "session-hanlder"

typedef (int) (*rpc_func)(ACS*, CARpcMsg*, CARpcMsg**);

rpc_func all_rpc_methods[100] = {
    NULL, /* rpc_none */
    rpc_fault,
    rpc_get_rpc_methods_req,
    rpc_get_rpc_methods_rsp,
    /* cpe -> acs */
    rpc_empty_req,
    NULL, /* rpc_empty_rsp */
    rpc_inform_req,
    NULL, /* rpc_inform_rsp */
    rpc_transfer_complete_req,
    NULL, /* rpc_transfer_complete_rsp */
    rpc_auto_transfer_complete_req,
    NULL, /* rpc_auto_transfer_complete_rsp */
    rpc_dustate_change_complete_req,
    NULL, /* rpc_du_state_change_complete_rsp */
    rpc_auto_dustate_change_complete_req,
    NULL, /* rpc_auto_du_state_change_complete_rsp */
    rpc_download_req,
    NULL, /* rpc_download_rsp */
    rpc_kicked_req,
    NULL, /* rpc_kicked_rsp */

    /* acs -> cpe */
    NULL, /* rpc_set_param_values_req */
    rpc_set_param_values_rsp,
    NULL, /* rpc_get_param_values_req */
    rpc_get_param_values_rsp,
    NULL, /* rpc_get_param_names_req */
    rpc_get_param_names_rsp,
    NULL, /* rpc_set_param_attributes_req */
    rpc_set_param_attributes_rsp,
    NULL, /* rpc_get_param_attributes_req */
    rpc_get_param_attributes_rsp,
    NULL, /* rpc_add_object_req */
    rpc_add_object_rsp,
    NULL, /* rpc_delete_object_req */
    rpc_delete_object_rsp,
    NULL, /* rpc_reboot_req */
    rpc_reboot_rsp,
    NULL, /* rpc_download_req */
    rpc_download_rsp,
    NULL, /* rpc_schedule_download_req */
    rpc_schedule_download_rsp,
    NULL, /* rpc_upload_req */
    rpc_upload_rsp,
    NULL, /* rpc_factory_reset_req */
    rpc_factory_reset_rsp,
    NULL, /* rpc_get_queued_transfers_req */
    rpc_get_queued_transfers_rsp,
    NULL, /* rpc_get_all_queued_transfers_req */
    rpc_get_all_queued_transfers_rsp,
    NULL, /* rpc_cancel_transfers_req */
    rpc_cancel_transfers_rsp,
    NULL, /* rpc_schedule_inform_req */
    rpc_schedule_inform_rsp,
    NULL, /* rpc_change_dustate_req */
    rpc_change_dustate_rsp,
    rpc_max
};

#if 0
int struct_comm_with_acs(ACS *acs, CARpcMsg *send, CARpcMsg **recv)
{
    int ret;
    rpc_func method = NULL;

    if (!send) {
        LOGE(TAG, "Invalid param(acs: %p)", acs);
        return -1;
    }

    if (send->type >= CWMP_MSG_MAX) {
        LOGE(TAG, "Invalid RPC type(%d)", send->type);
        return -1;
    }
    method = all_rpc_methods[send->type];
    if (!method) {
        LOGE(TAG, "Invalid RPC type(%d): %s",
                send->type, CWMP_MSG_TYPE_STRING[send->type]);
        return -1;
    }
    ret = method(acs, send, recv);
    return ret;
}
#endif
