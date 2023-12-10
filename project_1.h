#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <math.h>

// this value is for z test when confidence is 95% (we can find it in z table)
const double z_crit = 1.96;

/* this structure is for entire sold computers RAMs in 2023. This is a treelike structure and I chose this structure
because the algorithm complexity of this structure is O(log(n)) (in worst case is o(n)) */
struct Population {
    struct Population *right;
    struct Population *left;
    int value;
    double count;
};

// this structure is for just a part of all sold computers RAMs in 2023 (I need these values for some statistics calculation)
struct Sample {
    struct Sample *right;
    struct Sample *left;
    int value;
    double count;
};

// this function is for adding items to Population stracture
void add(struct Population **head, int value) {
    if (0 == *head) 
    {
        *head = malloc(sizeof(struct Population));
        (*head)->left = 0;
        (*head)->right = 0;
        (*head)->value = value;
        (*head)->count = 1;
    }else { 
        struct Population * loc = *head;
        struct Population * newloc = 0;
        int bl = 0;

        while (0 == bl)
        {
            if (value > loc->value && loc->right != 0)
            {
                loc = loc->right;
            } else if (value > loc->value && loc->right == 0)
            {
                newloc = malloc(sizeof(struct Population));
                newloc->left = 0;
                newloc->right = 0;
                newloc->value = value;
                newloc->count = 1;
                loc->right = newloc;
                bl = 1;
            } else if (value < loc->value && loc->left != 0)
            {
                loc = loc->left;
            } else if (value < loc->value && loc->left == 0)
            {
                newloc = malloc(sizeof(struct Population));
                newloc->left = 0;
                newloc->right = 0;
                newloc->value = value;
                newloc->count = 1;
                loc->left = newloc;
                bl = 1;
            } else {
                loc->count += 1;
                bl = 1;
            } 
        }
    }
    return;
}

// this function is for adding items to Sample stracture
void add_s(struct Sample **head, int value) {
    if (0 == *head) 
    {
        *head = malloc(sizeof(struct Sample));
        (*head)->left = 0;
        (*head)->right = 0;
        (*head)->value = value;
        (*head)->count = 1;
    }else { 
        struct Sample * loc = *head;
        struct Sample * newloc = 0;
        int bl = 0;

        while (0 == bl)
        {
            if (value > loc->value && loc->right != 0)
            {
                loc = loc->right;
            } else if (value > loc->value && loc->right == 0)
            {
                newloc = malloc(sizeof(struct Sample));
                newloc->left = 0;
                newloc->right = 0;
                newloc->value = value;
                newloc->count = 1;
                loc->right = newloc;
                bl = 1;
            } else if (value < loc->value && loc->left != 0)
            {
                loc = loc->left;
            } else if (value < loc->value && loc->left == 0)
            {
                newloc = malloc(sizeof(struct Sample));
                newloc->left = 0;
                newloc->right = 0;
                newloc->value = value;
                newloc->count = 1;
                loc->left = newloc;
                bl = 1;
            } else {
                loc->count += 1;
                bl = 1;
            } 
        }
    }
    return;
}

// almost in all functions I used recursions because it is the easiest way to work with trees

// this function is for freeing Population stracure
void freeing(struct Population **head) {
    if (0 != (*head)->left)
    {
        freeing(&(*head)->left);
    } 
    if (0 != (*head)->right)
    {
        freeing(&(*head)->right);
    } 
    free((*head));
    return;
}

// this function is for freeing Sample stracure
void freeing_s(struct Sample **head) {
    if (0 != (*head)->left)
    {
        freeing_s(&(*head)->left);
    } 
    if (0 != (*head)->right)
    {
        freeing_s(&(*head)->right);
    } 
    free((*head));
    return;
}

// this function is for searching an item in population
void is_there(struct Population **head, int value, double *bl) {
    struct Population * loc = *head;
    if (value > loc->value)
    {
        is_there(&loc->right, value, bl);
    } else if (value < loc->value)
    {
        is_there(&loc->left, value, bl);
    } else
    {
        *bl = 1;
    }
    return;
}

