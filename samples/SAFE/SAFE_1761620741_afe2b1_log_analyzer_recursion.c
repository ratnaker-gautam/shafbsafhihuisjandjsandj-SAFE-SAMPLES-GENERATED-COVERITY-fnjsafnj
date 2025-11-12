//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_LEVELS 100

struct LogEntry {
    char line[MAX_LINE_LENGTH];
    int level;
    struct LogEntry* next;
};

int parse_log_level(const char* line) {
    if (strstr(line, "ERROR")) return 3;
    if (strstr(line, "WARN")) return 2;
    if (strstr(line, "INFO")) return 1;
    if (strstr(line, "DEBUG")) return 0;
    return -1;
}

void analyze_log_recursive(struct LogEntry* entries, int start, int end, int target_level, int* count) {
    if (start > end || start < 0 || end < 0 || !entries || !count) return;
    
    if (start == end) {
        if (entries[start].level == target_level) {
            (*count)++;
        }
        return;
    }
    
    int mid = start + (end - start) / 2;
    if (mid < start || mid > end) return;
    
    analyze_log_recursive(entries, start, mid, target_level, count);
    analyze_log_recursive(entries, mid + 1, end, target_level, count);
}

struct LogEntry* read_log_file(const char* filename, int* total_entries) {
    if (!filename || !total_entries) return NULL;
    
    FILE* file = fopen(filename, "r");
    if (!file) return NULL;
    
    struct LogEntry* entries = NULL;
    struct LogEntry* current = NULL;
    char buffer[MAX_LINE_LENGTH];
    int count = 0;
    
    while (fgets(buffer, sizeof(buffer), file) && count < MAX_LEVELS) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n') {
            buffer[len-1] = '\0';
        }
        
        if (len >= sizeof(buffer)) continue;
        
        struct LogEntry* new_entry = malloc(sizeof(struct LogEntry));
        if (!new_entry) break;
        
        strncpy(new_entry->line, buffer, sizeof(new_entry->line) - 1);
        new_entry->line[sizeof(new_entry->line) - 1] = '\0';
        new_entry->level = parse_log_level(buffer);
        new_entry->next = NULL;
        
        if (!entries) {
            entries = new_entry;
            current = new_entry;
        } else {
            current->next = new_entry;
            current = new_entry;
        }
        count++;
    }
    
    fclose(file);
    *total_entries = count;
    return entries;
}

void free_log_entries(struct LogEntry* entries) {
    while (entries) {
        struct LogEntry* next = entries->next;
        free(entries);
        entries = next;
    }
}

int main() {
    printf("Log Analyzer - Enter log filename: ");
    char filename[256];
    
    if (!fgets(filename, sizeof(filename), stdin)) {
        printf("Input error\n");
        return 1;
    }
    
    size_t len = strlen(filename);
    if (len > 0 && filename[len-1] == '\n') {
        filename[len-1] = '\0';
    }
    
    if (len >= sizeof(filename)) {
        printf("Filename too long\n");
        return 1;
    }
    
    int total_entries = 0;
    struct LogEntry* entries = read_log_file(filename, &total_entries);
    
    if (!entries || total_entries <= 0) {
        printf("No log entries found or file error\n");
        return 1;
    }
    
    struct LogEntry* entry_array = malloc(total_entries * sizeof(struct LogEntry));
    if (!entry_array) {
        printf("Memory allocation failed\n");
        free_log_entries(entries);
        return 1;
    }
    
    struct LogEntry* current = entries;
    for (int i = 0; i < total_entries && current; i++) {
        entry_array[i] = *current;
        current = current->next;
    }
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", total_entries);
    
    const char* levels[] = {"DEBUG", "INFO", "WARN", "ERROR"};
    for (int level = 0; level <= 3; level++) {
        int count = 0;
        analyze_log_recursive(entry_array, 0, total_entries - 1, level, &count);
        printf("%s: %d\n", levels[level], count);
    }
    
    free(entry_array);
    free_log_entries(entries);
    return 0;
}