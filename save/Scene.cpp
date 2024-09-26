#include "Scene.h"

using namespace std;

Face::Face(int i, int j, int k) {
    v[0] = i; v[1] = j; v[2] = k;
};

const Vec3f Face::normal(const vector<Vec3f> & vertices) {
    return normalize(cross(vertices[v[1]] - vertices[v[0]], vertices[v[2]] - vertices[v[0]]));
};

const float Face::area(const vector<Vec3f>& vertices) {
    return cross(vertices[v[1]] - vertices[v[0]], vertices[v[2]] - vertices[v[0]]).length();
};

const float Face::angle(const vector<Vec3f>& vertices, int i) {
    int j, k;
    if (i == 0)
        j = 1; k = 2;
    if (i == 1)
        j = 0; k = 2;
    if (i == 2)
        j = 1; k = 0;
    return acos(dot(normalize(vertices[v[j]] - vertices[v[i]]), normalize(vertices[v[k]] - vertices[v[i]])));
};

Vec3f Camera::getfront() {
	return front;
};
Vec3f Camera::gettop() {
	return top;
};
Vec3f Camera::getright() {
	return cross(front,top);
};

Vec3f Camera::getposition() {
    return position;
};

Camera::Camera(Vec3f pos, Vec3f fro, Vec3f to, float asp, float angle) :
    position(pos), asp_ratio(asp), view_angle(angle)
{
    front = normalize(fro);
    top = to - dot(front, to) * front;
};

Mesh::Mesh(Material* m) {
    material = m;
};

void Mesh::initialise(Vec3f col) {
    face_normals.resize(faces.size());
    vertice_normals.resize(vertices.size());

    compute_normals();
};

void Mesh::compute_normals() {
    for (int i = 0; i < vertices.size(); i++) {
        vertice_normals[i] = {0,0,0};
    }
    for (int i = 0; i < faces.size();i++) {
        face_normals[i] = faces[i].normal(vertices);

        vertice_normals[faces[i].v[0]] += faces[i].angle(vertices, 0) * faces[i].normal(vertices);
        vertice_normals[faces[i].v[1]] += faces[i].angle(vertices, 1) * faces[i].normal(vertices);
        vertice_normals[faces[i].v[2]] += faces[i].angle(vertices, 2) * faces[i].normal(vertices);
    }
    for (int i = 0; i < vertices.size(); i++) {
        vertice_normals[i] = normalize(vertice_normals[i]);
    }
};

const vector<Vec3f> Mesh::getvertices() {
    return vertices;
};

const Vec3f Mesh::getvertice(int i) {
    return vertices[i];
};

const Face Mesh::getface(int i){
    return faces[i];
};

void Mesh::addFace(int i, int j, int k){
    Face f = {i, j, k};
    faces.push_back(f);
};

const Material* Mesh::getMaterial() {
    return material;
};

void Mesh::addVertice(Vec3f vertice){
    vertices.push_back(vertice);
};

const int Mesh::getFacenb() {
    return faces.size();
};

bool Mesh::loadOFF(string file_name) {

    string line;
    ifstream file(file_name);

    if (!file.is_open())
    {
        cout << "probleme" << endl;
        return false;
    }

    // header "OFF"
    getline(file, line);
    if (line != "OFF")
        return false;

    // number of vertices, faces, edges
    getline(file, line);
    int nvertice, nface, nedge;
    istringstream specs;
    specs.str(line);
    specs >> nvertice >> nface >> nedge;

    // the vertices coordinates
    for (int i = 0; i < nvertice; i++) {
        getline(file, line);
        istringstream coords;
        coords.str(line);
        Vec3f vertice;
        coords >> vertice[0] >> vertice[1] >> vertice[2];
        vertices.push_back(vertice);
    }

    // the vertices coordinates
    for (int i = 0; i < nface; i++) {
        getline(file, line);
        istringstream corners;
        corners.str(line);
        int polygone;
        int l, j, k;
        corners >> polygone >> l >> j >> k;
        Face f(l,j,k);
        faces.push_back(f);
    }

    file.close();

    // center_scale();

    return true;
};

Material::Material(Vec3f col, float dif, float rough, float f0) : albedo(col), diffusion(dif), roughness(rough), F0(f0) {
    k = pow(roughness + 1.f, 2) / 8.f;
    alpha = pow(roughness, 2);
};


Vec3f Material::evaluateColorResponse(const Vec3f& normal, const Vec3f& wi, const Vec3f& wo, BRDF option) const {
    switch (option) {
    case Phong:
        return PhongResponse(normal, wi);
    case Microfacet:
        return MicrofacetResponse(normal, wi, wo);
    }
    
};

Vec3f Material::PhongResponse(const Vec3f& normal, const Vec3f& wi) const {
    float reflectance = diffusion / M_PI;
    float intensity = abs(dot(normal, wi)) * reflectance;
    return intensity * albedo;
};

Vec3f Material::MicrofacetResponse(const Vec3f& normal, const Vec3f& wi, const Vec3f& wo) const {
    Vec3f wh = normalize((wi + wo) / 2.f);
    // Normal Distribution
    float D = pow(alpha,2) / (M_PI * pow(pow(dot(normal, wh), 2) * (pow(alpha, 2) - 1) + 1, 2));
    // Fresnel Term
    float F = F0 + (1 - F0) * pow(2, (-5.55473f * dot(wi, wh) - 6.98316) * dot(wi, wh));
    // Geometric Term
    float G1 = dot(normal, wi) / (dot(normal, wi) * (1 - k) + k);
    float G2 = dot(normal, wo) / (dot(normal, wo) * (1 - k) + k);
    float G = G1 * G2;

    return albedo * D * F * G / (4.f * dot(normal, wo) * dot(normal, wi));

};