// this function is for finding most sold computer with RAM parameter
void most_sold(struct Population **head, int *most, int *value) {
    struct Population * loc = *head;
    if (0 != loc->left)
    {
        most_sold(&loc->left, most, value);
    } 
    if (0 != loc->right)
    {
        most_sold(&loc->right, most, value);
    } 
    if (*most < loc->count)
    {
        *most = loc->count;
        *value = loc->value;
    }
    return;
}

// this function is for calculating how many items there are (with counts) in population
void quantity_n(struct Population **head, double *n) {
    struct Population * loc = *head;
    *n += loc->count;
    if (0 != loc->left)
    {
        quantity_n(&loc->left, n);
    } 
    if (0 != loc->right)
    {
        quantity_n(&loc->right, n);
    }
    return;
}

// this function is for calculating how many items there are (with counts) in sample
void quantity_n_s(struct Sample **head, double *n) {
    struct Sample * loc = *head;
    *n += loc->count;
    if (0 != loc->left)
    {
        quantity_n_s(&loc->left, n);
    } 
    if (0 != loc->right)
    {
        quantity_n_s(&loc->right, n);
    }
    return;
}

// this function is for calculating the mean of population
void meano(struct Population **head, double *mean, int n) {
    struct Population * loc = *head;
    if (0 != loc->left)
    {
        meano(&loc->left, mean, n);
    } 
    if (0 != loc->right)
    {
        meano(&loc->right, mean, n);
    }
    *mean += (loc->value) * (loc->count) / n;
    return;
}

// this function is for calculating the mean of sample
void meano_s(struct Sample **head, double *mean, int n) {
    struct Sample * loc = *head;
    if (0 != loc->left)
    {
        meano_s(&loc->left, mean, n);
    } 
    if (0 != loc->right)
    {
        meano_s(&loc->right, mean, n);
    }
    *mean += (loc->value) * (loc->count) / n;
    return;
}

// this function is for calculating variance of population
void varianceo(struct Population **head, double *variance, double mean, int n) {
    struct Population * loc = *head;
    if (0 != loc->left)
    {
        varianceo(&loc->left, variance, mean, n);
    } 
    if (0 != loc->right)
    {
        varianceo(&loc->right, variance, mean, n);
    }
    *variance += (loc->value - mean) * (loc->value - mean) * loc->count / n;
    return;
}

// this function is for calculating standard deviation of population
void st_d(double *sigma, double variance) {
    *sigma = pow(variance, 0.5);
}

// this function is for calculating coefficient of variation of population (which says us whether population mean is representative or not)
void coef_var(double sigma, double mean, double *coef_variation) {
    *coef_variation = sigma / mean * 100;
    return;
}

// this function is for z test (which is more used in statistics)
void z_test(double mean, double mean2, double sigma, double m, double *z_stat) {
    *z_stat = (mean2 - mean) / (sigma / pow(m, 0.5));
    return;
}

// this function is for just calculating factorial of a number (which I used in binomial distribution)
int factorial(int n) {
    int sum = 1;
    for (int i = 2; i <= n; i++)
    {
        sum *= i;
    }
    return sum;
}

/* this is binomial distribution function. 0.2 is a probability that a manufactured computer is damaged and 0.8 is contrary version.
These probabilities are not real.
n is a size of sample of computers that are manufactured */
void binom_distribution(double n) {
    double binom;
    FILE *fptr = fopen("project_output.txt", "a");
    for (int i = 0; i <= n; i++)
    {
        binom = (factorial(n) / (factorial(n-i) * factorial(i))) * pow(0.2, i) * pow(0.8, n-i);
        fprintf(fptr, "Probability that %.lf of %.lf are not defected is %lf\n", n-i, n, binom);
    }
    fclose(fptr);
    return;
}