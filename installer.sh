#!/bin/bash

GAME_NAME="taller_tp"
INSTALL_PREFIX="/usr"
BIN_DIR="$INSTALL_PREFIX/bin"
ETC_DIR="/etc/$GAME_NAME"
VAR_DIR="/var/$GAME_NAME"

REPO_DIR="$(pwd)"

# Colores
GREEN='\033[0;32m'
BLUE='\033[94m'
NC='\033[0m'

echo -e "${BLUE}=== Instalador del juego $GAME_NAME ===${NC}"
sleep 1


# ACTUALIZAR SISTEMA
echo -e "${GREEN}→ Actualizando lista de paquetes...${NC}"
sudo apt update -y

# INSTALAR DEPENDENCIAS
echo -e "${GREEN}→ Instalando dependencias...${NC}"

sudo apt install -y \
    git build-essential cmake pkg-config wget unzip \
    libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev \
    qt6-base-dev qt6-base-dev-tools

# INSTALAR BOX2D 2.1.2
echo -e "${GREEN}→ Instalando Box2D 2.1.2...${NC}"

cd /tmp
wget -q https://storage.googleapis.com/google-code-archive-downloads/v2/code.google.com/box2d/Box2D_v2.1.2.zip

unzip -q Box2D_v2.1.2.zip
cd Box2D_v2.1.2/Box2D/Build

cmake .. \
    -DBOX2D_BUILD_EXAMPLES=OFF \
    -DBOX2D_BUILD_TESTBED=OFF \
    -DBOX2D_BUILD_UNIT_TESTS=OFF

make -j"$(nproc)"
sudo make install

echo -e "${GREEN}✓ Box2D 2.1.2 instalado correctamente.${NC}"

# COMPILAR TP
echo -e "${GREEN}→ Compilando proyecto...${NC}"

cd "$REPO_DIR"
mkdir -p build
cd build

cmake .. -DTALLER_CLIENT=ON -DTALLER_EDITOR=OFF -DTALLER_SERVER=ON -DTALLER_TESTS=OFF -DCMAKE_MESSAGE_LOG_LEVEL=VERBOSE

make -j"$(nproc)"

echo -e "${GREEN}✓ Compilación finalizada.${NC}"


#ESTRUCTURA DE INSTALACIÓN
echo -e "${GREEN}→ Creando directorios...${NC}"
sudo mkdir -p "$ETC_DIR"
sudo mkdir -p "$VAR_DIR"


#COPIAR BINARIOS
echo -e "${GREEN}→ Instalando ejecutables...${NC}"

sudo cp taller_server "$BIN_DIR/$GAME_NAME-server"
sudo cp taller_client "$BIN_DIR/$GAME_NAME-client"


#   COPIAR CONFIG & DATA
#echo -e "${GREEN}→ Instalando configuración en $ETC_DIR ...${NC}"
#sudo cp ../config/*.yaml "$ETC_DIR/"

#echo -e "${GREEN}→ Instalando assets en $VAR_DIR ...${NC}"
#sudo cp -r ../client/data/* "$VAR_DIR/"

echo -e "${GREEN}============================================${NC}"
echo -e "${GREEN} Instalación completa de $GAME_NAME         ${NC}"
echo -e "${GREEN}============================================${NC}"
