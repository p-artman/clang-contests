#include <stdio.h>

#define MAX_LENGTH 100

int char_to_int(int c);

int main() {
    int c;
    int num;
    int ok = 0;
    char str[MAX_LENGTH];
    int index = 0;

    // Read ASCII characters from stdin and find the maximum digit
    while ((c = getchar()) != EOF && c != '\n' && index < MAX_LENGTH - 1) {
        // str[index++] = c;  // Store the character in the string (no check)
        str[index] = c;  // Store the character in the string
        // Use sscanf to read from the string and check if the input is ok
        ok = sscanf(str + index, " %1d", &num);
        if (!ok) {
            printf("[debug] Wrong input! 'n/a' output will be printed.\n");
            printf("n/a\n");
            break;
        }
        printf("[debug] Iteration %d, number read from string: %d\n", index, num);
        index++;
    }
    str[index] = '\0';  // Null-terminate the string

    if (ok) {
        // Print the final string of ASCII values
        printf("[debug] The final string of ASCII values (ints only): %s\n", str);

        index = 0;
        int max_index = 0;
        int max = char_to_int(*str);
        for (char *p = str; *p != '\0'; p++) {
            if (max < char_to_int(*p)) {
                max = char_to_int(*p);
                max_index = index;
            }
            index++;
        }
        printf("[debug] First maximal digit: %d, index is: %d\n", max, max_index);
        printf("%d\n", max);
    }

    return 0;
}

int char_to_int(int c) { return c - '0'; }
