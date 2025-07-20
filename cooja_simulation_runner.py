#!/usr/bin/env python3
"""
Script d'automatisation des simulations Cooja pour RPL-AER
Collecte des données réelles pour les figures de l'article
"""

import subprocess
import os
import time
import json
import re
import argparse
from typing import Dict, List, Tuple
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

class CoojaSimulationRunner:
    def __init__(self, contiki_path: str = "/home/belacel/contiki-ng"):
        self.contiki_path = contiki_path
        self.project_dir = "/home/belacel/contiki-ng/examples/RPL_AER"
        self.results_dir = "cooja_results"
        self.log_file = "cooja_simulation.log"

        # Créer le dossier de résultats
        os.makedirs(self.results_dir, exist_ok=True)

    def build_firmware(self) -> bool:
        """Compile les firmwares pour Cooja"""
        print("🔨 Compilation des firmwares...")

        try:
            # Aller dans le répertoire du projet
            os.chdir(self.project_dir)

            # Compilation des firmwares
            subprocess.run([
                "make", "TARGET=cooja", "clean"
            ], check=True, capture_output=True)

            subprocess.run([
                "make", "TARGET=cooja", "rpl-aer-client.csc"
            ], check=True, capture_output=True)

            subprocess.run([
                "make", "TARGET=cooja", "rpl-aer-sink.csc"
            ], check=True, capture_output=True)

            print(" Firmwares compilés avec succès")
            return True

        except subprocess.CalledProcessError as e:
            print(f"❌ Erreur de compilation: {e}")
            return False

    def generate_simulation_scenario(self, num_nodes: int = 40,
                                   solar_ratio: float = 0.3,
                                   mobile_ratio: float = 0.3) -> str:
        """Génère un scénario de simulation Cooja"""
        print(f"📋 Génération du scénario: {num_nodes} nœuds...")

        # Utiliser le script existant
        cmd = [
            "python3", "generate_csc.py",
            "-n", str(num_nodes),
            "-s", str(solar_ratio),
            "-m", str(mobile_ratio),
            "-o", "rpl-aer-simulation-cooja.csc"
        ]

        try:
            subprocess.run(cmd, check=True, cwd=self.project_dir)
            return "rpl-aer-simulation-cooja.csc"
        except subprocess.CalledProcessError as e:
            return None

    def run_cooja_simulation(self, scenario_file: str, duration: int = 3600) -> bool:
        """Lance une simulation Cooja"""
        print(f" Lancement de la simulation Cooja...")
        print(f"   Scénario: {scenario_file}")
        print(f"   Durée: {duration} secondes")

        # Commande pour lancer Cooja en mode headless
        cmd = [
            "java", "-jar", f"{self.contiki_path}/tools/cooja/dist/cooja.jar",
            "-nogui", scenario_file,
            "-contiki", self.contiki_path
        ]

        try:
            # Lancer la simulation
            process = subprocess.Popen(
                cmd,
                cwd=self.project_dir,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                text=True
            )

            # Attendre la fin de la simulation
            print("⏳ Simulation en cours...")
            stdout, stderr = process.communicate(timeout=duration + 300)  # +5 min de marge

            # Sauvegarder les logs
            with open(f"{self.results_dir}/{self.log_file}", "w") as f:
                f.write("=== STDOUT ===\n")
                f.write(stdout)
                f.write("\n=== STDERR ===\n")
                f.write(stderr)

            if process.returncode == 0:
                print(" Simulation terminée avec succès")
                return True
            else:
                print(f"❌ Simulation échouée (code: {process.returncode})")
                return False

        except subprocess.TimeoutExpired:
            print("❌ Simulation interrompue (timeout)")
            process.kill()
            return False
        except Exception as e:
            print(f"❌ Erreur lors de la simulation: {e}")
            return False

    def parse_simulation_logs(self) -> Dict:
        """Parse les logs de simulation pour extraire les métriques"""
        print(" Analyse des logs de simulation...")

        log_path = f"{self.results_dir}/{self.log_file}"
        if not os.path.exists(log_path):
            print("❌ Fichier de log non trouvé")
            return {}

        metrics = {
            'energy_consumption': [],
            'packet_delivery_ratio': [],
            'latency': [],
            'throughput': [],
            'network_lifetime': 0,
            'total_packets': 0,
            'delivered_packets': 0,
            'failed_packets': 0
        }

        try:
            with open(log_path, 'r') as f:
                content = f.read()

            # Extraction des métriques (patterns à adapter selon les logs réels)

            # Consommation énergétique
            energy_pattern = r"Energy consumption: (\d+\.?\d*) mWh"
            energy_matches = re.findall(energy_pattern, content)
            if energy_matches:
                metrics['energy_consumption'] = [float(x) for x in energy_matches]

            # PDR
            pdr_pattern = r"PDR: (\d+\.?\d*)%"
            pdr_matches = re.findall(pdr_pattern, content)
            if pdr_matches:
                metrics['packet_delivery_ratio'] = [float(x) for x in pdr_matches]

            # Latence
            latency_pattern = r"Latency: (\d+\.?\d*) ms"
            latency_matches = re.findall(latency_pattern, content)
            if latency_matches:
                metrics['latency'] = [float(x) for x in latency_matches]

            # Throughput
            throughput_pattern = r"Throughput: (\d+\.?\d*) pkt/min"
            throughput_matches = re.findall(throughput_pattern, content)
            if throughput_matches:
                metrics['throughput'] = [float(x) for x in throughput_matches]

            print(f" Métriques extraites: {len(energy_matches)} échantillons")
            return metrics

        except Exception as e:
            print(f"❌ Erreur lors de l'analyse des logs: {e}")
            return {}

    def generate_figures_from_real_data(self, metrics: Dict):
        """Génère les figures à partir des données réelles de Cooja"""
        print("🎨 Génération des figures à partir des données réelles...")

        if not metrics:
            print("❌ Aucune métrique disponible")
            return

        # Figure 1: Consommation énergétique
        if metrics.get('energy_consumption'):
            plt.figure(figsize=(10, 6))
            plt.plot(metrics['energy_consumption'], 'b-', linewidth=2)
            plt.title('Consommation Énergétique Réelle (Cooja)')
            plt.xlabel('Temps (intervalles)')
            plt.ylabel('Consommation (mWh)')
            plt.grid(True, alpha=0.3)
            plt.savefig(f"{self.results_dir}/real_energy_consumption.png", dpi=300, bbox_inches='tight')
            plt.close()

        # Figure 2: PDR
        if metrics.get('packet_delivery_ratio'):
            plt.figure(figsize=(10, 6))
            plt.plot(metrics['packet_delivery_ratio'], 'g-', linewidth=2)
            plt.title('Packet Delivery Ratio Réel (Cooja)')
            plt.xlabel('Temps (intervalles)')
            plt.ylabel('PDR (%)')
            plt.grid(True, alpha=0.3)
            plt.savefig(f"{self.results_dir}/real_pdr.png", dpi=300, bbox_inches='tight')
            plt.close()

        # Figure 3: Latence
        if metrics.get('latency'):
            plt.figure(figsize=(10, 6))
            plt.plot(metrics['latency'], 'r-', linewidth=2)
            plt.title('Latence Réelle (Cooja)')
            plt.xlabel('Temps (intervalles)')
            plt.ylabel('Latence (ms)')
            plt.grid(True, alpha=0.3)
            plt.savefig(f"{self.results_dir}/real_latency.png", dpi=300, bbox_inches='tight')
            plt.close()

        # Sauvegarder les métriques en JSON
        with open(f"{self.results_dir}/real_metrics.json", 'w') as f:
            json.dump(metrics, f, indent=2)
        print(" Métriques sauvegardées en JSON")

    def run_complete_simulation(self, num_nodes: int = 40,
                               solar_ratio: float = 0.3,
                               mobile_ratio: float = 0.3,
                               duration: int = 3600) -> bool:
        """Exécute une simulation complète avec Cooja"""
        print(" === SIMULATION COOJA RPL-AER ===")
        print(f" Configuration:")
        print(f"   - Nœuds: {num_nodes}")
        print(f"   - Solaires: {solar_ratio*100:.1f}%")
        print(f"   - Mobiles: {mobile_ratio*100:.1f}%")
        print(f"   - Durée: {duration}s")
        print()

        # Étape 1: Compilation
        if not self.build_firmware():
            return False

        # Étape 2: Génération du scénario
        scenario_file = self.generate_simulation_scenario(num_nodes, solar_ratio, mobile_ratio)
        if not scenario_file:
            return False

        # Étape 3: Simulation
        if not self.run_cooja_simulation(scenario_file, duration):
            return False

        # Étape 4: Analyse des résultats
        metrics = self.parse_simulation_logs()

        # Étape 5: Génération des figures
        self.generate_figures_from_real_data(metrics)

        print(" Simulation Cooja terminée avec succès!")
        print(f" Résultats dans: {self.results_dir}/")

        return True

