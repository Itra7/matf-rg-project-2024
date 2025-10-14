//
// Created by filip on 1/22/25.
//

#ifndef MAINCONTROLLER_HPP
#define MAINCONTROLLER_HPP
#include <chrono>
#include <engine/core/Controller.hpp>
#include "Utils.hpp"
#include "../../engine/libs/assimp/contrib/rapidjson/include/rapidjson/document.h"

#define POSITIONLAMP1 VECTOR3(2.0f, -5.0f, 1.0f)
#define POSITIONLAMP2 VECTOR3(-0.5f, -5.0f, 1.0f)
#define TREE1POSITION VECTOR3(0.0f, -4.6f, 10.0f)


namespace app {

class MainController : public engine::core::Controller {
    void initialize() override;
    bool loop() override;
    void draw_backpack();
    void update_camera();
    void update() override;
    void begin_draw() override;
    void draw_skyboxes();

    void draw_ground();

    void draw_house();

    void draw_tree1();

    void draw_tree2();

    void draw_sun();

    void draw_lamp();

    void draw() override;
    void end_draw() override;
    void draw_ufo();

public:
    std::string_view name() const override {
        return "app::MainController";
    }
    std::vector<glm::mat4> tree1_models;
    std::vector<glm::mat4> tree2_models;
    bool pressed = false;
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
    std::chrono::time_point<std::chrono::high_resolution_clock> current_time;
};

}



#endif //MAINCONTROLLER_HPP
