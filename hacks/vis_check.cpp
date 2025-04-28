#include "vis_check.hpp"

#include <vector>
#include <functional>

#include "../vector.hpp"
#include "../math.hpp"

void CollectIntersections(const Vector3& rayOrigin, const Vector3& rayDir,
    float maxDistance, std::vector<std::pair<float, int>>& intersections) {
  std::function<void(const BVHNode*)> traverse = [&](const BVHNode* node) {
        if (!node || !node->bounds.RayIntersects(rayOrigin, rayDir))
            return;
        if (node->IsLeaf()) {
            for (const auto& tri : node->triangles) {
                float t;
                if (RayIntersectsTriangle(rayOrigin, rayDir, tri, t)) {
                    if (t > 1e-7f && t < maxDistance) {
                        intersections.push_back(std::make_pair(t, tri.materialIndex));
                    }
                }
            }
        }
        else {
            if (node->left)
                traverse(node->left.get());
            if (node->right)
                traverse(node->right.get());
        }
        };
    traverse(bvhRoot.get());
}

std::vector<std::tuple<float, int, float, int>> IsPointVisible(const Vector3& point1, const Vector3& point2) {
    Vector3 rayDir = { point2.x - point1.x, point2.y - point1.y, point2.z - point1.z };
    float totalDistance = sqrt(rayDir.dot(rayDir));
    rayDir = { rayDir.x / totalDistance, rayDir.y / totalDistance, rayDir.z / totalDistance };
    std::vector<std::pair<float, int>> hits;
    CollectIntersections(point1, rayDir, totalDistance, hits);
    std::sort(hits.begin(), hits.end(), [](const std::pair<float, int>& a, const std::pair<float, int>& b) {
        return a.first < b.first;
        });

    std::vector<std::tuple<float, int, float, int>> result;
    float prevExitT = 0.0f;
    for (size_t i = 0; i + 1 < hits.size(); i += 2) {
        float entryT = hits[i].first;
        int entryMat = hits[i].second;
        float exitT = hits[i + 1].first;
        int exitMat = hits[i + 1].second;
        float gap = entryT - prevExitT;
        float thickness = exitT - entryT;
        result.push_back(std::make_tuple(gap, entryMat, thickness, exitMat));
        prevExitT = exitT;
    }
    return result;
}
