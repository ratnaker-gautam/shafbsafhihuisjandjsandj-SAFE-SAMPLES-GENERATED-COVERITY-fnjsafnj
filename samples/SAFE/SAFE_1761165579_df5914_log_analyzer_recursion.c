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
};

int parse_log_level(const char* line) {
    if (strstr(line, "ERROR")) return 3;
    if (strstr(line, "WARN")) return 2;
    if (strstr(line, "INFO")) return 1;
    if (strstr(line, "DEBUG")) return 0;
    return -1;
}

void analyze_log_recursive(struct LogEntry* entries, int start, int end, int current_level, int* level_counts) {
    if (start > end || current_level >= MAX_LEVELS) {
        return;
    }
    
    if (start == end) {
        if (entries[start].level >= 0 && entries[start].level < MAX_LEVELS) {
            level_counts[entries[start].level]++;
        }
        return;
    }
    
    int mid = start + (end - start) / 2;
    
    if (entries[mid].level >= 0 && entries[mid].level < MAX_LEVELS) {
        level_counts[entries[mid].level]++;
    }
    
    analyze_log_recursive(entries, start, mid - 1, current_level + 1, level_counts);
    analyze_log_recursive(entries, mid + 1, end, current_level + 1, level_counts);
}

int read_log_file(const char* filename, struct LogEntry** entries) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        return -1;
    }
    
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
            struct LogEntry* new_entries = realloc(*entries, capacity * sizeof(struct LogEntry));
            if (!new_entries) {
                free(*entries);
                fclose(file);
                return -1;
            }
            *entries = new_entries;
        }
        
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        
        if (len >= MAX_LINE_LENGTH - 1) {
            buffer[MAX_LINE_LENGTH - 1] = '\0';
        }
        
        strncpy((*entries)[count].line, buffer, MAX_LINE_LENGTH - 1);
        (*entries)[count].line[MAX_LINE_LENGTH - 1] = '\0';
        (*entries)[count].level = parse_log_level(buffer);
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
        if (entry_count == -1) {
            printf("Error: Cannot open or read file: %s\n", argv[1]);
        } else {
            printf("No log entries found.\n");
        }
        if (entries) free(entries);
        return 1;
    }
    
    int level_counts[MAX_LEVELS] = {0};
    analyze_log_recursive(entries, 0, entry_count - 1, 0, level_counts);
    
    printf("Log Level Analysis:\n");
    printf("DEBUG: %d\n", level_counts[0]);
    printf("INFO:  %d\n", level_counts[1]);
    printf("WARN:  %d\n", level_counts[2]);
    printf("ERROR: %d\n", level_counts[3]);
    
    int other_count = 0;
    for (int i = 4; i < MAX_LEVELS; i++) {
        other_count += level_counts[i];
    }
    if (other_count > 0) {
        printf("OTHER: %d\n", other_count);
    }
    
    printf("Total entries: %d\n", entry_count);
    
    free(entries);
    return 0;
}