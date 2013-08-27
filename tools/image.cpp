#include "image.h"

Img::Img() {

}

void Img::load_image(string name) {
	img = imread(name, 1);
}

void Img::save_as_bmp(string name) {

	imwrite("name", img);
}

void Img::show_image() {
	imshow("image", img);
	waitKey(0);
}

void Img::replace_black_with_black() {
	
	for (int x = 0; x < img.rows; x++) {
		for(int y = 0; y < img.cols; y++) {

			Vec3b pixel_img = img.at<Vec3b>(x, y);
			
			if (pixel_img[0] == 0 && pixel_img[1] == 0 && pixel_img[2] == 0) {
				img.at<cv::Vec3b>(x,y)[0] = 1;
			}
		}
	}
}

void Img::swap_color(int r, int g, int b, int r1, int g1, int b1) {

	for (int x = 0; x < img.rows; x++) {
		for(int y = 0; y < img.cols; y++) {

			Vec3b pixel_img = img.at<Vec3b>(x, y);
			
			if (pixel_img[0] == b && pixel_img[1] == g && pixel_img[2] == r) {
				img.at<cv::Vec3b>(x,y)[0] = b1;
				img.at<cv::Vec3b>(x,y)[1] = g1;
				img.at<cv::Vec3b>(x,y)[2] = r1;
			}
		}
	}
}

void Img::convert_white_to_black() {

	for (int x = 0; x < img.rows; x++) {
		for(int y = 0; y < img.cols; y++) {

			Vec3b pixel_img = img.at<Vec3b>(x, y);
			
			if (pixel_img[0] == 255 && pixel_img[1] == 255 && pixel_img[2] == 255) {
				img.at<cv::Vec3b>(x,y)[0] = 0;
				img.at<cv::Vec3b>(x,y)[1] = 0;
				img.at<cv::Vec3b>(x,y)[2] = 0;
			}
		}
	}
}


void Img::save_as_bmp_array(string name) {

	int img_count = 0;
	int most_left = -1;
	int most_right = -1;
	bool first_row = false;

	for (int x = 0; x < img.cols; x++) {
		bool found_nonblack_pixel = false;

		for(int y = 0; y < img.rows; y++) {

			Vec3b pixel_img = img.at<Vec3b>(y, x);
			
			//find first non black pixel
			if (pixel_img[0] != 0 || pixel_img[1] != 0 || pixel_img[2] != 0) {

				if (most_left == -1) {
					most_left = x;
					first_row = true;
				} else {
					first_row = false;
				}


				if (x - 1 >= 0 && (img.at<Vec3b>(y,x-1)[0] != 0 || img.at<Vec3b>(y,x-1)[1] != 0 || img.at<Vec3b>(y,x-1)[2] != 0)) {
					most_right = x;
					found_nonblack_pixel = true;
				}
			}

			if (y == img.rows - 1 && found_nonblack_pixel == false && first_row == false && most_left != -1 && most_right != -1) {
				//save image and start over
				Rect rect(most_left, 0, most_right - most_left, img.rows);
				//cout << most_left << " , " << most_right << endl;
				Mat sub_img = img(rect);

				stringstream ss;
				ss << name << img_count << ".bmp";

				imwrite(ss.str(), sub_img);

				img_count++;

				most_left = -1;
				most_right = -1;
			}
		}
	}
	cout << "succesfully created " << img_count << " pics!" << endl;
}