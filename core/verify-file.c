#include <syslog.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <openssl/sha.h>

#include "logging.h"
#include "verify-file.h"



static char* last_checksum = NULL;

const char* lizcheneyd_last_sum()
{
  return last_checksum;
}

void lizcheneyd_sha256_string(char hash[SHA256_DIGEST_LENGTH], char outbuf[65])
{
  int i = 0;

  for (i = 0; i < SHA256_DIGEST_LENGTH; i++) {
    sprintf(outbuf + (i * 2), "%02x", (unsigned char)hash[i]);
  }

  outbuf[64] = 0;
}

int lizcheneyd_sha256_file(const char *filename, char outputBuffer[65])
{
  FILE *fp = fopen(filename, "rb");

  if (!fp) {
    log_error("Unable to open %s while attempting to sha256sum it", filename);
    return -534;
  }
  else {
    unsigned char hash[SHA256_DIGEST_LENGTH];

    SHA256_CTX s256;
    SHA256_Init(&s256);

    const int buf_siz = 32768;
    unsigned char *buf = malloc(buf_siz);
    int bytes_read = 0;

    if (!buf) {
      return ENOMEM;
    }

    while ((bytes_read = fread(buf, 1, buf_siz, fp))) {
      SHA256_Update(&s256, buf, bytes_read);
      log_trace("Read %d bytes from %s for hashing", buf_siz, filename);
    }

    SHA256_Final(hash, &s256);
    lizcheneyd_sha256_string(hash, outputBuffer);
    fclose(fp);
    free(buf);
  }

  return 0;
}



int lizcheneyd_verify_file(const char* filename, const char* sha256_checksum)
{
  char* out_hash = malloc(65);
  lizcheneyd_sha256_file(filename, out_hash);

  last_checksum = out_hash;

  if (!strcmp(out_hash, sha256_checksum)) {
    return 1;
  }
  else { 
    return 0;
  }
}

void verify_liz_cheney_image(const char* filename, const char* sha256sum)
{
  if (!lizcheneyd_verify_file(filename, sha256sum)) {
    syslog(LOG_WARNING,
           "WARNING: Liz Cheney image download at %s has bad checksum!"
           "Expected %s, got %s",
           filename, sha256sum, last_checksum);
  }
}