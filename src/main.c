#include <SDL2/SDL.h>

#include "chip8.h"

#define SCALE 20
#define WINDOW_WIDTH WIDTH * SCALE
#define WINDOW_HEIGHT HEIGHT * SCALE

void draw_frame(SDL_Renderer **renderer, SDL_Rect *rect, chip8 *chip)
{
  rect->w = SCALE;
  rect->h = SCALE;
  SDL_SetRenderDrawColor(*renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(*renderer);

  for(int y = 0; y < HEIGHT; y++)
  {
    for(int x = 0; x < WIDTH; x++)
    {
      if(chip->graphics[x][y])
      {
        SDL_SetRenderDrawColor(*renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
      }
      else
      {
        SDL_SetRenderDrawColor(*renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
      }
      rect->x = x * SCALE;
      rect->y = y * SCALE;
      SDL_RenderFillRect(*renderer, rect);
    }
  }

  SDL_RenderPresent(*renderer);
}

int register_input(SDL_Event *ev, chip8 *chip)
{
  while(SDL_PollEvent(ev))
  {
    printf("REGISTER INPUT!!!\n");
    if(ev->type == SDL_QUIT)
    {
      return 1;
    }
    else if(ev->type == SDL_KEYDOWN)
    {
      switch(ev->key.keysym.sym)
      {
        case SDLK_1:
          chip->keypad[0] = 1;
          break;
        case SDLK_2:
          chip->keypad[1] = 1;
          break;
        case SDLK_3:
          chip->keypad[2] = 1;
          break;
        case SDLK_4:
          chip->keypad[3] = 1;
          break;
        case SDLK_q:
          chip->keypad[4] = 1;
          break;
        case SDLK_w:
          chip->keypad[5] = 1;
          break;
        case SDLK_e:
          chip->keypad[6] = 1;
          break;
        case SDLK_r:
          chip->keypad[7] = 1;
          break;
        case SDLK_a:
          chip->keypad[8] = 1;
          break;
        case SDLK_s:
          chip->keypad[9] = 1;
          break;
        case SDLK_d:
          chip->keypad[10] = 1;
          break;
        case SDLK_f:
          chip->keypad[11] = 1;
          break;
        case SDLK_z:
          chip->keypad[12] = 1;
          break;
        case SDLK_x:
          chip->keypad[13] = 1;
          break;
        case SDLK_c:
          chip->keypad[14] = 1;
          break;
        case SDLK_v:
          chip->keypad[15] = 1;
          break;
      }
    }
    else if(ev->type == SDL_KEYUP)
    {
      switch(ev->key.keysym.sym)
      {
        case SDLK_1:
          chip->keypad[0] = 0;
          break;
        case SDLK_2:
          chip->keypad[1] = 0;
          break;
        case SDLK_3:
          chip->keypad[2] = 0;
          break;
        case SDLK_4:
          chip->keypad[3] = 0;
          break;
        case SDLK_q:
          chip->keypad[4] = 0;
          break;
        case SDLK_w:
          chip->keypad[5] = 0;
          break;
        case SDLK_e:
          chip->keypad[6] = 0;
          break;
        case SDLK_r:
          chip->keypad[7] = 0;
          break;
        case SDLK_a:
          chip->keypad[8] = 0;
          break;
        case SDLK_s:
          chip->keypad[9] = 0;
          break;
        case SDLK_d:
          chip->keypad[10] = 0;
          break;
        case SDLK_f:
          chip->keypad[11] = 0;
          break;
        case SDLK_z:
          chip->keypad[12] = 0;
          break;
        case SDLK_x:
          chip->keypad[13] = 0;
          break;
        case SDLK_c:
          chip->keypad[14] = 0;
          break;
        case SDLK_v:
          chip->keypad[15] = 0;
          break;
      }
    }

  }
  return 0;
}

int main(int argc, char *argv[])
{
  chip8 chip;
  int exit_chip8 = 0;

  if(argc != 2)
  {
    print_emu_usage();
  }

//        SDL
// -------------------
  SDL_Event ev;
  SDL_Rect rect;
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Texture *texture;
  if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0)
  {
    SDL_Log("Failed to initialize SDL: %s\n", SDL_GetError());
    return -1;
  }

  window = SDL_CreateWindow("Chip8 Emu",
                            SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED,
                            WINDOW_WIDTH,
                            WINDOW_HEIGHT,
                            SDL_WINDOW_ALLOW_HIGHDPI);
  if(window == NULL)
  {
    SDL_Log("Failed to create window: %s\n", SDL_GetError());
    SDL_Quit();
    return -1;
  }

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if(renderer == NULL)
  {
    SDL_Log("Failed to create renderer: %s\n", SDL_GetError());
    SDL_DestroyWindow(window);
    SDL_Quit();
    return -1;
  }

  if(SDL_RenderSetLogicalSize(renderer, WINDOW_WIDTH, WINDOW_HEIGHT) < 0)
  {
    SDL_Log("Failed to set proper renderer size: %s\n", SDL_GetError());
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    return -1;
  }

  init_chip8_state(&chip);
  load_rom(argv[1], &chip);
  srand(time(0));

  // cuts down on the giant switch statement
  void (*func_arr[])(uint16_t, chip8*) =
  {
    exec_0_op, exec_1_op, exec_2_op, exec_3_op,
    exec_4_op, exec_5_op, exec_6_op, exec_7_op,
    exec_8_op, exec_9_op, exec_A_op, exec_B_op,
    exec_C_op, exec_D_op, exec_E_op, exec_F_op,
  };

  exit_chip8 = 0;
  while(!exit_chip8)
  {
    exit_chip8 = register_input(&ev, &chip);
    uint8_t nibble = chip.memory[chip.pc] >> 4;
    uint16_t inst = chip.memory[chip.pc] << 8 | chip.memory[chip.pc + 1];
    printf("Nibble: %02X\n", nibble);
    printf("Instruction: %04X\n", inst);
    printf("PC: %04X\n", chip.pc);
    func_arr[nibble](inst, &chip);
    print_registers(&chip);
    draw_frame(&renderer, &rect, &chip);
    chip.delay_timer--;
    chip.sound_timer--;
  }
}
