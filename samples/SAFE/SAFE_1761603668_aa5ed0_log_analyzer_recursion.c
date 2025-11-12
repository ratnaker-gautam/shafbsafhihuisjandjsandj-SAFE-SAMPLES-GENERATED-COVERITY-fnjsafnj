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
    if (strstr(line, "[ERROR]")) return 3;
    if (strstr(line, "[WARN]")) return 2;
    if (strstr(line, "[INFO]")) return 1;
    if (strstr(line, "[DEBUG]")) return 0;
    return -1;
}

void analyze_log_recursive(struct LogEntry* entries, int start, int end, int current_level) {
    if (start > end || current_level >= MAX_LEVELS) return;
    
    int error_count = 0;
    int warn_count = 0;
    int info_count = 0;
    int debug_count = 0;
    
    for (int i = start; i <= end; i++) {
        if (entries[i].level == 3) error_count++;
        else if (entries[i].level == 2) warn_count++;
        else if (entries[i].level == 1) info_count++;
        else if (entries[i].level == 0) debug_count++;
    }
    
    for (int i = 0; i < current_level; i++) printf("  ");
    printf("Level %d: ERRORS=%d, WARNINGS=%d, INFO=%d, DEBUG=%d\n",
           current_level, error_count, warn_count, info_count, debug_count);
    
    if (start == end) return;
    
    int mid = start + (end - start) / 2;
    if (mid > start && mid < end) {
        analyze_log_recursive(entries, start, mid, current_level + 1);
        analyze_log_recursive(entries, mid + 1, end, current_level + 1);
    }
}

int read_log_file(const char* filename, struct LogEntry** entries) {
    FILE* file = fopen(filename, "r");
    if (!file) return -1;
    
    int capacity = 100;
    int count = 0;
    *entries = malloc(capacity * sizeof(struct LogEntry));
    if (!*entries) {
        fclose(file);
        return -1;
    }
    
    char buffer[MAX_LINE_LENGTH];
    while (fgets(buffer, sizeof(buffer), file)) {
        if (count >= capacity) {
            capacity *= 2;
            if (capacity > 10000) capacity = 10000;
            struct LogEntry* new_entries = realloc(*entries, capacity * sizeof(struct LogEntry));
            if (!new_entries) break;
            *entries = new_entries;
        }
        
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n') buffer[len-1] = '\0';
        
        strncpy((*entries)[count].line, buffer, MAX_LINE_LENGTH - 1);
        (*entries)[count].line[MAX_LINE_LENGTH - 1] = '\0';
        (*entries)[count].level = parse_log_level(buffer);
        (*entries)[count].next = NULL;
        count++;
    }
    
    fclose(file);
    return count;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <logfile>\n", argv[0]);
        return 1;
    }
    
    struct LogEntry* entries = NULL;
    int entry_count = read_log_file(argv[1], &entries);
    
    if (entry_count <= 0) {
        printf("Error: Could not read log file or file is empty\n");
        if (entries) free(entries);
        return 1;
    }
    
    printf("Log Analysis Summary (recursive breakdown):\n");
    analyze_log_recursive(entries, 0, entry_count - 1, 0);
    
    int total_errors = 0;
    int total_warnings = 0;
    int total_info = 0;
    int total_debug = 0;
    
    for (int i = 0; i < entry_count; i++) {
        if (entries[i].level == 3) total_errors++;
        else if (entries[i].level == 2) total_warnings++;
        else if (entries[i].level == 1) total_info++;
        else if (entries[i].level == 0) total_debug++;
    }
    
    printf("\nTotal entries: %d\n", entry_count);
    printf("ERROR: %d, WARN: %d, INFO: %d, DEBUG: %d\n",
           total_errors, total_warnings, total_info, total_debug);
    
    free(entries);
    return 0;
}