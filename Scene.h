#include <vector>
#include <array>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include "utils.h"
#include "Vec3.cpp"

#pragma once

// typedef std::array<int, 3> Face;

struct Face {
	std::array<int, 3> v;
	Face(int i, int j, int k);

	const Vec3f normal(const std::vector<Vec3f>& vertices);
	const float area(const std::vector<Vec3f>& vertices);
	const float angle(const std::vector<Vec3f>& vertices, int i);
};

enum BRDF {Phong, Microfacet};

class Material {
private:
	Vec3f albedo;
	float diffusion;
	float roughness;
	float F0;
	float alpha;
	float k;

public:
	Material(Vec3f col, float dif, float roug, float f0);
	Vec3f evaluateColorResponse(const Vec3f& normal, const Vec3f& wi) const;
	// Vec3f evaluateColorResponse(const Vec3f& normal, const Vec3f& wi, const Vec3f& wo, enum BRDF) const;
	// Vec3f PhongResponse(const Vec3f& normal, const Vec3f& wi) const;
	// Vec3f MicrofacetResponse(const Vec3f& normal, const Vec3f& wi, const Vec3f& wo) const;
};

class Mesh {
private:
	std::vector<Vec3f> vertices;
	std::vector<Face> faces;
	std::vector<Vec3f> vertice_normals;
	std::vector<Vec3f> face_normals;
	Material* material;

public:
	Mesh(Material* m);
	const int getFacenb();
	void compute_normals();
	void initialise(Vec3f col = { 1.0f,0.0f,0.0f });
	const Face getface(int i);
	const std::vector<Vec3f> getvertices();
	const Vec3f getvertice(int i);
	void addFace(int i, int j, int k);
	void addVertice(Vec3f vertice);
	bool loadOFF(std::string file_name);
	const Material* getMaterial();
};

struct Camera {
    Camera(Vec3f pos, Vec3f fro, Vec3f to, float asp, float angle);
	Vec3f position;
	Vec3f front;
	Vec3f top;
	float asp_ratio;
	float view_angle;

	Vec3f getposition();
	Vec3f getfront();
	Vec3f gettop();
	Vec3f getright();
};
