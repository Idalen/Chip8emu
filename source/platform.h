#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>

class Platform{

    public:    
        Platform(const char* title, int windowWidth, int windowHeight, int textureWidth, int textureHeight);
        ~Platform();

        void update(void const* buffer, int pitch);
        bool process_input(uint8_t* keys);

    private:
        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_Texture* texture;

};