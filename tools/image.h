#include <iostream>
#include <sstream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

class Img {
public:
	Img();
	void load_image(string name);
	void replace_black_with_black();
	void swap_color(int r, int g, int b, int r1, int g1, int b1);
	void convert_white_to_black();
	void save_as_bmp(string name);
	void save_as_bmp_array(string name);
	void show_image();

private:
	Mat img;

};