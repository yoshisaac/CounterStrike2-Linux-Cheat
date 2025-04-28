#include <float.h>
#include <vector>

#include "../vector.hpp"
#include <cmath>

struct AABB {
    Vector3 min;
    Vector3 max;
    bool RayIntersects(const Vector3& rayOrigin, const Vector3& rayDir) {
        float tmin = FLT_MIN;
        float tmax = FLT_MAX;
        const float* rayOriginArr = &rayOrigin.x;
        const float* rayDirArr = &rayDir.x;
        const float* minArr = &min.x;
        const float* maxArr = &max.x;
        for (int i = 0; i < 3; ++i) {
            float invDir = 1.0f / rayDirArr[i];
            float t0 = (minArr[i] - rayOriginArr[i]) * invDir;
            float t1 = (maxArr[i] - rayOriginArr[i]) * invDir;
            if (invDir < 0.0f)
                std::swap(t0, t1);
            tmin = std::max(tmin, t0);
            tmax = std::min(tmax, t1);
        }
        return tmax >= tmin && tmax >= 0;
    }
};

#include <algorithm>
struct TriangleCombined {
    Vector3 v0, v1, v2;
    int materialIndex = -1;
    TriangleCombined() = default;
    TriangleCombined(const Vector3& v0_, const Vector3& v1_, const Vector3& v2_)
        : v0(v0_), v1(v1_), v2(v2_) {
    }
    TriangleCombined(const Vector3& v0_, const Vector3& v1_, const Vector3& v2_, int material)
        : v0(v0_), v1(v1_), v2(v2_), materialIndex(material) {
    }
    AABB ComputeAABB() const {
        Vector3 min_point, max_point;
        min_point.x = std::min({ v0.x, v1.x, v2.x });
        min_point.y = std::min({ v0.y, v1.y, v2.y });
        min_point.z = std::min({ v0.z, v1.z, v2.z });
        max_point.x = std::max({ v0.x, v1.x, v2.x });
        max_point.y = std::max({ v0.y, v1.y, v2.y });
        max_point.z = std::max({ v0.z, v1.z, v2.z });
        return { min_point, max_point };
    }
};


struct BVHNode {
  AABB bounds;
  BVHNode* left;
  BVHNode* right;
  std::vector<TriangleCombined> triangles;

  bool is_leaf() {
    return left == nullptr && right == nullptr;
  }
};
