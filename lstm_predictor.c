/**
 * @file lstm_predictor.c
 * @brief Simulated LSTM Predictor for Energy Forecasting in RPL-AER
 * Implements a lightweight LSTM-like predictor for energy forecasting, as described in the article (section IV-C).
 * Provides historical buffer, prediction, and MAE calculation for scientific evaluation.
 *
 * Reference: RPL-AER: A Secure, Sustainable, and Predictive Routing Protocol for Low-Power IoT Networks (section IV-C)
 */

#include "lstm_predictor.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

static float lstm_history[LSTM_WINDOW_SIZE];
static float lstm_predictions[LSTM_WINDOW_SIZE];
static int lstm_index = 0;
static int lstm_count = 0;

void lstm_predictor_init(void) {
    memset(lstm_history, 0, sizeof(lstm_history));
    memset(lstm_predictions, 0, sizeof(lstm_predictions));
    lstm_index = 0;
    lstm_count = 0;
}

void lstm_predictor_update(float value) {
    lstm_history[lstm_index] = value;
    lstm_index = (lstm_index + 1) % LSTM_WINDOW_SIZE;
    if (lstm_count < LSTM_WINDOW_SIZE) lstm_count++;
}

float lstm_predictor_predict(void) {
    // Simulated LSTM: weighted moving average
    float sum = 0.0f;
    float weight_sum = 0.0f;
    for (int i = 0; i < lstm_count; i++) {
        float weight = (float)(i + 1) / lstm_count;
        sum += lstm_history[i] * weight;
        weight_sum += weight;
    }
    float prediction = (weight_sum > 0) ? (sum / weight_sum) : 0.0f;
    // Store prediction for MAE
    lstm_predictions[lstm_index] = prediction;
    return prediction;
}

float lstm_predictor_mae(void) {
    // Mean Absolute Error between predictions and actuals
    float error_sum = 0.0f;
    int n = (lstm_count < LSTM_WINDOW_SIZE) ? lstm_count : LSTM_WINDOW_SIZE;
    for (int i = 0; i < n; i++) {
        error_sum += fabsf(lstm_predictions[i] - lstm_history[i]);
    }
    return (n > 0) ? (error_sum / n) : 0.0f;
}

void lstm_predictor_log(void) {
    printf("[LSTM] History: ");
    for (int i = 0; i < lstm_count; i++) printf("%.3f ", lstm_history[i]);
    printf("\n[LSTM] Prediction: %.3f\n", lstm_predictor_predict());
    printf("[LSTM] MAE: %.3f\n", lstm_predictor_mae());
}