
#include <engine/resources/Model.hpp>
#include <engine/resources/Shader.hpp>

namespace engine::resources {

void Model::draw(const Shader *shader) {
    shader->use();
    for (auto &mesh: m_meshes) {
        mesh.draw(shader);
    }
}

    void Model::initialize_instances(const std::vector<glm::mat4> &instances) {
        for (auto &mesh: m_meshes) {
            mesh.initialization_mesh_instances(instances);
        }
    }

    void Model::draw_instanced(const Shader *shader, const int size) {
        shader->use();
        for (auto &mesh : m_meshes) {
            mesh.draw_instanced(shader, size);
        }
    }

    void Model::destroy() {
        for (auto &mesh: m_meshes) {
            mesh.destroy();
        }
    }
}
