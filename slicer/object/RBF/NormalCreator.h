#pragma once

#include <vector>
#include <map>
#include "mymath.h"

typedef std::vector<std::vector<unsigned int>> imap;

class NormalCreator
{

    struct InputState
    {
        enum state
        {
            VERT,
            FACE,
            COMMENT,
            NONE
        };
        state myState;
        inline void Init(state _state)
        {
            myState = _state;
            remaining = 3;
        }
        inline state Step()
        {
            if (remaining == 0)
            {
                myState = NONE;
            }
            else
            {
                remaining--;
            }
            return myState;
        }

    private:
        int remaining = 3;
    };

    std::map<unsigned int, std::vector<unsigned int>> influencedBy;
    std::vector<vec3> vertices;
    imap faces;
    std::vector<vec3> normals;

    imap FindFacesOfVert(unsigned int);
    std::vector<vec3> CreateFaceNormals(const imap &);

    void CreateNormal(unsigned int);
    void CreateNormals();

public:
    NormalCreator(const std::string &);

    inline std::vector<vec3> GetNormals() const { return normals; }
    inline std::vector<vec3> GetVertices() const { return vertices; }
};