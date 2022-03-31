#ifndef VERIFY_FILE_H
#define VERIFY_FILE_H

const char* lizcheneyd_last_sum();
void lizcheneyd_sha256_string(char hash[SHA256_DIGEST_LENGTH], char outbuf[65]);
int lizcheneyd_sha256_file(const char *filename, char outputBuffer[65]);

/*
 * Verify a file by computing the SHA-256 checksum of the
 * file's content and comparing it against a SHA-256
 * checksum supplied in the sha256_checksum parameter.
 * 
 * --- RETURN VALUE --------------------------------------
 * Returns 0 when the checksum does not match.
 * Returns 1 when the checksum matches.
 */
int lizcheneyd_verify_file(const char *filename, const char *sha256_checksum);

/*
 * Wrapper for lizcheneyd_verify_file. This function prints messages to
 * lizcheneyd's log file when the checksum does not match.
 */
void verify_liz_cheney_image(const char* filename, const char* sha256sum);

#endif /* VERIFY_FILE_H */
