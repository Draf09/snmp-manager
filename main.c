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
    }
    return SNMP_ERR_NOERROR;
}

void init_myMIB(void) {
    oid myNetInterfaceTable_oid[] = {1,3,6,1,4,1,9999,2};
    netsnmp_handler_registration *reg;
    reg = netsnmp_create_handler_registration("myNetInterfaceTable", handle_myNetInterfaceTable,
                                              myNetInterfaceTable_oid, OID_LENGTH(myNetInterfaceTable_oid),
                                              HANDLER_CAN_RWRITE);
    netsnmp_register_scalar(reg);
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
