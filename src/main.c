#include <stdio.h>
#include <stdint.h>

#define CLIENT_NICKNAME_MAXLEN 9
#define CLIENT_NAME_MAXLEN 255

typedef struct {
    char nickname[CLIENT_NICKNAME_MAXLEN]; // Unique identifier
    char real_name[CLIENT_NAME_MAXLEN]; // Real name of the host running on
    char username[CLIENT_NAME_MAXLEN]; // Username of the Client on that host
    uint32_t server_id; // Server identifier  for the client
} Client;

enum CHOP_CMDS {
    KICK, // Eject a client from tHe channel 
    MODE, // Change the channel's mode
    INVITE, // Invite a client to an invite-only channel (mode +i) 
    TOPIC // Change the channel topic in a mode +t channel
};


uint8_t internal_client_set_string(char *c, char* name, uint8_t maxlen, uint8_t len){
    // TODO: Throw an error (?)
    if (maxlen < len ) {
        printf("[ERROR] maxlen(%zu) < len(%zu)\n", maxlen, len);
        return 1; // Maybe an error here?
    }

    for (int i = 0 ; i < len; i++) {
        c[i] = name[i];
    }

    c[len] = '\0';
    return 0;

}

void client_set_nickname(Client *c, char* nick, uint8_t len)
{
    if (internal_client_set_string(c->nickname, nick, CLIENT_NICKNAME_MAXLEN, len) != 0)
        printf("[ERROR]: Could not set the string in client_set_nickname()\n");
}

void client_set_realname(Client *c, char* name, uint8_t len)
{
    if (internal_client_set_string(c->real_name, name, CLIENT_NAME_MAXLEN, len) != 0)
        printf("[ERROR]: Could not set the string in client_set_realname()\n");
}

void client_set_username(Client *c, char* name, uint8_t len)
{
    if (internal_client_set_string(c->username, name, CLIENT_NAME_MAXLEN, len) != 0)
        printf("[ERROR]: Could not set the string in client_set_realname()\n");
}



int main() 
{

    Client c = {0};
    client_set_nickname(&c, "Hector", 6);
    client_set_realname(&c, "Hector Alarcon Flores", 51);
    client_set_username(&c, "hector-user", 11);
    printf("Nickname: %s\n", c.nickname);
    printf("Realname: %s\n", c.real_name);
    printf("Username: %s\n", c.username);
    printf("Hello world! : \n");
    return 0;
}
