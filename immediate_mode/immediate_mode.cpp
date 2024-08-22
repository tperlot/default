#include "immediate_mode.hpp"

//internal
#include "collision_2D.hpp"
#include "vkpixel_convert_util.hpp"


//external
#include <fmt/core.h>
#include <fmt/color.h>
#include <GLFW/glfw3.h>
#include <glm/fwd.hpp>

//standard

namespace Collision {
    ImmediateMode::ImmediateMode(Converter &converter, /*WARNING TODO your::Window &window*/) : convert{converter}, yourWindow{window} {

    }  //constructor

    //TASK NOTE default destructor

    std::array<glm::vec3, 3> ImmediateMode::mouseBounds(glm::vec3& mousePos) {

        float mouseXBound = convert.toVulkanCoord(((yourWindow.getExtent().width * 0.5) + 1), yourWindow.getExtent().width);
        float mouseYBound = convert.toVulkanCoord(((yourWindow.getExtent().height * 0.5) + 1), yourWindow.getExtent().height);

        std::array<glm::vec3, 3> mouseBounds;  //Will try triangle first.  Use square if that is too inaccurate.
        mouseBounds[0] = {mousePos.x, mousePos.y - mouseYBound, 0.0f};  //Top point
        mouseBounds[1] = {mousePos.x + mouseXBound, mousePos.y + mouseYBound, 0.0f};  //Bot right
        mouseBounds[2] = {mousePos.x - mouseXBound, mousePos.y + mouseYBound, 0.0f};  //Bot left

        return mouseBounds;
    }  //mouseBounds

    //NOTE This is different from the button hit check!  This is meant for bounding boxes with lower points to calculate.
    bool ImmediateMode::boundBoxCheck(size_t UI_ID, std::span<glm::vec3> boundBoxVerts, glm::vec3& boundBoxMidPoint, std::span<glm::vec3> mouseBox, glm::vec3& mousePos) {

        if (collider.hasCollided(boundBoxVerts, boundBoxMidPoint, mouseBox, mousePos)) {
            return true;
        } else if (CircleBack_ID == UI_ID) {  //This is required here in case a UI element was hot, had an acceptState queued, and passed where the bounding box lined up with the object.
            fmt::print(fg(fmt::color::yellow), "Button ID # {} has heat cycled and reset!\n", UI_ID);
            acceptState = -1;

            Active_ID = 0;
            CircleBack_ID = 0;
            Hot_ID = 0;
        }  //Collision check

        return false;
    }  //hitBoxCheck


    //NOTE This is different from the bounding box check!  This is meant for complex objects with a large number of points.
    bool ImmediateMode::allPointsCheck(size_t UI_ID, std::span<glm::vec3> objectVerts, glm::vec3& objectMidPoint, std::span<glm::vec3> mouseBox, glm::vec3& mousePos) {
        acceptState = glfwGetMouseButton(yourWindow.getGLFWwindow(), GLFW_MOUSE_BUTTON_LEFT);

    //TASK  Mouse collision check
        if (collider.hasCollided(objectVerts, objectMidPoint, mouseBox, mousePos)) {
            if (Active_ID == 0) {
                Hot_ID = UI_ID;
                CircleBack_ID = UI_ID;
            }
        } else if (CircleBack_ID == UI_ID) {  //This is required here in case a UI element was hot, had an acceptState queued, and passed inside the bounding box but outside of object.
            fmt::print(fg(fmt::color::yellow), "Button ID # {} has cycled and reset!\n", UI_ID);
            acceptState = -1;

            Active_ID = 0;
            CircleBack_ID = 0;
            Hot_ID = 0;
        }  //Collision check

    //TASK  Mouse active/hot check
        if (Hot_ID == UI_ID) {
            if (acceptState == GLFW_PRESS) {  //Abstraction could be something like acceptButton::pressed (LMB, X, A, Touch)
                fmt::print(fg(fmt::color::yellow_green), "Button ID # {} has been clicked!\n", UI_ID);
                Active_ID = UI_ID;
            }
        }

        if (Active_ID == UI_ID) {
            if (acceptState == GLFW_RELEASE) {  //Abstraction could be something like acceptButton::released (LMB, X, A, Touch)
                if (Hot_ID == UI_ID) {
                    fmt::print(fg(fmt::color::green), "Button ID # {} has been activated!\n", UI_ID);
                    Active_ID = 0;
                    return true;
                }
            }
        }

        return false;
    }  //bool allPointsCheck
}  //namespace
