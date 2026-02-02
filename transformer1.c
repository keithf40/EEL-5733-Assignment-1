#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int main() {
    // Create buffer
    char line[1024];

    while (fgets(line, sizeof(line), stdin)) {
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

        char *transaction_id = strtok(NULL, "|"); 
        if (transaction_id[0] == ' ')
            transaction_id++;

        char *real_estate_location = strtok(NULL, "|");
        if (real_estate_location[0] == ' ')
            real_estate_location++;

        char *original_price_str = strtok(NULL, "|");
        if (original_price_str[0] == ' ')
            original_price_str++;

        char *sale_price_str = strtok(NULL, "|");
        if (sale_price_str[0] == ' ')
            sale_price_str++;

        char *customer_rating = strtok(NULL, "|");
        if (customer_rating && customer_rating[0] == ' ')
            customer_rating++;

        // Convert original price to double
        char original_price_clean[50];
        int j = 0;
        for (int i = 0; original_price_str[i] != '\0'; i++) {
            if (original_price_str[i] != ',')
                original_price_clean[j++] = original_price_str[i];
        }
        original_price_clean[j] = '\0';
        double original_price = atof(original_price_clean);

        // Convert sale price to double
        char sale_price_clean[50];
        j = 0;
        for (int i = 0; sale_price_str[i] != '\0'; i++) {
            if (sale_price_str[i] != ',')
                sale_price_clean[j++] = sale_price_str[i];
        }
        sale_price_clean[j] = '\0';
        double sale_price = atof(sale_price_clean);

        // Calculate loss/gain
        double loss_gain = sale_price - original_price;
        // Convert loss/gain to money
        char loss_gain_str[100];
        char temp[50];
        
        // Handle decimal point
        sprintf(temp, "%.2f", fabs(loss_gain));
        char *dot = strchr(temp, '.');
        int len = dot - temp;
        
        // Handle negative sign
        int idx = 0;
        if (loss_gain < 0)
            loss_gain_str[idx++] = '-';

        // Handle commas
        for (int i = 0; i < len; i++) {
            if (i > 0 && (len - i) % 3 == 0)
                loss_gain_str[idx++] = ',';
            loss_gain_str[idx++] = temp[i];
        }

        // Formats string
        strcpy(&loss_gain_str[idx], dot);

        // Prints stdout and stderr
        fprintf(stdout, "%s, %s, %s, %s, %s, %s\n", agent_name, agent_id, transaction_id, real_estate_location, sale_price_str, loss_gain_str);
        
        fprintf(stderr, "%s, %s, %s", agent_id, real_estate_location, customer_rating);
    }
    return 0;
}
