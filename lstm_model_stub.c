/*
 * lstm_model_stub.c
 * Stub for LSTM model integration in RPL-AER
 * This file simulates the presence of an embedded LSTM model for energy prediction.
 * For real deployment, replace with actual model or TFLite integration.
 */

#include <stdio.h>
#include <stdint.h>
#include "lstm_model_stub.h"

#define LSTM_WINDOW_SIZE 10

static float lstm_history[LSTM_WINDOW_SIZE] = {0.5f};
static int lstm_index = 0;

void lstm_model_init(void) {
    for(int i=0; i<LSTM_WINDOW_SIZE; i++) lstm_history[i] = 0.5f;
    lstm_index = 0;
}

void lstm_model_update(float value) {
    lstm_history[lstm_index] = value;
    lstm_index = (lstm_index + 1) % LSTM_WINDOW_SIZE;
}

float lstm_model_predict(void) {
    float sum = 0.0f;
    for(int i=0; i<LSTM_WINDOW_SIZE; i++) sum += lstm_history[i];
    return sum / LSTM_WINDOW_SIZE; // Simple average as stub
}

/*
 * Stub for LSTM-based energy prediction.
 * Replace this function by a real TFLite call or embedded LSTM on STM32 for deployment.
 */
float predict_energy(int node_id, float current_energy, float past_values[])
{
    // Simule une prédiction basée sur la moyenne des valeurs passées et l'énergie courante
    float sum = 0.0f;
    for(int i=0; i<LSTM_WINDOW_SIZE; i++) sum += past_values[i];
    float avg = sum / LSTM_WINDOW_SIZE;
    // Pondération simple : 70% historique, 30% valeur courante
    return 0.7f * avg + 0.3f * current_energy;
}