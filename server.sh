#!/bin/bash

# Lanzador del servidor tp_taller

cd /usr/bin || exit 1

# Variable de config del server
export TP_TALLER_SERVER_CONFIG_FILE="/etc/tp_taller/config_files/server_config.yaml"

# Permitir cambiar puerto desde argumento, con default 8080
SERVER_PORT="${1:-9090}"

# Ejecutar servidor
./taller_server "$SERVER_PORT"
