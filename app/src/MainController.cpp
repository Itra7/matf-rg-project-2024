//
// Created by filip on 1/22/25.
//

#include "Utils.hpp"
#include "MainController.hpp"
#include <math.h>
#include <iostream>

#include "GUIController.hpp"
#include "../../engine/libs/glfw/include/GLFW/glfw3.h"

#define POINT_LIGHT_NUM 2
#define RADIUS 3
#define POSITIONLAMP1 VECTOR3(2.0f, -5.0f, 1.0f)
#define POSITIONLAMP2 VECTOR3(-0.5f, -5.0f, 1.0f)
#define TREE1POSITION VECTOR3(0.0f, -4.6f, 10.0f)

VECTOR3 SUN_POSITION = VECTOR3(0.0f,20.f,-15.0f);
VECTOR3 SUN_DIFFUSE = VECTOR3(1.0f, 0.5f, 0.5f);
VECTOR3 SUN_AMBIENT = VECTOR3(0.2f,0.2f, 0.2f);
VECTOR3 UFO_POSITON = VECTOR3(0.0f, 0.0f, -3.0f);
VECTOR3 HOUSE_POSITION = VECTOR3(0.0f, -5.05f, -2.0f);
VECTOR3 POSITIONLAMPS[2] = {POSITIONLAMP1, POSITIONLAMP2};

void set_shader(engine::resources::Shader *shader, engine::graphics::GraphicsController* graphics, const char* IDENTIFIER);

namespace app{

    class MainPlatformEventObserver : public engine::platform::PlatformEventObserver {
    public:
        void on_mouse_move(engine::platform::MousePosition position) override;
    };
    void MainPlatformEventObserver::on_mouse_move(engine::platform::MousePosition position) {
        auto gui_controller = engine::core::Controller::get<GUIController>();
        if (!gui_controller->is_enabled()) {
            auto camera = engine::core::Controller::get<engine::graphics::GraphicsController>()->camera();
            camera->rotate_camera(position.dx, position.dy);
        }
    }

