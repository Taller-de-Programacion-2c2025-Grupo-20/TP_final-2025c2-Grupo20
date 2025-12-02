#!/bin/bash

GAME_NAME="TP Taller Need For Speed"
INSTALL_PREFIX="/usr"
BIN_DIR="$INSTALL_PREFIX/bin"
ETC_DIR="/etc/$GAME_NAME"
VAR_DIR="/var/$GAME_NAME"
REPO_URL="https://github.com/Taller-de-Programacion-2c2025-Grupo-20/TP_final-2025c2-Grupo20.git"

WORKDIR="/tmp/${GAME_NAME}_client_repo"
REPO_DIR="$WORKDIR/TP_final-2025c2-Grupo20"

# Flag para saber si ya se instaló
CLIENT_FLAG="$ETC_DIR/client_installed.flag"

# Colores
GREEN='\033[0;32m'
BLUE='\033[94m'
NC='\033[0m'

echo -e "${BLUE}=== Instalador del CLIENTE de $GAME_NAME ===${NC}"
sleep 1

# Si ya está instalado y el binario existe, solo correr el cliente
if [ -x "$BIN_DIR/taller_client" ] && [ -f "$CLIENT_FLAG" ]; then
    echo -e "${GREEN}→ Cliente ya instalado. Saltando instalación.${NC}"

    read -p "Host del servidor (por defecto localhost): " SERVER_HOST
    SERVER_HOST=${SERVER_HOST:-localhost}

    read -p "Puerto del servidor (por defecto 9090): " SERVER_PORT
    SERVER_PORT=${SERVER_PORT:-9090}

    echo -e "${GREEN}→ Lanzando cliente contra ${SERVER_HOST}:${SERVER_PORT} ...${NC}"
    "$BIN_DIR/taller_client" "$SERVER_HOST" "$SERVER_PORT"
    exit 0
fi

# Si llega acá, es la primera vez (o no está bien instalado)

# LIMPIEZA PREVIA
sudo rm -rf "$WORKDIR"
mkdir -p "$WORKDIR"

# ACTUALIZAR SISTEMA
echo -e "${GREEN}→ Actualizando lista de paquetes...${NC}"
sudo apt update -y

# INSTALAR DEPENDENCIAS DEL CLIENTE
echo -e "${GREEN}→ Instalando dependencias del cliente...${NC}"
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

# CLONAR REPO
echo -e "${GREEN}→ Clonando repositorio...${NC}"
cd "$WORKDIR"
BRANCH="entrega_final"
git clone -b "$BRANCH" "$REPO_URL"

if [ $? -ne 0 ]; then
    echo "❌ Error al clonar el repositorio"
    exit 1
fi
echo -e "${GREEN}✓ Repositorio clonado en $REPO_DIR${NC}"

# COMPILAR SOLO CLIENT
echo -e "${GREEN}→ Compilando cliente...${NC}"
cd "$REPO_DIR"
mkdir -p build
cd build

cmake .. \
    -DTALLER_CLIENT=ON \
    -DTALLER_EDITOR=OFF \
    -DTALLER_SERVER=OFF \
    -DTALLER_TESTS=OFF \
    -DINSTALL_MODE=ON \
    -DCMAKE_MESSAGE_LOG_LEVEL=VERBOSE

make -j"$(nproc)"

echo -e "${GREEN}✓ Compilación del cliente finalizada.${NC}"

# DIRECTORIOS
echo -e "${GREEN}→ Creando directorios...${NC}"
sudo mkdir -p "$ETC_DIR"
sudo mkdir -p "$VAR_DIR"
sudo mkdir -p "$VAR_DIR/SDL_data"

# INSTALAR BINARIO DEL CLIENTE
echo -e "${GREEN}→ Instalando ejecutable del cliente...${NC}"
sudo cp taller_client "$BIN_DIR/taller_client"

# INSTALAR DATA DEL CLIENTE (sprites, sonidos, HUD, etc.)
echo -e "${GREEN}→ Instalando data del cliente en $VAR_DIR/SDL_data ...${NC}"
sudo cp -r "$REPO_DIR/client/data/"* "$VAR_DIR/SDL_data"

# Crear flag de instalación
echo -e "${GREEN}→ Marcando instalación como completada...${NC}"
echo "installed_on=$(date)" | sudo tee "$CLIENT_FLAG" >/dev/null

echo -e "${GREEN}============================================${NC}"
echo -e "${GREEN} Instalación completa del CLIENTE $GAME_NAME${NC}"
echo -e "${GREEN}============================================${NC}"

# EJECUTAR CLIENTE AL FINAL (primera vez)
read -p "Host del servidor (por defecto localhost): " SERVER_HOST
SERVER_HOST=${SERVER_HOST:-localhost}

read -p "Puerto del servidor (por defecto 9090): " SERVER_PORT
SERVER_PORT=${SERVER_PORT:-9090}

echo -e "${GREEN}→ Lanzando cliente contra ${SERVER_HOST}:${SERVER_PORT} ...${NC}"
"$BIN_DIR/taller_client" "$SERVER_HOST" "$SERVER_PORT"
