//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_LEVELS 100

struct LogEntry {
    char level[16];
    char message[MAX_LINE_LENGTH];
    int timestamp;
};

int parse_log_recursive(FILE *file, struct LogEntry *entries, int max_entries, int current_depth) {
    if (current_depth >= max_entries || current_depth >= MAX_LEVELS) {
        return current_depth;
    }

    char buffer[MAX_LINE_LENGTH];
    if (fgets(buffer, sizeof(buffer), file) == NULL) {
        return current_depth;
    }

    if (strlen(buffer) == 0 || buffer[0] == '\n') {
        return parse_log_recursive(file, entries, max_entries, current_depth);
    }

    struct LogEntry *entry = &entries[current_depth];
    int timestamp;
    char level[16];
    char message[MAX_LINE_LENGTH];

    int parsed = sscanf(buffer, "%d %15s %1023[^\n]", &timestamp, level, message);
    if (parsed != 3) {
        return parse_log_recursive(file, entries, max_entries, current_depth);
    }

    if (timestamp < 0 || timestamp > 2147483647) {
        return parse_log_recursive(file, entries, max_entries, current_depth);
    }

    int valid_level = 0;
    const char *valid_levels[] = {"INFO", "WARNING", "ERROR", "DEBUG"};
    for (int i = 0; i < 4; i++) {
        if (strcmp(level, valid_levels[i]) == 0) {
            valid_level = 1;
            break;
        }
    }

    if (!valid_level) {
        return parse_log_recursive(file, entries, max_entries, current_depth);
    }

    entry->timestamp = timestamp;
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';

    return parse_log_recursive(file, entries, max_entries, current_depth + 1);
}

void print_entries_recursive(struct LogEntry *entries, int count, int index, const char *filter_level) {
    if (index >= count) {
        return;
    }

    if (filter_level == NULL || strcmp(entries[index].level, filter_level) == 0) {
        printf("Time: %d | Level: %s | Message: %s\n", 
               entries[index].timestamp, entries[index].level, entries[index].message);
    }

    print_entries_recursive(entries, count, index + 1, filter_level);
}

int count_level_recursive(struct LogEntry *entries, int count, int index, const char *level, int current_count) {
    if (index >= count) {
        return current_count;
    }

    if (strcmp(entries[index].level, level) == 0) {
        current_count++;
    }

    return count_level_recursive(entries, count, index + 1, level, current_count);
}

int main(void) {
    struct LogEntry entries[100];
    int entry_count = 0;

    printf("Enter log entries (format: timestamp LEVEL message).\n");
    printf("Valid levels: INFO, WARNING, ERROR, DEBUG\n");
    printf("Enter 'END' on a separate line to finish.\n");
    printf("Example: 123456789 INFO System started\n\n");

    char buffer[MAX_LINE_LENGTH];
    int line_count = 0;

    while (line_count < 100) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }

        buffer[strcspn(buffer, "\n")] = '\0';

        if (strcmp(buffer, "END") == 0) {
            break;
        }

        if (strlen(buffer) == 0) {
            continue;
        }

        if (strlen(buffer) >= MAX_LINE_LENGTH - 1) {
            printf("Line too long, skipping.\n");
            continue;
        }

        FILE *temp = tmpfile();
        if (temp == NULL) {
            printf("Error creating temporary file.\n");
            return 1;
        }

        fputs(buffer, temp);
        rewind(temp);

        int new_count = parse_log_recursive(temp, entries, 100, entry_count);
        fclose(temp);

        if (new_count > entry_count) {
            entry_count = new_count;
            printf("Entry added successfully.\n");
        } else {
            printf("Invalid format. Use: timestamp LEVEL message\n");
        }

        line_count++;
    }

    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 0;
    }

    printf("\n=== All Log Entries ===\n");
    print_entries_recursive(entries, entry_count, 0, NULL);

    printf("\n=== Statistics ===\n");
    const char *levels[] = {"INFO", "WARNING", "ERROR", "DEBUG"};
    for (int i = 0; i < 4; i++) {
        int count = count_level_recursive(entries, entry_count, 0, levels[i], 0);
        printf("%s