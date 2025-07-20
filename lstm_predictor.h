/**
 * @file lstm_predictor.h
 * @brief Header for Simulated LSTM Predictor (RPL-AER)
 * Provides API for energy forecasting, MAE calculation, and logging.
 *
 * Reference: RPL-AER: A Secure, Sustainable, and Predictive Routing Protocol for Low-Power IoT Networks (section IV-C)
 */

#ifndef LSTM_PREDICTOR_H
#define LSTM_PREDICTOR_H

#include "project-conf.h"

void lstm_predictor_init(void);
void lstm_predictor_update(float value);
float lstm_predictor_predict(void);
float lstm_predictor_mae(void);
void lstm_predictor_log(void);

#endif // LSTM_PREDICTOR_H