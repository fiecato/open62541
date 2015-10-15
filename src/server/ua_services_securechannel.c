#include "ua_server_internal.h"
#include "ua_services.h"
#include "ua_securechannel_manager.h"

void Service_OpenSecureChannel(UA_Server *server, UA_Connection *connection,
                               const UA_OpenSecureChannelRequest *request,
                               UA_OpenSecureChannelResponse *response) {
    // todo: if(request->clientProtocolVersion != protocolVersion)
    if(request->requestType == UA_SECURITYTOKENREQUESTTYPE_ISSUE) {
        UA_SecureChannelManager_open(&server->secureChannelManager, connection, request, response);
        if(response->responseHeader.serviceResult == UA_STATUSCODE_GOOD)
            UA_LOG_DEBUG(server->logger, UA_LOGCATEGORY_SECURECHANNEL,
                         "Opened SecureChannel %i on Connection %i",
                         response->securityToken.channelId, connection->sockfd);
        else
            UA_LOG_DEBUG(server->logger, UA_LOGCATEGORY_SECURECHANNEL,
                         "Opening SecureChannel on Connection %i failed", connection->sockfd);
    } else {
        UA_SecureChannelManager_renew(&server->secureChannelManager, connection, request, response);
        if(response->responseHeader.serviceResult == UA_STATUSCODE_GOOD)
            UA_LOG_DEBUG(server->logger, UA_LOGCATEGORY_SECURECHANNEL,
                         "Renewed SecureChannel %i on Connection %i",
                         response->securityToken.channelId, connection->sockfd);
        else
            UA_LOG_DEBUG(server->logger, UA_LOGCATEGORY_SECURECHANNEL,
                         "Renewing SecureChannel on Connection %i failed", connection->sockfd);
    }
}

void Service_CloseSecureChannel(UA_Server *server, UA_Int32 channelId) {
    UA_LOG_DEBUG(server->logger, UA_LOGCATEGORY_SECURECHANNEL,
                 "Closing SecureChannel %i", channelId);
    UA_SecureChannelManager_close(&server->secureChannelManager, channelId);
    // 62451 Part 6 Chapter 7.1.4 - The server does not send a CloseSecureChannel response
}
