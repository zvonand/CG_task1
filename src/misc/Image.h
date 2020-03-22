//
// Created by andrez on 22.03.2020.
//

#ifndef RT_IMAGE_H
#define RT_IMAGE_H

#include "../Vectors/vectors.h"
#include <vector>


bool trace(
        const vec3 &orig, const vec3 &dir,
        const std::vector<std::unique_ptr<Object>> &objects,
        float &tNear, uint32_t &index, Vec2 &uv, Object **hitObject)
{
    *hitObject = nullptr;
    for (uint32_t k = 0; k < objects.size(); ++k) {
        float tNearK = infty;
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


vec3 castRay (const vec3 &orig, const vec3 &dir, const std::vector<std::unique_ptr<Object>> &objects,
              const std::vector<std::unique_ptr<Light>> &lights, const Options &options, uint32_t depth) {

    if (depth > options.maxDepth) {
        return options.backColor;
    }

    vec3 pixColor = vec3();
    float tnear = infty;
    Vec2 uv;
    uint32_t index = 0;
    Object *obj = NULL;
    if (trace(orig, dir, objects, tnear, index, uv, &obj)) {    //get hitten object, whose properties will be computed later
        vec3 hitPoint = orig + dir * tnear; //tnear == sort of proximity
        vec3 N;
        Vec2 st;
        obj->getSurfaceProperties(hitPoint, dir, index, uv, N, st);

        //add basic opaqueness to every figure regardless of its material to make reflections look better and to apply correct lighting model
        vec3 lightAmt = 0, specularColor = 0;
        vec3 shadowPointOrigin = (scalarProduct(dir, N) < 0) ? hitPoint + N * options.bias : hitPoint - N * options.bias;
        for (uint32_t i = 0; i < lights.size(); ++i) {
            vec3 lightDir = lights[i]->position - hitPoint;
            // square of the distance between hitPoint and the light
            float sqrdLightDst = scalarProduct(lightDir, lightDir);
            lightDir = toUnitVector(lightDir);
            float LdotN = std::max(0.f, scalarProduct(lightDir, N));
            Object *hitByShadow = nullptr;
            float tNearShadow = infty;

            // is the point shadowed, and is there anything closer to the light, that is the question/
            bool shadowed = trace(shadowPointOrigin, lightDir, objects, tNearShadow, index, uv, &hitByShadow) &&
                            tNearShadow * tNearShadow < sqrdLightDst;
            lightAmt += (1 - shadowed) * lights[i]->intensity * LdotN;
            vec3 reflectionDirection = reflect(-lightDir, N);
            specularColor += powf(std::max(0.f, -scalarProduct(reflectionDirection, dir)), obj->specularExponent) * lights[i]->intensity;
        }
        pixColor += lightAmt * obj->evalDiffuseColor(st) * obj->Kd + specularColor * obj->Ks;

        switch (obj->materialType) {
            case GLASS: {
                //TODO: add variation for reflection intecity
                vec3 reflectDir = toUnitVector(reflect(dir, N));
                vec3 refractDir = toUnitVector(refract(dir, N, obj->ior));
                vec3 reflectionRayOrig = (scalarProduct(reflectDir, N) < 0) ?
                                         hitPoint - N * options.bias :
                                         hitPoint + N * options.bias;
                vec3 refractionRayOrig = (scalarProduct(refractDir, N) < 0) ?
                                         hitPoint - N * options.bias :
                                         hitPoint + N * options.bias;
                vec3 reflectionColor = castRay(reflectionRayOrig, reflectDir, objects, lights, options,
                                               ++depth);
                vec3 refractionColor = castRay(refractionRayOrig, refractDir, objects, lights, options,
                                               ++depth);
                float kr;
                applyFresnel(dir, N, obj->ior, kr);
                pixColor += reflectionColor * kr + refractionColor * (1 - kr);
                break;
            }
            case MIRROR: {
                vec3 reflectionDirection = toUnitVector(reflect(dir, N));

                vec3 reflectionRayOrig = (scalarProduct(reflectionDirection, N) < 0) ?
                                         hitPoint - N * options.bias :
                                         hitPoint + N * options.bias;
                vec3 reflectionColor = castRay(reflectionRayOrig, reflectionDirection, objects, lights, options,
                                               ++depth);

                float kr;
                applyFresnel(dir, N, obj->ior, kr);
                pixColor += reflectionColor * kr;
                break;
            }
        }

    }

    return pixColor;
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

void SaveBMP(const char* fname, vec3 * pixels, int w, int h)
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
    auto *framebuffer = new vec3[options.width * options.height];
    float scale = tan(deg2rad(options.viewField * 0.5));
    float imageAspectRatio = options.width / (float)options.height;
    vec3 orig(0);       //consider viewpoint (0, 0, 0) and looking towards -infty(Oz)
    std::cout << "Start render\n";

#pragma omp parallel for
    for (uint32_t j = 0; j < options.height; ++j) {
        for (uint32_t i = 0; i < options.width; ++i) {
            // generate primary ray direction
            float x = (2 * (i + 0.5) / (float)options.width - 1) * imageAspectRatio * scale;
            float y = (1 - 2 * (j + 0.5) / (float)options.height) * scale;
            vec3 dir = toUnitVector(vec3(x, y, -1));

            framebuffer[j*options.width + i] = castRay(orig, dir, objects, lights, options, 0);
        }
    }
    std::cout << "Finished render\n";

    //Anti-aliasing by averaging the four neighbor rays
    SaveBMP(options.filename.c_str(), framebuffer, options.width, options.height);
    delete [] framebuffer;
}



#endif //RT_IMAGE_H
