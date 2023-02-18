#include <iostream>
#include <vector>
#include <bits/stdc++.h>
#include "NormalCreator.h"

NormalCreator::NormalCreator(const std::string &filename)
{

    std::fstream file;
    std::string word;

    try
    {

        file.open(filename.c_str());
    }
    catch (std::exception)
    {
        perror("Object input could not be opened.");
        exit(1);
    }

    InputState is;

    std::vector<double> cVert;
    std::vector<unsigned int> cFace;

    while (file >> word)
    {

        if (word.compare("v") == 0)
        {
            is.Init(InputState::VERT);
        }
        else if (word.compare("f") == 0)
        {
            is.Init(InputState::FACE);
        }
        else
        {
            InputState::state stateNow = is.Step();
            if (stateNow == InputState::VERT)
            {
                cVert.push_back(stod(word));
            }
            else if (stateNow == InputState::FACE)
            {
                cFace.push_back(stoi(word));
            }
        }

        if (cVert.size() == 3)
        {
            vertices.push_back(vec3(cVert[0], cVert[1], cVert[2]));
            std::vector<unsigned int> tv;
            influencedBy.insert({vertices.size() - 1, tv});
            cVert.clear();
        }
        if (cFace.size() == 3)
        {
            faces.push_back(cFace);
            influencedBy[cFace[0]].push_back(faces.size() - 1);
            influencedBy[cFace[1]].push_back(faces.size() - 1);
            influencedBy[cFace[2]].push_back(faces.size() - 1);
            cFace.clear();
        }
    }
    CreateNormals();
}

imap NormalCreator::FindFacesOfVert(unsigned int pi)
{
    imap result;
    for (const auto &a : influencedBy[pi])
    {
        result.push_back(faces[a]);
    }

    return result;
}

std::vector<vec3> NormalCreator::CreateFaceNormals(const imap &im)
{
    std::vector<vec3> result;

    for (const auto &a : im)
    {
        result.push_back(normalize(cross((vertices[a[1] - 1] - vertices[a[0] - 1]), (vertices[a[2] - 1] - vertices[a[0] - 1]))));
    }

    return result;
}

void NormalCreator::CreateNormal(unsigned int pi)
{
    std::vector<vec3> fNormals = CreateFaceNormals(FindFacesOfVert(pi + 1));
    vec3 result(0, 0, 0);
    for (const auto &a : fNormals)
    {
        result = result + a;
    }
    result = result / fNormals.size();
    if (fNormals.size() == 0)
    {
        normals.push_back(vec3(0, 0, 0));
    }
    else
    {

        normals.push_back(result);
    }
}

void NormalCreator::CreateNormals()
{

    for (unsigned int i = 0; i < vertices.size(); i++)
    {
        CreateNormal(i);
    }
}
