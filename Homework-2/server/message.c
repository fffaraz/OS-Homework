#include <stdio.h>

#include "message.h"

int serialize_message(char* buffer, const Message* message){
	return sprintf(buffer, "%s:%s:%d:%s\n", message->verb, message->arg, message->group_id, message->client_name);
}

int deserialize_message(char* buffer, Message* message){
	return sscanf(buffer, "%s:%[^:]:%d:%s", message->verb, message->arg, &message->group_id, message->client_name); 
}
