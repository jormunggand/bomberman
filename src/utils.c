#include "utils.h"

#include <stdbool.h>



bool is_in_tab(int x, int* tab, int size) {
    for (int i = 0; i < size; i++)
        if (tab[i] == x)
            return true;
    return false;
}

int max(int a, int b) {
    return a > b ? a : b;
}

int min(int a, int b) {
    return a < b ? a : b;
}

int init(SDL_Window **window, SDL_Renderer **renderer, int w, int h){
    if( 0 != SDL_Init(SDL_INIT_VIDEO)) 
    {
        fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
        return -1;
    }
    if (0 != TTF_Init()) {
        fprintf(stderr, "Erreur TTF_Init : %s", TTF_GetError());
    }
    if (0 != SDLNet_Init())  
    {
        fprintf(stderr, "Erreur SDLNet_Init : %s", SDLNet_GetError());
        return -1;
    }


    if(0 != SDL_CreateWindowAndRenderer(w, h, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE, window, renderer))
    {
        fprintf(stderr, "Erreur SDL_CreateWindowAndRenderer : %s", SDL_GetError());
        return -1;
    }
    return 0;
}

SDL_Texture* loadImage(const char path[], SDL_Renderer *renderer){
    SDL_Surface *tmp = NULL; 
    SDL_Texture *texture = NULL;
    tmp = IMG_Load(path);
    if(NULL == tmp)
    {
        fprintf(stderr, "Erreur IMG_Load : %s", SDL_GetError());
        return NULL;
    }
    texture = SDL_CreateTextureFromSurface(renderer, tmp);
    SDL_FreeSurface(tmp);
    if(NULL == texture) {
        fprintf(stderr, "Erreur SDL_CreateTextureFromSurface : %s", SDL_GetError());
        return NULL;
    }
    return texture;
}

int setColor(SDL_Renderer *renderer, SDL_Color color){
    return SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

bool point_in_rect(SDL_Rect rect, int x, int y) {
    return x >= rect.x && (x <= rect.x + rect.w) && y >= rect.y && (y <= rect.y + rect.h);
} 
double **createGaussianKernel(int radius, double sigma) {
    int size = 2 * radius + 1;
    double **kernel = malloc(size * sizeof(double *));
    for (int i = 0; i < size; i++) {
        kernel[i] = malloc(size * sizeof(double));
    }

    double sum = 0.0;
    for (int y = -radius; y <= radius; y++) {
        for (int x = -radius; x <= radius; x++) {
            double exponent = -(x*x + y*y) / (2 * sigma * sigma);
            kernel[y + radius][x + radius] = exp(exponent) / (2 * M_PI * sigma * sigma);
            sum += kernel[y + radius][x + radius];
        }
    }

    // Normalize the kernel
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            kernel[y][x] /= sum;
        }
    }

    return kernel;
}

double *create1DGaussianKernel(int radius, double sigma) {
    int size = 2 * radius + 1;
    double *kernel = malloc(size * sizeof(double));

    double sum = 0.0;
    for (int x = -radius; x <= radius; x++) {
        double exponent = -(x*x) / (2 * sigma * sigma);
        kernel[x + radius] = exp(exponent) / (sqrt(2 * M_PI) * sigma);
        sum += kernel[x + radius];
    }

    // Normalize the kernel
    for (int x = 0; x < size; x++) {
        kernel[x] /= sum;
    }

    return kernel;
}

void gaussian_blur(Uint32 *pixels, int width, int height) {
    // Define the 1D Gaussian kernel
    double* kernel = create1DGaussianKernel(10, 10.0);

    // Create new arrays to store the intermediate and final blurred pixels
    Uint32 *intermediate_pixels = malloc(width * height * sizeof(Uint32));
    Uint32 *blurred_pixels = malloc(width * height * sizeof(Uint32));

    SDL_PixelFormat *format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);

    // Apply the Gaussian kernel horizontally
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            double red = 0.0, green = 0.0, blue = 0.0;
            for (int kx = -10; kx <= 10; kx++) {
                int nx = x + kx;
                if (nx >= 0 && nx < width) {
                    Uint32 pixel = pixels[y * width + nx];
                    Uint8 r, g, b;
                    SDL_GetRGB(pixel, format, &r, &g, &b);
                    red += r * kernel[kx + 10];
                    green += g * kernel[kx + 10];
                    blue += b * kernel[kx + 10];
                }
            }
            intermediate_pixels[y * width + x] = SDL_MapRGBA(format, (int)red, (int)green, (int)blue, 255);
        }
    }

    // Apply the Gaussian kernel vertically
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            double red = 0.0, green = 0.0, blue = 0.0;
            for (int ky = -10; ky <= 10; ky++) {
                int ny = y + ky;
                if (ny >= 0 && ny < height) {
                    Uint32 pixel = intermediate_pixels[ny * width + x];
                    Uint8 r, g, b;
                    SDL_GetRGB(pixel, format, &r, &g, &b);
                    red += r * kernel[ky + 10];
                    green += g * kernel[ky + 10];
                    blue += b * kernel[ky + 10];
                }
            }
            blurred_pixels[y * width + x] = SDL_MapRGBA(format, (int)red, (int)green, (int)blue, 255);
        }
    }

    // Replace the original pixel array with the blurred one
    memcpy(pixels, blurred_pixels, width * height * sizeof(Uint32));

    // Free the memory
    free(intermediate_pixels);
    free(blurred_pixels);
    free(kernel);
}