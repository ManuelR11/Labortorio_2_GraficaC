#include <iostream>
#include <SDL.h>

const int FRAMEBUFFER_WIDTH = 100;
const int FRAMEBUFFER_HEIGHT = 100;
const int FRAMEBUFFER_SIZE = FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT;
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;

struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

Color framebuffer[FRAMEBUFFER_SIZE];
Color clearColor = {0, 0, 0, 255};
Color currentColor = {255, 255, 255, 255};

// Function to set a specific pixel in the framebuffer to the currentColor
void point(int x, int y) {
    if (x >= 0 && x < FRAMEBUFFER_WIDTH && y >= 0 && y < FRAMEBUFFER_HEIGHT) {
        framebuffer[y * FRAMEBUFFER_WIDTH + x] = currentColor;
    }
}

// Function to count the number of live neighbors for a given cell
int countLiveNeighbors(int x, int y) {
    int count = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) continue; // Skip the center cell (the cell itself)
            int neighborX = x + i;
            int neighborY = y + j;
            if (neighborX >= 0 && neighborX < FRAMEBUFFER_WIDTH &&
                neighborY >= 0 && neighborY < FRAMEBUFFER_HEIGHT) {
                Color neighborColor = framebuffer[neighborY * FRAMEBUFFER_WIDTH + neighborX];
                if (neighborColor.r == 255 && neighborColor.g == 255 && neighborColor.b == 255) {
                    count++;
                }
            }
        }
    }
    return count;
}

// Function to update the state of the cells based on the game rules
void updateCells() {
    Color newFramebuffer[FRAMEBUFFER_SIZE];
    for (int y = 0; y < FRAMEBUFFER_HEIGHT; y++) {
        for (int x = 0; x < FRAMEBUFFER_WIDTH; x++) {
            int liveNeighbors = countLiveNeighbors(x, y);
            Color currentCell = framebuffer[y * FRAMEBUFFER_WIDTH + x];
            if (currentCell.r == 255 && currentCell.g == 255 && currentCell.b == 255) {
                // Cell is alive
                if (liveNeighbors < 2 || liveNeighbors > 3) {
                    // Underpopulation or overpopulation, cell dies
                    newFramebuffer[y * FRAMEBUFFER_WIDTH + x] = clearColor;
                } else {
                    // Survival, cell remains alive
                    newFramebuffer[y * FRAMEBUFFER_WIDTH + x] = currentCell;
                }
            } else {
                // Cell is dead
                if (liveNeighbors == 3) {
                    // Reproduction, dead cell becomes alive
                    newFramebuffer[y * FRAMEBUFFER_WIDTH + x] = currentColor;
                } else {
                    // Cell remains dead
                    newFramebuffer[y * FRAMEBUFFER_WIDTH + x] = clearColor;
                }
            }
        }
    }
    // Copy the new framebuffer to the original framebuffer
    memcpy(framebuffer, newFramebuffer, sizeof(newFramebuffer));
}

void renderBuffer(SDL_Renderer* renderer) {
    // Create a texture
    SDL_Texture* texture = SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_ABGR8888,
            SDL_TEXTUREACCESS_STREAMING,
            FRAMEBUFFER_WIDTH,
            FRAMEBUFFER_HEIGHT
    );

    // Update the texture with the pixel data from the framebuffer
    SDL_UpdateTexture(
            texture,
            NULL,
            framebuffer,
            FRAMEBUFFER_WIDTH * sizeof(Color)
    );

    // Copy the texture to the renderer
    SDL_RenderCopy(renderer, texture, NULL, NULL);

    // Destroy the texture
    SDL_DestroyTexture(texture);
}

void render(SDL_Renderer* renderer) {
    // Draw a point
    point(FRAMEBUFFER_WIDTH / 2, FRAMEBUFFER_HEIGHT / 2);

    // Update the cells based on the game rules
    updateCells();

    // Render the framebuffer to the screen
    renderBuffer(renderer);
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window* window = SDL_CreateWindow("The Game of Life", 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

    SDL_Renderer* renderer = SDL_CreateRenderer(
            window,
            -1,
            SDL_RENDERER_ACCELERATED
    );

    // Estado inicial del juego de la vida
    // Patrón "Glider"
    point(1, 0);
    point(2, 1);
    point(0, 2);
    point(1, 2);
    point(2, 2);

    // Patrón "Blinker"
    point(10, 10);
    point(10, 11);
    point(10, 12);

    // Patrón "Toad"
    point(20, 20);
    point(21, 20);
    point(22, 20);
    point(19, 21);
    point(20, 21);
    point(21, 21);

    // Inicialización aleatoria del estado inicial del juego de la vida
    // Comenta o ajusta esto si no deseas mantener la inicialización aleatoria original
    srand(static_cast<unsigned>(time(0)));
    for (int y = 0; y < FRAMEBUFFER_HEIGHT; y++) {
        for (int x = 0; x < FRAMEBUFFER_WIDTH; x++) {
            // Asignar aleatoriamente células vivas (probabilidad del 30%)
            if (rand() % 100 < 30) {
                point(x, y);
            }
        }
    }

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        // Call our render function
        render(renderer);

        // Present the frame buffer to the screen
        SDL_RenderPresent(renderer);

        // Delay to limit the frame rate
        SDL_Delay(100); // You can adjust this value for a suitable delay between frames
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
