#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "InputImage.hpp"

static int glb_mouse_x;
static int glb_mouse_y;
static bool glb_mouse_click = false;
static bool glb_mouse_left = false;

/****************************************************
brief	:	RGB$B2hA|$N(B3ch$BA4$F$K(BGlay$B2hA|$N(B1ch$B$rBeF~$9$k(B
note	:	opencv$B$N;EMM>e(B?$B!"$7$V$7$V%/%i%9$+$i30$9$3$H$K!#(B
*****************************************************/
static void my_mouse_callback(int event, int x, int y, int flags, void* param)
{
	switch (event){
		case EVENT_MOUSEMOVE:
			if (glb_mouse_click){
				glb_mouse_x = x;
				glb_mouse_y = y;
			}
			break;

		case EVENT_LBUTTONDOWN:
			glb_mouse_click = true;
			glb_mouse_x = x;
			glb_mouse_y = y;
			break;

		case EVENT_LBUTTONUP:
			glb_mouse_left = true;
			glb_mouse_click = false;
			break;
	}
}

/****************************************************
brief	: $B%3%s%9%H%i%/%?(B	
note	:	
*****************************************************/
InputImage::InputImage(Mat1f mat_image)	
{
	mat_input = mat_image.clone();
	cvtColor(mat_input, mat_gray, COLOR_BGR2GRAY);
	mat_draw = copy_GlaychForRGBch(mat_gray, mat_input);
	cvtColor(mat_draw, mat_yuv, COLOR_BGR2YCrCb);
}


/****************************************************
brief	:	RGB$B2hA|$N(B3ch$BA4$F$K(BGlay$B2hA|$N(B1ch$B$rBeF~$9$k(B
note	:	
*****************************************************/
Mat3f InputImage::copy_GlaychForRGBch(Mat1f gray, Mat3f color)
{
	int y, x, c;
	float* gray_pix;
	float* color_pix;
	Mat3f ret = color.clone();
	gray_pix = gray.ptr<float>(0, 0);
	color_pix = ret.ptr<float>(0, 0);

	for(y=0; y<gray.rows; y++)
	{
		for(x=0; x<gray.cols; x++)
		{
			for(c=0; c<color.channels(); c++)
			{
				*color_pix = *gray_pix;
				color_pix++;
			}
			gray_pix++;
		}
	}
	return ret;
}

/****************************************************
brief	:	Gray$B2hA|$K?'$r$D$1$k(B
note	:	
*****************************************************/
void InputImage::draw_Trajectory(Mat3f* img)
{
	int i, j;
	float red, green, blue;
	int y, x;
	int r = MARK_RADIUS;
	int r2 = r * r;
	float* color_pix;

	y = mouse_y - r;
	for(i=-r; i<r+1 ; i++, y++)
	{
		x = mouse_x - r;
		color_pix = mat_input.ptr<float>(y, x);
		for(j=-r; j<r+1; j++, x++)
		{
			//$B%^!<%/$r1_7A$K$9$k(B
			if(i*i + j*j > r2)
			{
				color_pix += mat_input.channels();
				continue;
			}

			//$B6-3&>r7o$r0U<1(B
			if(y<0 || y>=mat_input.rows || x<0 || x>=mat_input.cols)
			{
				break;
			}

			blue = *color_pix;
			color_pix++;
			green = *color_pix;
			color_pix++;
			red = *color_pix;
			color_pix++;
			circle(*img, Point2d(x, y), 0, Scalar(blue, green, red), -1);
		}
	}
}


/****************************************************
brief	:	$B%G%P%C%/MQ!"2hA|I=<((B
note	:	
*****************************************************/
void InputImage::show_Image(int num)
{
	namedWindow("input", WINDOW_AUTOSIZE);
	switch(num)
	{
		case IMG_INPUT:
			imshow("input", mat_input);
			break;
		case IMG_GRAY:
			imshow("input", mat_gray);
			break;
		case IMG_DRAW:
			imshow("input", mat_draw);
			break;
		case IMG_YUV:
			imshow("input", mat_yuv);
			break;
		case IMG_DRAWYUV:
			imshow("input", mat_draw_yuv);
			break;
		default:
			break;
	}
	waitKey();
}


/****************************************************
brief	:	$B2hA|%G!<%?$N%2%C%?!<(B
note	:	
*****************************************************/
Mat3f InputImage::get_Image(int num)
{
	switch(num)
	{
		case IMG_DRAW:
			return mat_draw;
			break;
		case IMG_YUV:
			return mat_yuv;
			break;
		case IMG_DRAWYUV:
			return mat_draw_yuv;
			break;
		default:
			return mat_input;
			break;
	}
}


/****************************************************
brief	: $B2hA|$K?'$rEI$k(B
note	: $B$b$H$b$H?'$NIU$$$F$$$k2hA|$N?'$rLa$9$@$1(B	
*****************************************************/
void InputImage::draw_Image(void)
{
	namedWindow("draw", WINDOW_AUTOSIZE);
	imshow("draw", mat_draw);
	setMouseCallback("draw", my_mouse_callback, (void *)&mat_draw);
	while (1){
		mouse_x = glb_mouse_x;
		mouse_y = glb_mouse_y;
		mouse_click = glb_mouse_click;
		mouse_left = glb_mouse_left;

		// $B%^%&%9$N%/%j%C%/$r2!$7$F$$$k4V!"50@W$rLO<L$9$k(B
		if (mouse_click) {
			draw_Trajectory(&mat_draw);
			imshow("draw", mat_draw);
		}
		// Esc$B$G=*N;(B
		if (waitKey(2) == 27)
			break;
	}
	cvtColor(mat_draw, mat_draw_yuv, COLOR_BGR2YCrCb);
	waitKey();
}
