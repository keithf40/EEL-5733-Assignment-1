#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct Entry {
    char key[50];       // Stores Agent ID or State Code
    char name[50];      // Stores Agent Name
    double max_val;     // The actual signed value
    double max_abs_val; // The absolute value for comparison
    int is_set;
};

int main() {
    // Create buffer
    char line[1024];
    struct Entry agents[200];
    struct Entry states[100];

    // Initialize arrays
    memset(agents, 0, sizeof(agents));
    memset(states, 0, sizeof(states));

    while (fgets(line, sizeof(line), stdin)) {
        if (strlen(line) < 10)
            continue;

        // Replace ", " with "|"
        for (int i = 0; line[i] != '\0'; i++) {
            if (line[i] == ',' && line[i+1] == ' ')
                line[i] = '|';
        }

        // Tokenize
        char *agent_name = strtok(line, "|");

        char *agent_id = strtok(NULL, "|");
        if (agent_id[0] == ' ')
            agent_id++;

        // Skip Transaction ID
        strtok(NULL, "|");
        
        char *real_estate_location = strtok(NULL, "|");
        if (real_estate_location[0] == ' ')
            real_estate_location++;

        // Skip sale price
        strtok(NULL, "|");

        char *loss_gain_str = strtok(NULL, "|");
        if (loss_gain_str[0] == ' ')
            loss_gain_str++;
        
        loss_gain_str[strcspn(loss_gain_str, "\n")] = 0;

        // Convert loss/gain to double
        char loss_gain_clean[50];
        int j = 0;
        for(int i=0; loss_gain_str[i] != '\0'; i++) {
            if(loss_gain_str[i] != ',')
                loss_gain_clean[j++] = loss_gain_str[i];
        }
        loss_gain_clean[j] = '\0';
        
        double loss_gain = atof(loss_gain_clean);
        double loss_gain_abs = fabs(loss_gain);

        // Update Agent Max
        int found = -1;
        for(int i=0; i<200; i++) {
            if(agents[i].is_set && strcmp(agents[i].key, agent_id) == 0) {
                found = i;
                break;
            }
        }

        if(found == -1) {
            for(int i=0; i<200; i++) {
                if(!agents[i].is_set) {
                    found = i; 
                    agents[i].is_set = 1;
                    strcpy(agents[i].key, agent_id);
                    strcpy(agents[i].name, agent_name);
                    agents[i].max_abs_val = -1.0;
                    break;
                }
            }
        }

        if(found != -1 && loss_gain_abs > agents[found].max_abs_val) {
            agents[found].max_abs_val = loss_gain_abs;
            agents[found].max_val = loss_gain;
        }

        // Update State Max
        found = -1;
        for(int i=0; i<100; i++) {
            if(states[i].is_set && strcmp(states[i].key, real_estate_location) == 0) {
                found = i;
                break;
            }
        }

        if(found == -1) {
            for(int i=0; i<100; i++) {
                if(!states[i].is_set) {
                    found = i; 
                    states[i].is_set = 1;
                    strcpy(states[i].key, real_estate_location);
                    states[i].max_abs_val = -1.0;
                    break;
                }
            }
        }

        if(found != -1 && loss_gain_abs > states[found].max_abs_val) {
            states[found].max_abs_val = loss_gain_abs;
            states[found].max_val = loss_gain;
        }
    }

    // Output Agent Performance to stdout
    for(int i=0; i<200; i++) {
        if(agents[i].is_set) {
            // Convert loss/gain to money
            char loss_gain_str[100];
            char temp[50];

            // Handle decimal point
            sprintf(temp, "%.2f", fabs(agents[i].max_val));
            char *dot = strchr(temp, '.');
            int len = dot - temp;
            
            // Handle negative sign
            int idx = 0;
            if(agents[i].max_val < 0)
                loss_gain_str[idx++] = '-';

            // Handle commas
            for(int k=0; k<len; k++) {
                if(k > 0 && (len-k)%3 == 0)
                    loss_gain_str[idx++] = ',';
                loss_gain_str[idx++] = temp[k];
            }
            
            // Formats string
            strcpy(&loss_gain_str[idx], dot);

            // Prints stdout
            fprintf(stdout, "%s, %s, %s\n", agents[i].name, agents[i].key, loss_gain_str);
        }
    }

    // Output State Performance to stderr
    for(int i=0; i<100; i++) {
        if(states[i].is_set) {
            // Convert loss/gain to money
            char loss_gain_str[100];
            char temp[50];
            
            // Handle decimal point
            sprintf(temp, "%.2f", fabs(states[i].max_val));
            char *dot = strchr(temp, '.');
            int len = dot - temp;
            
            // Handle negative sign
            int idx = 0;
            if(states[i].max_val < 0)
                loss_gain_str[idx++] = '-';
            
            // Handle commas
            for(int k=0; k<len; k++) {
                if(k > 0 && (len-k)%3 == 0)
                    loss_gain_str[idx++] = ',';
                loss_gain_str[idx++] = temp[k];
            }

            // Formats string
            strcpy(&loss_gain_str[idx], dot);
            
            // Prints stderr
            fprintf(stderr, "%s, %s\n", states[i].key, loss_gain_str);
        }
    }
    return 0;
}
