#include <curl/curl.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <syslog.h>
#include <errno.h>
#include <uuid/uuid.h>
#include <openssl/sha.h>

#include "imgdown.h"

void get_image(const char *person)
{
  if (!strncasecmp(person, "Liz Cheney", 11)) {
    get_liz_cheney_image();
  }
}


void sha256_hash_string(char hash[SHA256_DIGEST_LENGTH], char outbuf[65])
{
  int i = 0;

  for(i = 0; i < SHA256_DIGEST_LENGTH; i++)
  {
    sprintf(outbuf + (i * 2), "%02x", (unsigned char)hash[i]);
  }

  outbuf[64] = 0;
}

int lizcheneyd_sha256_file(const char *filename, char outputBuffer[65])
{
  FILE *fp = fopen(filename, "rb");

  if (!fp) {
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
    }

    SHA256_Final(hash, &s256);
    sha256_hash_string(hash, outputBuffer);
    fclose(fp);
    free(buf);
  }

  return 0;
}

void verify_liz_cheney_image(const char* filename)
{
  char* out_hash = malloc(65);

  lizcheneyd_sha256_file(filename, out_hash);

  if (!strcmp(out_hash, liz_cheney_image_sha256sum)) {
    
  }
  else {
    syslog(0, "WARNING: Liz Cheney image download at %s has bad checksum!"
              "Expected %s, got %s",
              filename, liz_cheney_image_sha256sum, out_hash);
  }

  free(out_hash);
}

void get_liz_cheney_image()
{
  if (should_extract_image == 0) {
    return;
  }

  CURL *img = curl_easy_init();

  if (img) {
    uuid_t bin_uuid;
    uuid_generate_random(bin_uuid);
    char *text_uuid = malloc(37);
    uuid_unparse_lower(bin_uuid, text_uuid);

    char *filename = malloc(300);
    int filename_freed = 0;

    sprintf(filename, "/var/lib/lizcheneyd/liz-cheney-%s.jpg", text_uuid);

    FILE *lc_img_fp = fopen(filename, "wb");

    if (lc_img_fp == NULL) {
      // There was an error, let's not do that again.
      syslog(LOG_USER, "Failed to open the file %s.", filename);
      should_extract_image = 0;

      free(filename);
      filename_freed = 1;

      return;
    }

    curl_easy_setopt(img, CURLOPT_URL, liz_cheney_image_url);
    curl_easy_setopt(img, CURLOPT_WRITEFUNCTION, NULL);
    curl_easy_setopt(img, CURLOPT_WRITEDATA, lc_img_fp);
    curl_easy_setopt(img, CURLOPT_USERAGENT, preferred_lizcheneyd_user_agent);

    CURLcode curl_rc = curl_easy_perform(img);

    fclose(lc_img_fp);
    verify_liz_cheney_image(filename);

    if (filename_freed == 0) {
      free(filename);
      filename_freed = 1;
    }
  }
  else {
    // add error handlers here
  }
}
