#pragma once

//Engine
#include "Framework.h"
#pragma comment(lib, "../Debug/Framework.lib")

//Assimp
#include "Assimp/Importer.hpp"							//FBX File Reader
#include "Assimp/postprocess.h"							//Import Properties
#include "Assimp/scene.h"								//Bone, Mesh, Material -> Array
#pragma comment(lib, "Assimp/assimp-vc140-mt.lib")
