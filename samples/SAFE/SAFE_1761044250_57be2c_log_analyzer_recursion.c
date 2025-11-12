//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: log_analyzer
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
    if (current_depth >= max_entries || feof(file)) {
        return 0;
    }

    char buffer[MAX_LINE_LENGTH];
    if (fgets(buffer, sizeof(buffer), file) == NULL) {
        return 0;
    }

    buffer[strcspn(buffer, "\n")] = 0;

    if (strlen(buffer) == 0) {
        return parse_log_recursive(file, entries, max_entries, current_depth);
    }

    struct LogEntry *entry = &entries[current_depth];
    int parsed_fields = sscanf(buffer, "%d %15s %1023[^\n]", 
                              &entry->timestamp, entry->level, entry->message);

    if (parsed_fields != 3) {
        return parse_log_recursive(file, entries, max_entries, current_depth);
    }

    for (int i = 0; entry->level[i]; i++) {
        entry->level[i] = toupper((unsigned char)entry->level[i]);
    }

    int count = parse_log_recursive(file, entries, max_entries, current_depth + 1);
    return count + 1;
}

void print_logs_recursive(struct LogEntry *entries, int count, int index, const char *filter_level) {
    if (index >= count) {
        return;
    }

    if (filter_level == NULL || strcmp(entries[index].level, filter_level) == 0) {
        printf("Time: %d | Level: %-8s | Message: %s\n", 
               entries[index].timestamp, entries[index].level, entries[index].message);
    }

    print_logs_recursive(entries, count, index + 1, filter_level);
}

int count_logs_by_level_recursive(struct LogEntry *entries, int count, int index, const char *level) {
    if (index >= count) {
        return 0;
    }

    int current = (strcmp(entries[index].level, level) == 0) ? 1 : 0;
    return current + count_logs_by_level_recursive(entries, count, index + 1, level);
}

int main(void) {
    struct LogEntry entries[MAX_LEVELS];
    
    printf("Enter log data (format: timestamp LEVEL message). Empty line to finish:\n");
    
    int count = parse_log_recursive(stdin, entries, MAX_LEVELS, 0);
    
    if (count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }

    printf("\nParsed %d log entries:\n", count);
    print_logs_recursive(entries, count, 0, NULL);

    printf("\nLog level statistics:\n");
    const char *levels[] = {"INFO", "WARNING", "ERROR", "DEBUG"};
    for (int i = 0; i < 4; i++) {
        int level_count = count_logs_by_level_recursive(entries, count, 0, levels[i]);
        printf("%s: %d\n", levels[i], level_count);
    }

    char filter[16];
    printf("\nEnter level to filter (or 'all' for all): ");
    if (scanf("%15s", filter) == 1) {
        for (int i = 0; filter[i]; i++) {
            filter[i] = toupper((unsigned char)filter[i]);
        }
        
        printf("\nFiltered logs:\n");
        if (strcmp(filter, "ALL") == 0) {
            print_logs_recursive(entries, count, 0, NULL);
        } else {
            print_logs_recursive(entries, count, 0, filter);
        }
    }

    return 0;
}