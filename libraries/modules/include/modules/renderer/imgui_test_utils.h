#pragma once
#include "core/core.h"
#include "imgui.h"

namespace rpp
{
    class Renderer;
    class ImGuiImpl;

    /**
     * Needed interfaces for ImGui end-to-end testing. This class will be used for e2e tests only (be binded to python).
     */
    class RPP_E2E_BINDING ImGuiTestUtils
    {
    public:
        friend class Renderer;
        friend class ImGuiImpl; // to access global data for searching the items

    public:
        /**
         * Needed information for interacting with the ImGui item.
         */
        struct ItemData
        {
            /// @brief The name of the item.
            String label;
            /// @brief The position of the item.
            ImVec2 position;
        };

        /**
         * Indicates how the cursor should be interacted with the item.
         */
        enum class ImGuiItemAction : u8
        {
            IMGUI_ACTION_MOVE,  ///< Move action on the item.
            IMGUI_ACTION_CLICK, ///< Click action on the item.
            IMGUI_ACTION_COUNT,
        };

        struct GlobalData
        {
            /// @brief The current search label. If empty, no search is being performed. If matched with an item
            ///        the item will be selected and the label cleared.
            String label;

            /// @brief The action to perform on the item when found. If no action, set to IMGUI_ACTION_COUNT.
            ImGuiItemAction action = ImGuiItemAction::IMGUI_ACTION_COUNT;
        };

    public:
        /**
         * Starting the system
         * @note must be called once after Renderer::Initialize and before any ImGui related features.
         */
        static void Initialize();

        /**
         * Shutdown the system
         * @note must be called once before Renderer::Shutdown and after all ImGui related features.
         */
        static void Shutdown();

    public:
        /**
         * Used for grabbing the cursor to an item with a certain name.
         *
         * @param label The label of the item to move to. If the item is not found, nothing happens.
         *      If multiple items with the same label exist, the first one found is used.
         */
        static void MoveToItem(const String &label) RPP_E2E_BINDING;

    private:
        /**
         * Used each frame to update the ImGui testing utilities, performing actions on items if needed.
         *
         * @param deltaTime The time elapsed since the last update call.
         */
        static void Update(f32 deltaTime);

        /**
         * Used each time the item is found, reset the current item being interacted with.
         */
        static void ResetCurrentItem();

    private:
        /**
         * Store the item which the test framework is currently interacting with. If no item
         *  is being interacted with, this is set to nullptr.
         */
        static ItemData *s_pCurrentItemData;

        /**
         * Global data for the ImGui testing utilities.
         */
        static GlobalData *s_pData;
    };
} // namespace rpp
