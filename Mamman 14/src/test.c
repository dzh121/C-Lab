#include <stdio.h>
#include <string.h>

#define EXPECTED_LINES 35

const char *expected_output[EXPECTED_LINES] = {
    "     25 9",
    "0000100 0b680c",
    "0000101 000412",
    "0000102 340004",
    "0000103 000184",
    "0000104 111e04",
    "0000105 000001",
    "0000106 141e1c",
    "0000107 036804",
    "0000108 00042a",
    "0000109 0b3c14",
    "0000110 240814",
    "0000111 0003e2",
    "0000112 050004",
    "0000113 00042a",
    "0000114 ffffd4",
    "0000115 241014",
    "0000116 00004c",
    "0000117 140824",
    "0000118 000001",
    "0000119 24100c",
    "0000120 ffff7c",
    "0000121 09080c",
    "0000122 000001",
    "0000123 000001",
    "0000124 3c0004",
    "0000125 000061",
    "0000126 000062",
    "0000127 000063",
    "0000128 000064",
    "0000129 000000",
    "0000130 000006",
    "0000131 fffff7",
    "0000132 ffff9c",
    "0000133 00001f"
};

int main() {
    FILE *file = fopen("test.ob", "r");
    if (!file) {
        perror("Failed to open test.ob");
        return 1;
    }

    char line[256];
    int line_number = 0;
    while (fgets(line, sizeof(line), file)) {
        // Remove trailing newline
        line[strcspn(line, "\n")] = '\0';

        if (line_number >= EXPECTED_LINES) {
            printf("Error: Extra line in test.ob: %s\n", line);
            fclose(file);
            return 1;
        }

        if (strcmp(line, expected_output[line_number]) != 0) {
            printf("Mismatch on line %d:\nExpected: %s\nFound: %s\n",
                   line_number + 1, expected_output[line_number], line);
            fclose(file);
            return 1;
        }

        line_number++;
    }

    fclose(file);

    if (line_number < EXPECTED_LINES) {
        printf("Error: Missing lines in test.ob. Expected %d lines, but found %d.\n",
               EXPECTED_LINES, line_number);
        return 1;
    }

    printf("test.ob matches the expected output.\n");
    return 0;
}
