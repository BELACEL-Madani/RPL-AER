#!/usr/bin/env python3
"""
Usage: python3 generate_csc.py [options]
"""

import argparse
import random
import math
import sys
from typing import List, Tuple

class CSCGenerator:
    def __init__(self,
                 num_clients: int = 39,
                 solar_ratio: float = 0.3,
                 mobile_ratio: float = 0.3,
                 area_size: Tuple[float, float] = (500, 500),
                 sink_position: Tuple[float, float] = (50, 50),
                 random_seed: int = 12345,
                 simulation_duration: int = 3600):
        """
        Initialise le script de fichiers .csc

        Args:
            num_clients: Nombre de nœuds clients
            solar_ratio: Ratio de nœuds solaires (0.0-1.0)
            mobile_ratio: Ratio de nœuds mobiles (0.0-1.0)
            area_size: Taille de la zone de simulation (width, height)
            sink_position: Position du nœud sink (x, y)
            simulation_duration: Durée de simulation en secondes
        """
        self.num_clients = num_clients
        self.solar_ratio = solar_ratio
        self.mobile_ratio = mobile_ratio
        self.area_size = area_size
        self.sink_position = sink_position
        self.random_seed = random_seed
        self.simulation_duration = simulation_duration

        # Calcul des nombres de nœuds par type
        self.num_solar = int(num_clients * solar_ratio)
        self.num_mobile = int(num_clients * mobile_ratio)

        # Initialisation du script aléatoire
        random.seed(random_seed)

    def generate_random_position(self, min_distance: float = 20.0) -> Tuple[float, float]:
        """Génère une position aléatoire avec distance minimale du sink"""
        while True:
            x = random.uniform(0, self.area_size[0])
            y = random.uniform(0, self.area_size[1])

            # Vérifier la distance minimale du sink
            distance = math.sqrt((x - self.sink_position[0])**2 + (y - self.sink_position[1])**2)
            if distance >= min_distance:
                return (x, y)

    def generate_mote_attributes(self, mote_id: int) -> str:
        """Génère les attributs d'un nœud mote"""
        # Déterminer le type d'énergie
        if mote_id < self.num_solar:
            energy_type = "solar"
        else:
            energy_type = "battery"

        # Déterminer la mobilité
        if mote_id < self.num_mobile:
            mobility_type = "mobile"
        else:
            mobility_type = "static"

        return f"energy={energy_type};mobility={mobility_type};type=client;id={mote_id}"

    def generate_client_mote(self, mote_id: int) -> str:
        """Génère le XML pour un nœud client"""
        position = self.generate_random_position()
        attributes = self.generate_mote_attributes(mote_id)

        return f"""      <!-- Client {mote_id}: {attributes.split(';')[0].split('=')[1].title()} + {attributes.split(';')[1].split('=')[1].title()} -->
      <mote>
        <motetype_identifier>RPL_AER_CLIENT</motetype_identifier>
        <interface_config>
          <interface_class>org.contikios.cooja.interfaces.Position</interface_class>
          <x>{position[0]:.1f}</x>
          <y>{position[1]:.1f}</y>
        </interface_config>
        <interface_config>
          <interface_class>org.contikios.cooja.interfaces.MoteAttributes</interface_class>
          <attributes>{attributes}</attributes>
        </interface_config>
      </mote>"""

    def generate_csc_content(self) -> str:
        """Génère le contenu complet du fichier .csc"""
        # En-tête du fichier
        header = f'''<?xml version="1.0" encoding="UTF-8"?>
<simconf xmlns="http://www.sics.se/contiki/cooja">
  <simulation>
    <title>RPL-AER Simulation (Generated)</title>
    <randomseed>{self.random_seed}</randomseed>
    <motedelay_us>10000</motedelay_us>
    <radiomedium>org.contikios.cooja.radiomediums.UDGM</radiomedium>
    <motetype>
      <identifier>RPL_AER_CLIENT</identifier>
      <description>RPL-AER UDP Client</description>
      <source>udp-client.c</source>
      <firmware>rpl-aer-client.csc</firmware>
    </motetype>
    <motetype>
      <identifier>RPL_AER_SERVER</identifier>
      <description>RPL-AER UDP Server (Sink)</description>
      <source>rpl-aer-sink.c</source>
      <firmware>rpl-aer-sink.csc</firmware>
    </motetype>
    <motes>
      <!-- Sink node -->
      <mote>
        <motetype_identifier>RPL_AER_SERVER</motetype_identifier>
        <interface_config>
          <interface_class>org.contikios.cooja.interfaces.Position</interface_class>
          <x>{self.sink_position[0]:.1f}</x>
          <y>{self.sink_position[1]:.1f}</y>
        </interface_config>
      </mote>'''

        # Génération des nœuds clients
        client_motes = []
        for i in range(self.num_clients):
            client_motes.append(self.generate_client_mote(i + 1))

        # Fin du fichier
        footer = f'''
    </motes>
    <events>
      <!-- Démarrage de la simulation -->
      <event>
        <time>0</time>
        <command>log.log("=== RPL-AER Simulation démarrée (Generated) ===\\n");</command>
      </event>
      <!-- Arrêt de la simulation après {self.simulation_duration}s -->
      <event>
        <time>{self.simulation_duration * 1000}</time>
        <command>log.log("=== RPL-AER Simulation terminée ===\\n"); sim.stop();</command>
      </event>
    </events>
    <plugin>
      <classname>org.contikios.cooja.plugins.LogListener</classname>
      <plugin_config/>
      <width>800</width>
      <height>200</height>
      <location_x>0</location_x>
      <location_y>0</location_y>
    </plugin>
    <plugin>
      <classname>org.contikios.cooja.plugins.PowerTracker</classname>
      <plugin_config/>
      <width>400</width>
      <height>200</height>
      <location_x>0</location_x>
      <location_y>210</location_y>
    </plugin>
    <plugin>
      <classname>org.contikios.cooja.plugins.PacketLossTracker</classname>
      <plugin_config/>
      <width>400</width>
      <height>200</height>
      <location_x>410</location_x>
      <location_y>210</location_y>
    </plugin>
    <plugin>
      <classname>org.contikios.cooja.plugins.Visualizer</classname>
      <plugin_config/>
      <width>800</width>
      <height>600</height>
      <location_x>0</location_x>
      <location_y>420</location_y>
    </plugin>
  </simulation>
</simconf>'''

        return header + '\n'.join(client_motes) + footer

    def save_to_file(self, filename: str):
        content = self.generate_csc_content()
        with open(filename, 'w') as f:
            f.write(content)
        print(f"Configuration: {self.num_clients} clients, {self.num_solar} solaires, {self.num_mobile} mobiles")

