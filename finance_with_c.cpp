#include <stdio.h>
#include <stdlib.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_eigen.h>

// Function to load CSV data into a matrix
gsl_matrix* load_csv(const char* filename, int rows, int cols) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    gsl_matrix* data = gsl_matrix_alloc(rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            double val;
            fscanf(fp, "%lf,", &val);
            gsl_matrix_set(data, i, j, val);
        }
    }

    fclose(fp);
    return data;
}

// Compute daily returns
gsl_matrix* compute_returns(gsl_matrix* prices) {
    int rows = prices->size1;
    int cols = prices->size2;

    gsl_matrix* returns = gsl_matrix_alloc(rows - 1, cols);

    for (int i = 1; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            double prev = gsl_matrix_get(prices, i - 1, j);
            double curr = gsl_matrix_get(prices, i, j);
            double ret = (curr - prev) / prev;
            gsl_matrix_set(returns, i - 1, j, ret);
        }
    }
    return returns;
}

// Compute covariance matrix
gsl_matrix* covariance_matrix(gsl_matrix* data) {
    int rows = data->size1;
    int cols = data->size2;

    gsl_vector* mean = gsl_vector_alloc(cols);
    gsl_matrix* cov = gsl_matrix_calloc(cols, cols);

    // Compute mean for each column
    for (int j = 0; j < cols; j++) {
        double sum = 0.0;
        for (int i = 0; i < rows; i++)
            sum += gsl_matrix_get(data, i, j);
        gsl_vector_set(mean, j, sum / rows);
    }

    // Compute covariance
    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < cols; j++) {
            double sum = 0.0;
            for (int k = 0; k < rows; k++) {
                double xi = gsl_matrix_get(data, k, i) - gsl_vector_get(mean, i);
                double xj = gsl_matrix_get(data, k, j) - gsl_vector_get(mean, j);
                sum += xi * xj;
            }
            gsl_matrix_set(cov, i, j, sum / (rows - 1));
        }
    }

    gsl_vector_free(mean);
    return cov;
}

int main() {
    // Example stocks
    const char* stocks[] = {"RELIANCE.NS", "TCS.NS", "INFY.NS", "HDFCBANK.NS", "ITC.NS"};
    int n_stocks = 5;

    // Assume 21 trading days of prices stored in "data.csv"
    // Each row = day, each column = stock closing price
    gsl_matrix* prices = load_csv("data.csv", 21, n_stocks);

    gsl_matrix* returns = compute_returns(prices);
    gsl_matrix* cov = covariance_matrix(returns);

    // Eigenvalue decomposition
    gsl_vector* eval = gsl_vector_alloc(n_stocks);
    gsl_matrix* evec = gsl_matrix_alloc(n_stocks, n_stocks);
    gsl_eigen_symmv_workspace* w = gsl_eigen_symmv_alloc(n_stocks);

    gsl_eigen_symmv(cov, eval, evec, w);
    gsl_eigen_symmv_sort(eval, evec, GSL_EIGEN_SORT_VAL_DESC);

    printf("\n--- Eigenvalues ---\n");
    for (int i = 0; i < n_stocks; i++) {
        printf("%f\n", gsl_vector_get(eval, i));
    }

    printf("\n--- Main Market Trend (Eigenvector for largest Eigenvalue) ---\n");
    gsl_vector_view main_vec = gsl_matrix_column(evec, 0);
    for (int i = 0; i < n_stocks; i++) {
        printf("%s: %.4f\n", stocks[i], gsl_vector_get(&main_vec.vector, i));
    }

    printf("\nAnalysis:\n");
    printf("The largest eigenvalue represents the main market movement.\n");
    printf("Stocks with higher eigenvector values influence the trend more.\n");

    gsl_eigen_symmv_free(w);
    gsl_vector_free(eval);
    gsl_matrix_free(evec);
    gsl_matrix_free(cov);
    gsl_matrix_free(returns);
    gsl_matrix_free(prices);

    return 0;
}
