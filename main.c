#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>

#define OID_LENGTH(oid) (sizeof(oid)/sizeof(oid[0]))

typedef struct {
    int interfaceId;
    char interfaceName[50];
} interfaceEntry;

// Example data
interfaceEntry interfaces[] = {
    {1, "eth0"},
    {2, "eth1"},
    {3, "wlp4s0"}
};

#define INTERFACE_TABLE_SIZE (sizeof(interfaces) / sizeof(interfaceEntry))


int handle_myNetInterfaceTable(netsnmp_log_handler *handler,
                               netsnmp_handler_registration *reginfo,
                               netsnmp_agent_request_info *reqinfo,
                               netsnmp_priv_alg_info *requests) {
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
    const oid myNetInterfaceTable_oid[] = {1,3,6,1,4,1,9999,2};
    netsnmp_register_table_data(
        netsnmp_create_handler_registration("myNetInterfaceTable", handle_myNetInterfaceTable,
                                            myNetInterfaceTable_oid, OID_LENGTH(myNetInterfaceTable_oid),
                                            HANDLER_CAN_RWRITE),
        netsnmp_create_table_data_set("myNetInterfaceTable", NULL, 0, 0, 0, 0),
        NULL);
}

static int handle_myObject(netsnmp_mib_handler *handler,
                           netsnmp_handler_registration *reginfo,
                           netsnmp_agent_request_info *reqinfo,
                           netsnmp_request_info *requests) {
    // Implementação da lógica para manipulação de requisições
    // Add logic to handle GET, GETNEXT, SET requests
    return SNMP_ERR_NOERROR;
}

int main(int argc, char **argv) {

    const oid myNetInterfaceTable_oid[] = {1,3,6,1,4,1,9999,2}; // Example OID, adjust as necessary
    
    init_agent("myAgent");

    int myMIB();

    init_snmp("myAgent");

    netsnmp_register_scalar(
        netsnmp_create_handler_registration("myNetInterfaceTable", handle_myNetInterfaceTable,
                                            myNetInterfaceTable_oid, OID_LENGTH(myNetInterfaceTable_oid),
                                            HANDLER_CAN_RWRITE));
    
    snmp_log(LOG_INFO, "Iniciando agente SNMP.\n");

    while (1) {
        agent_check_and_process(1); // 1 = block until a packet arrives
    }

    snmp_shutdown("myAgent");
    return 0;
}
