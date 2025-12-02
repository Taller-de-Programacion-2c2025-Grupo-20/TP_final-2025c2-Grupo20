#!/bin/bash

# Lanzador del cliente tp_taller

cd /usr/bin || exit 1

# Variable de config del cliente
export TP_TALLER_CLIENT_CONFIG_FILE="/etc/tp_taller/config_files/client_config.yaml"

# Permitir cambiar host y puerto desde argumentos, con defaults
SERVER_HOST="${1:-localhost}"
SERVER_PORT="${2:-9090}"

# Ejecutar cliente
./taller_client "$SERVER_HOST" "$SERVER_PORT"