def main():
    parser = argparse.ArgumentParser(description="Script de fichiers .csc pour RPL-AER")
    parser.add_argument("-n", "--num-clients", type=int, default=39,
                       help="Nombre de nœuds clients (défaut: 39)")
    parser.add_argument("-s", "--solar-ratio", type=float, default=0.3,
                       help="Ratio de nœuds solaires (0.0-1.0, défaut: 0.3)")
    parser.add_argument("-m", "--mobile-ratio", type=float, default=0.3,
                       help="Ratio de nœuds mobiles (0.0-1.0, défaut: 0.3)")
    parser.add_argument("-w", "--width", type=float, default=500.0,
                       help="Largeur de la zone de simulation (défaut: 500)")
    parser.add_argument("-H", "--height", type=float, default=500.0,
                       help="Hauteur de la zone de simulation (défaut: 500)")
    parser.add_argument("-d", "--duration", type=int, default=3600,
                       help="Durée de simulation en secondes (défaut: 3600)")
    parser.add_argument("-r", "--random-seed", type=int, default=12345,
    parser.add_argument("-o", "--output", type=str, default="rpl-aer-simulation-generated.csc",
                       help="Nom du fichier de sortie (défaut: rpl-aer-simulation-generated.csc)")

    args = parser.parse_args()

    # Validation des paramètres
    if args.solar_ratio < 0 or args.solar_ratio > 1:
        print("Erreur: solar_ratio doit être entre 0.0 et 1.0")
        sys.exit(1)

    if args.mobile_ratio < 0 or args.mobile_ratio > 1:
        print("Erreur: mobile_ratio doit être entre 0.0 et 1.0")
        sys.exit(1)

    # Génération du fichier
    generator = CSCGenerator(
        num_clients=args.num_clients,
        solar_ratio=args.solar_ratio,
        mobile_ratio=args.mobile_ratio,
        area_size=(args.width, args.height),
        simulation_duration=args.duration,
        random_seed=args.random_seed
    )

    generator.save_to_file(args.output)

    # Affichage des statistiques
    print(f"\nStatistiques de la simulation:")
    print(f"- Nœuds clients: {args.num_clients}")
    print(f"- Nœuds solaires: {generator.num_solar} ({args.solar_ratio*100:.1f}%)")
    print(f"- Nœuds mobiles: {generator.num_mobile} ({args.mobile_ratio*100:.1f}%)")
    print(f"- Zone de simulation: {args.width}x{args.height}")
    print(f"- Durée: {args.duration} secondes")
    print(f"- Seed aléatoire: {args.random_seed}")

if __name__ == "__main__":
    main()