# 🚗 Manual de Usuario – Need For Speed 2D
## 1️⃣ Instalación de Dependencias y Compilación
### ⬇️ Instalación con instalador

Descarga el archivo installer.sh y ejecutá los siguientes comandos en una terminal:

```bash
chmod +x installer.sh
sudo ./installer.sh
```

El instalador descargará todas las dependencias y dejará instalado el servidor, el cliente y los archivos necesarios.
Al finalizar verás el mensaje:

```bash
============================================
Instalación completa de tp_taller
============================================
```

### 🔨 Compilación manual

Si preferís compilar el proyecto manualmente:

Clonar el repositorio:

```bash
git clone https://github.com/Taller-de-Programacion-2c2025-Grupo-20/TP_final-2025c2-Grupo20.git
```

Crear carpeta de build:
```bash
cd TP_final-2025c2-Grupo20/
mkdir build
cd build
```


Ejecutar CMake y Make:

```bash
cmake .. -DTALLER_CLIENT=ON -DTALLER_EDITOR=OFF -DTALLER_SERVER=ON -DTALLER_TESTS=OFF
make -j$(nproc)
```


Si querés compilar los tests:
```bash
cmake .. -DTALLER_TESTS=ON
make -j$(nproc)
```


Esto generará los ejecutables ./taller_server y ./taller_client.

## 2️⃣ Ejecución del Juego
### 🖥 Servidor

Abrí una terminal nueva y ejecutá:
```bash
taller_server <puerto>
```

Ejemplo:
```bash
taller_server 8080
```

Para cerrar el servidor, presioná q en la terminal.

### 👨🏻‍💻 Cliente

Ejecutá en otra terminal:
```bash
taller_client <host> <puerto>
```

Ejemplo local:
```bash
taller_client localhost 8080
```
### ➕ Extra

En el repositorio se incluyen:
```bash
client.sh

server.sh
``` 
Podés moverlos donde quieras (por ejemplo, el escritorio), darles permisos:
```bash
chmod +x client.sh server.sh
```

y ejecutarlos sin tener que escribir host ni puerto manualmente.

## 3️⃣ ¿Cómo jugar?

Al abrir el cliente, verás la pantalla de inicio:
Ingresá tu nombre de usuario para continuar.

<img width="500" alt="Captura desde 2025-12-05 03-18-19" src="https://github.com/user-attachments/assets/ebfc8b42-6f26-423b-94cb-da4bff3138dd" />


### Unirse o Crear una Partida

En el lobby tendrás dos opciones:

- Crear partida -> Elegí un nombre -> Serás el host -> Elegís el mapa y podés iniciar la partida.

- Unirse a una partida -> Seleccioná una de la lista -> Verás cuántos jugadores hay conectados.
<img width="500" alt="Captura desde 2025-12-05 03-35-10" src="https://github.com/user-attachments/assets/cf909e29-e0f6-46b1-a3f1-414eaf0b0f83" />


### Pre–partida

Si sos host, verás botones para:

- Seleccionar mapa

- Seleccionar auto

- Iniciar partida

<img width="500" alt="Captura desde 2025-12-05 11-25-25" src="https://github.com/user-attachments/assets/ec147d73-1e3b-4dc1-8e31-c8fb203b5355" />

El resto de los jugadores solo podrá ver el mapa elegido y seleccionar su auto.
<img width="500" alt="Captura desde 2025-12-05 11-26-54" src="https://github.com/user-attachments/assets/65b2abca-1257-40ba-9e40-fb7d6ef5de5a" />


## 4️⃣ Mapas y Autos
### 🗺️ Mapas disponibles

- Liberty City

- Vice City

- San Andreas

Cada mapa tiene su estilo y recorridos de carrera.

<p align="left">

  <img width="200" alt="Captura desde 2025-12-05 11-31-15" src="https://github.com/user-attachments/assets/374623d3-b0bc-4d12-94bc-924fa88c5a9a" />
  <img width="200" alt="Captura desde 2025-12-05 11-31-33" src="https://github.com/user-attachments/assets/3250c4e8-f82d-449a-bb81-c3160ae43630" />
  <img width="200" alt="Captura desde 2025-12-05 11-31-50" src="https://github.com/user-attachments/assets/74e8648e-1867-4587-8871-be2256890fba" />

