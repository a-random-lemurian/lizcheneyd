#include <curl/curl.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <uuid/uuid.h>

#include "imgdown.h"
#include "logging.h"
#include "check-perms.h"
#include "verify-file.h"

/*
 * Default lizcheneyd image directory. Always add an ending forward
 * slash to the image. If you use Windows, don't even try, since this
 * software only works with Linux with limited macOS support.
 * 
 * TODO: allow changing this
 */
static char* lizcheneyd_dir = "/var/lib/lizcheneyd/";

static int should_extract_image = 1;

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

void lizcheneyd_get_image_of(struct liz_cheney_image *lci)
{
  size_t malloc_siz = 0;

  malloc_siz = strlen(lizcheneyd_dir) + strlen("liz-cheney-") + 37 + 20;

  uuid_t binuuid;
  uuid_generate_random(binuuid);

  char* file_uuid = malloc(37);
  char* file_name = malloc(malloc_siz);
  char* file_path = malloc(malloc_siz);

  uuid_unparse_lower(binuuid, file_uuid);
  sprintf(file_name, "liz-cheney-%s.jpg", file_uuid);
  sprintf(file_path, "%s%s", lizcheneyd_dir, file_name);

  lizcheneyd_get_image(lci->url, file_path, 1);

  if (!lizcheneyd_verify_file(file_path, lci->sha256sum)) {
    log_error("Bad checksum for Liz Cheney image. Expected %s but got %s."
              "File in question: %s", lci->sha256sum,
              lizcheneyd_last_sum(), file_path);
  }
  else {
    log_trace("Checksum for Liz Cheney image matches.");
  }

  free(file_uuid);
  free(file_name);
  free(file_path);

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
    else {
      log_trace("Opened file %s.", out_path);
    }

    curl_easy_setopt(img, CURLOPT_URL, url);
    curl_easy_setopt(img, CURLOPT_WRITEFUNCTION, NULL);
    curl_easy_setopt(img, CURLOPT_WRITEDATA, img_fp);
    curl_easy_setopt(img, CURLOPT_USERAGENT, preferred_lizcheneyd_user_agent);

    log_trace("Set all options properly, starting download.");
    CURLcode curl_rc = curl_easy_perform(img);
    log_trace("Download finished.");
    fclose(img_fp);
  }
  else {
    // add error handlers here
  }

  log_trace("Cleaning up libcurl.");
  curl_easy_cleanup(img);
}

int lizcheneyd_check_dir_access_wrapper()
{
  return lizcheneyd_check_dir_access(lizcheneyd_dir,
                                     strlen(lizcheneyd_dir) + 1);
}
