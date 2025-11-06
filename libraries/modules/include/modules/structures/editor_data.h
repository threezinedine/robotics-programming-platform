#pragma once
#include "core/core.h"
#include "utils.h"

namespace rpp
{
    /**
     * System data for editor application.
     */
    struct RPP_JSON EditorDataDescription
    {
        Array<String> recentProjects RPP_JSON_KEY("recentProjects"); ///< List of recent projects opened in the editor.
    };

    /**
     * Main object which is managed by the editor application.
     */
    class EditorData
    {
        STRUCTURE_SAVE_LOAD_DEFINE(EditorData);

    public:
        /**
         * @brief Get the list of recent projects opened in the editor.
         * @return Array of recent project file paths.
         */
        inline const Array<String> &GetRecentProjects() const { return m_recentProjects; }

    public:
        /**
         * Reorder if the project path already exists.
         *
         * @param projectFilePath The project file path to add to recent projects.
         */
        void AddRecentProject(const String &projectFilePath);

    private:
        Array<String> m_recentProjects; ///< List of recent projects opened in the editor.
    };
} // namespace rpp
