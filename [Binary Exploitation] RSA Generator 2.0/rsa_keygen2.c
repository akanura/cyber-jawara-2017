/*
 * Cyber Jawara 2017 - RSA Key generator 2.0
 *
 * gcc rsa_keygen2.c -o rsa_keygen2
 * socat -d -d -d TCP4-LISTEN:41337,reuseaddr,fork EXEC:"./rsa_keygen2" &
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void rand_str(char *str, size_t len) {
    char cset[] = "0123456789"
                  "abcdefghijklmnopqrstuvwxyz"
                  "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    while (len-- > 0) {
        size_t idx = (double) rand() / RAND_MAX * (sizeof cset - 1);
        *str++ = cset[idx];
    }

    *str = '\0';
}

size_t escape_gets(char *pass, size_t limit) {
    size_t sz;
    char c;
    sz = 0;

    while (sz++ < limit) {
        c = getchar();
        if (c == '\n' || c == '\0') {
            break;
        } else if (c == '\'') {
            *pass++ = '\'';
            *pass++ = '\\';
            *pass++ = '\'';
            *pass++ = '\'';
        } else {
            *pass++ = c;
        }
    }

    *pass = '\0';
    return sz;
}

void rsa_keygen() {
    char passphrase[128];
    char private_gen[320];
    char public_gen[320];
    char tmp[128];
    char dir[32];
    size_t len;

    puts("");
    puts(" --//-- CJ RSA Key Generator --//-- ");
    puts("");

    rand_str(dir, 30);
    sprintf(tmp, "mkdir dir/%s 2>/dev/null ", dir);

    passphrase[0] = '\0';
    len = 0;
    while (len < 4) {
        puts("Passphrase: ");
        len = escape_gets(passphrase, 127);
        if (len < 4 || len > 127) {
            puts("You must type in 4 to 127 characters");
        }
    }
    puts("");

    system(tmp);

    sprintf(private_gen,
            "openssl genrsa -aes128 -passout 'pass:%s' -out "
            "dir/%s/private.pem 2048 2>/dev/null",
            passphrase, dir);

    sprintf(public_gen,
            "openssl rsa -passin 'pass:%s' -in dir/%s/private.pem "
            "-outform PEM -pubout -out dir/%s/public.pem 2>/dev/null",
            passphrase, dir, dir);

    system(private_gen);
    system(public_gen);

    sprintf(tmp, "cat dir/%s/private.pem 2>/dev/null ", dir);
    system(tmp);
    sprintf(tmp, "cat dir/%s/public.pem 2>/dev/null ", dir);
    system(tmp);
}

void init() {
    char buff[1];
    buff[0] = 0;
    setvbuf(stdout, buff, _IOFBF, 1);
    srand(time(0));
}

int main() {
    init();
    rsa_keygen();
    return 0;
}