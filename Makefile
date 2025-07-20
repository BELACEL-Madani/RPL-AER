# Makefile for RPL-AER Project
# RPL-AER: Adaptive Energy-Responsive RPL Protocol

CONTIKI_PROJECT = udp-client udp-server rpl-aer-sink

# Include Contiki-NG Makefile
include $(CONTIKI)/Makefile.include

# Target platforms
TARGET ?= cooja

# Compiler flags for RPL-AER
CFLAGS += -DRPL_AER=1
CFLAGS += -DMCS_METRIC=1
CFLAGS += -DLSTM_PREDICTION=1
CFLAGS += -DSECURITY_ACK=1
CFLAGS += -DRSSI_ANOMALY=1

# Enable IPv6 and RPL
CFLAGS += -DUIP_CONF_IPV6=1
CFLAGS += -DRPL_CONF_STATS=1
CFLAGS += -DRPL_CONF_LEAF_ONLY=0

# Energy monitoring
CFLAGS += -DENABLE_ENERGY_MONITOR=1
CFLAGS += -DPOWER_TRACE=1

# Debug flags
CFLAGS += -DDEBUG=1
CFLAGS += -DPRINTF=1

# Solar energy simulation
CFLAGS += -DSOLAR_ENERGY_SIM=1
CFLAGS += -DENERGY_HARVESTING=1

# Security features
CFLAGS += -DATTACK_SIMULATION=1
CFLAGS += -DSINKHOLE_DETECTION=1

# Mobility support
CFLAGS += -DMOBILITY_SUPPORT=1
CFLAGS += -DRANDOM_WAYPOINT=1

# All targets
all: $(CONTIKI_PROJECT)

# Clean
clean:
	rm -f *.o *.elf *.hex *.bin *.co
	rm -f *.csc *.log *.txt
	rm -rf build/

# Help
help:
	@echo "RPL-AER Makefile targets:"
	@echo "  all          - Build all targets"
	@echo "  clean        - Clean build files"
	@echo "  cooja        - Build for Cooja simulation"
	@echo "  z1           - Build for Z1 mote"
	@echo "  sky          - Build for Sky mote"
	@echo "  help         - Show this help" 

PROJECT_SOURCEFILES += lstm_predictor.c mcs_calculator.c trust_module.c 