def main():
    parser = argparse.ArgumentParser(description="Runner de simulation Cooja pour RPL-AER")
    parser.add_argument("-n", "--nodes", type=int, default=40,
                       help="Nombre de nœuds (défaut: 40)")
    parser.add_argument("-s", "--solar", type=float, default=0.3,
                       help="Ratio de nœuds solaires (défaut: 0.3)")
    parser.add_argument("-m", "--mobile", type=float, default=0.3,
                       help="Ratio de nœuds mobiles (défaut: 0.3)")
    parser.add_argument("-d", "--duration", type=int, default=3600,
                       help="Durée de simulation en secondes (défaut: 3600)")
    parser.add_argument("-c", "--contiki-path", type=str,
                       default="/home/belacel/contiki-ng",
                       help="Chemin vers Contiki-NG")

    args = parser.parse_args()

    # Créer et exécuter le runner
    runner = CoojaSimulationRunner(args.contiki_path)
    success = runner.run_complete_simulation(
        num_nodes=args.nodes,
        solar_ratio=args.solar,
        mobile_ratio=args.mobile,
        duration=args.duration
    )

    if success:
        print("\n Simulation Cooja réussie!")
        print(" Les données réelles sont maintenant disponibles")
    else:
        print("\n❌ Simulation Cooja échouée")
        print("🔧 Vérifiez les logs pour plus de détails")

if __name__ == "__main__":
    main()