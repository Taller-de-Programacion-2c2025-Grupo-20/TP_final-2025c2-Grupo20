#ifndef SERVERSTATE_H_
#define SERVERSTATE_H_

//Struct a utilizar para enviar estado actual del servidor para un auto
struct ServerState {
    float x;
    float y;
    float angle; 
    float speed;

    ServerState() : x(0.0f), y(0.0f), angle(0.0f), speed(0.0f) {}
    ServerState(float x, float y, float angle, float speed) : x(x), y(y), angle(angle), speed(speed) {}
};
#endif