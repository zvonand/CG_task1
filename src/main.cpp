#include <cstdlib>
#include <memory>
#include <vector>
#include <cstdint>
#include <fstream>
#include <cmath>
#include <unordered_map>
#include <omp.h>
#include <iostream>

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
        const Vec3f &orig, const Vec3f &dir,
        const std::vector<std::unique_ptr<Object>> &objects,
        float &tNear, uint32_t &index, Vec2f &uv, Object **hitObject)
{
    *hitObject = nullptr;
    for (uint32_t k = 0; k < objects.size(); ++k) {
        float tNearK = kInfinity;
        uint32_t indexK;
        Vec2f uvK;
        if (objects[k]->intersect(orig, dir, tNearK, indexK, uvK) && tNearK < tNear) {
            *hitObject = objects[k].get();
            tNear = tNearK;
            index = indexK;
            uv = uvK;
        }
    }

    return (*hitObject != nullptr);
}



Vec3f castRay(
        const Vec3f &orig, const Vec3f &dir,
        const std::vector<std::unique_ptr<Object>> &objects,
        const std::vector<std::unique_ptr<Light>> &lights,
        const Options &options,
        uint32_t depth,
        bool test = false)
{
    if (depth > options.maxDepth) {
        return options.backgroundColor;
    }

    Vec3f hitColor = Vec3f();//options.backgroundColor;
    float tnear = kInfinity;
    Vec2f uv;
    uint32_t index = 0;
    Object *hitObject = nullptr;
    if (trace(orig, dir, objects, tnear, index, uv, &hitObject)) {
        Vec3f hitPoint = orig + dir * tnear;
        Vec3f N; // normal
        Vec2f st; // st coordinates
        hitObject->getSurfaceProperties(hitPoint, dir, index, uv, N, st);
        Vec3f tmp = hitPoint;
        switch (hitObject->materialType) {
            case REFLECTION_AND_REFRACTION: {
                Vec3f reflectionDirection = normalize(reflect(dir, N));
                Vec3f refractionDirection = normalize(refract(dir, N, hitObject->ior));
                Vec3f reflectionRayOrig = (dotProduct(reflectionDirection, N) < 0) ?
                                          hitPoint - N * options.bias :
                                          hitPoint + N * options.bias;
                Vec3f refractionRayOrig = (dotProduct(refractionDirection, N) < 0) ?
                                          hitPoint - N * options.bias :
                                          hitPoint + N * options.bias;
                Vec3f reflectionColor = castRay(reflectionRayOrig, reflectionDirection, objects, lights, options,
                                                depth + 1, 1);
                Vec3f refractionColor = castRay(refractionRayOrig, refractionDirection, objects, lights, options,
                                                depth + 1, 1);
                float kr;
                fresnel(dir, N, hitObject->ior, kr);
                hitColor += reflectionColor * kr + refractionColor * (1 - kr);
                break;
            }
            case MIRROR: {
                Vec3f reflectionDirection = normalize(reflect(dir, N));
                Vec3f reflectionRayOrig = (dotProduct(reflectionDirection, N) < 0) ?
                                          hitPoint - N * options.bias :
                                          hitPoint + N * options.bias;
                Vec3f reflectionColor = castRay(reflectionRayOrig, reflectionDirection, objects, lights, options,
                                                depth + 1, 1);

                float kr;
                fresnel(dir, N, hitObject->ior, kr);
                hitColor += reflectionColor * kr;
                break;
            }
            case REFRACTION: {
                Vec3f refractionDirection = normalize(refract(dir, N, hitObject->ior));
                Vec3f refractionRayOrig = (dotProduct(refractionDirection, N) < 0) ?
                                          hitPoint - N * options.bias :
                                          hitPoint + N * options.bias;
                Vec3f refractionColor = castRay(refractionRayOrig, refractionDirection, objects, lights, options,
                                                depth + 1, 1);
                float kr;
                fresnel(dir, N, hitObject->ior, kr);
                hitColor += refractionColor * (1 - kr);
                break;
            }
        }
            Vec3f lightAmt = 0, specularColor = 0;
            Vec3f shadowPointOrig = (dotProduct(dir, N) < 0) ? hitPoint + N * options.bias : hitPoint - N * options.bias;
            for (uint32_t i = 0; i < lights.size(); ++i) {
                Vec3f lightDir = lights[i]->position - hitPoint;
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
                Vec3f reflectionDirection = reflect(-lightDir, N);
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

void SaveBMP(const char* fname, Vec3f * pixels, int w, int h)
{
    std::vector<Pixel> pixels2(w*h/4);
    #pragma omp parallel shared (pixels2) for num_threads(threads)
    int len = (w*h/4);
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
    auto *framebuffer = new Vec3f[options.width * options.height];
    Vec3f *pix = framebuffer;
    float scale = tan(deg2rad(options.fov * 0.5));
    float imageAspectRatio = options.width / (float)options.height;
    Vec3f orig(0);
    #pragma omp parallel shared (framebuffer) for num_threads(threads)
    for (uint32_t j = 0; j < options.height; ++j) {
        for (uint32_t i = 0; i < options.width; ++i) {
            // generate primary ray direction
            float x = (2 * (i + 0.5) / (float)options.width - 1) * imageAspectRatio * scale;
            float y = (1 - 2 * (j + 0.5) / (float)options.height) * scale;
            Vec3f dir = normalize(Vec3f(x, y, -1));
            *(pix++) = castRay(orig, dir, objects, lights, options, 0);
        }
    }

    //Added anti-aliasing by averaging the four neighbor rays
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

    std::vector<std::unique_ptr<Object>> objects;
    std::vector<std::unique_ptr<Light>> lights;

    initObjects (objects, lights, sceneId);

    // setting up options
    Options options = setOptions(outFilePath);

    // finally, render
    render(options, objects, lights);

    return 0;
}