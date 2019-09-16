#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include <errno.h>

/*!
	@file
    @brief String sort
*/

enum errorsCode {
    TESTS_FAILED = 10
};

struct lineIndex {
    char *startIndex;
    char *endIndex;
};

int getFileSize(char inPath[]);

int readFile(char inPath[], char *text, size_t textSize);

int strCmpForStruct(const void *string1, const void *string2);

int strCmp(char *str1, char *str2);

int nRows(const char str[], size_t textSize, char element);

int writeFile(char outPath[], lineIndex *text, size_t rows);

int writeFile(char outPath[], char **text, size_t rows);

int strBackCmpForStruct(const void *string1, const void *string2);

int strBackCmp(const char *str1Start, char *str1End, const char *str2Start, char *str2End);

void fillIndex(lineIndex *index, char *text, size_t textSize);

void getIndexCopy(lineIndex *index, lineIndex *indexCopy, size_t rows);

int tests();

bool nRowsChecker(char *str, size_t textSize, int awaitN);

bool strCmpChecker(char *str1, char *str2, int awaitVal);

bool strBackCmpChecker(const char *str1Start, char *str1End, const char *str2Start, char *str2End, int awaitVal);

int main() {
    int failedTests = tests();
    printf("\n%d tests failed\n", failedTests);
    if (failedTests) return TESTS_FAILED;

    char inPath[FILENAME_MAX] = R"(..\verse.txt)";
    char outPath[FILENAME_MAX] = R"(..\sortVerse.txt)";

//    printf("Enter input file path:\n");
//    gets(inPath);
//    printf("Enter output file path:\n");
//    gets(outPath);

    int textSize = getFileSize(inPath);

    char *text = (char *) calloc(textSize + 1, sizeof(char));

    if (readFile(inPath, text, textSize)) return errno;
    int rows = nRows(text, textSize, '\n');

    lineIndex index[rows] = {};
    lineIndex defaultIndex[rows] = {};

    fillIndex(index, text, textSize);
    getIndexCopy(index, defaultIndex, rows);

    qsort(index, rows, sizeof(lineIndex), strCmpForStruct);
    if (writeFile(outPath, index, rows)) return errno;

    qsort(index, rows, sizeof(lineIndex), strBackCmpForStruct);
    if (writeFile(outPath, index, rows)) return errno;

    if (writeFile(outPath, defaultIndex, rows)) return errno;

    free(text);
    return 0;
}

/*! get file size in bytes
 *
 * @param[in] inPath path to input file
 *
 * @return file size in bytes
 */
int getFileSize(char *inPath) {
    assert(inPath != nullptr);

    FILE *myFile = fopen(inPath, "r");
    if (!myFile) {
        perror("File opening failed");
        return errno;
    }
    fseek(myFile, 0, SEEK_END);
    int textSize = ftell(myFile);
    fclose(myFile);
    return textSize;
}

/*! read text from file
 *
 * @param[in] inPath path to input file
 * @param[out] *text pointer to buffer for text
 * @param[in] textSize file size in bytes
 *
 * @return 0 if read success, errno if read fall
 */
int readFile(char inPath[], char *text, size_t textSize) {
    assert(inPath != "");
    assert(text != nullptr);

    FILE *myFile = fopen(inPath, "r");
    if (!myFile) {
        perror("File opening failed");
        return errno;
    }
    fread(text, 1, textSize, myFile);
    fclose(myFile);
    return 0;
}

/*! String comparison
 *
 * @param[in] *string1 pointer to struct with pointers to string
 * @param[in] *string2 pointer to struct with pointers to string
 *
 * @return negative value if string2 > string1, positive value if string1 > string2, 0 if string1 = string2
 */
int strCmpForStruct(const void *string1, const void *string2) {
    assert(string1 != nullptr);
    assert(string2 != nullptr);

    char *str1 = (*((lineIndex *) string1)).startIndex;
    char *str2 = (*((lineIndex *) string2)).startIndex;

    return strCmp(str1, str2);
}

/*! strings compare
 *
 * @param[in] str1 Pointer to str1
 * @param[in] str2 Pointer to str2
 * @return negative value if string2 > string1, positive value if string1 > string2, 0 if string1 = string2
 */
int strCmp(char *str1, char *str2) {
    assert(str1 != nullptr);
    assert(str2 != nullptr);

    while (*str1 != '\0' || *str2 != '\0') {
        while (*str1 < 'A' || *str1 > 'z') str1++;
        while (*str2 < 'A' || *str2 > 'z') str2++;

        if (*str1 < 'a') *str1 += ('a' - 'A');
        if (*str2 < 'a') *str2 += ('a' - 'A');

        if (*str1 - *str2 != 0) return *str1 - *str2;
        if (*(str1 + 1) == '\0' && *(str2 + 1) == '\0') return 0;
        str1++;
        str2++;
    }
}

