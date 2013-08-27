#include "image.h"

int main(void) {

	Img a;
	a.load_image("genie.png");
	//a.replace_black_with_black();
	//a.swap_color(255, 255, 255, 0,  0, 0);
	//a.show_image();
	a.save_as_bmp_array("genie");

	cin.ignore();
}
