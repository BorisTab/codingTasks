#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/*!
	@file
    @brief myStdlib
*/

int AtoI(const char str[]);

char *ItoA(int num);

int PutS(char str[]);

char *fGetS(char *str, size_t count, FILE *stream);

int main() {
    char str[] = "42";
    int a = AtoI(str);
    printf("atoi: %d\n", a);
    printf("itoa: %s\n", ItoA(a));
    PutS("out from PutS\n");
    fGetS(str, 6, stdin);
    printf("got by fGetS: %s", str);
    return 0;
}

/*! convert string to integer
 *
 * @param[in] *str pointer to string for convert
 *
 * @return converted number
 */
int AtoI(const char *str) {
    assert(str != nullptr);

    int n = 0;
    for (int i = 0; str[i] >= '0' && str[i] <= '9'; i++) {
        n = 10 * n + (str[i] - '0');
    }
    return n;
}

/*! convert integer to string
 *
 * @param[in] num number for convert
 *
 * @return pointer to converted string
 */
char *ItoA(int num) {
    char *str = nullptr;
    int i = 0;
    char sign = '+';
    if (num < 0) {
        sign = '-';
        num -= 2 * num;
    }
    for (i = 0; num > 0; i++) {
        str = (char *) realloc(str, (i + 1) * sizeof(char));
        str[i] = num % 10 + '0';
        num /= 10;
    }
    char *ss = nullptr;
    if (sign == '-') {
        ss = (char *) calloc(i + 1, sizeof(char));
        ss[0] = '-';
        for (int j = 0; j < i; j++) {
            ss[j + 1] = str[i - j - 1];
        }
    } else {
        ss = (char *) calloc(i, sizeof(char));
        for (int j = 0; j < i; j++) {
            ss[j] = str[i - 1 - j];
        }
    }
    return ss;
}

/*! print string in stdout
 *
 * @param[in] *str pointer to string
 *
 * @return 0 on success, EOF on failure
 */
int PutS(char *str) {
    assert(str != nullptr);

    int i;
    for (i = 0; str[i] != '\n' && str[i] != '\0'; i++) {
        putc(str[i], stdout);
    }
    if (str[i] == '\n') {
        if (putc('\n', stdout) == EOF) {
            return EOF;
        }
    }
    return 0;
}

/*! get string from file
 *
 * @param[in] str pointer to string
 * @param[in] count max number of characters
 * @param[in] stream file stream to read
 *
 * @return str on success, null pointer on failure
 */
char *fGetS(char *str, size_t count, FILE *stream) {
    assert(str != nullptr);
    assert(stream != nullptr);

    if (!count) return nullptr;

    for (size_t i = 0; i < count; i++) {
        *str = getc(stream);
        str++;
    }
    str++;
    *str = '\0';
    return str;
}
