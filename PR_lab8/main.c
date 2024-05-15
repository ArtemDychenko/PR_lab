#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <png.h>
#include <pthread.h>
#include <time.h>

#define M_PI 3.14159265358979323846


#define NUM_THREADS 1

// Struktura danych dla wątków
typedef struct {
    png_bytep* image;
    int num_rows;      // Wskaźnik na wiersz obrazu
    unsigned int width;  // Szerokość obrazu
    unsigned int height;
    int current_row;
    int thread_id;
    int radius;
} ThreadData;

void *negative(void *arg) {
    ThreadData *data = (ThreadData *)arg;   
    unsigned int width = data->width;

    for (int i = 0; i < data->num_rows; i++) {
        for (unsigned int x = 0; x < width; x++) {
            png_byte *pixel = &(data->image[data->current_row + i][x * 4]);  // Format RGBA, więc 4 bajty na piksel
            // Negacja kolorów RGB
            pixel[0] = 255 - pixel[0];  // Red
            pixel[1] = 255 - pixel[1];  // Green
            pixel[2] = 255 - pixel[2];  // Blue   
        }
    }
    pthread_exit(NULL);
}

void applyGaussianBlur(void *arg) {
    ThreadData *data = (ThreadData *)arg;   
    int radius = data->radius;
    unsigned int width = data->width;
   
    unsigned int height = data->height;

    double sigma = radius / 3.0;
    int kernelSize = radius * 2 + 1;
    double *kernel = (double *)malloc(kernelSize * sizeof(double));
    double kernelSum = 0.0;

    // Tworzenie maski rozmycia Gaussa
    for (int i = -radius; i <= radius; i++) {
        double weight = exp(-(i * i) / (2 * sigma * sigma)) / (sqrt(2 * M_PI) * sigma);
        kernel[i + radius] = weight;
        kernelSum += weight;
    }

    // Normalizacja maski
    for (int i = 0; i < kernelSize; i++) {
        kernel[i] /= kernelSum;
    }

    png_bytep *tempImage = (png_bytep *)malloc(sizeof(png_bytep) * height);
    for (int y = 0; y < height; y++) {
        tempImage[y] = (png_byte *)malloc(sizeof(png_byte) * width * 4);
        memcpy(tempImage[y], data->image[y], sizeof(png_byte) * width * 4);
    }

    // Rozmycie poziome
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            double red = 0.0, green = 0.0, blue = 0.0, alpha = 0.0;
            for (int i = -radius; i <= radius; i++) {
                int newX = x + i;
                if (newX >= 0 && newX < width) {
                    png_byte *pixel = &(tempImage[y][newX * 4]);
                    double weight = kernel[i + radius];
                    red += pixel[0] * weight;
                    green += pixel[1] * weight;
                    blue += pixel[2] * weight;
                    alpha += pixel[3] * weight;
                }
            }
            png_byte *outputPixel = &(data->image[y][x * 4]);
            outputPixel[0] = (png_byte)red;
            outputPixel[1] = (png_byte)green;
            outputPixel[2] = (png_byte)blue;
            outputPixel[3] = (png_byte)alpha;
        }
    }

    // Zwolnij zaalokowaną pamięć
    for (int y = 0; y < height; y++) {
        free(tempImage[y]);
    }
    free(tempImage);
    free(kernel);
}

// Funkcja wczytująca obraz PNG
png_bytep *loadPNG(const char *filename, unsigned int *width, unsigned int *height) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        fprintf(stderr, "Nie można otworzyć pliku %s\n", filename);
        exit(1);
    }

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) {
        fclose(fp);
        fprintf(stderr, "Błąd inicjalizacji struktury PNG\n");
        exit(1);
    }

    png_infop info = png_create_info_struct(png);
    if (!info) {
        png_destroy_read_struct(&png, NULL, NULL);
        fclose(fp);
        fprintf(stderr, "Błąd inicjalizacji informacji PNG\n");
        exit(1);
    }

    png_init_io(png, fp);
    png_set_sig_bytes(png, 0);
    png_read_info(png, info);

    *width = png_get_image_width(png, info);
    *height = png_get_image_height(png, info);
    png_byte color_type = png_get_color_type(png, info);
    png_byte bit_depth = png_get_bit_depth(png, info);

    if (bit_depth == 16)
        png_set_strip_16(png);

    if (color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png);

    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(png);

    if (png_get_valid(png, info, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png);

    if (color_type == PNG_COLOR_TYPE_RGB ||
        color_type == PNG_COLOR_TYPE_GRAY ||
        color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

    if (color_type == PNG_COLOR_TYPE_GRAY ||
        color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(png);

    png_read_update_info(png, info);

    png_bytep *row_pointers = (png_bytep *)malloc(sizeof(png_bytep) * (*height));
    for (int y = 0; y < *height; y++) {
        row_pointers[y] = (png_byte *)malloc(png_get_rowbytes(png, info));
    }

    png_read_image(png, row_pointers);
    fclose(fp);
    png_destroy_read_struct(&png, &info, NULL);

    return row_pointers;
}

// Funkcja zapisująca obraz PNG
void savePNG(const char *filename, png_bytep *row_pointers, unsigned int width, unsigned int height) {
 FILE *fp = fopen(filename, "wb");
    if (!fp) {
        fprintf(stderr, "Nie można otworzyć pliku %s do zapisu\n", filename);
        exit(1);
    }

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) {
        fclose(fp);
        fprintf(stderr, "Błąd inicjalizacji struktury PNG\n");
        exit(1);
    }

    png_infop info = png_create_info_struct(png);
    if (!info) {
        png_destroy_write_struct(&png, NULL);
        fclose(fp);
        fprintf(stderr, "Błąd inicjalizacji informacji PNG\n");
        exit(1);
    }

    png_init_io(png, fp);
    png_set_IHDR(png, info, width, height, 8, PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_ADAM7,
                 PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    png_write_info(png, info);

    png_write_image(png, row_pointers);
    png_write_end(png, NULL);

    fclose(fp);
    png_destroy_write_struct(&png, &info);
}


int main() {
    char *inputFilename = "blue.png";
    char *outputFilename = "output_image.png";
  
    unsigned int width, height;
    png_bytep *row_pointers = loadPNG(inputFilename, &width, &height);

    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];
    int rc;

    unsigned int rows_per_thread = height / NUM_THREADS;
    unsigned int remaining_rows = height % NUM_THREADS;

    unsigned int current_row = 0;
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_data[i].width = width;
        thread_data[i].height = height;
        // Obliczanie liczby wierszy dla bieżącego wątku
        unsigned int rows_for_thread = rows_per_thread + (i < remaining_rows ? 1 : 0);
        thread_data[i].num_rows = rows_for_thread;
        thread_data[i].image = row_pointers;
        thread_data[i].current_row = current_row;
        thread_data[i].thread_id = i;
        thread_data[i].radius = 11;
        current_row += rows_for_thread;
    }

      // Uruchamianie wątków
    for (int i = 0; i < NUM_THREADS; i++) {
        rc = pthread_create(&threads[i], NULL, applyGaussianBlur, (void *)&thread_data[i]);
        if (rc) {
            fprintf(stderr, "Błąd: Kod powrotu z pthread_create() jest %d\n", rc);
            exit(1);
        }
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    savePNG(outputFilename, row_pointers, width, height);
   
    // Zwolnij pamięć
    for (int y = 0; y < height; y++) {
        free(row_pointers[y]);
    }
    free(row_pointers);

    return 0;
}