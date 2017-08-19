

#include "led-matrix.h"
#include "transformer.h"

#include <math.h>
#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <signal.h>
#include <string>
#include "png++/png.hpp"
#include <iostream>

using namespace png;
using namespace std;


using rgb_matrix::GPIO;
using rgb_matrix::RGBMatrix;
using rgb_matrix::Canvas;
using rgb_matrix::UArrangementTransformer;

class PixelPiece
{
	private:
		image<rgb_pixel> imageSource;
	
	public:	
  		PixelPiece();
  		~PixelPiece();

  		void SetPixelPieceImage(string fileName);
	
		rgb_pixel GetPixel(int x, int y);
};

PixelPiece::PixelPiece()
{}
PixelPiece::~PixelPiece()
{}
void PixelPiece::SetPixelPieceImage(string fileName)
{
	imageSource.read(fileName);
}

rgb_pixel PixelPiece::GetPixel(int x, int y)
{
	imageSource.get_pixel(x, y);
}



volatile bool interrupt_received = false;
static void InterruptHandler(int signo) 
{
  interrupt_received = true;
}



static void DrawOnCanvas(Canvas *canvas) 
{
  /*
   * Let's create a simple animation. We use the canvas to draw
   * pixels. We wait between each step to have a slower animation.
   */
  //canvas->Fill(0, 0, 255);

  uint8_t brightness = 100;
  int center_x = canvas->width() / 2;
  int center_y = canvas->height() / 2;
  float radius_max = canvas->width() / 2;
  float angle_step = 1.0 / 360;
  for (float a = 0, r = 0; r < radius_max; a += angle_step, r += angle_step) {
    if (interrupt_received)
      return;
    float dot_x = cos(a * 2 * M_PI) * r;
    float dot_y = sin(a * 2 * M_PI) * r;

    // SetPixel(self, int x, int y, uint8_t red, uint8_t green, uint8_t blue): 
//    canvas->SetPixel(center_x+dot_x, center_y+dot_y, brightness, 0, 0);
    canvas->SetPixel(0, 0, 0, 0, brightness);

    canvas->SetPixel(31, 31, brightness, 0, 0);
    //canvas->SetPixel(0, 31, 0, brightness, brightness);
    //canvas->SetPixel(31, 0, brightness, 0, brightness);
	  
	  
	// SetPixel(x, y, red, green, blue)

    usleep(1 * 1000);  // wait a little to slow down things.
  }
}


int main(int argc, char* argv[])
{

	RGBMatrix::Options matrix_options;
  	rgb_matrix::RuntimeOptions runtime_opt;
	PIxelPiece pixelPiece;
	
	// Read in pixel file to display.
	// ...
	// ...


  	matrix_options.rows = 16;
  	matrix_options.chain_length = 2;
  	matrix_options.parallel = 1;

  	RGBMatrix *matrix = CreateMatrixFromFlags(&argc, &argv, &matrix_options);

  	matrix->ApplyStaticTransformer(UArrangementTransformer(matrix_options.parallel));

  	Canvas *canvas = matrix;

  	// It is always good to set up a signal handler to cleanly exit when we
	// receive a CTRL-C for instance. The DrawOnCanvas() routine is looking
	// for that.
	signal(SIGTERM, InterruptHandler);
	signal(SIGINT, InterruptHandler);


  	DrawOnCanvas(canvas);

  	// Animation finished. Shut down the RGB matrix.
  	canvas->Clear();
  	delete canvas;
	
	return 0;
}


