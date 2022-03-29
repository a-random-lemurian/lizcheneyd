#include <uuid/uuid.h>
#include <stdio.h>
#include <errno.h>
#include <syslog.h>
#include <stddef.h>
#include <stdlib.h>

#include "imgdown.h"
#include "logging.h"


int lizcheneyd_root_access_check()
{
  int uid = getuid();
  if (uid != 0) {
    log_warn("UID is not 0. lizcheneyd must be run as root.");
    log_warn("Work on allowing lizcheneyd to operate without root access is ongoing.");
    log_warn("Sleeping for 30 seconds.");

    sleep(30);
  }
  else if (uid == 0) {
    log_info("lizcheneyd is running as root.");
  }

  return uid;
}


/*
 * Create a temporary dummy file in the lizcheneyd directory
 * to check for access.
 */
int lizcheneyd_check_dir_access(const char* directory, size_t dir_siz)
{
  log_trace("Starting a permissions check for %s", directory);
  int rc = 0;

  uuid_t bin_uuid;
  uuid_generate_random(bin_uuid);
  char *text_uuid = malloc(65);
  uuid_unparse_lower(bin_uuid, text_uuid);

  char *filename = malloc(130 + dir_siz);

  sprintf(filename, "%s.file_check.%s.tmp", directory, text_uuid);
  log_trace("Using %s as dummy file.", filename);

  FILE* fp = fopen(filename, "w");

  if (fp == NULL) {
    if (errno == EACCES) {
      syslog(LOG_ERR, "Unable to open files in %s!", directory);
      syslog(LOG_ERR, "For the rest of session, will not "
             "download any images of Liz Cheney.");
      log_error("Unable to open files in %s, disabling Liz Cheney image "
                "downloading for now.", directory);
      set_should_extract_image(0);
      rc = 1;
    }
  }
  else {
    log_trace("Successfully opened %s.", filename);
    int rc = remove(filename);
    if (rc == 0) {
      syslog(LOG_NOTICE,
             "Permissions check for %s: Able to create and delete.",
             directory);
      log_info("Permissions check for %s: Able to create and delete files.",
               directory);
    }
    else {
      syslog(LOG_ERR, "Unable to delete files in %s. lizcheneyd will "
                      "be unable to clean up after itself. Please manually "
                      "remove all Liz Cheney images in %s when you feel "
                      "like it. Or contact your local system administrator.",
                      directory, directory);
      rc = 1;
    }
  }

  free(filename);
  free(text_uuid);

  return 0;
}
