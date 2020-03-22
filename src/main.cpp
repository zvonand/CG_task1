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

    int sceneId = 1;
    if (cmdLineParams.find("-scene") != cmdLineParams.end()) {
        sceneId = atoi(cmdLineParams["-scene"].c_str());
    }

    if (cmdLineParams.find("-threads") != cmdLineParams.end()) {
        threads = atoi(cmdLineParams["-threads"].c_str());
    }
    omp_set_num_threads(threads);

    if (sceneId == 1) {
        scene1(setOptions(outFilePath));
    }

    return 0;
}