</p>

### 🚘 Autos disponibles

Tenés 7 vehículos para elegir:
- Auto verde
- Auto rojo
- Descapotable
- Celeste
- Jeep
- Camioneta
- Camión

Cada auto tiene:

- Velocidad máxima
- Aceleración
- Vida
- Fuerza de giro
- Sensibilidad
- Etc.

<img width="300" alt="Captura desde 2025-12-05 11-35-55" src="https://github.com/user-attachments/assets/713ffc49-b0b4-4d82-a6fc-16fadd8d8fe2" />

## 5️⃣ Controles del Juego
### 🎮 Vehículo
| Accion             | Tecla |
|--------------------|-------|
| Acelera            | W o ↑ |
| Frenar / Reversa   | S o ↓ |
| Girar izquierda    | A o ← |
|Girar derecha       | D o → |

### 🛠️ Comprar mejoras
| Accion                 | Tecla |
|------------------------|-------|
| Mayor velocidad maxima |   1   |
| Mayor aceleracion      |   2   |
| Mas vida               |   3   |

### ❌ Cerrar el juego
| Accion       | Tecla |
|--------------|-------|
| Cerrar juego |   Q   |



## 6️⃣ La Partida
### 🏁 Objetivo

Cada partida tiene 3 carreras.
Gana quien tenga el mejor tiempo total sumando las tres.

### ⚠️ ¿Como se pierde?

Si:

- No llegás a la meta a tiempo

- Morís en la carrera

→ Resultado: obtenés el peor tiempo posible + penalización por mejoras.

### ⏱️ Mejoras antes de cada carrera

Tenés 15 segundos para comprar mejoras.
Las mejoras aparecen marcadas en amarillo.


<img width="500" alt="Captura desde 2025-12-05 11-39-12" src="https://github.com/user-attachments/assets/45c26130-17cb-471f-b3e9-6c82362e4ddf" />

### 🧭 HUD

Incluye:

- Vida

- Velocidad

- Tiempo restante

- Checkpoints faltantes

- Mejoras compradas

<img width="400" height="73" alt="Captura desde 2025-12-05 11-39-44" src="https://github.com/user-attachments/assets/68d27b87-4377-4632-8d4c-ee4bc5f8b09b" />


### 🗺️ Minimapa

Te muestra:

- Jugadores (vos sos el que tiene borde blanco)

- Posición del siguiente checkpoint (bandera verde)
<img width="200" alt="Captura desde 2025-12-05 12-27-53" src="https://github.com/user-attachments/assets/129390a9-983c-4834-ae0a-14066a5ee30b" />


### 🏎️ Choques

- Con paredes: daño según velocidad al impactar

- Con otros jugadores: recibe más daño el que venía más rápido

### 🏆 Fin de la carrera

Termina cuando:

- Pasa el tiempo (10 minutos)

- No quedan jugadores en el mapa

Al terminar todas las carreras aparece la pantalla final con: Ranking, Tiempos, “¡Victoria!” o “Has Perdido :(”

<p align="left">

  <img width="200" alt="Captura desde 2025-12-05 12-51-37" src="https://github.com/user-attachments/assets/93c1c812-ac91-4607-b58b-d2817a958fca" />
  <img width="200" alt="Captura desde 2025-12-05 12-51-28" src="https://github.com/user-attachments/assets/2cb43c02-ca3a-4ef0-b79e-afeaa5d946aa" />

</p>


## 7️⃣ Configuración del Juego

Podés modificar parámetros editando: `/etc/tp_taller/config_files/config.yaml`

Cambios posibles:

- Número de carreras

- Tiempo de carrera

- Tiempo de compra

- Daño de colisiones

- Penalización de mejoras

- Atributos de autos (velocidad, aceleración, vida, peso, giro, etc.)
