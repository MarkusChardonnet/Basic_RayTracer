#include "Image.h"
#include "Scene.h"
#include "utils.h"
#include <cmath>
#include <iostream>
#include <string>
#include <random>

#pragma once

class LightSource {
private:
	const Vec3f position;
	const Vec3f color;
	float intensity;

public:
	LightSource(Vec3f pos, float intens, Vec3f col = { 1.0f, 1.0f , 1.0f });
	const Vec3f getPosition();
	const float getIntensity();
};

class LightSquare : public LightSource {
private:
	Vec3f height;
	Vec3f width;
	const float size;
public:
	LightSquare(Vec3f pos, float intens, Vec3f normal, float len, Vec3f col = { 1.0f, 1.0f , 1.0f });
	const Vec3f getPosition();
};

class View {
    private:
	Camera* camera;
	std::vector<Mesh*> meshes;
	Image* background;
	Image* result;
	LightSource* lightsource;
	int height;
	int width;

    public:
	void addMesh(Mesh* m);
	View(Camera* C, Image* I, LightSource* L);
	~View();
	void rayTrace();
	void rayTraceRandom(int raynb = 5);
	Vec3f RayPixel(const int& x, const int& y);
	std::vector<Vec3f> RayPixelRandom(const int& x, const int& y, int N = 5);
	void exp(std::string name);
};

class RayTracer {
private:
	Vec3f origin;
	Vec3f ray;
	float* closest;
	Vec3f object_normal;
	Vec3f intersection;
	int mesh_idx;

public:
	RayTracer(Vec3f origin_, Vec3f ray_);
	bool search_intersection(std::vector<Mesh*>& meshes);
	Vec3f getResponse(LightSource* L, std::vector<Mesh*>& meshes);
};