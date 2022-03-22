#ifndef IMGDOWN_H
#define IMGDOWN_H
#include <openssl/sha.h>

void lizcheneyd_get_image_of(const char* person);
void lizcheneyd_sha256_string(char hash[SHA256_DIGEST_LENGTH], char outbuf[65]);
int lizcheneyd_sha256_file(const char *filename, char outputBuffer[65]);
void verify_liz_cheney_image(const char* filename);
void get_liz_cheney_image();
void lizcheneyd_set_uagent(char* new_agent);

#endif /* IMGDOWN_H */
