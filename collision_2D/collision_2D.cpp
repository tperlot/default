#include "collision_2D.hpp"

//internal

//external
#include <fmt/core.h>
#include <fmt/color.h>

//standard

namespace Collision {
    Collision2D::Collision2D() {

    }  //constructor

    //TASK NOTE default destructor

    glm::vec3 Collision2D::tripleCross(glm::vec3& singleOp, glm::vec3& doubleOp, glm::vec3& tripleOp) {
        glm::vec3 crossProduct {};

        crossProduct = glm::cross(glm::cross(singleOp, doubleOp), tripleOp);

        return crossProduct;
    }  //tripleCross

    //NOTE Technically this is called the "Support Function" in GJK, in case you need to look it up later.  There is a speed up for this but I don't know what it is.
    glm::vec3 Collision2D::mostExtreme(std::span<glm::vec3> candidates, glm::vec3& direction) {
        glm::vec3 extremePoint = {0, 0, 0};  //This is initialized to 0 at all points in case no candidate is found
        double greatestDistance = -std::numeric_limits<double>::infinity();
        double currDistance = 0;

        // fmt::print(fg(fmt::color::steel_blue), "\t\t\t{}, {}, {} - Direction\n", direction.x, direction.y, direction.z);

        for (glm::vec3 candidate : candidates) {
            currDistance = glm::dot(candidate, direction);

            // fmt::print(fg(fmt::color::steel_blue), "\n\tDistance of {} with greatest distance {}\n", currDistance, greatestDistance);
            // fmt::print(fg(fmt::color::steel_blue), "\t\t\tCandidate - {}, {}, {} \t\n", candidate.x, candidate.y, candidate.z);

            if (currDistance > greatestDistance) {
                greatestDistance = currDistance;
                extremePoint = candidate;
            }
        }

        // fmt::print(fg(fmt::color::aqua), "\n{}, {}, {} - Extreme point in candidate sized {}\n", extremePoint.x, extremePoint.y, extremePoint.z, candidates.size());

        return extremePoint;
    }  //mostExtreme

    glm::vec3 Collision2D::perpendiculo(glm::vec3& coordinates) {
        glm::vec3 inverto {};

        inverto.x = coordinates.x;
        inverto.y = -coordinates.y;
        inverto.z = coordinates.z;

        return inverto;
    }

    glm::vec3 Collision2D::invertCoords(glm::vec3& invertThis) {
        glm::vec3 inverted {};

        inverted.x = -invertThis.x;
        inverted.y = -invertThis.y;
        inverted.z = invertThis.z;  //WARNING This must be changed for 3D applications

        return inverted;
    }  //invertCoords

