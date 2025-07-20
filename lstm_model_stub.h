#ifndef LSTM_MODEL_STUB_H
#define LSTM_MODEL_STUB_H

void lstm_model_init(void);
void lstm_model_update(float value);
float lstm_model_predict(void);
float predict_energy(int node_id, float current_energy, float past_values[]);

#endif // LSTM_MODEL_STUB_H