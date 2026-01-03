#include "rtweekend.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"

double hit_sphere(const point3& center, double radius, const ray& r)
{
    vec3 oc = center - r.origin();
    auto a = r.direction().length_squared();
    auto h = dot(r.direction(), oc);
    auto c = oc.length_squared() - radius * radius;
    auto discriminant = h * h - a * c;
    if (discriminant < 0)
    {
        return -1.0;
    }
    else 
    {
        return (h - std::sqrt(discriminant)) / a;
    }
}

color ray_color(const ray& r, const hittable& world)
{
    hit_record rec;
    if (world.hit(r, 0, infinity, rec)) {
        return 0.5 * (rec.normal + color(1, 1, 1));
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}


int main()
{
	// Image
    auto aspect_ratio = 16.0 / 9.0;
    int imageWidth = 400;

    // Calculate the image height, and ensure that it's at least 1.
    int imageHeight = int(imageWidth / aspect_ratio);
    imageHeight = (imageHeight < 1) ? 1 : imageHeight;

    // World
    hittable_list world;
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));


    // Camera
    auto focalLength = 1.0;
    auto viewportHeight = 2.0;
    auto viewportWidth = viewportHeight * (double(imageWidth) / imageHeight);
    auto cameraCenter = point3(0, 0, 0);

    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    auto viewportU = vec3(viewportWidth, 0, 0);
    auto viewportV = vec3(0, -viewportHeight, 0);

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    auto pixelDeltaU = viewportU / imageWidth;
    auto pixelDeltaV = viewportV / imageHeight;

    // Calculate the location of the upper left pixel.
    auto viewportUpperLeft = cameraCenter
        - vec3(0, 0, focalLength) - viewportU / 2 - viewportV / 2;
    auto pixel00Loc = viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);

	// Render
    std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

    for (int j = 0; j < imageHeight; ++j)
    {
        // progress indicator
        std::clog << "\rScanlines remaining: " << (imageHeight - j) << ' ' << std::flush;

        // image data
        for (int i = 0; i < imageWidth; i++)
        {
            auto pixelCenter = pixel00Loc + (i * pixelDeltaU) + (j * pixelDeltaV);
            auto rayDirection = pixelCenter - cameraCenter;
            ray r(cameraCenter, rayDirection);

            color pixelColor = ray_color(r, world);
            write_color(std::cout, pixelColor);
        }
    }



	return 0;
}