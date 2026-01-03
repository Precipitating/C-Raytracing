#include <iostream>
#include "color.h"
#include "vec3.h"
int main()
{
	// Image
	int imageWidth = 256;
	int imageHeight = 256;

	// Render
    std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

    for (int j = 0; j < imageHeight; ++j)
    {
        // progress indicator
        std::clog << "\rScanlines remaining: " << (imageHeight - j) << ' ' << std::flush;

        // image data
        for (int i = 0; i < imageWidth; i++)
        {
            auto pixel_color = color(double(i) / (imageWidth - 1), double(j) / (imageHeight - 1), 0);
            write_color(std::cout, pixel_color);
        }
    }



	return 0;
}