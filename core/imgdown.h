#ifndef IMGDOWN_H
#define IMGDOWN_H
#include <openssl/sha.h>

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

void get_image(const char* person);
void sha256_hash_string(char hash[SHA256_DIGEST_LENGTH], char outbuf[65]);
int lizcheneyd_sha256_file(const char *filename, char outputBuffer[65]);
void verify_liz_cheney_image(const char* filename);
void get_liz_cheney_image();

#endif /* IMGDOWN_H */
