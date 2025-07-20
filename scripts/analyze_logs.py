#!/usr/bin/env python3
"""
Extrait PDR, Latency, Throughput, Energy, Attaques détectées
Génère metrics_summary.txt et des CSV pour figures
"""
import re
import os
import csv

LOG_FILE = "cooja_results/rpl-aer-output.log"
SUMMARY_FILE = "cooja_results/metrics_summary.txt"
QOS_CSV = "cooja_results/figure_data_qos.csv"
ENERGY_CSV = "cooja_results/figure_data_energy.csv"
ATTACKS_CSV = "cooja_results/figure_data_attacks.csv"

# Expressions régulières pour extraire les métriques
re_pdr = re.compile(r"\[PERF\].*PDR=([0-9.]+)%")
re_latency = re.compile(r"\[PERF\].*Latency=([0-9.]+)")
re_throughput = re.compile(r"\[PERF\].*Throughput=([0-9.]+)")
re_energy = re.compile(r"\[HARVEST\].*stored=([0-9.]+)J")
re_attack = re.compile(r"\[ATTACK\]")
re_fp = re.compile(r"\[FP\]")

pdrs, latencies, throughputs, energies = [], [], [], []
attack_count, fp_count = 0, 0

with open(LOG_FILE, "r") as f:
    for line in f:
        m = re_pdr.search(line)
        if m:
            pdrs.append(float(m.group(1)))
        m = re_latency.search(line)
        if m:
            latencies.append(float(m.group(1)))
        m = re_throughput.search(line)
        if m:
            throughputs.append(float(m.group(1)))
        m = re_energy.search(line)
        if m:
            energies.append(float(m.group(1)))
        if re_attack.search(line):
            attack_count += 1
        if re_fp.search(line):
            fp_count += 1

# Calculs
avg_pdr = sum(pdrs)/len(pdrs) if pdrs else 0
avg_latency = sum(latencies)/len(latencies) if latencies else 0
avg_throughput = sum(throughputs)/len(throughputs) if throughputs else 0
avg_energy = sum(energies)/len(energies) if energies else 0

# Résumé
with open(SUMMARY_FILE, "w") as f:
    f.write(f"PDR moyen: {avg_pdr:.2f}%\n")
    f.write(f"Latence moyenne: {avg_latency:.2f} ms\n")
    f.write(f"Throughput moyen: {avg_throughput:.2f} pkts/s\n")
    f.write(f"Énergie moyenne stockée: {avg_energy:.2f} J\n")
    f.write(f"Attaques détectées: {attack_count}\n")
    f.write(f"Faux positifs: {fp_count}\n")

# CSV QoS
with open(QOS_CSV, "w", newline="") as f:
    writer = csv.writer(f)
    writer.writerow(["PDR", "Latency", "Throughput"])
    for i in range(len(pdrs)):
        writer.writerow([pdrs[i], latencies[i] if i < len(latencies) else '', throughputs[i] if i < len(throughputs) else ''])

# CSV Energy
with open(ENERGY_CSV, "w", newline="") as f:
    writer = csv.writer(f)
    writer.writerow(["StoredEnergy"])
    for e in energies:
        writer.writerow([e])

# CSV Attacks
with open(ATTACKS_CSV, "w", newline="") as f:
    writer = csv.writer(f)
    writer.writerow(["AttackEvent"])
    for i in range(attack_count):
        writer.writerow([1])
    for i in range(fp_count):
        writer.writerow([0])