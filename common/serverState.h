#ifndef SERVERSTATE_H_
#define SERVERSTATE_H_

//Struct a utilizar para enviar estado actual del servidor para un auto
struct ServerState {
    float x;
    float y;
    float angle; 

    ServerState(float x, float y, float angle) : x(x), y(y), angle(angle) {}
};
#endif