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
        if (strlen(buffer) > 0) {
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
    
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    filename[strcspn(filename, "\n")] = 0;
    
    if (strlen(filename) == 0) {
        printf("Invalid filename\n");
        return 1;
    }
    
    int total_entries = 0;
    struct LogEntry* entries = read_log_file(filename, &total_entries);
    
    if (!entries) {
        printf("Could not read log file: %s\n", filename);
        return 1;
    }
    
    printf("Found %d log entries\n", total_entries);
    
    struct LogEntry* temp_array = malloc(total_entries * sizeof(struct LogEntry));
    if (!temp_array) {
        printf("Memory allocation failed\n");
        free_log_entries(entries);
        return 1;
    }
    
    struct LogEntry* current = entries;
    for (int i = 0; i < total_entries && current; i++) {
        temp_array[i] = *current;
        current = current->next;
    }
    
    const char* level_names[] = {"DEBUG", "INFO", "WARN", "ERROR"};
    int level_counts[] = {0, 0, 0, 0};
    
    for (int level = 0; level < 4; level++) {
        analyze_log_recursive(temp_array, 0, total_entries - 1, level, &level_counts[level]);
        printf("%s: %d entries\n", level_names[level], level_counts[level]);
    }
    
    int total_counted = 0;
    for (int i = 0; i < 4; i++) {
        total_counted += level_counts[i];
    }
    
    printf("Uncategorized: %d entries\n", total_entries - total_counted);
    
    free(temp_array);
    free_log_entries(entries);
    
    return 0;
}