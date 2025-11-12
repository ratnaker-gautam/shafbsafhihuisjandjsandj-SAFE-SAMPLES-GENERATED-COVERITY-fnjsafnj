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
    if (start > end || start < 0 || end < 0) {
        return;
    }
    
    if (entries[start].level == target_level) {
        (*count)++;
    }
    
    if (start == end) {
        return;
    }
    
    analyze_log_recursive(entries, start + 1, end, target_level, count);
}

struct LogEntry* read_log_file(const char* filename, int* total_entries) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        return NULL;
    }
    
    struct LogEntry* entries = NULL;
    struct LogEntry* current = NULL;
    char buffer[MAX_LINE_LENGTH];
    int count = 0;
    
    while (fgets(buffer, sizeof(buffer), file) && count < MAX_LEVELS) {
        if (strlen(buffer) == 0) continue;
        
        buffer[strcspn(buffer, "\n")] = 0;
        
        struct LogEntry* new_entry = malloc(sizeof(struct LogEntry));
        if (!new_entry) {
            break;
        }
        
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

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <logfile>\n", argv[0]);
        return 1;
    }
    
    int total_entries = 0;
    struct LogEntry* entries = read_log_file(argv[1], &total_entries);
    
    if (!entries || total_entries == 0) {
        printf("No log entries found or file cannot be read.\n");
        return 1;
    }
    
    struct LogEntry* temp_array = malloc(total_entries * sizeof(struct LogEntry));
    if (!temp_array) {
        free_log_entries(entries);
        return 1;
    }
    
    struct LogEntry* current = entries;
    for (int i = 0; i < total_entries && current; i++) {
        memcpy(&temp_array[i], current, sizeof(struct LogEntry));
        current = current->next;
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", total_entries);
    
    int error_count = 0;
    int warn_count = 0;
    int info_count = 0;
    int debug_count = 0;
    int unknown_count = 0;
    
    analyze_log_recursive(temp_array, 0, total_entries - 1, 3, &error_count);
    analyze_log_recursive(temp_array, 0, total_entries - 1, 2, &warn_count);
    analyze_log_recursive(temp_array, 0, total_entries - 1, 1, &info_count);
    analyze_log_recursive(temp_array, 0, total_entries - 1, 0, &debug_count);
    
    for (int i = 0; i < total_entries; i++) {
        if (temp_array[i].level == -1) {
            unknown_count++;
        }
    }
    
    printf("ERROR: %d\n", error_count);
    printf("WARN:  %d\n", warn_count);
    printf("INFO:  %d\n", info_count);
    printf("DEBUG: %d\n", debug_count);
    printf("UNKNOWN: %d\n", unknown_count);
    
    free(temp_array);
    free_log_entries(entries);
    
    return 0;
}