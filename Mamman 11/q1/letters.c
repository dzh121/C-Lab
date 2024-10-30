#include <stdio.h>
#include <ctype.h>

int main() {
    /* Variable declarations and initializations */
    int c, sentence_count, in_quotes;
    sentence_count = in_quotes = 0;

    /* Print instructions */
    printf("Please enter a text for it to be reformatted according to the following rules:\n");
    printf("1. If the first character of a sentence is a lowercase letter, convert it to uppercase.\n");
    printf("2. Within quoted text, convert all lowercase letters to uppercase.\n");
    printf("3. In other text, convert all uppercase letters to lowercase (if not in rules 1 or 2).\n");
    printf("4. Digits (0–9) will not be printed and will be skipped.\n");
    printf("5. Any non-alphabetic, non-digit character will be printed as is, including punctuation and whitespace.\n");
    printf("Your text:\n");

    /* Main text processing loop */
    while ((c = getchar()) != EOF) {
        /* Toggle in_quotes when encountering quotes */
        if (c == '"') {
            in_quotes = !in_quotes;
            printf("%c", c);
            continue;
        }

        /* Reset sentence count when encountering a period outside of quotes */
        if (c == '.' && !in_quotes) {
            sentence_count = 0;
            printf("%c", c);
            continue;
        }

        /* Print punctuation and whitespace */
        if (isspace(c) || ispunct(c)) {
            printf("%c", c);
        }
        /* Skip digits */
        else if (!isdigit(c)) {
            sentence_count++; /* Count characters in the current sentence to check if its first */

            /* Rule 1: Capitalize the first alphabetic character of a sentence */
            if (sentence_count == 1 && isalpha(c)) {
                printf("%c", toupper(c));
            }
            /* Rule 2: Convert lowercase to uppercase if inside quotes */
            else if (in_quotes && isalpha(c)) {
                printf("%c", toupper(c));
            }
            /* Rule 3: Convert uppercase letters to lowercase elsewhere */
            else if (isalpha(c)) {
                printf("%c", tolower(c));
            }
        }
    }

    return 0;
}
