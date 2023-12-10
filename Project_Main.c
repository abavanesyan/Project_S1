#include "project_1.h"

// due to my knowledge of C programming language and statistics that I have gathered from C and statistics classes this semester, I wrote this project!!!

int main() {
// at first I gathered data from a file
    struct Population *head = 0;
    char k;
    int op_p = open("project_population.txt", O_RDONLY);
    if (-1 != op_p)
    {
        int l = 1;
        int cnt = 0;
        while (0 != l)
        {
            l = read(op_p, &k, 1);
            cnt++;
        }
        close(op_p);

        int op_p2 = open("project_population.txt", O_RDONLY);
        char file[cnt];
        read(op_p2, file, cnt);
        close(op_p2);

        for (int i = 0; i < cnt-2; i++)
        {
            if ((' ' == file[i] && isdigit(file[i+1])) || (0 == i && isdigit(file[0])))
            {
                add(&head, atoi(&file[i]));
            }
            
        }
    } else {
        fprintf(stderr, "%s\n", strerror(errno));
    }

// this is gathering data for sample
    struct Sample *head_s = 0;
    int op_s = open("project_sample.txt", O_RDONLY);
    if (-1 != op_s)
    {
        int l2 = 1;
        int cnt2 = 0;
        while (0 != l2)
        {
            l2 = read(op_s, &k, 1);
            cnt2++;
        }
        close(op_s);

        int op_s2 = open("project_sample.txt", O_RDONLY);
        char file2[cnt2];
        read(op_s2, file2, cnt2);
        close(op_s2);

        for (int j = 0; j < cnt2-2; j++)
        {
            if ((' ' == file2[j] && isdigit(file2[j+1])) || (0 == j && isdigit(file2[0])))
            {
                add_s(&head_s, atoi(&file2[j]));
            }
            
        }
    } else {
        fprintf(stderr, "%s\n", strerror(errno));
    }

// here I tested whether everything was working
    int value, most;
    most_sold(&head, &most, &value);

    double n, m;
    quantity_n(&head, &n);
    quantity_n_s(&head_s, &m);

    double mean, mean_s;
    meano(&head, &mean, n);
    meano_s(&head_s, &mean_s, n);

    double variance;
    varianceo(&head, &variance, mean, n);

    double sigma;
    st_d(&sigma, variance);

    double coef_variation;
    coef_var(sigma, variance, &coef_variation);

    double z_stat;
    z_test(mean, mean_s, sigma, m, &z_stat);

    double bl;
    is_there(&head, 256, &bl);

// I saved outputs in a file
    FILE *fptr = fopen("project_output.txt", "w");
    fprintf(fptr, "The most sold is %dGB and it was sold %d times\n", value, most);
    fprintf(fptr, "Population size is %.lf\n", n);
    fprintf(fptr, "Sample size is %.lf\n", m);
    fprintf(fptr, "Mean of population is %lf\n", mean);
    fprintf(fptr, "Mean of sample is %lf\n", mean_s);
    fprintf(fptr, "Variance of population is %lf\n", variance);
    fprintf(fptr, "Standard deviation of population is %lf\n", sigma);

    if (coef_variation <= 15)
    {
        fprintf(fptr, "Mean is representative\n");
    } else if (coef_variation > 15 && coef_variation <= 33)
    {
        fprintf(fptr, "Mean is weak representative\n");
    } else {
        fprintf(fptr, "Mean is not representative\n");
    }

    if (z_stat >= 0 && z_stat > z_crit)
    {
        fprintf(fptr, "With 95%% confidence population mean significantly differs from sample mean\n");
    } else if (z_stat < 0 && z_stat < (-z_crit))
    {
        fprintf(fptr, "With 95%% confidence population mean significantly differs from sample mean\n");
    } else {
        fprintf(fptr, "With 95%% confidence population mean unsignificantly differs (or equal to) from sample mean\n");
    }

    if (bl == 1)
    {
        fprintf(fptr, "There is such an item in population\n");
    } else {
        fprintf(fptr, "There is not such an item in population\n");
    }
    
    fclose(fptr);

    binom_distribution(3);

// it is to avoid memory leak
    freeing(&head);
    freeing_s(&head_s);

    return 0;
}