

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
	
		void DrawPixelCanvas(Canvas* canvas);

  		void SetPixelPieceImage(string fileName);
	
		rgb_pixel GetPixel(int x, int y);
};

void PixelPiece::SetPixelPieceImage(string fileName)
{
	imageSource.read(fileName);
}

rgb_pixel PixelPiece::GetPixel(int x, int y)
{
	return imageSource.get_pixel(x, y);
}

void PixelPiece::DrawPixelCanvas(Canvas* canvas)
{
	for(int x = 0; x < 32; ++x)
	{
		for(int y = 0; y < 32; ++y)
		{
			rgb_pixel pix = GetPixel(x, y);
			
			canvas->SetPixel(x, y, (int)pix.red, (int)pix.green, (int)pix.blue);
		}
	}
}

volatile bool interrupt_received = false;
static void InterruptHandler(int signo) 
{
  interrupt_received = true;
}



static void DrawOnCanvas(Canvas *canvas, PixelPiece* piece) 
{
  /*
   * Let's create a simple animation. We use the canvas to draw
   * pixels. We wait between each step to have a slower animation.
   */
  //canvas->Fill(0, 0, 255);
  
  while(true)
  {	  
	  //canvas->SetPixel(0, 0, 0, 0, brightness);
	  
	  piece->DrawPixelCanvas(canvas);
	  
	  if (interrupt_received)
		return; 
	  
	  usleep(1 * 1000);	
  }   
	  
	// SetPixel(x, y, red, green, blue)
}


int main(int argc, char* argv[])
{

	RGBMatrix::Options matrix_options;
  	rgb_matrix::RuntimeOptions runtime_opt;
	PixelPiece pixelPiece;
	
	
	// Read in pixel file to display.
	pixelPiece.SetPixelPieceImage("bowl.png");
	// ...

	matrix_options.brightness = 50;
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


  	DrawOnCanvas(canvas, &pixelPiece);

  	// Animation finished. Shut down the RGB matrix.
  	canvas->Clear();
  	delete canvas;
	
	return 0;
}


