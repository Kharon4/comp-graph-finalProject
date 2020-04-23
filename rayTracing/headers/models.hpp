/**
 * @file models.hpp
 * @brief Exports function(s) for loading .obj files.
 */

#ifndef MODELS_HPP
#define MODELS_HPP 1

#include <vector>
#include <unordered_map>
#include <string>
#include <vec3.h>

class Model {

     /* Data Members */
    public:
        std::vector<vec3d> vertices;
        std::vector<vec3d> uvs;
        std::vector<vec3d> normals;
        std::vector<vec3d> vertex_diffuse_colors;

    private:
        std::unordered_map<std::string, vec3d> material_diffuse_colors;

    /* Class Methods */
    public:
        /* Constructor */
        Model();

        /**
         * Loads vertices , uv map coordinates and surface normals from the specified obj file.
         *
         * @param fileName name of the obj file
         */
        bool loadObj(std::string directory, std::string fileName);

    private:
        bool loadMtl(std::string directory, std::string fileName);
};

#endif