# RPL-AER Test Plan

## 1. Simulation Scenario
- **Number of nodes:** 40 (1 sink, 39 clients)
- **Node density:** 0.3 (solar ratio)
- **Mobility:** 0.2 (20% mobile nodes)
- **Simulation duration:** 1800 seconds (30 minutes)
- **Topology:** Random, area 500x500
- **Sink position:** (50, 50)
- **Traffic:** UDP periodic, all clients to sink
- **Attacks simulated:** Sinkhole, Selective Forwarding, Rank Attack, Hello Flood (randomly triggered)

## 2. Modules Activated
- MCS metric
- LSTM energy prediction
- Trust management
- Harvest (solar energy)
- Attack detection

## 3. Logs to Observe
- `[MCS]` : Routing metric (NRE, PEC, ECS)
- `[LSTM]` : Energy predictions
- `[HARVEST]` : Harvested/stored/consumed energy
- `[TRUST]` : Trust scores
- `[ATTACK]`, `[FP]`, `[TRUST_ANOMALY]` : Attack detection
- `[PERF]` : PDR, Latency, Throughput, Lifetime

## 4. Metrics to Extract
- **PDR (Packet Delivery Ratio)**
- **Average Latency**
- **Throughput**
- **Node/network lifetime**
- **Energy consumption**
- **Number of attacks detected**
- **False positive rate**

## 5. Validation Thresholds
- **PDR:** > 95%
- **Average Latency:** < 150 ms
- **Attack detection rate:** > 90%
- **False positive rate:** < 5%
- **Network lifetime:** > 90% nodes alive at 1500s

## 6. How to Run
1. Compile with `./build_and_test.sh -c -g`
2. Launch Cooja with `cooja cooja_results/rpl-aer-simulation.csc`
3. Observe logs in `cooja_results/rpl-aer-output.log`
4. Extract metrics with `simulate_results.py` or manually from logs

## 7. What to Check in the Logs
- Presence of all scientific logs ([MCS], [LSTM], [HARVEST], [TRUST], [ATTACK], [FP], [TRUST_ANOMALY], [PERF])
- No compilation/runtime errors
- Metrics are consistent and within thresholds

---

**This test plan ensures scientific reproducibility and compliance with IEEE IoT standards.**