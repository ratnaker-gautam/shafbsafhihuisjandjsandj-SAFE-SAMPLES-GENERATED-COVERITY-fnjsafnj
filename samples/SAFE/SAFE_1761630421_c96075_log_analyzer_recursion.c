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

void analyze_log_recursive(struct LogEntry* entries, int start, int end, int target_level) {
    if (start > end) return;
    
    int mid = start + (end - start) / 2;
    
    if (entries[mid].level >= target_level) {
        printf("%s", entries[mid].line);
    }
    
    analyze_log_recursive(entries, start, mid - 1, target_level);
    analyze_log_recursive(entries, mid + 1, end, target_level);
}

int read_log_file(const char* filename, struct LogEntry** entries) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Cannot open file %s\n", filename);
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
        
        strncpy((*entries)[count].line, buffer, MAX_LINE_LENGTH - 1);
        (*entries)[count].line[MAX_LINE_LENGTH - 1] = '\0';
        (*entries)[count].level = parse_log_level(buffer);
        count++;
    }
    
    fclose(file);
    return count;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <logfile> <level>\n", argv[0]);
        fprintf(stderr, "Levels: 0=DEBUG, 1=INFO, 2=WARN, 3=ERROR\n");
        return 1;
    }
    
    char* endptr;
    long level = strtol(argv[2], &endptr, 10);
    if (*endptr != '\0' || level < 0 || level > 3) {
        fprintf(stderr, "Error: Invalid level. Must be 0-3\n");
        return 1;
    }
    
    struct LogEntry* entries = NULL;
    int count = read_log_file(argv[1], &entries);
    if (count <= 0) {
        if (entries) free(entries);
        return 1;
    }
    
    printf("Log entries with level >= %ld:\n", level);
    analyze_log_recursive(entries, 0, count - 1, (int)level);
    
    free(entries);
    return 0;
}