    //TODO WARNING NOTE This needs to be reevaluated as I didn't have a full brain writing it.
    bool Collision2D::hasCollided(std::span<glm::vec3> object1, glm::vec3& object1Mid, std::span<glm::vec3> object2, glm::vec3& object2Mid) {
        std::array<glm::vec3, 3> minkowski;
        object1Mid.z = 0;
        object2Mid.z = 0;
        glm::vec3 direction = glm::normalize(object2Mid - object1Mid);

        // fmt::println("Direction = {}, {}, {}", direction.x, direction.y, direction.z);

        //handle edge cases so that there's always a direction
        if ((direction.x == 0) && (direction.y == 0)) {
            direction.x = 1.0f;
        }

        glm::vec3 negDirection = invertCoords(direction);

        // fmt::println("Reverse direction = {}, {}, {}", negDirection.x, negDirection.y, negDirection.z);

    //Point 1
        minkowski[0] = mostExtreme(object1, direction) - mostExtreme(object2, negDirection);

        // if (glm::dot(minkowski[0], direction) >= 0) {
        //     fmt::println("This exitted with dot product equal to {}", glm::dot(minkowski[0], direction));
        //     return false;  //There are no points passing the origin
        // }

        direction = negDirection;  //Flip directions since initially this was direction FROM origin to make it direction TO origin to attempt to enclose it.
        negDirection = invertCoords(direction);

        // fmt::println("Direction after first point = {}, {}, {}", direction.x, direction.y, direction.z);

    //points for evaluation
        glm::vec3 origin = {0, 0, 0};
        glm::vec3 alphaPoint = {0, 0, 0};
        glm::vec3 betaPoint = {0, 0, 0};
        glm::vec3 thetaPoint = {0, 0, 0};

    //vectors for evaluation
        glm::vec3 alphaO = {0, 0, 0};  //from newest point to origin
        glm::vec3 alphaBeta = {0, 0, 0};
        glm::vec3 alphaTheta = {0, 0, 0};

        glm::vec3 alphaBetaPerpend = {0, 0, 0};
        glm::vec3 alphaThetaPerpend = {0, 0, 0};

    //Point 2
        minkowski[1] = mostExtreme(object1, direction) - mostExtreme(object2, negDirection);  //first iteration is [1]
        alphaPoint = minkowski[1];

        if (glm::dot(alphaPoint, direction) <= 0) {
            return false;
        }

        alphaO =  origin - alphaPoint;  //from the newest point to the origin just invert coordinates

        // fmt::println("alphaOrigin vector = {}, {}, {}", alphaO.x, alphaO.y, alphaO.z);

        //This block of code becomes if (minkIndex < 2) if you decide to put it in the while loop
        betaPoint = minkowski[0];
        alphaBeta = betaPoint - alphaPoint;
        direction = tripleCross(alphaBeta, alphaO, alphaBeta);  //WARNING NOTE Not convinced these aren't swapped.  When I did try swapping them the program crashed, but detection not working as is.
        if (((direction.x * direction.x) + (direction.y * direction.y)) == 0) {
            direction = perpendiculo(alphaBeta);  //NOTE TODO Consider removing the function since it's not used anywhere but here.
        }
        // fmt::println("Direction after second point = {}, {}, {}", direction.x, direction.y, direction.z);


    //Point 3 and reassignment
        while (true) {

            minkowski[2] = mostExtreme(object1, direction) - mostExtreme(object2, negDirection);
            alphaPoint = minkowski[2];

            if (glm::dot(alphaPoint, direction) <= 0) {
                return false;
            }

            alphaO = origin - alphaPoint;  //from the newest point to the origin just invert coordinates

            // fmt::println("alphaOrigin vector = {}, {}, {}", alphaO.x, alphaO.y, alphaO.z);

            betaPoint = minkowski[1];
            thetaPoint = minkowski[0];

            alphaBeta = betaPoint - alphaPoint;
            alphaTheta = thetaPoint - alphaPoint;

            alphaThetaPerpend = tripleCross(alphaBeta, alphaTheta, alphaTheta);  //Voronoi region

            // fmt::println("alphaThetaPerpend has values of {}, {}, {}", alphaThetaPerpend.x, alphaThetaPerpend.y, alphaThetaPerpend.z);

            if (glm::dot(alphaThetaPerpend, alphaO) >= 0) {
                direction = alphaThetaPerpend;
                negDirection = invertCoords(direction);

                // fmt::println("Direction after AC perp = {}, {}, {}", direction.x, direction.y, direction.z);
            } else {
                alphaBetaPerpend = tripleCross(alphaTheta, alphaBeta, alphaBeta);  //Voronoi region

                if (glm::dot(alphaBetaPerpend, alphaO) < 0) {
                    return true;
                }

                minkowski[0] = minkowski[1];  //swap out theta (oldest point)
                direction = alphaBetaPerpend;
                negDirection = invertCoords(direction);

                // fmt::println("Direction after AB perp = {}, {}, {}", direction.x, direction.y, direction.z);
            }

            minkowski[1] = minkowski[2];  //swap out beta (second point)
        }  //while true

        return false;
    }  //hasCollided


}  //namespace
