#include "bomberman.h"
#define SDL_MAIN_HANDLED
#include <SDL.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
static void bomberman_game_mode_init(game_mode_t *game_mode)
{
    game_mode->timer = 60;
}

static void bomberman_map_init(cell_t *map)
{
}

static void bomberman_player_init(player_t *player)
{
    player->position.x = 0;
    player->position.y = 0;
    player->number_of_lifes = 1;
    player->number_of_bombs = 1;
    player->score = 0;
    player->speed = 1;
}

int bomberman_graphics_init(SDL_Window **window, SDL_Renderer **renderer, SDL_Texture **texture){
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) != 0)
    {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return -1;
    }

    *window = SDL_CreateWindow("SDL is active!", 100, 100, 512, 512, 0);
    if (!*window)
    {
        SDL_Log("Unable to create window: %s", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!*renderer){
        SDL_Log("Unable to create renderer: %s", SDL_GetError());
        SDL_DestroyWindow(*window);
        SDL_Quit();
        return -1;
    }
    

    int width;
    int height;
    int channels;
    unsigned char *pixels = stbi_load("spiderman.png", &width, &height, &channels, 4);
    if (!pixels)
    {
        SDL_Log("Unable to open image");
        SDL_DestroyRenderer(*renderer);
        SDL_DestroyWindow(*window);
        SDL_Quit();
        return -1;
    }

    SDL_Log("Image width: %d height: %d channels: %d", width, height, channels);

    *texture = SDL_CreateTexture(*renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC, width, height);
    if (!*texture)    
    {
        SDL_Log("Unable to create texture: %s", SDL_GetError());
        free(pixels);
        SDL_DestroyRenderer(*renderer);
        SDL_DestroyWindow(*window);
        SDL_Quit();
        return -1;
    }

    SDL_UpdateTexture(*texture, NULL, pixels, width * 4);
    SDL_SetTextureAlphaMod(*texture, 255);
    SDL_SetTextureBlendMode(*texture, SDL_BLENDMODE_BLEND);
    free(pixels);
    return 0;
}

int main(int argc, char **argv)
{
    //main
    game_mode_t game_mode;
    cell_t map[64 * 64];
    player_t player;

    //time
    double delta_time = 0; //time between last and current frame (in seconds)
    Uint64 last_frame_time;
    Uint64 current_frame_time = 0;

    //debug
    int show_debug = 0; //0: false, 1:true
    float clock_time = 1; //debug clock - 1Hz
    float clock_counter = clock_time;

    //movement
    int window_obj_multiplier = 100;
    double delta_movement = 0;

    //input
    const Uint8 *keys;

    bomberman_game_mode_init(&game_mode);

    bomberman_map_init(map);

    bomberman_player_init(&player);

    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;

    if (bomberman_graphics_init(&window, &renderer, &texture)){
        return -1;
    }

    // game loop
    int running = 1;

    while (running)
    {
        //update delta time
        last_frame_time = current_frame_time;
        current_frame_time = SDL_GetTicks64();
        delta_time = (double)(current_frame_time - last_frame_time) * 0.001; //get delta_time in seconds
        
        if(show_debug)
        {   
            //clock functions
            clock_counter -= delta_time;
            if (clock_counter <= 0)
            {
                clock_counter = clock_time;
                printf("FPS: %f\n", 1 / delta_time); //print fps on terminal
            }
        }

        //events manager
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = 0;
            }
        }
        SDL_PumpEvents();
        keys = SDL_GetKeyboardState(NULL);
        delta_movement = player.speed * window_obj_multiplier * delta_time;
        player.position.x += (keys[SDL_SCANCODE_RIGHT] | keys[SDL_SCANCODE_D]) * (int)delta_movement;
        player.position.x -= (keys[SDL_SCANCODE_LEFT] | keys[SDL_SCANCODE_A]) * (int)delta_movement;
        player.position.y += (keys[SDL_SCANCODE_DOWN] | keys[SDL_SCANCODE_S]) * (int)delta_movement;
        player.position.y -= (keys[SDL_SCANCODE_UP] | keys[SDL_SCANCODE_W]) * (int)delta_movement;

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_Rect target_rect = {player.position.x, player.position.y, 32, 32};
        SDL_RenderCopy(renderer, texture, NULL, &target_rect);

        SDL_RenderPresent(renderer);
    }

    return 0;
}