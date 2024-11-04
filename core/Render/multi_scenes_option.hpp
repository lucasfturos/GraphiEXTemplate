#pragma once

#include <memory>
#include <vector>

class Scene;

class MultiScenesOption {
  private:
    std::vector<std::shared_ptr<Scene>> m_Scenes;
    std::size_t m_CurrentSceneIndex = 0;

  public:
    void addScene(std::shared_ptr<Scene> scene) { m_Scenes.push_back(scene); }

    std::shared_ptr<Scene> getCurrentScene() const {
        if (m_Scenes.empty())
            return nullptr;
        return m_Scenes[m_CurrentSceneIndex];
    }

    void setCurrentSceneIndex(std::size_t index) {
        if (index < m_Scenes.size()) {
            m_CurrentSceneIndex = index;
        }
    }

    std::size_t getCurrentSceneIndex() const { return m_CurrentSceneIndex; }

    std::size_t getScenesCount() const { return m_Scenes.size(); }

    std::vector<std::string> getSceneNames() const {
        std::vector<std::string> names;
        for (auto i = 0U; i < m_Scenes.size(); ++i) {
            names.push_back("Scene " + std::to_string(i + 1));
        }
        return names;
    }
};