/*! String back comparison
 *
 * @param[in] *string1 pointer to struct with pointers to string
 * @param[in] *string2 pointer to struct with pointers to string
 *
 * @return negative value if string2 > string1, positive value if string1 > string2, 0 if string1 = string2
 */
int strBackCmpForStruct(const void *string1, const void *string2) {
    assert(string1 != nullptr);
    assert(string2 != nullptr);

    char *str1End = (*((lineIndex *) string1)).endIndex;
    char *str2End = (*((lineIndex *) string2)).endIndex;

    char *str1Start = (*((lineIndex *) string1)).startIndex;
    char *str2Start = (*((lineIndex *) string2)).startIndex;

    return strBackCmp(str1Start, str1End, str2Start, str2End);
}

/*! back compare for strings
 *
 * @param[in] str1Start Pointer to first letter in str1
 * @param[in] str1End Pointer to last letter in str1
 * @param[in] str2Start Pointer to first letter in str2
 * @param[in] str2End Pointer to last letter in str2
 * @return n > 0 negative value if string2 > string1, positive value if string1 > string2, 0 if string1 = string2
 */
int strBackCmp(const char *str1Start, char *str1End, const char *str2Start, char *str2End) {
    while (str1End != str1Start || str2End != str2Start) {
        while (*str1End < 'A' || *str1End > 'z') str1End--;
        while (*str2End < 'A' || *str2End > 'z') str2End--;

        if (*str1End < 'a') *str1End += ('a' - 'A');
        if (*str2End < 'a') *str2End += ('a' - 'A');

        if (*str1End - *str2End != 0) return *str1End - *str2End;
        if (str1End - 1 == str1Start && str2End - 1 == str2Start) return 0;

        str1End--;
        str2End--;
    }
}

/*! Count rows in string
 *
 * @param[in] str input string
 * @param[in] textSize size of input string in bytes
 * @param[in] element element to compare
 *
 * @return number of rows
 */
int nRows(const char *str, size_t textSize, char element) {
    assert(str != nullptr);

    int nRows = 1;
    for (size_t i = 0; i < textSize; i++)
        if (str[i] == element) {
            nRows++;
        }
    return nRows;
}

/*! write text to file
 *
 * @param[in] outPath path to output file
 * @param[out] *text pointer to struct with pointers to strings
 * @param[in] textSize file size in bytes
 *
 * @return 0 if write success, errno if write fall
 */
int writeFile(char outPath[], lineIndex *text, size_t rows) {
    assert(outPath != nullptr);
    assert(text != nullptr);

    FILE *myFile = fopen(outPath, "a");
    if (!myFile) {
        perror("File opening failed");
        return errno;
    }

    for (size_t i = 0; i < rows; i++) {
        fprintf(myFile, "%s\n", text[i].startIndex);
    }
    fprintf(myFile, "\n\n");
    fclose(myFile);
    return 0;
}

/*! write text to file
 *
 * @param[in] outPath path to output file
 * @param[out] *text pointer to struct with pointers to strings
 * @param[in] textSize file size in bytes
 *
 * @return 0 if write success, 1 if write fall
 */
int writeFile(char outPath[], char **text, size_t rows) {
    assert(outPath != nullptr);
    assert(text != nullptr);

    FILE *myFile = fopen(outPath, "a");
    if (!myFile) {
        perror("File opening failed");
        return errno;
    }

    for (size_t i = 0; i < rows; i++) {
        fprintf(myFile, "%s\n", text[i]);
    }
    fprintf(myFile, "\n\n");
    fclose(myFile);
    return 0;
}

/*! fill index by pointers on rows
 *
 * @param index
 * @param text
 * @param textSize
 */
void fillIndex(lineIndex *index, char *text, size_t textSize) {
    assert(index != nullptr);
    assert(text != nullptr);

    index[0].startIndex = text;
    int lines = 1;
    for (size_t i = 0; i < textSize; i++) {
        if (text[i] == '\n') {
            index[lines - 1].endIndex = &text[i] - 1;
            index[lines].startIndex = &text[i] + 1;
            lines++;
            text[i] = '\0';
        }
    }
    text[textSize] = '\0';
    index[lines - 1].endIndex = &text[textSize] - 1;
}

/*! get copy of index
 *
 * @param[in] index Pointer in index for coping
 * @param[out] indexCopy Pointer on copied index
 * @param[in] rows number of rows
 */
