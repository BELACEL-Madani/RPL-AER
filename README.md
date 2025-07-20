# RPL-AER: Adaptive Energy-Responsive RPL Protocol

## Description

RPL-AER est un protocole de routage adaptatif pour réseaux de capteurs sans fil (WSN) qui optimise la consommation énergétique en utilisant une métrique hybride MCS (Multi-Criteria Score) combinant :

- **NRE** (Normalized Residual Energy) : Énergie résiduelle normalisée
- **PEC** (Predictive Energy Consumption) : Prédiction de consommation via LSTM
- **ECS** (Environmental Context Score) : Score contextuel environnemental

## Caractéristiques principales

###  Gestion énergétique adaptative
- Monitoring en temps réel de l'énergie résiduelle
- Prédiction de consommation via réseau LSTM simulé
- Support des nœuds solaires et sur batterie
- Simulation de récolte d'énergie solaire

###  Sécurité légère
- Validation ACK avec timeout
- Détection d'anomalies RSSI
- Simulation d'attaques (sinkhole, selective forwarding)
- Résilience aux attaques

###  Mobilité et trafic
- Support de la mobilité (Random Waypoint)
- Trafic UDP périodique configurable
- Gestion des connexions dynamiques

###  Métriques de performance
- PDR (Packet Delivery Ratio)
- Latence moyenne
- Throughput
- Consommation énergétique
- Résilience aux attaques

## Structure du projet

```
RPL_AER/
├── Makefile                    # Configuration de compilation
├── project-conf.h              # Configuration du projet
├── rpl-aer.h                   # En-têtes du protocole
├── rpl-aer.c                   # Implémentation RPL-AER
├── udp-client.c                # Client UDP
├── udp-server.c                # Serveur UDP
├── rpl-aer-sink.c              # Nœud sink
├── rpl-aer-simulation.csc      # Scénario Cooja (40 nœuds)
├── generate_csc.py             # Script de scénarios
├── build_and_test.sh           # Script de compilation
└── README.md                   # Ce fichier
```

## Installation et compilation

### Prérequis

1. **Contiki-NG** installé à `/home/belacel/contiki-ng`
2. **Python 3** pour le script de scénarios
3. **Cooja** pour la simulation

### Compilation rapide

```bash
# Compilation simple
./build_and_test.sh

# Compilation avec nettoyage
./build_and_test.sh -c

./build_and_test.sh -c -g
```

### Compilation manuelle

```bash
# Compiler les firmwares
make TARGET=cooja rpl-aer-client.csc
make TARGET=cooja rpl-aer-sink.csc

# Nettoyer
make clean
```

## Simulation

### Lancement de la simulation

```bash
# Utiliser le scénario prédéfini (40 nœuds)
cooja rpl-aer-simulation.csc

cooja rpl-aer-simulation-generated.csc
```

### Configuration du scénario

Le fichier `rpl-aer-simulation.csc` contient :
- **1 nœud sink** (serveur)
- **39 nœuds clients** avec répartition :
  - 11 nœuds solaires + mobiles (28%)
  - 28 nœuds batterie + statiques (72%)
- **Zone de simulation** : 500x500 mètres
- **Durée** : 1 heure (3600 secondes)
- **Plugins** : LogListener, PowerTracker, PacketLossTracker, Visualizer

## Script de scénarios

### Utilisation basique

```bash
# Générer un scénario avec paramètres par défaut
python3 generate_csc.py

# Générer un scénario personnalisé
python3 generate_csc.py -n 50 -s 0.4 -m 0.2 -o my-simulation.csc
```

### Options disponibles

