//
// Created by andrez on 15.03.2020.
//

#include "Scene.h"
#include "Objects/Checkboard.h"
#include <fstream>
#include <iostream>

void scene0(std::vector<std::unique_ptr<Object>> &objects, std::vector<std::unique_ptr<Light>> &lights) {
    Vec3 verts[4] = {{-50, -2.5, 0}, {50, -2.5, 0}, {50, -1, -50}, {-50, -1, -50}};
    auto *surface = new Checkboard(verts);
    objects.push_back(std::unique_ptr<MeshTriangle>(surface));

    auto *sph1 = new Sphere(Vec3(-1, 1, -4), 0.8, GLASS, 1.5, Vec3(0.6, 0.5, 0.56));
    objects.push_back(std::unique_ptr<Sphere>(sph1));

    auto *sph2 = new Sphere(Vec3(0.4, -0.5, -2), 0.5, MIRROR, 1.3, Vec3(1, 0.5, 0));
    objects.push_back(std::unique_ptr<Sphere>(sph2));

    auto *sph3 = new Sphere(Vec3(-2, -0.8, -3), 0.65, MIRROR, 1.5, Vec3(0, 0.7, 0.2));
    objects.push_back(std::unique_ptr<Sphere>(sph3));

    auto *sph4 = new Sphere(Vec3(0, -0.7, -3.4), 0.5, GLASS, 1.5, Vec3(0.45, 0.12, 0.87));
    objects.push_back(std::unique_ptr<Sphere>(sph4));

    auto *sph5 = new Sphere(Vec3(2.6, -0.4, -3), 0.37, GLASS, 1.5, Vec3(1, 0.2, 0.15));
    objects.push_back(std::unique_ptr<Sphere>(sph5));

    //importMesh("src/Mesh/Teapot.obj", objects);

    lights.push_back(std::unique_ptr<Light>(new Light(Vec3(10, 20, 10), 0.7)));
    lights.push_back(std::unique_ptr<Light>(new Light(Vec3(-5, 15, 8), 0.56)));
    lights.push_back(std::unique_ptr<Light>(new Light(Vec3(-10, 17, 4), 0.4)));
}

void importMesh(const char* filename, std::vector<std::unique_ptr<Object>> &objects) {
    std::ifstream fin;
    fin.open(filename, std::ios::in);
    if (!fin.is_open()) {
        std::cout << "Not opened\n";
    }
    int verts, faces;
    fin >> verts >> faces;

    Vec3 * verticesVector = new Vec3[verts];
    uint32_t * facesOrder = new uint32_t [faces*3];

    char type;
    for (int i = 0; i < verts; ++i) {
        float a, b, c;
        fin >> type;
        fin >> a >> b >> c;
        verticesVector[i] = Vec3(a, b, c - 10);
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
