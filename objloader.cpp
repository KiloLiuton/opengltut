#include <vector>
#include <stdio.h>
#include <string>
#include <cstring>

#include "objloader.hpp"


bool loadOBJ(
    const char* path,
    std::vector<glm::vec3>& out_vertices,
    std::vector<glm::vec2>& out_uvs,
    std::vector<glm::vec3>& out_normals
)
{
    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    std::vector<glm::vec3> tmp_vertices;
    std::vector<glm::vec2> tmp_uvs;
    std::vector<glm::vec3> tmp_normals;
    FILE* file = fopen(path, "r");
    if (file == NULL) {
        fprintf(stderr, "Cannot open file %s!\n", path);
        return false;
    }
    char lineHeader[128];
    int res = fscanf(file, "%s", lineHeader); // Read up to blank character (space, newline, ...)
    while(res != EOF) {
        if (strcmp(lineHeader, "v") == 0) {
            glm::vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            tmp_vertices.push_back(vertex);
        } else if (strcmp(lineHeader, "vt") == 0) {
            glm::vec2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y);
            tmp_uvs.push_back(uv);
        } else if (strcmp(lineHeader, "vn") == 0) {
            glm::vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            tmp_normals.push_back(normal);
        } else if (strcmp(lineHeader, "f") == 0) {
            std::string v1, v2, v3;
            unsigned int vIndex[3], uvIndex[3], nIndex[3];
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
                &vIndex[0], &uvIndex[0], &nIndex[0],
                &vIndex[1], &uvIndex[1], &nIndex[1],
                &vIndex[2], &uvIndex[2], &nIndex[2]
            );
            if (matches != 9) {
                fprintf(stderr, "OBJ file\n%s\ncan't be loaded by our humble parser :C\n", path);
                return false;
            }
            vertexIndices.push_back(vIndex[0]);
            vertexIndices.push_back(vIndex[1]);
            vertexIndices.push_back(vIndex[2]);
            uvIndices.push_back(uvIndex[0]);
            uvIndices.push_back(uvIndex[1]);
            uvIndices.push_back(uvIndex[2]);
            normalIndices.push_back(nIndex[0]);
            normalIndices.push_back(nIndex[1]);
            normalIndices.push_back(nIndex[2]);
        } else {
            // Probably a comment, so read until end of line
            char trashBuffer[512];
            fgets(trashBuffer, 512, file);
        }
        res = fscanf(file, "%s", lineHeader);
    }

    for (unsigned int i=0; i<vertexIndices.size(); i++ ) {
        unsigned int vidx  = vertexIndices[i];
        unsigned int uvidx = uvIndices[i];
        unsigned int nidx  = normalIndices[i];

        glm::vec3 out_vertex = tmp_vertices[vidx-1];
        glm::vec2 out_uv     = tmp_uvs[uvidx-1];
        glm::vec3 out_normal = tmp_normals[nidx-1];

        out_vertices.push_back(out_vertex);
        out_uvs.push_back(out_uv);
        out_normals.push_back(out_normal);
    }
    fclose(file);
    return true;
}
