#include "Image.h"

using namespace std;

Vec3f Image::basecolor = { 0.1f, 0.1f, 0.9f };

Image::Image(int height_, int width_, Vec3f color) :
height(height_), width(width_)
{
	for(int i = 0; i < height; i++) {
		std::vector<Vec3f> stack;
		for(int j = 0; j < width; j++) {
			stack.push_back(color);
		}
		pixels.push_back(stack);
	}
}

Image::Image(int height_, int width_) :
Image(height_, width_, basecolor)
{}

unsigned char float2char(float f) {
	unsigned char c;
	c = static_cast<unsigned char>(max(min(1.0f, f),0.0f) * 255);
	return c;
}

void Image::savePPM(string output) {
	ofstream o("output/" + output + ".ppm");
	o << "P6" << endl << width << ' ' << height << endl << "255" << endl;

	for(int i = 0; i < height; i++) {
		for(int j = 0; j < width; j++) {
			o << float2char(pixels[i][j][0]) << float2char(pixels[i][j][1]) << float2char(pixels[i][j][2]);
		}
	}

	o.close();
}

int Image::getheight() {
	return height;
}
int Image::getwidth() {
	return width;
}

void Image::blue_white_v() {
	for(int i = 0; i < height; i++) {
		float t = i / (float)height;
		for(int j = 0; j < width; j++) {
			Vec3f color = { t,t,1.0f };
			SetPixelColor(j, i, color);
		}
	}
}

void Image::SetPixelColor(int x, int y, Vec3f p){
    pixels[y][x] = p;
};

Vec3f Image::GetPixelColor(int x, int y){
    return pixels[y][x];
};