    void MainController::initialize() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        platform->register_platform_event_observer(std::make_unique<MainPlatformEventObserver>());
        engine::graphics::OpenGL::enable_depth_testing();
    }

    bool MainController::loop() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        auto dt = platform->dt();
        if (platform->key(engine::platform::KeyId::KEY_ESCAPE).is_down()) {
            return false;
        }
        return true;
    }

    void MainController::draw_ground() {
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
        engine::resources::Model *ground = resources->model("ground");
        engine::resources::Shader *shader = resources->shader("basic");
        shader->use();
        shader->set_mat4("projection", graphics->projection_matrix());
        shader->set_mat4("view", graphics->camera()->view_matrix());
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -5.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.3f));
        shader->set_mat4("model", model);
        ground->draw(shader);
    }

    void MainController::draw_house() {
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
        engine::resources::Model *kuca = resources->model("kuca2");
        engine::resources::Shader *shader = resources->shader("kuca2");
        shader->use();
        shader->set_mat4("projection", graphics->projection_matrix());
        shader->set_mat4("view", graphics->camera()->view_matrix());
        shader->set_vec3("dirLight.direction", SUN_POSITION);
        shader->set_float("material.shininess", 32);
        shader->set_vec3("dirLight.ambient", SUN_AMBIENT);
        shader->set_vec3("dirLight.diffuse", SUN_DIFFUSE);
        shader->set_vec3("dirLight.specular", VECTOR3(1.0f, 1.0f, 1.0f));
        for (int i = 0; i < POINT_LIGHT_NUM; i++) {
            std::string s = "pointLights[" + std::to_string(i) + "].";
            shader->set_vec3(s + "position", POSITIONLAMPS[i]);
            shader->set_float(s + "constant", 1.0f);
            shader->set_float(s + "linear", 0.09f);
            shader->set_float(s + "quadratic", 0.032f);
            shader->set_vec3(s + "ambient", VECTOR3(0.1f,0.1f,0.1f));
            shader->set_vec3(s + "diffuse", VECTOR3(0.3f, 0.3f, 0.3f));
            shader->set_vec3(s + "specular", VECTOR3(1.0f, 1.0f, 1.0f));
        }
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, HOUSE_POSITION);
        model = glm::scale(model, glm::vec3(2.6f));
        shader->set_mat4("model", model);
        kuca->draw(shader);
    }

    void MainController::draw_tree1() {
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
        engine::resources::Model *tree = resources->model("tree1");
        engine::resources::Shader *shader = resources->shader("tree1");
        shader->use();
        shader->set_mat4("projection", graphics->projection_matrix());
        shader->set_mat4("view", graphics->camera()->view_matrix());
        shader->set_vec3("dirLight.direction", SUN_POSITION);
        shader->set_float("material.shininess", 32);
        shader->set_vec3("dirLight.ambient", SUN_AMBIENT);
        shader->set_vec3("dirLight.diffuse", SUN_DIFFUSE);
        shader->set_vec3("dirLight.specular", VECTOR3(1.0f, 1.0f, 1.0f));
        for (int i = 0; i < POINT_LIGHT_NUM; i++) {
            std::string s = "pointLights[" + std::to_string(i) + "].";
            shader->set_vec3(s + "position", POSITIONLAMPS[i]);
            shader->set_float(s + "constant", 1.0f);
            shader->set_float(s + "linear", 0.09f);
            shader->set_float(s + "quadratic", 0.032f);
            shader->set_vec3(s + "ambient", VECTOR3(0.1f,0.1f,0.1f));
            shader->set_vec3(s + "diffuse", VECTOR3(0.3f, 0.3f, 0.3f));
            shader->set_vec3(s + "specular", VECTOR3(1.0f, 1.0f, 1.0f));
        }


        const long instanced = 60;

        if (tree1_models.empty()) {
            float y = TREE1POSITION.y;
            for (int i = 0; i < 30; i++) {
                float angle = (2*M_PI*i)/(instanced/2);
                float x = 10.0 * sin(angle);
                float z = 10.0 * cos(angle);
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(x, y, z));
                model = glm::scale(model, glm::vec3(0.6f));
                tree1_models.push_back(model);
            }
            for (int i = 0; i < 30; i++) {
                float angle = (2*M_PI*i)/(instanced/2);
                float x = 17.5 * sin(angle);
                float z = 17.5 * cos(angle);
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(x, y, z));
                model = glm::scale(model, glm::vec3(0.9f));
                tree1_models.push_back(model);
            }
        }

        tree->initialize_instances(tree1_models);
        tree->draw_instanced(shader, instanced);
    }

    void MainController::draw_tree2() {
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
        engine::resources::Model *tree = resources->model("tree2");
        engine::resources::Shader *shader = resources->shader("tree2");
        shader->use();
        shader->set_mat4("projection", graphics->projection_matrix());
        shader->set_mat4("view", graphics->camera()->view_matrix());
        shader->set_vec3("dirLight.direction", SUN_POSITION);
        shader->set_float("material.shininess", 32);
        shader->set_vec3("dirLight.ambient", SUN_AMBIENT);
        shader->set_vec3("dirLight.diffuse", SUN_DIFFUSE);
        shader->set_vec3("dirLight.specular", VECTOR3(1.0f, 1.0f, 1.0f));
        for (int i = 0; i < POINT_LIGHT_NUM; i++) {
            std::string s = "pointLights[" + std::to_string(i) + "].";
            shader->set_vec3(s + "position", POSITIONLAMPS[i]);
            shader->set_float(s + "constant", 1.0f);
            shader->set_float(s + "linear", 0.09f);
            shader->set_float(s + "quadratic", 0.032f);
            shader->set_vec3(s + "ambient", VECTOR3(0.1f,0.1f,0.1f));
            shader->set_vec3(s + "diffuse", VECTOR3(0.3f, 0.3f, 0.3f));
            shader->set_vec3(s + "specular", VECTOR3(1.0f, 1.0f, 1.0f));
        }
        const long instanced = 60;

        if (tree2_models.empty()) {
            float y = TREE1POSITION.y;
            for (int i = 0; i < 30; i++) {
                float angle = (2*M_PI*i)/(instanced/2);
                float x = 14.0 * sin(angle);
                float z = 14.0 * cos(angle);
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(x, y, z));
                model = glm::scale(model, glm::vec3(1.6f));
                tree2_models.push_back(model);
            }
            for (int i = 0; i < 30; i++) {
                float angle = (2*M_PI*i)/(instanced/2);
                float x = 21.5 * sin(angle);
                float z = 21.5 * cos(angle);
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(x, y, z));
                model = glm::scale(model, glm::vec3(1.8f));
                tree2_models.push_back(model);
            }
        }
        tree->initialize_instances(tree2_models);
        tree->draw_instanced(shader, instanced);
    }

    void MainController::draw_sun() {
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
        engine::resources::Model *sun = resources->model("Sun");
        engine::resources::Shader *shader = resources->shader("basic");
        shader->use();
        shader->set_mat4("projection", graphics->projection_matrix());
        shader->set_mat4("view", graphics->camera()->view_matrix());
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, SUN_POSITION);
        model = glm::scale(model, VECTOR3(0.1f,0.1f,0.1f));
        shader->set_mat4("model", model);

        sun->draw(shader);
    }

    void MainController::draw_lamp() {
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
        engine::resources::Model *lamp = resources->model("lamp");
        engine::resources::Shader *shader = resources->shader("lamp");
        shader->use();
        shader->set_mat4("projection", graphics->projection_matrix());
        shader->set_mat4("view", graphics->camera()->view_matrix());
        shader->set_vec3("dirLight.direction", SUN_POSITION);
        shader->set_float("material.shininess", 32);
        shader->set_vec3("dirLight.ambient", SUN_AMBIENT);
        shader->set_vec3("dirLight.diffuse", SUN_DIFFUSE);
        shader->set_vec3("dirLight.specular", VECTOR3(1.0f, 1.0f, 1.0f));
        for (int i = 0; i < POINT_LIGHT_NUM; i++) {
            std::string s = "pointLights[" + std::to_string(i) + "].";
            shader->set_vec3(s + "position", POSITIONLAMPS[i]);
            shader->set_float(s + "constant", 1.0f);
            shader->set_float(s + "linear", 0.09f);
            shader->set_float(s + "quadratic", 0.032f);
            shader->set_vec3(s + "ambient", VECTOR3(0.1f,0.1f,0.1f));
            shader->set_vec3(s + "diffuse", VECTOR3(0.3f, 0.3f, 0.3f));
            shader->set_vec3(s + "specular", VECTOR3(1.0f, 1.0f, 1.0f));
        }


        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, POSITIONLAMP1);
        model = glm::scale(model, VECTOR3(0.1f,0.1f,0.1f));
        shader->set_mat4("model", model);

        lamp->draw(shader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, POSITIONLAMP2);
        model = glm::scale(model, VECTOR3(0.1f,0.1f,0.1f));
        shader->set_mat4("model", model);
        lamp->draw(shader);
    }

    void MainController::draw_ufo() {
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
        engine::resources::Model *ufo = resources->model("ufo");
        engine::resources::Shader *shader = resources->shader("ufo");
        shader->use();
        shader->set_mat4("projection", graphics->projection_matrix());
        shader->set_mat4("view", graphics->camera()->view_matrix());
        shader->set_vec3("dirLight.direction", SUN_POSITION);
        shader->set_float("material.shininess", 32);
        shader->set_vec3("dirLight.ambient", SUN_AMBIENT);
        shader->set_vec3("dirLight.diffuse", SUN_DIFFUSE);
        shader->set_vec3("dirLight.specular", VECTOR3(1.0f, 1.0f, 1.0f));
        for (int i = 0; i < POINT_LIGHT_NUM; i++) {
            std::string s = "pointLights[" + std::to_string(i) + "].";
            shader->set_vec3(s + "position", POSITIONLAMPS[i]);
            shader->set_float(s + "constant", 1.0f);
            shader->set_float(s + "linear", 0.09f);
            shader->set_float(s + "quadratic", 0.032f);
            shader->set_vec3(s + "ambient", VECTOR3(0.1f,0.1f,0.1f));
            shader->set_vec3(s + "diffuse", VECTOR3(0.3f, 0.3f, 0.3f));
            shader->set_vec3(s + "specular", VECTOR3(1.0f, 1.0f, 1.0f));
        }
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, UFO_POSITON);
        model = glm::scale(model, VECTOR3(0.2f,0.2f,0.2f));
        shader->set_mat4("model", model);


        ufo->draw(shader);
    }

    void MainController::update_camera() {
        auto gui_controller = engine::core::Controller::get<GUIController>();
        if (gui_controller->is_enabled()) {
            return;
        }
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
        auto camera = graphics->camera();
        float dt = platform->dt();
        if (platform->key(engine::platform::KeyId::KEY_W).is_down()) {
            camera->move_camera(engine::graphics::Camera::Movement::FORWARD, dt);
        }
        if (platform->key(engine::platform::KeyId::KEY_S).is_down()) {
            camera->move_camera(engine::graphics::Camera::Movement::BACKWARD, dt);
        }
        if (platform->key(engine::platform::KeyId::KEY_A).is_down()) {
            camera->move_camera(engine::graphics::Camera::Movement::LEFT, dt);
        }
        if (platform->key(engine::platform::KeyId::KEY_D).is_down()) {
            camera->move_camera(engine::graphics::Camera::Movement::RIGHT, dt);
        }
    }

    void MainController::update() {
        static int lock = 0;
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        update_camera();
        if (platform->key(engine::platform::KeyId::KEY_G).is_down()) {
            SUN_POSITION = VECTOR3((float)cos(glfwGetTime())*10.0f, 10.0f, 3.0+(float)sin(glfwGetTime())*7.0f);
        }
        if (platform->key(engine::platform::KeyId::KEY_C).is_down()) {
            if (lock <= 7) {
                SUN_AMBIENT += VECTOR3(0.0f, 0.09f, 0.001f);
                SUN_DIFFUSE += VECTOR3(0.0f,0.09f,0.001f);
                lock++;
            }
        }
        if (platform->key(engine::platform::KeyId::KEY_T).state() == engine::platform::Key::State::JustPressed){
            pressed = true;
            start_time = std::chrono::high_resolution_clock::now();
        }

        if (pressed){
            current_time = std::chrono::high_resolution_clock::now();
            auto elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time).count();
            if (elapsed_time >= 2 && elapsed_time <= 3) {
                UFO_POSITON += VECTOR3(0.0f, 0.15f, 0.0f);
                HOUSE_POSITION += VECTOR3(0.0f, 0.15f, 0.0f);
            }

            if (elapsed_time >= 6) {
                UFO_POSITON += VECTOR3(0.0f, 10000.15f, 0.0f);
                HOUSE_POSITION += VECTOR3(0.0f, 10000.15f, 0.0f);
                pressed = false;
            }
        }
    }
    void MainController::begin_draw() {
        engine::graphics::OpenGL::clear_buffers();
    }
    void MainController::draw_skyboxes() {
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto skybox    = resources->skybox("mountain_skybox");
        auto shader    = resources->shader("skybox");
        auto graphics  = engine::core::Controller::get<engine::graphics::GraphicsController>();
        graphics->draw_skybox(shader, skybox);
    }

    void MainController::draw() {
        draw_ground();
        draw_tree1();
        draw_tree2();
        draw_sun();
        draw_house();
        draw_ufo();
        draw_lamp();
    }
    void MainController::end_draw() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        platform->swap_buffers();
    }

} // namespace app
