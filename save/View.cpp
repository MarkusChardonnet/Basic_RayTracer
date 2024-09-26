#include "View.h"

using namespace std;

View::View(Camera* C, Image* I, LightSource* L)
{
    background = I;
    camera = C;
    lightsource = L;
    result = new Image(I->getheight(), I->getwidth());
    height = I->getheight();
    width = I->getwidth();
};

View::~View() {
    delete camera;
    delete background;
    for(auto m: meshes)
	    delete m;
	delete result;
    delete lightsource;
};

void View::rayTrace() {
    for(int x = 0; x < width; x++) {
        for(int y = 0; y < height; y++) {
            Vec3f ray = RayPixel(x,y);
            RayTracer R(camera->getposition(), ray);
            bool intersect = R.search_intersection(meshes);
            if (intersect)
                result->SetPixelColor(x, y, R.getResponse(lightsource, meshes));
            else
                result->SetPixelColor(x, y, background->GetPixelColor(x,y));
        }
    }
};

void View::rayTraceRandom(int raynb) {
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            vector<Vec3f> rays = RayPixelRandom(x, y, raynb);
            vector<Vec3f> responses;
            for (int i = 0; i < raynb; i++) {
                RayTracer R(camera->getposition(), rays[i]);
                bool intersect = R.search_intersection(meshes);
                if (intersect) {
                    R.getResponse(lightsource, meshes);
                    responses.push_back({ 0.0f,1.0f,1.0f });
                }
                else
                    responses.push_back(background->GetPixelColor(x, y));
            }
            Vec3f response = { 0.0f,0.0f,0.0f };
            for (auto r : responses)
                response = response + r;
            response = response / (float)raynb;
            result->SetPixelColor(x, y, response);
        }
    }
};

Vec3f View::RayPixel(const int& x, const int& y) {
	float v = ((float)y - (float)height / 2.0f) * (tan(camera->view_angle) / (((float)height / 2.0f) * camera->asp_ratio));
	float h = ((float)x - (float)width / 2.0f) * (tan(camera->view_angle) / ((float)width / 2.0f));

	Vec3f res = camera->getfront() - v * camera->gettop() + h * camera->getright();

	return normalize(res);
};

vector<Vec3f> View::RayPixelRandom(const int& x, const int& y, int N) {
    float c = tan(camera->view_angle) / ((float)height / 2.0f);
    float vp = ((float)y + 0.5f - (float)height / 2.0f) * c / camera->asp_ratio;
    float vm = ((float)y - 0.5f - (float)height / 2.0f) * c / camera->asp_ratio;
    float hp = ((float)x + 0.5f - (float)width / 2.0f) * c;
    float hm = ((float)x - 0.5f - (float)width / 2.0f) * c;

    std::default_random_engine generator;
    std::uniform_real_distribution<float> distribution_v(vm, vp);
    std::uniform_real_distribution<float> distribution_h(hm, hp);
    float h, v;

    Vec3f front = camera->getfront(); Vec3f down = -camera->gettop(); Vec3f right = camera->getright();

    vector<Vec3f> rays;

    for (int i = 0; i < N; i++) {
        v = distribution_v(generator);
        h = distribution_h(generator);
        rays.push_back(front + v * down + h * right);
    }

    return rays;
};

void View::exp(string name) {
    background->savePPM("base");
    result->savePPM(name);
};

LightSource::LightSource(Vec3f pos, float intens, Vec3f col)
    : color(col), intensity(intens), position(pos) {};

RayTracer::RayTracer(Vec3f origin_, Vec3f ray_):origin(origin_), ray(ray_) {
    closest = nullptr;
};

bool RayTracer::search_intersection(vector<Mesh*>& meshes) {
    bool res = false;

    const int meshnb = meshes.size();

    for (int j = 0; j < meshnb; j++) {
        const int nf = (*meshes[j]).getFacenb();
        for (int i = 0; i < nf; i++) {

            Face f = (*meshes[j]).getface(i);

            const Vec3f A = (*meshes[j]).getvertice(f.v[0]) - origin;
            const Vec3f B = (*meshes[j]).getvertice(f.v[1]) - origin;
            const Vec3f C = (*meshes[j]).getvertice(f.v[2]) - origin;

            array<float, 4> P = Plane(A, B, C);
            float den = ray[0] * P[0] + ray[1] * P[1] + ray[2] * P[2];
            if (den == 0.0f)
                continue;
            float lam = -P[3] / den;
            if (lam <= 0)
                continue;
            Vec3f D = lam * ray;

            bool intersect = PointInTriangle(D, A, B, C);

            if (intersect) {
                res = true;
                if (closest == nullptr) {
                    closest = new float(lam);
                    object_normal = f.normal((*meshes[j]).getvertices());
                    intersection = origin + lam * ray;
                    mesh_idx = j;
                }
                else if (*closest > lam) {
                    *closest = lam;
                    object_normal = f.normal((*meshes[j]).getvertices());
                    intersection = origin + lam * ray;
                    mesh_idx = j;
                }
            }
        }
    }
    return res;
};

void View::addMesh(Mesh* m) {
    meshes.push_back(m);
};

Vec3f RayTracer::getResponse(LightSource* L, vector<Mesh*>& meshes) {
    const Vec3f wi = normalize(intersection - L->getPosition());
    RayTracer light(L->getPosition(), wi);
    light.search_intersection(meshes);

    if (length(light.intersection - intersection) < 1e-4) {
        return L->getIntensity() * meshes[mesh_idx]->getMaterial()->evaluateColorResponse(object_normal, wi) * abs(dot(object_normal, wi));
    }
    else {
        return { 0.0f,0.0f,0.0f };
    }
};

const Vec3f LightSource::getPosition() {
    return position;
};

const float LightSource::getIntensity() {
    return intensity;
};

LightSquare::LightSquare(Vec3f pos, float intens, Vec3f normal, float len, Vec3f col) : 
LightSource(pos, intens, col), size(len) {
    if (length(cross(normal, { 1.0f,0.0f,0.0f })) > 1e-4)
        height = normalize(cross(normal, { 1.0f,0.0f,0.0f }));
    else
        height = normalize(cross(normal, { 0.0f,1.0f,0.0f }));
    width = normalize(cross(normal, height));
};

const Vec3f LightSquare::getPosition() {
    std::default_random_engine generator;
    std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
    float a = distribution(generator); float b = distribution(generator);

    return LightSquare::getPosition() + size * (a * height + b * width);
};
