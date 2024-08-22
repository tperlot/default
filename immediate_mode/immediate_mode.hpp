#pragma once

//internal
#include "interface/collision_2D.hpp"
#include "interface/vkpixel_convert_util.hpp"
#include "interface/window.hpp"

//external
#include <glm/glm.hpp>

//standard
#include <array>
#include <span>  //C++20 type for passing any variant of array.  It is a reference by design.

namespace Collision {
    class ImmediateMode {
    public:
        ImmediateMode(Converter &converter, /*your::Window &window*/);
        //TASK NOTE default destructor

        std::array<glm::vec3, 3> mouseBounds(glm::vec3& mousePos);

        bool boundBoxCheck(size_t UI_ID, std::span<glm::vec3> hitBoxVerts, glm::vec3& hitboxMidPoint, std::span<glm::vec3> mouseBox, glm::vec3& mousePos);
        bool allPointsCheck(size_t UI_ID, std::span<glm::vec3> objectVerts, glm::vec3& objectMidPoint, std::span<glm::vec3> mouseBox, glm::vec3& mousePos);

    private:
        int acceptState = -1;
        size_t CircleBack_ID = 0;
        size_t Active_ID = 0;
        size_t Hot_ID = 0;

        Collision2D collider;
        Converter &convert;
        //WARNING TODO Add your window here!!

    };

}  //namespace
