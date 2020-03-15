//
// Created by andrez on 15.03.2020.
//

#include "Scene.h"

void scene0(std::vector<std::unique_ptr<Object>> &objects, std::vector<std::unique_ptr<Light>> &lights) {
    auto *sph1 = new Sphere(Vec3f(-1, 0.1, -2.4), 0.8);
    sph1->materialType = REFLECTION_AND_REFRACTION;
    sph1->diffuseColor = Vec3f(0.6, 0.5, 0.56);
    auto *sph2 = new Sphere(Vec3f(0.4, -0.5, -2), 0.5);
    sph2->ior = 1.5;
    sph2->diffuseColor = Vec3f(0, 1, 0);
    sph2->materialType = MIRROR;
    objects.push_back(std::unique_ptr<Sphere>(sph1));
    objects.push_back(std::unique_ptr<Sphere>(sph2));
    Vec3f verts[4] = {{-10,-1.5,0}, {10,-1.5,0}, {10,0,-16}, {-10,0,-16}};
    uint32_t vertIndex[6] = {0, 1, 3, 1, 2, 3};
    Vec2f st[4] = {{0, 0}, {1, 0}, {1, 1}, {0, 1}};
    auto *mesh = new MeshTriangle(verts, vertIndex, 2, st);
    mesh->materialType = REFLECTION_AND_REFRACTION;

    objects.push_back(std::unique_ptr<MeshTriangle>(mesh));

    lights.push_back(std::unique_ptr<Light>(new Light(Vec3f(10, 20, 10), 0.45)));
    //lights.push_back(std::unique_ptr<Light>(new Light(Vec3f(30, 50, -12), 0.4)));
}
