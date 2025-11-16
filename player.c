#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <windows.h>
#include "include/stb_image.h"
// #include "stb_image_resize2.h"
#include "include/old_image_resize.h"

HANDLE hConsole;

char ramp[] = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/|()1{}[]?-_+~<>i!lI;:,\"^`'. ";

char map_intensity(unsigned char g)
{
    int n = sizeof(ramp) - 1; // number of characters
    int idx = (g * n) / 255;
    if (idx > n) idx = n;
    return ramp[idx];
}

void enhance_color(unsigned char *r, unsigned char *g, unsigned char *b)
{
    float factor = 1.25f; // brightness multiplier
    *r = (unsigned char) fmin((*r) * factor, 255);
    *g = (unsigned char) fmin((*g) * factor, 255);
    *b = (unsigned char) fmin((*b) * factor, 255);

}


void set_cursor(int x, int y)
{
    COORD coord = {
        .X = x,
        .Y = y,
    };
    SetConsoleCursorPosition(hConsole, coord);
}

void set_truecolor(unsigned char r, unsigned char g, unsigned char b)
{
    printf("\x1b[38;2;%d;%d;%dm", r, g, b);
}

void show_frame(stbi_uc *input_data, int width, int height,
                stbi_uc *output_data, int target_width, int target_height, char *frame_buffer)
{
    // Resize
    stbir_resize_uint8(
        input_data, width, height, 0,
        output_data, target_width, target_height, 0,
        3);
    printf("\x1b[H");

    char *ptr = frame_buffer;

    for (int y = 0; y < target_height; y++)
    {
        for (int x = 0; x < target_width; x++)
        {
            int idx = (y * target_width + x) * 3;
            unsigned char r = output_data[idx + 0];
            unsigned char g = output_data[idx + 1];
            unsigned char b = output_data[idx + 2];
            // Enhance vibrancy
            // enhance_color(&r, &g, &b);


            unsigned char gray = (unsigned char)(r * 0.349f + g * 0.527f + b * 0.124f);
            char ch = map_intensity(gray);

            ptr += sprintf(ptr, "\x1b[38;2;%d;%d;%dm%c", r, g, b, ch);
        }
        ptr += sprintf(ptr, "\n");
    }
    printf("%s", frame_buffer);
    fflush(stdout);
}

int main(int argc, char *argv[])
{

    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    int target_width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int target_height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    target_width -= target_width * 0.08;
    target_height -= target_height * 0.08;

    int w, h, channels;

    int num_frames = atoi(argv[2]); // convert argv[2] to int
    unsigned char *data = (unsigned char *)malloc(target_width * target_height * 3);
    char *frame_buffer = malloc(target_width * target_height * 32);

    for (int i = 0; i < num_frames; i++)
    {
        char filename[256];
        sprintf(filename, "%s/frame_%06d.png", argv[1], i);

        stbi_uc *input_pixels = stbi_load(filename, &w, &h, &channels, 3);
        if (!input_pixels)
        {
            printf("Failed to load %s\n", filename);
            continue;
        }

        show_frame(input_pixels, w, h, data, target_width, target_height, frame_buffer);

        stbi_image_free(input_pixels);
        Sleep(1000/atoi(argv[3]));
    }

    free(data);

    return 0;
}
