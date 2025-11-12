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

void analyze_log_recursive(FILE* file, int current_depth, int* level_counts) {
    if (current_depth >= MAX_LEVELS || file == NULL) return;
    
    char buffer[MAX_LINE_LENGTH];
    if (fgets(buffer, sizeof(buffer), file) == NULL) return;
    
    if (strlen(buffer) >= MAX_LINE_LENGTH - 1) return;
    
    int level = parse_log_level(buffer);
    if (level >= 0 && level < 4) {
        level_counts[level]++;
    }
    
    analyze_log_recursive(file, current_depth + 1, level_counts);
}

int main(void) {
    const char* log_data[] = {
        "2024-01-15 10:30:15 INFO System started",
        "2024-01-15 10:30:16 DEBUG Initializing modules",
        "2024-01-15 10:30:17 WARN Low disk space",
        "2024-01-15 10:30:18 ERROR Database connection failed",
        "2024-01-15 10:30:19 INFO Retrying connection",
        "2024-01-15 10:30:20 DEBUG Connection parameters",
        "2024-01-15 10:30:21 INFO Connection established",
        "2024-01-15 10:30:22 WARN High memory usage",
        "2024-01-15 10:30:23 ERROR File not found",
        "2024-01-15 10:30:24 INFO Operation completed"
    };
    
    FILE* temp_file = tmpfile();
    if (temp_file == NULL) {
        fprintf(stderr, "Error creating temporary file\n");
        return 1;
    }
    
    for (int i = 0; i < 10; i++) {
        if (fprintf(temp_file, "%s\n", log_data[i]) < 0) {
            fprintf(stderr, "Error writing to file\n");
            fclose(temp_file);
            return 1;
        }
    }
    
    rewind(temp_file);
    
    int level_counts[4] = {0};
    analyze_log_recursive(temp_file, 0, level_counts);
    
    printf("Log Level Analysis:\n");
    printf("DEBUG: %d entries\n", level_counts[0]);
    printf("INFO: %d entries\n", level_counts[1]);
    printf("WARN: %d entries\n", level_counts[2]);
    printf("ERROR: %d entries\n", level_counts[3]);
    
    int total = level_counts[0] + level_counts[1] + level_counts[2] + level_counts[3];
    printf("Total analyzed: %d entries\n", total);
    
    fclose(temp_file);
    return 0;
}