#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_WORD_LEN 32

// Function to load dictionary words into memory
void load_dictionary(char **dictionary, int *word_count) {
    FILE *file = fopen("/usr/share/dict/words", "r");
    if (!file) {
        perror("Failed to open dictionary");
        exit(EXIT_FAILURE);
    }

    char line[MAX_WORD_LEN];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;  // Remove newline character
        if (strlen(line) >= 4) {        // Only consider words with 4 or more characters
            dictionary[*word_count] = strdup(line);
            (*word_count)++;
        }
    }

    fclose(file);
}

// Function to check if an onion address has valid consecutive English words
bool is_valid_onion_address(const char *address, char **dictionary, int word_count) {
    int consecutive_word_count = 0;
    char current_word[MAX_WORD_LEN] = {0};
    int current_index = 0;

    for (int i = 0; i < strlen(address); i++) {
        current_word[current_index++] = address[i];
        current_word[current_index] = '\0';

        for (int j = 0; j < word_count; j++) {
            if (strcmp(current_word, dictionary[j]) == 0) {
                consecutive_word_count++;
                current_index = 0;  // Reset for the next word
                break;
            }
        }

        if (consecutive_word_count >= 3) {
            return true;  // If there are at least 3 consecutive words
        }
    }

    return false;
}

int main() {
    char *dictionary[100000];
    int word_count = 0;

    // Load the dictionary of words with at least 4 characters
    load_dictionary(dictionary, &word_count);

    char onion_address[256];

    // Read each onion address from stdin (from mkp224o)
    while (fgets(onion_address, sizeof(onion_address), stdin)) {
        onion_address[strcspn(onion_address, "\n")] = 0;  // Remove newline

        // Check if the onion address is valid based on consecutive words
        if (is_valid_onion_address(onion_address, dictionary, word_count)) {
            printf("Valid onion address: %s\n", onion_address);
        }
    }

    // Free allocated dictionary memory
    for (int i = 0; i < word_count; i++) {
        free(dictionary[i]);
    }

    return 0;
}