#include <vector>
#include "Image.h"
#include "Scene.h"
#include "View.h"
#include "Vec3.cpp"

using namespace std;

int main(int argc, char** argv) {
    float height = 400;
    float width = 600;
    Image* I = new Image(height, width);
    I->blue_white_v();

    Vec3f pos = { 1.0f, 0.0f, 2.0f }; Vec3f fro = { 0.0f, 0.0f, -1.0f }; Vec3f to = { 0.0f, 1.0f, 0.0f };
    Camera* C = new Camera(pos, fro, to, 1.5f, M_PI / 4.0f);

    Vec3f light = { 2.0f,2.0f,2.0f };
    LightSource* L = new LightSquare(light, 3.0f, -light, 0.2f);

    View V(C, I, L);

    Material* Mat = new Material(Vec3f{0.5f,0.5f,0.5f}, 0.5f, 0.5f, 0.5f);

    Mesh* M = new Mesh(Mat);
    M->loadOFF("data/example_lowres.off.txt");
    V.addMesh(M);

    V.rayTraceRandom(5);
    V.exp("result");

    return 0;
};
