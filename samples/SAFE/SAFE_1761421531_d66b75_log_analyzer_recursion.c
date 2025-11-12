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
    if (start > end || start < 0 || end < 0 || count == NULL) return;
    
    if (entries[start].level == target_level) {
        (*count)++;
    }
    
    analyze_log_recursive(entries, start + 1, end, target_level, count);
}

struct LogEntry* read_log_file(const char* filename, int* total_entries) {
    if (filename == NULL || total_entries == NULL) return NULL;
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) return NULL;
    
    struct LogEntry* entries = NULL;
    struct LogEntry* current = NULL;
    char buffer[MAX_LINE_LENGTH];
    int count = 0;
    
    while (fgets(buffer, sizeof(buffer), file) != NULL && count < MAX_LEVELS) {
        buffer[strcspn(buffer, "\n")] = '\0';
        
        if (strlen(buffer) == 0) continue;
        
        struct LogEntry* new_entry = malloc(sizeof(struct LogEntry));
        if (new_entry == NULL) break;
        
        strncpy(new_entry->line, buffer, sizeof(new_entry->line) - 1);
        new_entry->line[sizeof(new_entry->line) - 1] = '\0';
        new_entry->level = parse_log_level(buffer);
        new_entry->next = NULL;
        
        if (entries == NULL) {
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
    while (entries != NULL) {
        struct LogEntry* next = entries->next;
        free(entries);
        entries = next;
    }
}

int count_entries_recursive(struct LogEntry* entries, int target_level) {
    if (entries == NULL) return 0;
    
    int count = (entries->level == target_level) ? 1 : 0;
    return count + count_entries_recursive(entries->next, target_level);
}

int main() {
    printf("Log Analyzer - Enter log filename: ");
    char filename[256];
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    filename[strcspn(filename, "\n")] = '\0';
    
    if (strlen(filename) == 0) {
        printf("Invalid filename\n");
        return 1;
    }
    
    int total_entries;
    struct LogEntry* entries = read_log_file(filename, &total_entries);
    
    if (entries == NULL) {
        printf("Could not read log file: %s\n", filename);
        return 1;
    }
    
    printf("Log file loaded with %d entries\n", total_entries);
    printf("\nAnalysis Results:\n");
    
    const char* level_names[] = {"DEBUG", "INFO", "WARN", "ERROR"};
    for (int level = 0; level <= 3; level++) {
        int count = count_entries_recursive(entries, level);
        printf("%s: %d entries\n", level_names[level], count);
    }
    
    struct LogEntry* current = entries;
    int error_count = 0;
    while (current != NULL) {
        if (current->level == 3) {
            if (error_count < 5) {
                printf("ERROR entry: %s\n", current->line);
            }
            error_count++;
        }
        current = current->next;
    }
    
    if (error_count > 5) {
        printf("... and %d more ERROR entries\n", error_count - 5);
    }
    
    free_log_entries(entries);
    return 0;
}