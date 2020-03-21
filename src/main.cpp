#include <cstdlib>
#include <memory>
#include <vector>
#include <fstream>
#include <cmath>
#include <unordered_map>
#include <iostream>
#include <omp.h>

#include "Vectors/vectors.h"
#include "misc/misc.h"
#include "Objects/Object.h"
#include "Objects/Light.h"
#include "Scene.h"

int threads = 4;

/* Find the closest intersected surface.
 * If no surface intersected, return false
 */
bool trace(
        const Vec3 &orig, const Vec3 &dir,
        const std::vector<std::unique_ptr<Object>> &objects,
        float &tNear, uint32_t &index, Vec2 &uv, Object **hitObject)
{
    *hitObject = nullptr;
    for (uint32_t k = 0; k < objects.size(); ++k) {
        float tNearK = kInfinity;
        uint32_t indexK;
        Vec2 uvK;
        if (objects[k]->intersect(orig, dir, tNearK, indexK, uvK) && tNearK < tNear) {
            *hitObject = objects[k].get();
            tNear = tNearK;
            index = indexK;
            uv = uvK;
        }
    }

    return (*hitObject != nullptr);
}



Vec3 castRay(
        const Vec3 &orig, const Vec3 &dir,
        const std::vector<std::unique_ptr<Object>> &objects,
        const std::vector<std::unique_ptr<Light>> &lights,
        const Options &options,
        uint32_t depth,
        bool test = false)
{
    if (depth > options.maxDepth) {
        return options.backgroundColor;
    }

    Vec3 hitColor = Vec3();//options.backgroundColor;
    float tnear = kInfinity;
    Vec2 uv;
    uint32_t index = 0;
    Object *hitObject = nullptr;
    if (trace(orig, dir, objects, tnear, index, uv, &hitObject)) {
        Vec3 hitPoint = orig + dir * tnear;
        Vec3 N; // normal
        Vec2 st; // st coordinates
        hitObject->getSurfaceProperties(hitPoint, dir, index, uv, N, st);
        Vec3 tmp = hitPoint;
        switch (hitObject->materialType) {
            case GLASS: {
                Vec3 reflectionDirection = normalize(reflect(dir, N));
                Vec3 refractionDirection = normalize(refract(dir, N, hitObject->ior));
                Vec3 reflectionRayOrig = (dotProduct(reflectionDirection, N) < 0) ?
                                          hitPoint - N * options.bias :
                                          hitPoint + N * options.bias;
                Vec3 refractionRayOrig = (dotProduct(refractionDirection, N) < 0) ?
                                          hitPoint - N * options.bias :
                                          hitPoint + N * options.bias;
                Vec3 reflectionColor = castRay(reflectionRayOrig, reflectionDirection, objects, lights, options,
                                                depth + 1, 1);
                Vec3 refractionColor = castRay(refractionRayOrig, refractionDirection, objects, lights, options,
                                                depth + 1, 1);
                float kr;
                applyFresnel(dir, N, hitObject->ior, kr);
                hitColor += reflectionColor * kr + refractionColor * (1 - kr);
                break;
            }
            case MIRROR: {
                Vec3 reflectionDirection = normalize(reflect(dir, N));
                Vec3 reflectionRayOrig = (dotProduct(reflectionDirection, N) < 0) ?
                                          hitPoint - N * options.bias :
                                          hitPoint + N * options.bias;
                Vec3 reflectionColor = castRay(reflectionRayOrig, reflectionDirection, objects, lights, options,
                                                depth + 1, 1);

                float kr;
                applyFresnel(dir, N, hitObject->ior, kr);
                hitColor += reflectionColor * kr;
                break;
            }
        }
            Vec3 lightAmt = 0, specularColor = 0;
            Vec3 shadowPointOrig = (dotProduct(dir, N) < 0) ? hitPoint + N * options.bias : hitPoint - N * options.bias;
            for (uint32_t i = 0; i < lights.size(); ++i) {
                Vec3 lightDir = lights[i]->position - hitPoint;
                // square of the distance between hitPoint and the light
                float lightDistance2 = dotProduct(lightDir, lightDir);
                lightDir = normalize(lightDir);
                float LdotN = std::max(0.f, dotProduct(lightDir, N));
                Object *shadowHitObject = nullptr;
                float tNearShadow = kInfinity;

                // is the point in shadow, and is the nearest occluding object closer to the object than the light itself?
                bool inShadow = trace(shadowPointOrig, lightDir, objects, tNearShadow, index, uv, &shadowHitObject) &&
                                tNearShadow * tNearShadow < lightDistance2;
                lightAmt += (1 - inShadow) * lights[i]->intensity * LdotN;
                Vec3 reflectionDirection = reflect(-lightDir, N);
                specularColor += powf(std::max(0.f, -dotProduct(reflectionDirection, dir)), hitObject->specularExponent) * lights[i]->intensity;
            }

            hitColor += lightAmt * hitObject->evalDiffuseColor(st) * hitObject->Kd + specularColor * hitObject->Ks;

    }

    return hitColor;
}

