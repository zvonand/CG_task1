//
// Created by andrez on 15.03.2020.
//

#include "Scene.h"
#include "Objects/Checkboard.h"
#include "misc/misc.h"
#include "misc/Image.h"



void scene1 (Options options) {
    std::vector<std::unique_ptr<Object>> objects;
    std::vector<std::unique_ptr<Light>> lights;

    vec3 verts[4] = {{-50, -1.5, 0}, {50, -1.5, 0}, {50, 0, -50}, {-50, 0, -50}};
    auto *surface = new Checkboard(verts);
    objects.push_back(std::unique_ptr<MeshTriangle>(surface));

    //TODO: swap parameters so one can omit unnecessary ones
    auto *sph_1 = new Sphere(vec3(-1, 1, -5), 0.8, OPAQUE, vec3(0.6, 0.5, 0.56));
    objects.push_back(std::unique_ptr<Sphere>(sph_1));

    auto *sph_2 = new Sphere(vec3(0.4, -0.1, -3), 0.5, GLASS, vec3(0, 0, 0));
    objects.push_back(std::unique_ptr<Sphere>(sph_2));

    auto *sph_3 = new Sphere(vec3(-2, -0.8, -4), 0.65, MIRROR, vec3(0, 0.7, 0.2));
    objects.push_back(std::unique_ptr<Sphere>(sph_3));

    auto *sph_4 = new Sphere(vec3(0, -0.7, -4.4), 0.5, MIRROR, vec3(0.45, 0.12, 0.87));
    objects.push_back(std::unique_ptr<Sphere>(sph_4));

    auto *sph_5 = new Sphere(vec3(2.6, -0.4, -3.67), 0.37, GLASS, vec3(1, 0, 0.75));
    objects.push_back(std::unique_ptr<Sphere>(sph_5));

    auto *sph_6 = new Sphere(vec3(1, 0, -2), 0.2, MIRROR, vec3(0.8, 0.2, 0));
    objects.push_back(std::unique_ptr<Sphere>(sph_6));

    auto *sph_7 = new Sphere(vec3(1.4, 0, -4.6), 0.41, GLASS, vec3(0.6, 0.41, 0.6));
    objects.push_back(std::unique_ptr<Sphere>(sph_7));

    auto *sph_8 = new Sphere(vec3(0.21, -0.8, -3), 0.3, GLASS, vec3(0.6, 0.6, 0.4));
    objects.push_back(std::unique_ptr<Sphere>(sph_8));

    lights.push_back(std::unique_ptr<Light>(new Light(vec3(15, 20, 10), 0.9)));
    //lights.push_back(std::unique_ptr<Light>(new Light(vec3(-15, 15, 8), 0.1)));
    lights.push_back(std::unique_ptr<Light>(new Light(vec3(-10, 10, 6), 0.8)));

    render(options, objects, lights);
}

void importMesh(const char* filename, std::vector<std::unique_ptr<Object>> &objects) {
    std::ifstream fin;
    fin.open(filename, std::ios::in);
    if (!fin.is_open()) {
        std::cout << "Not opened\n";
    }
    int verts, faces;
    fin >> verts >> faces;

    vec3 * verticesVector = new vec3[verts];
    uint32_t * facesOrder = new uint32_t [faces*3];

    char type;
    for (int i = 0; i < verts; ++i) {
        float a, b, c;
        fin >> type;
        fin >> a >> b >> c;
        verticesVector[i] = vec3(a, b, c - 10);
    }
    for (int i = 0; i < faces; ++i) {
        uint32_t a, b, c;
        fin >> type;
        fin >> a >> b >> c;

        facesOrder[3*i] = --a;
        facesOrder[3*i + 1] = --b;
        facesOrder[3*i + 2] = --c;
    }
    fin.close();
    std::cout << "File imported\n";
    Vec2 st[4] = {{0, 0}, {1, 0}, {1, 1}, {0, 1}};
    auto *mesh = new MeshTriangle(verticesVector, facesOrder, faces, st);
    mesh->materialType = OPAQUE;
    objects.push_back(std::unique_ptr<MeshTriangle>(mesh));
}
