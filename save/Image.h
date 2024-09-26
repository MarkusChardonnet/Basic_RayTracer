#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "Vec3.cpp"

#pragma once


class Image {
	private:
		static Vec3f basecolor;
	    std::vector<std::vector<Vec3f>> pixels;
		float height;
		float width;
    public:
		Image(int height_, int width_, Vec3f color);
		Image(int height_, int width_);
		void savePPM(std::string output);
		int getheight();
		int getwidth();
		void SetPixelColor(int x, int y, Vec3f p);
		Vec3f GetPixelColor(int x, int y);
		void blue_white_v();
};


