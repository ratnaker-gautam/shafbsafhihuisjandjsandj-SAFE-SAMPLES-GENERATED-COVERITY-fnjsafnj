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
    if (line == NULL || entry == NULL) return 0;
    
    size_t len = strlen(line);
    if (len < 30 || len >= MAX_LINE_LENGTH) return 0;
    
    int scanned = sscanf(line, "%19s %9s %[^\n]", 
                        entry->timestamp, entry->level, entry->message);
    
    if (scanned != 3) return 0;
    
    for (int i = 0; entry->timestamp[i]; i++) {
        if (!isdigit(entry->timestamp[i]) && entry->timestamp[i] != '-' && 
            entry->timestamp[i] != ':' && entry->timestamp[i] != '.') {
            return 0;
        }
    }
    
    const char* valid_levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    int valid = 0;
    for (int i = 0; i < 4; i++) {
        if (strcmp(entry->level, valid_levels[i]) == 0) {
            valid = 1;
            break;
        }
    }
    
    return valid;
}

void analyze_log_recursive(FILE* file, int* counts, int depth) {
    if (depth >= MAX_LEVELS) return;
    if (file == NULL || counts == NULL) return;
    
    char line[MAX_LINE_LENGTH];
    if (fgets(line, sizeof(line), file) == NULL) return;
    
    struct LogEntry entry;
    if (parse_log_line(line, &entry)) {
        if (strcmp(entry.level, "INFO") == 0) counts[0]++;
        else if (strcmp(entry.level, "WARN") == 0) counts[1]++;
        else if (strcmp(entry.level, "ERROR") == 0) counts[2]++;
        else if (strcmp(entry.level, "DEBUG") == 0) counts[3]++;
    }
    
    analyze_log_recursive(file, counts, depth + 1);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <logfile>\n", argv[0]);
        return 1;
    }
    
    FILE* file = fopen(argv[1], "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Cannot open file %s\n", argv[1]);
        return 1;
    }
    
    int counts[4] = {0};
    analyze_log_recursive(file, counts, 0);
    
    fclose(file);
    
    printf("Log Analysis Results:\n");
    printf("INFO entries: %d\n", counts[0]);
    printf("WARN entries: %d\n", counts[1]);
    printf("ERROR entries: %d\n", counts[2]);
    printf("DEBUG entries: %d\n", counts[3]);
    
    int total = counts[0] + counts[1] + counts[2] + counts[3];
    if (total > 0) {
        printf("\nDistribution:\n");
        printf("INFO: %.1f%%\n", (counts[0] * 100.0) / total);
        printf("WARN: %.1f%%\n", (counts[1] * 100.0) / total);
        printf("ERROR: %.1f%%\n", (counts[2] * 100.0) / total);
        printf("DEBUG: %.1f%%\n", (counts[3] * 100.0) / total);
    }
    
    return 0;
}