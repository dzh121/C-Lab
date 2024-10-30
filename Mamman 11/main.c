#include <stdio.h>
#include<ctype.h>

int main() {
	int c;
	int sentence_count, in_quotes;
	sentence_count = in_quotes = 0;
	while ((c = getchar()) != EOF) {
		if (c == '"') {
			in_quotes = !in_quotes;
			printf("%c", c);
			continue;
		}
		if (c == '.' && !in_quotes) {
			sentence_count = 0;
			printf("%c", c);
			continue;
		}
		if (isspace(c) || ispunct(c)) {
			printf("%c", c);
		} else if (!isdigit(c)) {
			sentence_count++;

			if (sentence_count == 1 && isalpha(c)) {
				printf("%c", toupper(c));
			} else if (in_quotes && isalpha(c)) {
				printf("%c", toupper(c));
			} else if (isalpha(c))
				printf("%c", tolower(c));
		}
	}

	return 0;
}
