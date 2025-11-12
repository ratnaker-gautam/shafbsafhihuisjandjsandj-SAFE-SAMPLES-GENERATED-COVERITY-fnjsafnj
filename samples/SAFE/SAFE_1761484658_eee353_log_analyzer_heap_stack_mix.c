//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[20];
    char level[10];
    char message[256];
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
        if (strcmp(level, valid_levels[i]) == 0) return 1;
    }
    return 0;
}

LogCollection* create_log_collection(int initial_capacity) {
    if (initial_capacity <= 0) return NULL;
    
    LogCollection* collection = malloc(sizeof(LogCollection));
    if (!collection) return NULL;
    
    collection->entries = malloc(sizeof(LogEntry) * initial_capacity);
    if (!collection->entries) {
        free(collection);
        return NULL;
    }
    
    collection->count = 0;
    collection->capacity = initial_capacity;
    return collection;
}

void destroy_log_collection(LogCollection* collection) {
    if (collection) {
        free(collection->entries);
        free(collection);
    }
}

int add_log_entry(LogCollection* collection, const char* timestamp, const char* level, const char* message) {
    if (!collection || !timestamp || !level || !message) return 0;
    if (collection->count >= collection->capacity) return 0;
    
    struct tm tm;
    if (!parse_timestamp(timestamp, &tm)) return 0;
    if (!validate_log_level(level)) return 0;
    if (strlen(message) >= 256) return 0;
    
    LogEntry* entry = &collection->entries[collection->count];
    
    strncpy(entry->timestamp, timestamp, 19);
    entry->timestamp[19] = '\0';
    
    strncpy(entry->level, level, 9);
    entry->level[9] = '\0';
    
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    
    collection->count++;
    return 1;
}

void analyze_logs(LogCollection* collection) {
    if (!collection || collection->count == 0) return;
    
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
    int line_count = 0;
    
    while (fgets(line, sizeof(line), file) && line_count < collection->capacity) {
        char timestamp[20], level[10], message[256];
        
        if (sscanf(line, "%19s %9s %255[^\n]", timestamp, level, message) == 3) {
            if (add_log_entry(collection, timestamp, level, message)) {
                line_count++;
            }
        }
    }
    
    fclose(file);
    return line_count;
}

int main() {
    LogCollection* collection = create_log_collection(MAX_ENTRIES);
    if (!collection) {
        printf("Failed to create log collection\n");
        return 1;
    }
    
    char filename[256];
    printf("Enter log file name: ");
    if (scanf("%255s", filename) != 1) {
        printf("Invalid input\n");
        destroy_log_collection(collection);
        return 1;
    }
    
    int entries_read = read_log_file(filename, collection);
    if (entries_read == 0) {
        printf("No valid log entries found or file not accessible\n");
        destroy_log_collection(collection);
        return 1;
    }
    
    printf("Successfully read %d log entries\n", entries_read);
    analyze_logs(collection);
    
    destroy_log_collection(collection);
    return 0;
}