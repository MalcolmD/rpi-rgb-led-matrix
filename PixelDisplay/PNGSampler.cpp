#include "png++/png.hpp"
#include <iostream>
using namespace png;




int main()
{
	// Open the image
	image<rgb_pixel> img("planet.png");

	// Get first pixel from image
	rgb_pixel pix = img.get_pixel(7,2);

	std::cout << "Pixel: " << "7, " << "2: " << (int)pix.red << ", " << (int)pix.green << ", " << (int)pix.blue << std::endl;


	return 0;
}