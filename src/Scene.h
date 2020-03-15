//
// Created by andrez on 15.03.2020.
//

#ifndef RT_SCENE_H
#define RT_SCENE_H

#include <cstdlib>
#include <memory>
#include <vector>
#include <cstdint>
#include <fstream>
#include <cmath>
#include <unordered_map>

#include "Vectors/vectors.h"
#include "misc/misc.h"
#include "Objects/Object.h"
#include "Objects/Sphere.h"
#include "Bitmap/Bitmap.h"
#include "Objects/MeshTriangle.h"
#include "Objects/Light.h"

void scene0 (std::vector<std::unique_ptr<Object>>& objects, std::vector<std::unique_ptr<Light>>& lights);


#endif //RT_SCENE_H
