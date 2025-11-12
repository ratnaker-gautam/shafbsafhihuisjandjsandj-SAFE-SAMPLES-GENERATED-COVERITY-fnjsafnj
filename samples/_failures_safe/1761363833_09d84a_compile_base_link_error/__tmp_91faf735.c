//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_LEVELS 100

struct LogEntry {
    char level[16];
    char message[MAX_LINE_LENGTH];
    int timestamp;
};

int parse_log_level(const char* level_str) {
    if (strcmp(level_str, "DEBUG") == 0) return 0;
    if (strcmp(level_str, "INFO") == 0) return 1;
    if (strcmp(level_str, "WARNING") == 0) return 2;
    if (strcmp(level_str, "ERROR") == 0) return 3;
    if (strcmp(level_str, "CRITICAL") == 0) return 4;
    return -1;
}

void print_log_entry(const struct LogEntry* entry) {
    printf("[%s] %d: %s\n", entry->level, entry->timestamp, entry->message);
}

int read_log_entry(FILE* file, struct LogEntry* entry) {
    char line[MAX_LINE_LENGTH];
    if (fgets(line, sizeof(line), file) == NULL) {
        return 0;
    }
    
    char level[16];
    int timestamp;
    char message[MAX_LINE_LENGTH];
    
    int result = sscanf(line, "[%15[^]]] %d: %1023[^\n]", level, &timestamp, message);
    if (result != 3) {
        return -1;
    }
    
    if (parse_log_level(level) < 0) {
        return -1;
    }
    
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    entry->timestamp = timestamp;
    
    return 1;
}

void analyze_logs_recursive(FILE* file, int min_level, int* stats, int depth) {
    if (depth >= MAX_LEVELS) {
        return;
    }
    
    struct LogEntry entry;
    int result = read_log_entry(file, &entry);
    
    if (result == 0) {
        return;
    }
    
    if (result > 0) {
        int level = parse_log_entry(entry.level);
        if (level >= 0 && level < 5) {
            stats[level]++;
        }
        
        if (level >= min_level) {
            print_log_entry(&entry);
        }
    }
    
    analyze_logs_recursive(file, min_level, stats, depth + 1);
}

int main(void) {
    printf("Log Analyzer - Enter log entries (format: [LEVEL] TIMESTAMP: MESSAGE)\n");
    printf("Supported levels: DEBUG, INFO, WARNING, ERROR, CRITICAL\n");
    printf("Enter 'END' on a separate line to finish input\n\n");
    
    FILE* temp_file = tmpfile();
    if (temp_file == NULL) {
        fprintf(stderr, "Error: Cannot create temporary file\n");
        return 1;
    }
    
    char line[MAX_LINE_LENGTH];
    while (1) {
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) > 0) {
            fprintf(temp_file, "%s\n", line);
        }
    }
    
    rewind(temp_file);
    
    int min_level;
    printf("\nEnter minimum log level to display (0=DEBUG, 1=INFO, 2=WARNING, 3=ERROR, 4=CRITICAL): ");
    if (scanf("%d", &min_level) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        fclose(temp_file);
        return 1;
    }
    
    if (min_level < 0 || min_level > 4) {
        fprintf(stderr, "Error: Level must be between 0 and 4\n");
        fclose(temp_file);
        return 1;
    }
    
    printf("\nFiltered Log Entries (level >= %d):\n", min_level);
    printf("====================================\n");
    
    int stats[5] = {0};
    analyze_logs_recursive(temp_file, min_level, stats, 0);
    
    printf("\nStatistics:\n");
    printf("DEBUG:    %d\n", stats[0]);
    printf("INFO:     %d\n", stats[1]);
    printf("WARNING:  %d\n", stats[2]);
    printf("ERROR:    %d\n", stats[3]);
    printf("CRITICAL: %d\n", stats[4]);
    
    fclose(temp_file);
    return 0;
}