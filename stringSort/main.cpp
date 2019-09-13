#include <stdio.h>
#include <malloc.h>
#include <assert.h>

/*!
	@file
    @brief String sort
*/

int getFileSize(char inPath[]);

int readFile(char inPath[], char *text, int textSize);

int strCmp(const void *string1, const void *string2);

int nRows(const char str[], int textSize);

int writeFile(char outPath[], char *text, int textSize);

int strBackCmp(const void *string1, const void *string2);

struct lineIndex {
    char *startIndex;
    char *endIndex;
};

int main() {
    char inPath[100] = R"(C:\Users\Boris\CLionProjects\stringSort\verse.txt)";
    char outPath[100] = R"(C:\Users\Boris\CLionProjects\stringSort\sortVerse.txt)";
//    printf("Enter input file path:\n");
//    gets(inPath);
//    printf("Enter output file path:\n");
//    gets(outPath);
    int textSize = getFileSize(inPath);

    char *text = (char *) calloc(textSize + 1, sizeof(char));
    char *defaultText = text;

    if (readFile(inPath, text, textSize)) return 1;
    int rows = nRows(text, textSize);
    lineIndex index[rows] = {};
    index[0].startIndex = text;
    int j = 1;
    for (int i = 0; i < textSize; i++) {
        if (text[i] == '\n') {
            index[j - 1].endIndex = &text[i] - 1;
            index[j].startIndex = &text[i] + 1;
            j++;
            text[i] = '\0';
        }
    }
    text[textSize] = '\0';
    index[j - 1].endIndex = &text[textSize] - 1;

    qsort(index, rows, sizeof(lineIndex), strCmp);
    for (int i = 0; i < rows; i++) {
        printf("%s\n", index[i].startIndex);
    }
    printf("\n\n\n\n\n");
    qsort(index, rows, sizeof(lineIndex), strBackCmp);
    for (int i = 0; i < rows; i++) {
        printf("%s\n", index[i].startIndex);
    }

    printf("\n\n\n\n");
    fwrite(defaultText, 1, textSize, stdout);
    if (writeFile(outPath, index->startIndex, textSize)) return 1;
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

    FILE *myFile;
    myFile = fopen(inPath, "r");
    if (!myFile) {
        perror("File opening failed");
        return -1;
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
 * @return 0 if read success, 1 if read fall
 */
int readFile(char inPath[], char *text, int textSize) {
    assert(inPath != "");
    assert(text != nullptr);
    assert(textSize > 0);

    FILE *myFile;
    myFile = fopen(inPath, "r");
    if (!myFile) {
        perror("File opening failed");
        return 1;
    }
    fread(text, 1, textSize, myFile);
    fclose(myFile);
    return 0;
}

/*! String comparison
 *
 * @param[in] *string1 pointer to first string
 * @param[in] *string2 pointer to second string
 *
 * @return negative value if string2 > string1, positive value if string1 > string2, 0 if string1 = string2
 */
int strCmp(const void *string1, const void *string2) {
    assert(string1 != nullptr);
    assert(string2 != nullptr);

    char *str1 = (*((lineIndex *) string1)).startIndex;
    char *str2 = (*((lineIndex *) string2)).startIndex;
    while (*str1 != '\0' || *str2 != '\0') {
        while (*str1 < 'A' || *str1 > 'z') str1++;
        while (*str2 < 'A' || *str2 > 'z') str2++;

        if (*str1 - *str2 != 0) return *str1 - *str2;
        if (*(str1 + 1) == '\0' && *(str2 + 1) == '\0') return 0;
        str1++;
        str2++;
    }
}

int strBackCmp(const void *string1, const void *string2) {
    assert(string1 != nullptr);
    assert(string2 != nullptr);

    char *str1End = (*((lineIndex *) string1)).endIndex;
    char *str2End = (*((lineIndex *) string2)).endIndex;

    char *str1Start = (*((lineIndex *) string1)).startIndex;
    char *str2Start = (*((lineIndex *) string2)).startIndex;

    while (str1End != str1Start || str2End != str2Start) {
        while (*str1End < 'A' || *str1End > 'z') str1End--;
        while (*str2End < 'A' || *str2End > 'z') str2End--;

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
 *
 * @return number of rows
 */
int nRows(const char *str, int textSize) {
    assert(str != nullptr);
    assert(textSize > 0);

    int nRows = 1;
    for (int i = 0; i < textSize; i++)
        if (str[i] == '\n') {
            nRows++;
        }
    return nRows;
}

/*! write text to file
 *
 * @param[in] outPath path to output file
 * @param[out] *text pointer to buffer for text
 * @param[in] textSize file size in bytes
 *
 * @return 0 if write success, 1 if write fall
 */
int writeFile(char outPath[], char *text, int textSize) {
    assert(outPath != "");
    assert(text != nullptr);
    assert(textSize > 0);

    FILE *myFile;
    myFile = fopen(outPath, "w");
    if (!myFile) {
        perror("File opening failed");
        return 1;
    }
    fwrite(text, 1, textSize, myFile);
    fclose(myFile);
    return 0;
}
