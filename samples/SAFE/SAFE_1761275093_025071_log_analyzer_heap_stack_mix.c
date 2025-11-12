//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
} LogEntry;

typedef struct {
    LogEntry *entries;
    int count;
    int capacity;
} LogCollection;

int parse_timestamp(const char *str, struct tm *tm) {
    return sscanf(str, "%d-%d-%d %d:%d:%d",
                  &tm->tm_year, &tm->tm_mon, &tm->tm_mday,
                  &tm->tm_hour, &tm->tm_min, &tm->tm_sec) == 6;
}

int is_valid_level(const char *level) {
    const char *valid_levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    for (int i = 0; i < 4; i++) {
        if (strcmp(level, valid_levels[i]) == 0) return 1;
    }
    return 0;
}

int add_log_entry(LogCollection *collection, const char *line) {
    if (collection->count >= collection->capacity) {
        int new_cap = collection->capacity * 2;
        if (new_cap <= 0) new_cap = 16;
        LogEntry *new_entries = realloc(collection->entries, new_cap * sizeof(LogEntry));
        if (!new_entries) return 0;
        collection->entries = new_entries;
        collection->capacity = new_cap;
    }

    LogEntry *entry = &collection->entries[collection->count];
    struct tm tm = {0};
    
    if (sscanf(line, "%31s %15s %255[^\n]", 
               entry->timestamp, entry->level, entry->message) != 3) {
        return 0;
    }

    if (!parse_timestamp(entry->timestamp, &tm)) return 0;
    if (!is_valid_level(entry->level)) return 0;
    if (strlen(entry->message) == 0) return 0;

    collection->count++;
    return 1;
}

void analyze_logs(LogCollection *collection) {
    int level_counts[4] = {0};
    const char *levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    
    for (int i = 0; i < collection->count; i++) {
        for (int j = 0; j < 4; j++) {
            if (strcmp(collection->entries[i].level, levels[j]) == 0) {
                level_counts[j]++;
                break;
            }
        }
    }

    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", collection->count);
    for (int i = 0; i < 4; i++) {
        printf("%s: %d\n", levels[i], level_counts[i]);
    }
}

int main(void) {
    char buffer[MAX_LINE_LEN];
    LogCollection collection = {0};
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Enter 'END' on a separate line to finish input\n");

    while (fgets(buffer, sizeof(buffer), stdin)) {
        if (strlen(buffer) > 0 && buffer[strlen(buffer)-1] == '\n') {
            buffer[strlen(buffer)-1] = '\0';
        }
        
        if (strcmp(buffer, "END") == 0) break;
        
        if (strlen(buffer) == 0) continue;
        
        if (!add_log_entry(&collection, buffer)) {
            printf("Invalid log entry: %s\n", buffer);
        }
    }

    if (collection.count > 0) {
        analyze_logs(&collection);
    } else {
        printf("No valid log entries to analyze.\n");
    }

    free(collection.entries);
    return 0;
}