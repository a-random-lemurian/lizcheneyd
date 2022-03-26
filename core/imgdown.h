#ifndef IMGDOWN_H
#define IMGDOWN_H
#include <openssl/sha.h>

int should_extract_images();
void set_should_extract_image(int new_value);
void lizcheneyd_set_uagent(char *new_agent);
void lizcheneyd_get_image_of(const char *person);
void lizcheneyd_sha256_string(char hash[SHA256_DIGEST_LENGTH], char outbuf[65]);
int lizcheneyd_sha256_file(const char *filename, char outputBuffer[65]);
int lizcheneyd_verify_file(const char *filename, const char *sha256_checksum);
void verify_liz_cheney_image(const char *filename);
void lizcheneyd_get_image(const char *url, const char *out_path, int is_binary);
int lizcheneyd_check_dir_access_wrapper();
#endif /* IMGDOWN_H */
