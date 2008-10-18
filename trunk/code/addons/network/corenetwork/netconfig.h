#ifndef CORENETWORK_NETCONFIG_H
#define CORENETWORK_NETCONFIG_H

#define NUM_CLIENTS 500
#define SERVER_DEFAULT_PORT 12345
#define CLIENT_DEFAULT_PORT 12345

enum NetErrorCode
{
    CONNECTION_REQUEST_ACCEPTED,
    CONNECTION_ATTEMPT_FAILED,
    ALREADY_CONNECTED,
    NO_FREE_INCOMING_CONNECTIONS,
    DISCONNECTION_NOTIFICATION,
    CONNECTION_LOST,
    NEW_INCOMING_CONNECTION
};
#endif // CORENETWORK_NETCONFIG_H