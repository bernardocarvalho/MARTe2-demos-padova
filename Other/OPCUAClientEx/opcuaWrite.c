#include <stdio.h>
#include "open62541.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Invalid number of arguments\n");
        return -1;
    }
    UA_Client *client = UA_Client_new(UA_ClientConfig_default);
    UA_StatusCode retval = UA_Client_connect(client, "opc.tcp://localhost:4840");
    if(retval != UA_STATUSCODE_GOOD) {
        UA_Client_delete(client);
        return (int)retval;
    }

    UA_Variant value;
    UA_Variant_init(&value);
    UA_NodeId node;
    const UA_UInt32 pathSize = 2;
    char* path[pathSize];
    path[0] = "Demo";
    path[1] = argv[1];
    /* Building request for Browse Service */
    UA_BrowseRequest bReq;
    UA_BrowseRequest_init(&bReq);
    bReq.requestedMaxReferencesPerNode = 0u;
    bReq.nodesToBrowse = UA_BrowseDescription_new();
    bReq.nodesToBrowseSize = 1u;

    /* Building request for TranslateBrowsePathsToNodeIds */
    UA_BrowsePath browsePath;
    UA_BrowsePath_init(&browsePath);
    browsePath.startingNode = UA_NODEID_NUMERIC(0u, 85u); /* UA_NS0ID_OBJECTSFOLDER */
    browsePath.relativePath.elements = (UA_RelativePathElement*)UA_Array_new(pathSize, &UA_TYPES[UA_TYPES_RELATIVEPATHELEMENT]);
    browsePath.relativePath.elementsSize = pathSize;

    UA_UInt32 tempNumericNodeId = 85u; /* UA_NS0ID_OBJECTSFOLDER */
    UA_UInt16 tempNamespaceIndex = 0u;
    UA_UInt32 ids[pathSize];
    ids[0] = UA_NS0ID_ORGANIZES;
    ids[1] = UA_NS0ID_HASCOMPONENT;
    char* tempStringNodeId;
    for (UA_UInt32 j = 0u; j < pathSize; j++) {
        UA_RelativePathElement *elem = &browsePath.relativePath.elements[j];
        elem->referenceTypeId = UA_NODEID_NUMERIC(0u, ids[j]);
        elem->targetName = UA_QUALIFIEDNAME_ALLOC(1u, path[j]);
    }
    UA_TranslateBrowsePathsToNodeIdsRequest tbpReq;
    UA_TranslateBrowsePathsToNodeIdsRequest_init(&tbpReq);
    tbpReq.browsePaths = &browsePath;
    tbpReq.browsePathsSize = 1u;
    UA_TranslateBrowsePathsToNodeIdsResponse tbpResp = UA_Client_Service_translateBrowsePathsToNodeIds(client, tbpReq);
    bool ok = (tbpResp.results[0].statusCode == 0x00U); /* UA_STATUSCODE_GOOD */
    if (ok) {
        UA_BrowsePathTarget *ref = &(tbpResp.results[0].targets[0]);
        node = ref->targetId.nodeId;
    }
    UA_TranslateBrowsePathsToNodeIdsResponse_deleteMembers(&tbpResp);
    UA_Array_delete(browsePath.relativePath.elements, pathSize, &UA_TYPES[UA_TYPES_RELATIVEPATHELEMENT]);
    UA_BrowseDescription_delete(bReq.nodesToBrowse);

    UA_UInt32 number = atoi(argv[2]);
    UA_Variant_setScalar(&value, &number, &UA_TYPES[UA_TYPES_UINT32]);
    retval = UA_Client_writeValueAttribute(client, node, &value);

    if(retval == UA_STATUSCODE_GOOD &&
            UA_Variant_hasScalarType(&value, &UA_TYPES[UA_TYPES_UINT32])) {
        UA_Client_readValueAttribute(client, node, &value);
        printf("Value updated: %s %d \n", argv[1], *((UA_UInt32*)value.data));
    }

    /* Clean up */
    UA_Variant_deleteMembers(&value);
    UA_Client_delete(client); /* Disconnects the client internally */
    return UA_STATUSCODE_GOOD;
}
