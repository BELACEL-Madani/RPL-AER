#!/bin/bash

# Script de compilation et test pour RPL-AER
# Usage: ./build_and_test.sh [options]

set -e  # Arrêter en cas d'erreur

# Couleurs pour l'affichage
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Variables par défaut
CONTIKI_PATH="/home/belacel/contiki-ng"
PROJECT_DIR="$(pwd)"
BUILD_TARGET="cooja"
CLEAN_BUILD=false
GENERATE_CSC=false
TEST_SIMULATION=false

# Fonction d'affichage
print_info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Fonction d'aide
show_help() {
    echo "Script de compilation et test pour RPL-AER"
    echo ""
    echo "Usage: $0 [options]"
    echo ""
    echo "Options:"
    echo "  -c, --clean          Nettoyer avant compilation"
    echo "  -g, --generate-csc   Générer un nouveau fichier .csc"
    echo "  -t, --test           Tester la simulation"
    echo "  -h, --help           Afficher cette aide"
    echo ""
    echo "Exemples:"
    echo "  $0                    # Compilation simple"
    echo "  $0 -c -g             # Nettoyer, compiler et générer .csc"
    echo "  $0 -g -t             # Générer .csc et tester"
}

# Parsing des arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -c|--clean)
            CLEAN_BUILD=true
            shift
            ;;
        -g|--generate-csc)
            GENERATE_CSC=true
            shift
            ;;
        -t|--test)
            TEST_SIMULATION=true
            shift
            ;;
        -h|--help)
            show_help
            exit 0
            ;;
        *)
            print_error "Option inconnue: $1"
            show_help
            exit 1
            ;;
    esac
done

# Ajout de l'option --full
if [[ "$1" == "--full" ]]; then
    echo -e "\033[1;34m[INFO]\033[0m Mode FULL: build, csc, simulation, analyse..."
    ./build_and_test.sh -c -g
    # Lancement Cooja en batch si possible (sinon message)
    if command -v cooja &> /dev/null; then
        echo -e "\033[1;34m[INFO]\033[0m Lancement de Cooja en mode graphique. Veuillez charger le scénario : cooja_results/rpl-aer-simulation.csc"
        echo -e "\033[1;33m[ATTENTION]\033[0m Après la simulation, sauvegardez les logs dans cooja_results/rpl-aer-output.log puis relancez ./build_and_test.sh --full pour l'analyse."
        exit 0
    else
        echo -e "\033[1;31m[ERREUR]\033[0m Cooja non trouvé. Installez-le ou lancez la simulation manuellement."
        exit 1
    fi
    if [ -f "cooja_results/rpl-aer-output.log" ]; then
        python3 analyze_logs.py
        echo -e "\033[1;32m[SUCCESS]\033[0m Analyse terminée. Résultats dans cooja_results/"
    else
        echo -e "\033[1;31m[ERREUR]\033[0m Log de simulation non trouvé. Vérifiez la simulation."
        exit 1
    fi
    exit 0
}

# Vérification de l'environnement
print_info "Vérification de l'environnement..."

if [ ! -d "$CONTIKI_PATH" ]; then
    print_error "Contiki-NG non trouvé à: $CONTIKI_PATH"
    print_info "Veuillez modifier CONTIKI_PATH dans le script"
    exit 1
fi

if [ ! -f "Makefile" ]; then
    print_error "Makefile non trouvé dans le répertoire courant"
    exit 1
fi

print_success "Environnement vérifié"

# Nettoyage si demandé
if [ "$CLEAN_BUILD" = true ]; then
    print_info "Nettoyage du projet..."
    make clean
    print_success "Nettoyage terminé"
fi

# Compilation
print_info "Compilation pour la cible $BUILD_TARGET..."

# Compilation des firmwares
make TARGET=$BUILD_TARGET rpl-aer-client.csc
make TARGET=$BUILD_TARGET rpl-aer-sink.csc

print_success "Compilation terminée"


if [ -f "rpl-aer-client.csc" ]; then
else
    print_error "Firmware client manquant"
    exit 1
fi

if [ -f "rpl-aer-sink.csc" ]; then
else
    print_error "Firmware sink manquant"
    exit 1
fi

# Génération du fichier .csc si demandé
if [ "$GENERATE_CSC" = true ]; then
    print_info "Génération d'un nouveau fichier .csc..."

    if [ -f "generate_csc.py" ]; then
        python3 generate_csc.py -n 39 -s 0.3 -m 0.3 -o rpl-aer-simulation-generated.csc
    else
        print_warning "Script generate_csc.py non trouvé, utilisation du fichier existant"
    fi
fi

# Test de la simulation si demandé
if [ "$TEST_SIMULATION" = true ]; then
    print_info "Test de la simulation..."

    # Vérification de Cooja
    if command -v cooja &> /dev/null; then
        print_info "Cooja trouvé, lancement de la simulation..."
        # Note: Cooja nécessite une interface graphique
        print_warning "Cooja nécessite une interface graphique pour fonctionner"
        print_info "Pour lancer manuellement: cooja rpl-aer-simulation.csc"
    else
        print_warning "Cooja non trouvé dans le PATH"
        print_info "Pour installer Cooja:"
        print_info "  cd $CONTIKI_PATH/tools/cooja"
        print_info "  ant jar"
    fi
fi

# Affichage des informations finales
print_success "Build terminé avec succès!"
echo ""
echo "  - rpl-aer-client.csc (firmware client)"
echo "  - rpl-aer-sink.csc (firmware sink)"
echo "  - rpl-aer-simulation.csc (scénario de simulation)"
if [ "$GENERATE_CSC" = true ]; then
fi
echo ""
print_info "Pour lancer la simulation:"
echo "  cooja rpl-aer-simulation.csc"
echo ""
print_info "Pour générer un nouveau scénario:"
echo "  python3 generate_csc.py -n 50 -s 0.4 -m 0.2"
echo ""
print_info "Pour nettoyer:"
echo "  make clean"