#include <cstring>
#include <filesystem>
#include "models.hpp"

#define safe 0

#if safe
#define fopen fopen_s
#define fscanf fscanf_s
#endif


Model::Model() {
    this->vertices = std::vector<vec3d>();
    this->uvs = std::vector<vec3d>();
    this->normals = std::vector<vec3d>();
}


bool Model::loadObj(std::string directory, std::string fileName) {
    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    std::vector<vec3d> temp_vertices;
    std::vector<vec3d> temp_uvs;
    std::vector<vec3d> temp_normals;
    std::unordered_map<unsigned int, std::string> material_vertex_indices;
    std::string current_material;
    std::string filepath = (std::filesystem::path(directory) /  std::filesystem::path(fileName)).string();
    FILE *file = fopen(filepath.c_str(), "rb");
    if (!file) {
        printf("Model could not be opened");
        return false;
    }

    current_material.resize(128);
    while (1) {
        char lineHeader[128];
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF) break;

        // match line header and extract vector info
        if (strcmp(lineHeader, "v") == 0) {
            vec3d vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            temp_vertices.push_back(vertex);
        } else if (strcmp(lineHeader, "vt") == 0) {
            vec3d uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y );
            uv.z = 0;
            temp_uvs.push_back(uv);
        } else if (strcmp(lineHeader, "vn") == 0) {
            vec3d normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
            temp_normals.push_back(normal);
        }else if (strcmp(lineHeader, "f") == 0) {
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];

            int matches = fscanf(file, "%d//%d %d//%d %d//%d\n", &vertexIndex[0], &normalIndex[0],
                                &vertexIndex[1], &normalIndex[1], &vertexIndex[2], &normalIndex[2]);
            if (matches != 6){
                printf("File can't be read by our simple parser : ( Try exporting with other options\n");
                return false;
            }

            vertexIndices.push_back(vertexIndex[0]);
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);

            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);

            material_vertex_indices.insert_or_assign(vertexIndex[0], current_material);
            material_vertex_indices.insert_or_assign(vertexIndex[1], current_material);
            material_vertex_indices.insert_or_assign(vertexIndex[2], current_material);
        } else if(strcmp(lineHeader, "mtllib") == 0) {
            char filename[25];
            fscanf(file, "%s\n", &filename[0]);
            loadMtl(directory, std::string(filename));
        } else if (strcmp(lineHeader, "usemtl") == 0) {
            char buffer[128];
            fscanf(file, "%s\n", &buffer[0]);
            current_material = buffer;
        }
    }

    // Go through and deindex the data
    for (unsigned int i = 0; i < vertexIndices.size(); i++) {
        unsigned int vertexIndex = vertexIndices[i];
        vec3d vertex = temp_vertices[vertexIndex - 1];  // the indices start from 1, not 0
        this->vertices.push_back(vertex);

        // unsigned int uvIndex = uvIndices[i];
        // glm::vec2 uv = temp_uvs[uvIndex - 1];
        // this->uvs.push_back(uv);

        unsigned int normalIndex = normalIndices[i];
        vec3d normal = temp_normals[normalIndex - 1];
        this->normals.push_back(normal);

        std::string material = material_vertex_indices[vertexIndex];
        this->vertex_diffuse_colors.push_back(this->material_diffuse_colors[material]);
    }

    fclose(file);
    return true;
}

bool Model::loadMtl(std::string directory, std::string fileName) {
    /*
        1. Load The file
        2. Find each material
        3. Get the kd value for each
    */

   std::string filepath = (std::filesystem::path(directory) /  std::filesystem::path(fileName)).string();
   FILE *file = fopen(filepath.c_str(), "rb");
    if (!file) {
        printf("Material file could not be opened");
        return false;
    }

    std::string current_material("");
    while (1) {
        char lineHeader[128];
        int res = fscanf(file, "%s", &lineHeader[0]);
        if (res == EOF) break;
        if (strcmp(lineHeader, "newmtl") == 0) {
            // printf("%u ", current_material.c_str());
            fscanf(file, "%s\n", &lineHeader[0]);
            current_material  = &lineHeader[0];
        }else if(strcmp(lineHeader, "Kd") == 0) {
            if (!current_material.empty()) {
                vec3d colors;

                // Parse the values and add it to the hashmap
                fscanf(file, "%f %f %f\n", &colors.x, &colors.y, &colors.z);
                this->material_diffuse_colors.insert_or_assign(current_material, colors);

                // clear the current material for the next material
                current_material.clear();
            }
        }
    }
    return true;
}