void WriteBMP(const char* fname, Pixel* a_pixelData, int width, int height)
{
    int paddedsize = (width*height/4) * sizeof(Pixel);

    unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0};
    unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0};

    bmpfileheader[ 2] = (unsigned char)(paddedsize    );
    bmpfileheader[ 3] = (unsigned char)(paddedsize>> 8);
    bmpfileheader[ 4] = (unsigned char)(paddedsize>>16);
    bmpfileheader[ 5] = (unsigned char)(paddedsize>>24);

    bmpinfoheader[ 4] = (unsigned char)(width/2    );
    bmpinfoheader[ 5] = (unsigned char)(width/2>> 8);
    bmpinfoheader[ 6] = (unsigned char)(width/2>>16);
    bmpinfoheader[ 7] = (unsigned char)(width/2>>24);
    bmpinfoheader[ 8] = (unsigned char)(height/2    );
    bmpinfoheader[ 9] = (unsigned char)(height/2>> 8);
    bmpinfoheader[10] = (unsigned char)(height/2>>16);
    bmpinfoheader[11] = (unsigned char)(height/2>>24);

    std::ofstream out(fname, std::ios::out | std::ios::binary);
    out.write((const char*)bmpfileheader, 14);
    out.write((const char*)bmpinfoheader, 40);
    out.write((const char*)a_pixelData, paddedsize);
    out.flush();
    out.close();
}

void SaveBMP(const char* fname, Vec3 * pixels, int w, int h)
{
    std::vector<Pixel> pixels2(w*h/4);

    int len = (w*h/4);
    #pragma omp parallel for
    for (int i = 0; i < h/2; ++i) {
        for (int j = 0; j < w/2; ++j) {
            Pixel px;
            px.r = (unsigned char) (255 * clamp (0, 1,  (pixels[2*(w*i + j)].x + pixels[2*(w*i + j)+1].x
                                                  + pixels[2*(w*i + j) + w].x + pixels[2*(w*i + j) + w + 1].x) / 4));
            px.g = (unsigned char) (255 * clamp (0, 1,  (pixels[2*(w*i + j)].y + pixels[2*(w*i + j)+1].y
                                                  + pixels[2*(w*i + j) + w].y + pixels[2*(w*i + j) + w + 1].y) / 4));
            px.b = (unsigned char) (255 * clamp (0, 1,  (pixels[2*(w*i + j)].z + pixels[2*(w*i + j)+1].z
                                                  + pixels[2*(w*i + j) + w].z + pixels[2*(w*i + j) + w + 1].z) / 4));
            pixels2[len - (i*w/2 + j+1)] = px;
        }
    }
    WriteBMP(fname, &pixels2[0], w, h);
}


void render (const Options &options, const std::vector<std::unique_ptr<Object>> &objects, const std::vector<std::unique_ptr<Light>> &lights) {
    auto *framebuffer = new Vec3[options.width * options.height];
    float scale = tan(deg2rad(options.fov * 0.5));
    float imageAspectRatio = options.width / (float)options.height;
    Vec3 orig(0);
    std::cout << "Start render\n";

    #pragma omp parallel for
    for (uint32_t j = 0; j < options.height; ++j) {
        for (uint32_t i = 0; i < options.width; ++i) {
            // generate primary ray direction
            float x = (2 * (i + 0.5) / (float)options.width - 1) * imageAspectRatio * scale;
            float y = (1 - 2 * (j + 0.5) / (float)options.height) * scale;
            Vec3 dir = normalize(Vec3(x, y, -1));

            framebuffer[j*options.width + i] = castRay(orig, dir, objects, lights, options, 0);
        }
    }
    std::cout << "Finished render\n";

    //Anti-aliasing by averaging the four neighbor rays
    SaveBMP(options.filename.c_str(), framebuffer, options.width, options.height);
    delete [] framebuffer;
}

void initObjects(std::vector<std::unique_ptr<Object>>& objects, std::vector<std::unique_ptr<Light>>& lights, int sceneId) {
    if (sceneId == 0) {
        scene0 (objects, lights);
    }
}



int main(int argc, const char **argv) {
    std::unordered_map<std::string, std::string> cmdLineParams;

    for (int i = 0; i < argc; i++) {
        std::string key(argv[i]);

        if (key.size() > 0 && key[0] == '-') {
            if (i != argc - 1) {
                cmdLineParams[key] = argv[i + 1];
                i++;
            } else {
                cmdLineParams[key] = "";
            }
        }
    }

    std::string outFilePath = "zout.bmp";
    if (cmdLineParams.find("-out") != cmdLineParams.end()) {
        outFilePath = cmdLineParams["-out"];
    }

    int sceneId = 0;
    if (cmdLineParams.find("-scene") != cmdLineParams.end()) {
        sceneId = atoi(cmdLineParams["-scene"].c_str());
    }

    if (cmdLineParams.find("-threads") != cmdLineParams.end()) {
        threads = atoi(cmdLineParams["-threads"].c_str());
    }
    omp_set_num_threads(threads);

    std::vector<std::unique_ptr<Object>> objects;
    std::vector<std::unique_ptr<Light>> lights;

    initObjects (objects, lights, sceneId);

    // setting up options
    Options options = setOptions(outFilePath);

    // finally, render
    render(options, objects, lights);

    return 0;
}