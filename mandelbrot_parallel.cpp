#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <string.h>

#include <algorithm>
#include <complex>
#include <iostream>
#include <vector>

#include "stb_image_write.h"

#define MaxRepeats 10000

using namespace std;

struct Color {
    unsigned char r, g, b;
};

complex<double> trap(0.5, 0);

void hsv_to_rgb(double h, double s, double v, unsigned char &r,
                                unsigned char &g, unsigned char &b) {
    double c = v * s;
    double x = c * (1 - abs(fmod(h / 60.0, 2) - 1));
    double m = v - c;
    double r1, g1, b1;

    if (h >= 0 && h < 60) {
        r1 = c;
        g1 = x;
        b1 = 0;
    } else if (h >= 60 && h < 120) {
        r1 = x;
        g1 = c;
        b1 = 0;
    } else if (h >= 120 && h < 180) {
        r1 = 0;
        g1 = c;
        b1 = x;
    } else if (h >= 180 && h < 240) {
        r1 = 0;
        g1 = x;
        b1 = c;
    } else if (h >= 240 && h < 300) {
        r1 = x;
        g1 = 0;
        b1 = c;
    } else {
        r1 = c;
        g1 = 0;
        b1 = x;
    }

    r = static_cast<unsigned char>((r1 + m) * 255);
    g = static_cast<unsigned char>((g1 + m) * 255);
    b = static_cast<unsigned char>((b1 + m) * 255);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cout << "Usage: " << argv[0] << " <inputfile> <outputfile>"
                            << std::endl;
        return 1;
    }

    char *inputfile_name = argv[1];
    string outputfile_name = argv[2];

    if (freopen(inputfile_name, "r", stdin) == nullptr) {
        std::cout << "Error: Unable to open input file: " << inputfile_name
                            << std::endl;
        return 1;
    }

    int HEIGHT, WIDTH;
    scanf("height=%d\n", &HEIGHT);
    scanf("width=%d", &WIDTH);
    cout << "Width: " << WIDTH << " Height: " << HEIGHT << endl;
    unsigned char *image = new unsigned char[HEIGHT * WIDTH * 3];

    /*
    TODO:
    1. Use MPI to parallelize across different processes.
    2. Use OpenMP to collapse the two for loops.
    3. Implement load balancing.
    */

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            complex<double> c((x - WIDTH / 2.0) * 4.0 / WIDTH,
                                                (y - HEIGHT / 2.0) * 4.0 / HEIGHT);
            complex<double> z = 0;
            double min_dist = numeric_limits<double>::max();
            int repeats = 0;

            for (repeats = 0; repeats <= MaxRepeats; repeats++) {
                z = z * z + c;
                double dist = abs(z - trap);
                if (dist < min_dist) {
                    min_dist = dist;
                }
                if (abs(z) > 2.0) break;
            }

            int index = (y * WIDTH + x) * 3;
            if (repeats > MaxRepeats) {
                image[index] = 0;
                image[index + 1] = 0;
                image[index + 2] = 0;
            } else {
                double hue = 360.0 * min_dist / 2.0;
                double saturation = 1.0;
                double value = repeats < MaxRepeats ? 1.0 : 0.0;
                hsv_to_rgb(hue, saturation, value, image[index], image[index + 1],
                                     image[index + 2]);
            }
        }
    }

    stbi_write_png(outputfile_name.c_str(), WIDTH, HEIGHT, 3, image, WIDTH * 3);
    cout << "Image saved to " << outputfile_name << endl;
    return 0;
}
