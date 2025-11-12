//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 256
#define MAX_ENTRIES 100

typedef struct {
    char timestamp[20];
    char level[10];
    char message[128];
} LogEntry;

typedef struct {
    LogEntry* entries;
    int count;
    int capacity;
} LogCollection;

int parse_timestamp(const char* str, struct tm* tm) {
    return sscanf(str, "%d-%d-%d %d:%d:%d",
                  &tm->tm_year, &tm->tm_mon, &tm->tm_mday,
                  &tm->tm_hour, &tm->tm_min, &tm->tm_sec) == 6;
}

int validate_log_level(const char* level) {
    const char* valid_levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    for (int i = 0; i < 4; i++) {
        if (strcmp(level, valid_levels[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

LogCollection* create_log_collection(int capacity) {
    if (capacity <= 0 || capacity > 10000) return NULL;
    
    LogCollection* collection = malloc(sizeof(LogCollection));
    if (!collection) return NULL;
    
    collection->entries = malloc(capacity * sizeof(LogEntry));
    if (!collection->entries) {
        free(collection);
        return NULL;
    }
    
    collection->count = 0;
    collection->capacity = capacity;
    return collection;
}

void destroy_log_collection(LogCollection* collection) {
    if (collection) {
        free(collection->entries);
        free(collection);
    }
}

int add_log_entry(LogCollection* collection, const char* timestamp, 
                  const char* level, const char* message) {
    if (!collection || !timestamp || !level || !message) return 0;
    if (collection->count >= collection->capacity) return 0;
    
    struct tm tm = {0};
    if (!parse_timestamp(timestamp, &tm)) return 0;
    if (!validate_log_level(level)) return 0;
    
    size_t msg_len = strlen(message);
    if (msg_len == 0 || msg_len >= 128) return 0;
    
    LogEntry* entry = &collection->entries[collection->count];
    
    strncpy(entry->timestamp, timestamp, 19);
    entry->timestamp[19] = '\0';
    
    strncpy(entry->level, level, 9);
    entry->level[9] = '\0';
    
    strncpy(entry->message, message, 127);
    entry->message[127] = '\0';
    
    collection->count++;
    return 1;
}

void analyze_logs(LogCollection* collection) {
    if (!collection || collection->count == 0) {
        printf("No logs to analyze\n");
        return;
    }
    
    int level_counts[4] = {0};
    const char* levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    
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

int read_log_file(const char* filename, LogCollection* collection) {
    if (!filename || !collection) return 0;
    
    FILE* file = fopen(filename, "r");
    if (!file) return 0;
    
    char line[MAX_LINE_LEN];
    int lines_read = 0;
    
    while (fgets(line, sizeof(line), file) && lines_read < 50) {
        char timestamp[20], level[10], message[128];
        
        if (sscanf(line, "%19s %9s %127[^\n]", timestamp, level, message) == 3) {
            if (add_log_entry(collection, timestamp, level, message)) {
                lines_read++;
            }
        }
    }
    
    fclose(file);
    return lines_read;
}

int main() {
    LogCollection* logs = create_log_collection(50);
    if (!logs) {
        printf("Failed to create log collection\n");
        return 1;
    }
    
    printf("Enter log filename: ");
    char filename[100];
    if (scanf("%99s", filename) != 1) {
        printf("Invalid filename\n");
        destroy_log_collection(logs);
        return 1;
    }
    
    int entries_read = read_log_file(filename, logs);
    if (entries_read == 0) {
        printf("No valid log entries found or file not found\n");
        printf("Using sample data for demonstration\n");
        
        add_log_entry(logs, "2024-01-15 10:30:00", "INFO", "System started");
        add_log_entry(logs, "2024-01-15 10:31:15", "WARN", "High memory usage");
        add_log_entry(logs, "