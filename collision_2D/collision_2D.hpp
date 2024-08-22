#pragma once

//internal

//external
#include <glm/glm.hpp>

//standard
#include <span> //C++20 type for passing any variant of array.  It is a reference by design.

namespace Collision {
    class Collision2D {
    public:
        Collision2D();
        //TASK NOTE default destructor

        bool hasCollided(std::span<glm::vec3> object1, glm::vec3& object1mid, std::span<glm::vec3> object2, glm::vec3& object2mid);  //NOTE GJK can only be used on convex objects

    private:
        glm::vec3 tripleCross(glm::vec3& singleOp, glm::vec3& doubleOp, glm::vec3& tripleOP);  //NOTE glm::vec3 are 12 bytes, so it is better to take by ref
        glm::vec3 mostExtreme(std::span<glm::vec3> candidates, glm::vec3& referencePoint);
        glm::vec3 perpendiculo(glm::vec3& coordinates);
        glm::vec3 invertCoords(glm::vec3& invertThis);

    };  //class collision2D

}  //namespace