| Option | Description | Défaut |
|--------|-------------|--------|
| `-n, --num-clients` | Nombre de nœuds clients | 39 |
| `-s, --solar-ratio` | Ratio de nœuds solaires (0.0-1.0) | 0.3 |
| `-m, --mobile-ratio` | Ratio de nœuds mobiles (0.0-1.0) | 0.3 |
| `-w, --width` | Largeur de la zone (mètres) | 500 |
| `-h, --height` | Hauteur de la zone (mètres) | 500 |
| `-d, --duration` | Durée de simulation (secondes) | 3600 |
| `-r, --random-seed` | Seed aléatoire | 12345 |
| `-o, --output` | Fichier de sortie | rpl-aer-simulation-generated.csc |

### Exemples d'utilisation

```bash
# Scénario dense (100 nœuds, 50% solaires)
python3 generate_csc.py -n 100 -s 0.5 -m 0.4

# Scénario urbain (25 nœuds, majorité statique)
python3 generate_csc.py -n 25 -s 0.2 -m 0.1

# Scénario agricole (60 nœuds, majorité solaire)
python3 generate_csc.py -n 60 -s 0.8 -m 0.2
```

## Configuration du protocole

### Paramètres MCS (project-conf.h)

```c
// Poids des métriques
#define NRE_WEIGHT 0.4f      // Énergie résiduelle
#define PEC_WEIGHT 0.35f     // Prédiction LSTM
#define ECS_WEIGHT 0.25f     // Contexte environnemental

// Configuration LSTM
#define LSTM_WINDOW_SIZE 10
#define LSTM_PREDICTION_HORIZON 5
```

### Paramètres énergétiques

```c
// Capacité batterie et efficacité solaire
#define BATTERY_CAPACITY 2000     // mAh
#define SOLAR_PANEL_EFFICIENCY 0.15f

// Irradiance solaire
```

### Paramètres de sécurité

```c
// Seuils et probabilités
#define RSSI_THRESHOLD -85
#define ATTACK_PROBABILITY 0.1f
#define SINKHOLE_PROBABILITY 0.05f
```

## Métriques et évaluation

### Métriques collectées

1. **Consommation énergétique**
   - Énergie résiduelle par nœud
   - Taux de consommation
   - Efficacité de récolte solaire

2. **Performance réseau**
   - PDR (Packet Delivery Ratio)
   - Latence moyenne
   - Throughput

3. **Sécurité**
   - Nombre d'attaques détectées
   - Taux de faux positifs
   - Résilience aux attaques

4. **Mobilité**
   - Stabilité des routes
   - Adaptation aux changements de topologie

### Visualisation

- **LogListener** : Logs en temps réel
- **PowerTracker** : Suivi de la consommation
- **PacketLossTracker** : Métriques de perte
- **Visualizer** : Interface graphique

## Scientific Logs and Results Organization

All scientific logs are automatically generated during simulation and saved in `cooja_results/rpl-aer-output.log`.

### Log Types
- `[MCS]` : Routing metric (NRE, PEC, ECS)
- `[LSTM]` : Energy predictions
- `[HARVEST]` : Harvested/stored/consumed energy
- `[TRUST]` : Trust scores
- `[ATTACK]`, `[FP]`, `[TRUST_ANOMALY]` : Attack detection
- `[PERF]` : PDR, Latency, Throughput, Lifetime

### Results Directory Structure
```
/cooja_results/
  ├─ rpl-aer-output.log
  ├─ metrics_summary.txt
  ├─ figure_data_energy.csv
  ├─ figure_data_qos.csv
  └─ ...
```

- All logs and metrics are ready for scientific exploitation and publication.
- Use `simulate_results.py` to extract and summarize metrics automatically.

## Metrics Extraction and Figures

After simulation, use the following command to extract metrics and generate CSV/figures:

```bash
python3 simulate_results.py --log cooja_results/rpl-aer-output.log
```

This will produce:
- `metrics_summary.txt`
- `figure_data_energy.csv`
- `figure_data_qos.csv`
- ...

All files are saved in `cooja_results/` for publication and analysis.

## Personnalisation

### Ajout de nouvelles métriques

