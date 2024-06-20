#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

typedef struct {
    int interfaceId;
    char interfaceName[50];
} interfaceEntry;

interfaceEntry interfaces[] = {
    {1, "eth0"},
    {2, "eth1"},
    {3, "wlp4s0"}
};
#define INTERFACE_TABLE_SIZE (sizeof(interfaces) / sizeof(interfaceEntry))

int handle_myNetInterfaceTable(netsnmp_mib_handler *handler,
                               netsnmp_handler_registration *reginfo,
                               netsnmp_agent_request_info *reqinfo,
                               netsnmp_request_info *requests) {
    for (netsnmp_request_info *request = requests; request; request = request->next) {
        int table_index = *(request->requestvb->name + request->requestvb->name_length - 1) - 1;
        if (reqinfo->mode == MODE_GET) {
            if (table_index < INTERFACE_TABLE_SIZE) {
                snmp_set_var_typed_value(request->requestvb, ASN_OCTET_STR,
                                         interfaces[table_index].interfaceName,
                                         strlen(interfaces[table_index].interfaceName));
            } else {
                netsnmp_set_request_error(reqinfo, request, SNMP_NOSUCHINSTANCE);
            }
        }

        // Implement other modes like SET and GETNEXT
       if (reqinfo->mode == MODE_SET_RESERVE1) {
            // Check if the new value is valid (e.g., check length for strings)
            if (table_index < INTERFACE_TABLE_SIZE && request->requestvb->type == ASN_OCTET_STR &&
                request->requestvb->val_len < sizeof(interfaces[table_index].interfaceName)) {
                // Reserve space or additional checks
            } else {
                netsnmp_set_request_error(reqinfo, request, SNMP_ERR_WRONGTYPE);
            }
        }

        if (reqinfo->mode == MODE_SET_ACTION) {
            // Perform the update
            if (table_index < INTERFACE_TABLE_SIZE) {
                memcpy(interfaces[table_index].interfaceName, request->requestvb->val.string,
                    request->requestvb->val_len);
                interfaces[table_index].interfaceName[request->requestvb->val_len] = '\0';  // Null-terminate string
            }
        }

        if (reqinfo->mode == MODE_SET_COMMIT) {
            if (table_index >= INTERFACE_TABLE_SIZE) {
                netsnmp_set_request_error(reqinfo, request, SNMP_NOSUCHINSTANCE);
            }
        }
        
        if (reqinfo->mode == MODE_SET_FREE) {
            if (table_index >= INTERFACE_TABLE_SIZE) {
                netsnmp_set_request_error(reqinfo, request, SNMP_NOSUCHINSTANCE);
            }
        }

        // getnext 
        if (reqinfo->mode == MODE_GETNEXT) {
            if (table_index < INTERFACE_TABLE_SIZE - 1) {
                table_index++;
                snmp_set_var_typed_value(request->requestvb, ASN_OCTET_STR,
                                        interfaces[table_index].interfaceName,
                                        strlen(interfaces[table_index].interfaceName));
                oid new_oid[] = {1,3,6,1,4,1,9999,2,table_index + 1};
                snmp_set_var_objid(request->requestvb, new_oid, OID_LENGTH(new_oid));
            } else {
                netsnmp_set_request_error(reqinfo, request, SNMP_ENDOFMIBVIEW);
            }
        }
    }
    return SNMP_ERR_NOERROR;
}

void init_myMIB(void) {
    oid myNetInterfaceTable_oid[] = {1,3,6,1,4,1,9999,2};
    netsnmp_handler_registration *reg;
    netsnmp_table_registration_info *table_info;

    reg = netsnmp_create_handler_registration("myNetInterfaceTable", handle_myNetInterfaceTable,
                                              myNetInterfaceTable_oid, OID_LENGTH(myNetInterfaceTable_oid),
                                              HANDLER_CAN_RWRITE);

    table_info = SNMP_MALLOC_TYPEDEF(netsnmp_table_registration_info);
    netsnmp_table_helper_add_indexes(table_info, ASN_INTEGER, 0); // Define the index type, here assuming integer
    table_info->min_column = 1;
    table_info->max_column = 2;

    netsnmp_register_table(reg, table_info);
}

int main(int argc, char **argv) {
    // Initialize the agent here
    netsnmp_daemonize(1, 0);
    init_agent("myAgent");
    init_myMIB();
    init_snmp("snmpd");

    snmp_log(LOG_INFO, "Starting SNMP Agent.\n");

    while (1) {
        agent_check_and_process(1); // 1 = block until a packet arrives
    }

    snmp_shutdown("myAgent");
    return 0;
}
