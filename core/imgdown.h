#ifndef IMGDOWN_H
#define IMGDOWN_H
#include <openssl/sha.h>

void get_image(const char* person);
void sha256_hash_string(char hash[SHA256_DIGEST_LENGTH], char outbuf[65]);
int lizcheneyd_sha256_file(const char *filename, char outputBuffer[65]);
void verify_liz_cheney_image(const char* filename);
void get_liz_cheney_image();

#endif /* IMGDOWN_H */
