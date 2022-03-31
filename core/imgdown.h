#ifndef IMGDOWN_H
#define IMGDOWN_H
#include <openssl/sha.h>

struct liz_cheney_image {
  char* url;
  char* sha256sum;
};

int should_extract_images();
void set_should_extract_image(int new_value);
void lizcheneyd_set_uagent(char *new_agent);

/*
 * Download an image of Liz Cheney. This function expects
 * a struct containing a URL and a SHA256 sum of the file
 * pointed to by the URL.
 *
 * The "lci" parameter must be a pointer to a Liz Cheney
 * image struct.
 * 
 * lci: [L]iz [C]heney [i]mage
 */
void lizcheneyd_get_image_of(struct liz_cheney_image *lci);

void lizcheneyd_get_image(const char *url, const char *out_path, int is_binary);
int lizcheneyd_check_dir_access_wrapper();
#endif /* IMGDOWN_H */
