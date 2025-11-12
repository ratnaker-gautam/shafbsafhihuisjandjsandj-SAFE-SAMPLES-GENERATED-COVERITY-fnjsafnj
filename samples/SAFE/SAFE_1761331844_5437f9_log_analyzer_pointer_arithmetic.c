//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[256];
};

int parse_log_line(char *line, struct LogEntry *entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char *ptr = line;
    while (*ptr && isspace(*ptr)) ptr++;
    if (*ptr == '\0') return 0;

    char *timestamp_start = ptr;
    while (*ptr && *ptr != ' ') ptr++;
    if (*ptr == '\0') return 0;
    size_t timestamp_len = ptr - timestamp_start;
    if (timestamp_len >= sizeof(entry->timestamp)) return 0;
    strncpy(entry->timestamp, timestamp_start, timestamp_len);
    entry->timestamp[timestamp_len] = '\0';

    ptr++;
    char *level_start = ptr;
    while (*ptr && *ptr != ' ') ptr++;
    if (*ptr == '\0') return 0;
    size_t level_len = ptr - level_start;
    if (level_len >= sizeof(entry->level)) return 0;
    strncpy(entry->level, level_start, level_len);
    entry->level[level_len] = '\0';

    ptr++;
    char *message_start = ptr;
    while (*ptr && *ptr != '\n' && *ptr != '\r') ptr++;
    size_t message_len = ptr - message_start;
    if (message_len >= sizeof(entry->message)) return 0;
    strncpy(entry->message, message_start, message_len);
    entry->message[message_len] = '\0';

    return 1;
}

int count_level_occurrences(struct LogEntry *entries, int count, const char *level) {
    int occurrences = 0;
    struct LogEntry *current = entries;
    struct LogEntry *end = entries + count;
    
    while (current < end) {
        if (strcmp(current->level, level) == 0) {
            occurrences++;
        }
        current++;
    }
    return occurrences;
}

void print_level_statistics(struct LogEntry *entries, int count) {
    const char *levels[] = {"INFO", "WARNING", "ERROR", "DEBUG"};
    int num_levels = sizeof(levels) / sizeof(levels[0]);
    
    printf("Log Level Statistics:\n");
    for (int i = 0; i < num_levels; i++) {
        int count_level = count_level_occurrences(entries, count, levels[i]);
        printf("%s: %d\n", levels[i], count_level);
    }
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE)\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }
        
        if (strncmp(line, "END", 3) == 0 && (line[3] == '\n' || line[3] == '\r' || line[3] == '\0')) {
            break;
        }
        
        if (parse_log_line(line, &entries[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid log format. Skipping line.\n");
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    printf("\nParsed %d log entries:\n", entry_count);
    for (int i = 0; i < entry_count; i++) {
        printf("%d: [%s] %s: %s\n", i + 1, entries[i].timestamp, entries[i].level, entries[i].message);
    }
    
    printf("\n");
    print_level_statistics(entries, entry_count);
    
    struct LogEntry *error_ptr = entries;
    struct LogEntry *end_ptr = entries + entry_count;
    printf("\nError entries:\n");
    int error_count = 0;
    while (error_ptr < end_ptr) {
        if (strcmp(error_ptr->level, "ERROR") == 0) {
            printf("[%s] %s\n", error_ptr->timestamp, error_ptr->message);
            error_count++;
        }
        error_ptr++;
    }
    if (error_count == 0) {
        printf("No ERROR entries found.\n");
    }
    
    return 0;
}