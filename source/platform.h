#include <SDL2/SDL.h>


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