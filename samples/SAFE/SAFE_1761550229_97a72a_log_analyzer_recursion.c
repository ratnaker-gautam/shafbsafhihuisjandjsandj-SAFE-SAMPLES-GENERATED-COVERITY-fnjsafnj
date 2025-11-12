//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_LEVELS 100

struct LogEntry {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LENGTH - 30];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (!line || !entry) return 0;
    
    if (strlen(line) >= MAX_LINE_LENGTH) return 0;
    
    int scanned = sscanf(line, "%19s %9s %[^\n]", 
                        entry->timestamp, entry->level, entry->message);
    
    if (scanned != 3) return 0;
    
    for (int i = 0; entry->level[i]; i++) {
        entry->level[i] = toupper((unsigned char)entry->level[i]);
    }
    
    return 1;
}

void analyze_log_recursive(FILE* file, int* error_count, int* warning_count, 
                          int* info_count, int depth) {
    if (!file || depth >= MAX_LEVELS) return;
    
    char line[MAX_LINE_LENGTH];
    
    if (fgets(line, sizeof(line), file) == NULL) return;
    
    struct LogEntry entry;
    if (parse_log_line(line, &entry)) {
        if (strcmp(entry.level, "ERROR") == 0) {
            (*error_count)++;
        } else if (strcmp(entry.level, "WARNING") == 0) {
            (*warning_count)++;
        } else if (strcmp(entry.level, "INFO") == 0) {
            (*info_count)++;
        }
    }
    
    analyze_log_recursive(file, error_count, warning_count, info_count, depth + 1);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <logfile>\n", argv[0]);
        return 1;
    }
    
    FILE* file = fopen(argv[1], "r");
    if (!file) {
        fprintf(stderr, "Error: Cannot open file %s\n", argv[1]);
        return 1;
    }
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    analyze_log_recursive(file, &error_count, &warning_count, &info_count, 0);
    
    fclose(file);
    
    printf("Log Analysis Results:\n");
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    printf("Total entries: %d\n", error_count + warning_count + info_count);
    
    return 0;
}