#include <curl/curl.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <syslog.h>
#include <errno.h>
#include <uuid/uuid.h>
#include <openssl/sha.h>

#include "imgdown.h"
#include "logging.h"
#include "check-perms.h"


static char* last_checksum = NULL;

/*
 * Default lizcheneyd image directory. Always add an ending forward
 * slash to the image. If you use Windows, don't even try, since this
 * software only works with Linux with limited macOS support.
 * 
 * TODO: allow changing this
 */
static char* lizcheneyd_dir = "/var/lib/lizcheneyd/";

static int should_extract_image = 1;
static const char *liz_cheney_image_url =
    "https://upload.wikimedia.org/wikipedia/commons/9/9a/"
    "Liz_Cheney_official_116th_Congress_portrait.jpg";

/*
 * lizcheneyd has a zero-tolerance policy for bad images, let's
 * enforce that using checksums.
 *
 * Checksum is of the image at the URL in the variable
 * liz_cheney_image_url.
 */
static const char *liz_cheney_image_sha256sum =
    "d6f8cfe9f487b0055e3074ea4ba9d8ad1a63767b4e339804687b8a2aabdda9b4";

/*
 * If you want to be honest and not lie to Wikimedia servers
 * about who you really are, add `-DLIZCHENEYD_USER_AGENT <user_agent>`
 * to the Makefile. Replace <user_agent> with preferred user
 * agent.
 * 
 * TODO: add a frontend --user-agent flag, so user agent is not hardcoded,
 * and make this the default.
 * 
 * TODO: be honest about user agent, eventually
 */
#ifndef LIZCHENEYD_USER_AGENT
#define LIZCHENEYD_USER_AGENT  \
  "Mozilla/5.0 (X11; Linux x86_64; rv:60.0) Gecko/20100101 Firefox/81.0"
#endif

static char* preferred_lizcheneyd_user_agent = LIZCHENEYD_USER_AGENT;

static int liz_cheney_images_downloaded = 0;

int should_extract_images()
{
  return should_extract_image;
}

void set_should_extract_image(int new_value)
{
  should_extract_image = new_value;
}

void lizcheneyd_set_uagent(char* new_agent)
{
  preferred_lizcheneyd_user_agent = new_agent;
}

void lizcheneyd_get_image_of(const char *person)
{
  size_t malloc_siz = 0;

  if (!strncasecmp(person, "Liz Cheney", 11)) {

    malloc_siz = strlen(lizcheneyd_dir) + strlen("liz-cheney-") + 37 + 20;

    uuid_t binuuid;
    uuid_generate_random(binuuid);

    char* file_uuid = malloc(37);
    char* file_name = malloc(malloc_siz);
    char* file_path = malloc(malloc_siz);

    uuid_unparse_lower(binuuid, file_uuid);
    sprintf(file_name, "liz-cheney-%s.jpg", file_uuid);
    sprintf(file_path, "%s%s", lizcheneyd_dir, file_name);

    lizcheneyd_get_image(liz_cheney_image_url, file_path, 1);

    if (!lizcheneyd_verify_file(file_path, liz_cheney_image_sha256sum)) {
      log_error("Bad checksum for Liz Cheney image. Expected %s but got %s."
                "File in question: %s", file_path,
                 liz_cheney_image_sha256sum, last_checksum);
    }

    free(file_uuid);
    free(file_name);
    free(file_path);
  }

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

  strncpy(last_checksum, out_hash, 64);

  if (!strcmp(out_hash, sha256_checksum)) {
    return 1;
  }
  else { 
    return 0;
  }
}

void verify_liz_cheney_image(const char* filename)
{
  if (!lizcheneyd_verify_file(filename, liz_cheney_image_sha256sum) == 0) {
    syslog(LOG_WARNING,
           "WARNING: Liz Cheney image download at %s has bad checksum!"
           "Expected %s, got %s",
           filename, liz_cheney_image_sha256sum, last_checksum);
  }
}

void lizcheneyd_get_image(const char* url, const char* out_path,
                          int is_binary)
{
  CURL *img = curl_easy_init();
  log_trace("Initialized libcurl.");

  if (img) {
    char* mode = "w";
    if (is_binary) {
      mode = "wb";
    }

    FILE *img_fp = fopen(out_path, "w");

    if (img_fp == NULL) {
      log_error("Unable to download file: "
                "couldn't open file %s for writing.", out_path);
      return;
    }

    curl_easy_setopt(img, CURLOPT_URL, liz_cheney_image_url);
    curl_easy_setopt(img, CURLOPT_WRITEFUNCTION, NULL);
    curl_easy_setopt(img, CURLOPT_WRITEDATA, img_fp);
    curl_easy_setopt(img, CURLOPT_USERAGENT, preferred_lizcheneyd_user_agent);

    CURLcode curl_rc = curl_easy_perform(img);
    fclose(img_fp);
  }
  else {
    // add error handlers here
  }
}

int lizcheneyd_check_dir_access_wrapper()
{
  return lizcheneyd_check_dir_access(lizcheneyd_dir,
                                     strlen(lizcheneyd_dir) + 1);
}
