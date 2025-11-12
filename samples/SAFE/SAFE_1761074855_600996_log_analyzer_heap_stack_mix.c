//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[20];
    char level[10];
    char message[256];
} LogEntry;

typedef struct {
    LogEntry *entries;
    size_t count;
    size_t capacity;
} LogCollection;

int parse_timestamp(const char *str, struct tm *tm) {
    return sscanf(str, "%4d-%2d-%2d %2d:%2d:%2d",
                  &tm->tm_year, &tm->tm_mon, &tm->tm_mday,
                  &tm->tm_hour, &tm->tm_min, &tm->tm_sec) == 6;
}

int validate_log_level(const char *level) {
    const char *valid_levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    for (size_t i = 0; i < 4; i++) {
        if (strcmp(level, valid_levels[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int add_log_entry(LogCollection *collection, const char *line) {
    if (collection->count >= collection->capacity) {
        size_t new_capacity = collection->capacity * 2;
        if (new_capacity < collection->capacity) return 0;
        LogEntry *new_entries = realloc(collection->entries, new_capacity * sizeof(LogEntry));
        if (!new_entries) return 0;
        collection->entries = new_entries;
        collection->capacity = new_capacity;
    }

    LogEntry *entry = &collection->entries[collection->count];
    struct tm tm = {0};
    
    if (sscanf(line, "%19s %9s %255[^\n]", 
               entry->timestamp, entry->level, entry->message) != 3) {
        return 0;
    }

    if (!parse_timestamp(entry->timestamp, &tm)) {
        return 0;
    }

    if (!validate_log_level(entry->level)) {
        return 0;
    }

    if (strlen(entry->message) == 0) {
        return 0;
    }

    collection->count++;
    return 1;
}

void analyze_logs(LogCollection *collection) {
    int level_counts[4] = {0};
    const char *levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    
    for (size_t i = 0; i < collection->count; i++) {
        const char *level = collection->entries[i].level;
        for (int j = 0; j < 4; j++) {
            if (strcmp(level, levels[j]) == 0) {
                level_counts[j]++;
                break;
            }
        }
    }

    printf("Log Analysis Results:\n");
    printf("Total entries: %zu\n", collection->count);
    for (int i = 0; i < 4; i++) {
        printf("%s: %d\n", levels[i], level_counts[i]);
    }
}

int main(void) {
    LogCollection collection = {0};
    collection.capacity = 10;
    collection.entries = malloc(collection.capacity * sizeof(LogEntry));
    if (!collection.entries) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    char line[MAX_LINE_LEN];
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Enter 'END' on a separate line to finish input\n");

    while (fgets(line, sizeof(line), stdin)) {
        if (strlen(line) > 0 && line[strlen(line) - 1] == '\n') {
            line[strlen(line) - 1] = '\0';
        }
        
        if (strcmp(line, "END") == 0) {
            break;
        }

        if (strlen(line) == 0) {
            continue;
        }

        if (!add_log_entry(&collection, line)) {
            fprintf(stderr, "Invalid log entry: %s\n", line);
        }
    }

    if (collection.count > 0) {
        analyze_logs(&collection);
    } else {
        printf("No valid log entries to analyze\n");
    }

    free(collection.entries);
    return 0;
}