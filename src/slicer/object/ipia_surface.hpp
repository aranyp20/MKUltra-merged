#pragma once

#include "frep.hpp"

#include "ipia.hh"
#include "implicit-curvatures.hh"

class ipia_surface : public frep_object
{

    std::array<Geometry::Point3D, 2> boundingBox(const Geometry::TriMesh &mesh)
    {
        Geometry::Point3D boxmin, boxmax;
        const auto &points = mesh.points();
        boxmin = boxmax = points[0];
        for (const auto &p : points)
            for (int i = 0; i < 3; ++i)
            {
                boxmin[i] = std::min(boxmin[i], p[i]);
                boxmax[i] = std::max(boxmax[i], p[i]);
            }
        // Add 5%
        auto mean = (boxmin + boxmax) / 2;
        boxmin = mean + (boxmin - mean) * 1.05;
        boxmax = mean + (boxmax - mean) * 1.05;
        return {boxmin, boxmax};
    }

    std::array<size_t, 3> computeResolution(const std::array<Geometry::Point3D, 2> &bbox, size_t size)
    {
        std::array<size_t, 3> resolution;
        auto axis = bbox[1] - bbox[0];
        double axis_delta = axis.norm() / size / std::sqrt(3);
        resolution[0] = std::max<size_t>((size_t)std::ceil(axis[0] / axis_delta), 2);
        resolution[1] = std::max<size_t>((size_t)std::ceil(axis[1] / axis_delta), 2);
        resolution[2] = std::max<size_t>((size_t)std::ceil(axis[2] / axis_delta), 2);
        return resolution;
    }

    void approximateNormals(std::vector<IPIA::PointNormal> &pns,
                            const std::list<Geometry::TriMesh::Triangle> &tris)
    {
        // Weights according to:
        //   N. Max, Weights for computing vertex normals from facet normals.
        //     Journal of Graphics Tools, Vol. 4(2), 1999.
        for (const auto &tri : tris)
        {
            for (size_t i = 0; i < 3; ++i)
            {
                size_t i0 = tri[i], i1 = tri[(i + 1) % 3], i2 = tri[(i + 2) % 3];
                Geometry::Vector3D v1 = pns[i0].p - pns[i2].p, v2 = pns[i1].p - pns[i0].p;
                double w = v1.normSqr() * v2.normSqr();
                pns[i0].n += (v1 ^ v2) / (w == 0.0 ? 1.0 : w);
            }
        }
        for (auto &pn : pns)
            if (pn.n.norm() > Geometry::epsilon)
                pn.n.normalize();
    }

    std::shared_ptr<IPIA> surface;

public:
    std::array<Geometry::Point3D, 2> bbox;
    std::array<size_t, 3> dc_res;

    ipia_surface(const ipia_surface &) = default;
    ipia_surface()
    {
        auto mesh = Geometry::TriMesh::readOBJ("stanford-bunny.obj");

        bbox = boundingBox(mesh);
        size_t control = 50;
        auto size = computeResolution(bbox, control);
        size_t res = 20;
        dc_res = computeResolution(bbox, res);
        surface = std::make_shared<IPIA>(3, bbox, size);

        std::vector<IPIA::PointNormal> samples;
        size_t n = mesh.points().size();
        samples.resize(n);
        for (size_t i = 0; i < n; ++i)
            samples[i] = {mesh[i], {0, 0, 0}};

        approximateNormals(samples, mesh.triangles());

        double step = 0.001;
        size_t iterations = 20;
        double tol = 0.00001;
        size_t done_iterations = surface->fit(samples, step, iterations, tol);
    }

    interval fn(const interval &X, const interval &Y, double h) const override
    {
        return interval(-1000, 1000);
    }
    double fn(const vec3 &p) const override
    {
        return (*surface)({p.x, p.y, p.z});
    }
    dnum fn(const dnum &X, const dnum &Y, const dnum &h) const override
    {
        return dnum(1, 0);
    }

    bounding_box get_prefered_box() const override
    {
        return bounding_box(vec3(bbox[0][0], bbox[0][1], bbox[0][2]), bbox[1][0] - bbox[0][0], bbox[1][2] - bbox[0][2]);
    }
};