1. Modifier `rpl-aer.h` pour ajouter la structure
2. Implémenter dans `rpl-aer.c`
3. Mettre à jour `rpl_aer_calculate_mcs()`

### Modification du comportement

1. Ajuster les poids dans `project-conf.h`
2. Modifier les seuils de sécurité
3. Adapter les paramètres LSTM

### Nouveaux types de nœuds

1. Créer un nouveau fichier source
2. Ajouter au `Makefile`
3. Mettre à jour le script de scénarios

## Dépannage

### Problèmes de compilation

```bash
# Vérifier l'environnement Contiki-NG
echo $CONTIKI

# Nettoyer et recompiler
make clean
make TARGET=cooja
```

### Problèmes de simulation

1. **Cooja ne démarre pas** : Vérifier l'installation Java
2. **Firmwares manquants** : Recompiler avec `./build_and_test.sh -c`
3. **Erreurs de plugin** : Vérifier les plugins Cooja installés

### Problèmes de performance

1. **Simulation lente** : Réduire le nombre de nœuds
2. **Mémoire insuffisante** : Augmenter la heap Java
3. **Logs trop verbeux** : Ajuster `LOG_CONF_LEVEL_*` dans `project-conf.h`

## Contribution

Pour contribuer au projet :

1. Fork le repository
2. Créer une branche feature
3. Implémenter les modifications
4. Tester avec `./build_and_test.sh -c -g -t`
5. Soumettre une pull request

## Licence

Ce projet est sous licence MIT. Voir le fichier LICENSE pour plus de détails.

## Contact

Pour toute question ou suggestion :
- Créer une issue sur le repository
- Contacter l'équipe RPL-AER

---

**RPL-AER Team** - 2024

# === RPL-AER Advanced Modules ===

## LSTM Predictor (Energy Forecasting)
- Module : lstm_predictor.c/.h
- Activation : inclus par défaut, logs via [LSTM] dans la sortie Cooja
- Pour mesurer le MAE/prédiction : voir les logs [LSTM]

## MCS Calculator (Multi-Criteria Score)
- Module : mcs_calculator.c/.h
- Formule : MCS = α·NRE + β·ETXnorm + γ·Trustscore (voir logs [MCS])
- Activation : inclus par défaut

## Trust Module (Security/Anomaly Detection)
- Module : trust_module.c/.h
- Score de confiance logué via [TRUST]

## Extraction des métriques
- Les logs Cooja contiennent :
  - [MCS] : Score MCS, NRE, ETXnorm, Trust, poids dynamiques
  - [LSTM] : Historique, prédiction, MAE
  - [TRUST] : Score de confiance, composantes, poids
  - [PERF] : PDR, latency, throughput, power, attack detection

## Activation dans Cooja
- Compiler normalement (make udp-client.c TARGET=cooja ...)

## Pour toute métrique, chercher le tag correspondant dans les logs.

> **Note:** In Cooja, you must manually configure the log output redirection:
> - Open the "Simulation Log" window
> - Click "Save Log..." and select `cooja_results/rpl-aer-output.log`
> This ensures all scientific logs ([MCS], [LSTM], [HARVEST], [TRUST], [ATTACK], [FP], [TRUST_ANOMALY], [PERF]) are saved for analysis and publication.

## Final Validation Checklist

- All modules (MCS, LSTM, Trust, Harvest, Attack) are enabled in `project-conf.h`
- The simulation runs in Cooja without errors using `cooja_results/rpl-aer-simulation.csc`
- All scientific logs ([MCS], [LSTM], [HARVEST], [TRUST], [ATTACK], [FP], [TRUST_ANOMALY], [PERF]) appear in `cooja_results/rpl-aer-output.log`
- Metrics are extracted and summarized in `cooja_results/metrics_summary.txt`
- The project compiles and runs on Contiki-NG/Cooja without manual intervention
- The structure is ready for GitHub/IEEE IoT submission
