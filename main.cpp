#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "InputImage.hpp"
#include "BilateralGrid.hpp"

#define ICCG_LOOP_MAX	(200)
#define ICCG_EPS		(0.01)

using namespace cv;
using namespace std;

const String keys =
	"{help h usage |      | print this message   }"
	"{@image      |      | image for input   }";

int main(int argc, char **argv)
{
    String imgName;
	Mat3f mat_in;
	Mat3f mat_bg_in;
	Mat3f mat_bg_draw_in;
	
    CommandLineParser parser(argc, argv, keys);
	if(parser.has("h") || argc != 2){
		parser.printMessage();
		return 0;
	}
    imgName = parser.get<String>(0);

	mat_in= imread(imgName, 1)/255;

	//$BF~NO2hA|:n@.MQ$N%/%i%9(B
	InputImage InImg(mat_in);
	mat_bg_in = InImg.get_Image(IMG_YUV);

	//$B=i4|%;%C%H%"%C%W(B
	BilateralGrid BiGr(mat_bg_in);
	BiGr.construct_SliceMatrix();
	BiGr.construct_BlurMatrix();
	BiGr.calc_Bistochastic();
	BiGr.construct_AMatrix_step1();
	cout << "Bistochastic Fin" << endl;

	InImg.draw_Image();
	mat_bg_draw_in = InImg.get_Image(IMG_DRAWYUV);
	cout << "process" << endl;
	BiGr.set_DrawImage(mat_bg_draw_in);
	BiGr.construct_AMatrix_step2();
	BiGr.execute_ICCG(ICCG_LOOP_MAX, ICCG_EPS);
	BiGr.show_Image(BG_COLORIZED);

	imwrite("draw.png" , InImg.get_Image(IMG_DRAW)*255);
	imwrite("colorized.png" , BiGr.get_Image(BG_COLORIZED)*255);
}
