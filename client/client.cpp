#include "client.h"
using namespace SDL2pp;


int Client::runClient() {
try {
	// Initialize SDL library
	SDL sdl(SDL_INIT_VIDEO);

	// Create main window: 1440x860 dimensions, resizable, "SDL2pp demo" title
	Window window("SDL2pp demo",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			1600, 800,
			SDL_WINDOW_RESIZABLE);

	// Create accelerated video renderer with default driver
	Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);

	Surface surface(DATA_PATH "/Mobile - Grand Theft Auto 4 - Miscellaneous - Cars.png");
	surface.SetColorKey(true, SDL_MapRGB(surface.Get()->format, 163, 163, 13));
	Texture sprites(renderer, surface);

	Texture background(renderer, DATA_PATH "/Game Boy _ GBC - Grand Theft Auto - Backgrounds - Liberty City.png");
    
std::array<float, 2> initial_position = protocol.getInitialPos();
float pos_x = initial_position[0];
float pos_y = initial_position[1];
int   actual_pos = 0; 

std::map<int, Rect> carPositions = {
    {0, Rect(0, 69, 40, 31)}, {1, Rect(40, 69, 40, 31)}, {2, Rect(80, 65, 40, 38)}, {3, Rect(120, 65, 40, 38)},
    {4, Rect(160, 63, 40, 42)}, {5, Rect(200, 63, 40, 41)}, {6, Rect(240, 65, 40, 38)}, {7, Rect(280, 69, 40, 31)},
    {8, Rect(0, 110, 40, 31)}, {9, Rect(40, 108, 40, 31)}, {10, Rect(80, 103, 40, 40)}, {11, Rect(120, 105, 40, 39)},
    {12, Rect(160, 104, 40, 40)}, {13, Rect(200, 104, 40, 40)}, {14, Rect(240, 104, 40, 40)}, {15, Rect(280, 104, 40, 40)}
};

constexpr uint32_t TICK_MS = 16; 
uint64_t t1 = SDL_GetTicks64();

while (true) {
    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
        if (ev.type == SDL_QUIT) {
            InputCmd quit{};
            quit.key = InputKey::Quit;
            quit.action = InputAction::Press;
            quit.client_ts_ms = SDL_GetTicks64();
            protocol.sendInput(quit);
            return 0;
        }
        if (ev.type == SDL_KEYDOWN || ev.type == SDL_KEYUP) {
            InputCmd cmd{};
            cmd.client_ts_ms = SDL_GetTicks64();
            cmd.action = (ev.type == SDL_KEYDOWN) ? InputAction::Press : InputAction::Release;
            switch (ev.key.keysym.sym) {
                case SDLK_UP:    cmd.key = InputKey::Up;    break;
                case SDLK_DOWN:  cmd.key = InputKey::Down;  break;
                case SDLK_LEFT:  cmd.key = InputKey::Left;  break;
                case SDLK_RIGHT: cmd.key = InputKey::Right; break;
                case SDLK_q:
                case SDLK_ESCAPE: cmd.key = InputKey::Quit; break;
                default:         cmd.key = InputKey::Unknown; break;
            }
            protocol.sendInput(cmd);
            if (cmd.key == InputKey::Quit && cmd.action == InputAction::Press) return 0;
        }
    }

    uint64_t now = SDL_GetTicks64();
    int64_t diff = static_cast<int64_t>(now - t1);


    int64_t ticks_to_run = diff / TICK_MS;  
    for (int64_t i = 0; i < ticks_to_run; ++i) {
        protocol.serverTick(TICK_MS);  
        t1 += TICK_MS;
    }

    if (auto st = protocol.pollState()) {
        pos_x = st->x;
        pos_y = st->y;
        actual_pos = st->sprite_idx;
    }

    renderer.SetDrawColor(0, 0, 0, 255);
    renderer.Clear();

    Rect srcRect(0, 0, 1600, 800), dstRect(0, 0, 1600, 800);
    renderer.Copy(background, srcRect, dstRect);

    const Rect& spr = carPositions[actual_pos];
    int draw_x = static_cast<int>(std::lround(pos_x));
    int draw_y = static_cast<int>(std::lround(pos_y));
    renderer.Copy(sprites, spr, Rect(draw_x, draw_y, spr.GetW(), spr.GetH()));

    renderer.Present();

    now  = SDL_GetTicks64();
    diff = static_cast<int64_t>(t1 - now);
    if (diff > 0) SDL_Delay(static_cast<uint32_t>(diff));
}

	return 0;
} catch (std::exception& e) {
	std::cerr << e.what() << std::endl;
	return 1;
}
}

Client::Client(const char* hostname, const char* port)
        :protocol(ClientProtocol(hostname, port)) 
        {}

int main(int argc, const char* argv[]) {

    if (argc != 3) {
        std::cerr << "Cantidad de parametros incorrecta\n";
        return EXIT_FAILURE;
    }

    Client client(argv[1], argv[2]);

    client.runClient();

    return EXIT_SUCCESS;
}