void getIndexCopy(lineIndex *index, lineIndex *indexCopy, size_t rows) {
    assert(index != nullptr);
    assert(indexCopy != nullptr);

    for (size_t i = 0; i < rows; i++) {
        indexCopy[i].startIndex = index[i].startIndex;
        indexCopy[i].endIndex = index[i].endIndex;
    }
}

int tests() {
    int errors = 0;

    printf("1 ");
    !nRowsChecker("\n\n\n\nskd", 7, 5) ? errors++ : NULL;

    printf("2 ");
    !nRowsChecker("ggg\nd\nd\ndnn", 11, 4) ? errors++ : NULL;

    printf("3 ");
    !nRowsChecker("\n\r\0\0\n\r\n", 7, 4) ? errors++ : NULL;

    printf("4 ");
    !strCmpChecker("aaaa", "abffgaa", 2) ? errors++ : NULL;

    printf("5 ");
    !strCmpChecker("aaaabb", "aaaabaa", 1) ? errors++ : NULL;

    printf("6 ");
    !strCmpChecker("aaaa", "aaaa", 0) ? errors++ : NULL;

    printf("7 ");
    char *str1 = "aaba";
    char *str2 = "bbba";
    !strBackCmpChecker(str1, str1 + 3, str2, str2 + 3, 2) ? errors++ : NULL;

    printf("8 ");
    str1 = "fjf,,sa,,s,z,s,";
    str2 = "vff$@#*^e!s..;";
    !strBackCmpChecker(str1, str1 + 14, str2, str2 + 13, 1) ? errors++ : NULL;

    printf("9 ");
    str1 = "fjf,,sa,,s,z,s,";
    str2 = "fjf(sa,,sz),s";
    !strBackCmpChecker(str1, str1 + 14, str2, str2 + 13, 0) ? errors++ : NULL;
    return errors;
}

bool nRowsChecker(char *str, size_t textSize, int awaitN) {
    int gotN = nRows(str, textSize, '\n');
    if (gotN != awaitN) {
        printf("Error\n"
               "Test failed: str = %s, text size = %lld\n"
               "  Got:   gotN = %d\n"
               "Await: awaitN = %d\n\n",
               str, textSize, gotN, awaitN);
        return false;
    }
    printf("success\n");
    return true;
}

bool strCmpChecker(char *str1, char *str2, int awaitVal) {
    int gotVal = strCmp(str1, str2);
    if (gotVal > 0 && awaitVal == 2) {
        printf("Error\n"
               "Test failed: str1 = %s\n"
               "             str2 = %s\n"
               "  Got:   gotVal = str1 bigger\n"
               "Await: awaitVal = str2 bigger\n\n",
               str1, str2);
        return false;
    }
    if (gotVal < 0 && awaitVal == 1) {
        printf("Error\n"
               "Test failed: str1 = %s\n"
               "             str2 = %s\n"
               "  Got: str2 bigger\n"
               "Await: str1 bigger\n\n",
               str1, str2);
        return false;
    }
    if (gotVal != 0 && awaitVal == 0) {
        printf("Error\n"
               "Test failed: str1 = %s\n"
               "             str2 = %s\n"
               "  Got: str1 != str2\n, gotVal = %d"
               "Await: str1 = str2\n\n",
               str1, str2, gotVal);
        return false;
    }
    printf("success\n");
    return true;
}

bool strBackCmpChecker(const char *str1Start, char *str1End, const char *str2Start, char *str2End, int awaitVal) {
    int gotVal = strBackCmp(str1Start, str1End, str2Start, str2End);
    if (gotVal > 0 && awaitVal == 2) {
        printf("Error\n"
               "Test failed: str1Start = %s, str1End = %s\n"
               "             str2Start = %s, str2End = %s\n"
               "  Got: str1 bigger\n"
               "Await: str2 bigger\n\n",
               str1Start, str1End, str2Start, str2End);
        return false;
    }
    if (gotVal < 0 && awaitVal == 1) {
        printf("Error\n"
               "Test failed: str1Start = %s, str1End = %s\n"
               "             str2Start = %s, str2End = %s\n"
               "  Got: str2 bigger\n"
               "Await: str1 bigger\n\n",
               str1Start, str1End, str2Start, str2End);
        return false;
    }
    if (gotVal != 0 && awaitVal == 0) {
        printf("Error\n"
               "Test failed: str1Start = %s, str1End = %c\n"
               "             str2Start = %s, str2End = %c\n"
               "  Got: str1 != str2, gotVal = %d\n"
               "Await: str1 = str2\n\n",
               str1Start, *str1End, str2Start, *str2End, gotVal);
        return false;
    }
    printf("success\n");
    return true;
}
