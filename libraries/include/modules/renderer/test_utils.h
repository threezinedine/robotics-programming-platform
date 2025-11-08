#pragma once
#if defined(RPP_USE_TEST)
#include "core/core.h"
#include "imgui.h"

namespace rpp
{
    class Renderer;
    class ImGuiImpl;

    /**
     * Used mainly for testing purposes, the assertions of the items with certain labels.
     *
     * @param label The label to assign to the item.
     */
    // typedef void (*LabelCheckingCallback)(const String &label);
    using LabelCheckingCallback = std::function<void(const String &, void *, ImGuiID)>; // TODO: replace with self-defined function type

    /**
     * Needed interfaces for ImGui end-to-end testing. This class will be used for e2e tests only (be binded to python).
     */
    class RPP_E2E_BINDING TestUtils
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
            /// @brief The renderer id associated with this item.
            u32 rendererId;
        };

        /**
         * Indicates how the cursor should be interacted with the item.
         */
        enum class ImGuiItemAction : u8
        {
            IMGUI_ACTION_MOVE,              ///< Move action on the item.
            IMGUI_ACTION_CLICK,             ///< Click action on the item.
            IMGUI_ACTION_FIND_ITEM,         ///< Just find the item without any action.
            IMGUI_ACTION_ASSERT_TEXT_INPUT, ///< Just find the item without any action.
            IMGUI_ACTION_TYPE,              ///< Type text into the focused item.
            IMGUI_ACTION_ENTER,             ///< Type enter action
            IMGUI_ACTION_COUNT,
        };

        struct GlobalData
        {
            /// @brief The current search label. If empty, no search is being performed. If matched with an item
            ///        the item will be selected and the label cleared.
            String label;

            /// @brief The action to perform on the item when found. If no action, set to IMGUI_ACTION_COUNT.
            ImGuiItemAction action = ImGuiItemAction::IMGUI_ACTION_COUNT;

            /// @brief The text to type into the focused item when the action is IMGUI_ACTION_TYPE.
            String text;

            /// @brief The current character index being typed into the focused item.
            u32 characterIndex;

            /// @brief An optional callback function for additional label checking.
            LabelCheckingCallback labelCheckingCallback = nullptr;
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

        /**
         * Used mainly for checking if an item with a certain name exists.
         *
         * @param label The label of the item to search for.
         * @return TRUE if the item is found, FALSE otherwise.
         */
        static b8 IsItemFound(const String &label) RPP_E2E_BINDING;

        /**
         * Used for asserting a condition in the test framework.
         *
         * @param condition The condition to assert.
         * @param message The message to display if the assertion fails.
         */
        static void Assert(b8 condition, const String &message) RPP_E2E_BINDING;

        /**
         * Used for performing a left click on the current item being interacted with.
         * @return TRUE if the click action was performed, FALSE otherwise.
         */
        static void LeftClick(const String &label) RPP_E2E_BINDING;

        /**
         * Used for typing anything, disregarding the focused item.
         *
         * @param text The text to type into the focused item.
         */
        static void Type(const String &text) RPP_E2E_BINDING;

        /**
         * Used for returning the input
         */
        static void Enter() RPP_E2E_BINDING;

        /**
         * Used for asserting the value of an InputText item with a certain label.
         *
         * @param label The label of the InputText item to check.
         * @param expectedValue The expected value of the InputText item.
         */
        static void AssertInputTextValue(const String &label, const String &expectedValue) RPP_E2E_BINDING;

        /**
         * Used for finding the renderer ID by its name.
         *
         * @param rendererName The name of the renderer to find.
         *
         * @return The ID of the renderer if found, otherwise returns an `INVALID_ID`.
         */
        static u32 FindRendererIdByName(const String &rendererName) RPP_E2E_BINDING;

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

    public:
        static inline void SystemUpdate() { s_findingFrameCount++; }

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

        /**
         * Tracking if an item was found, useful for multiple renderer scenarios so that the test framework
         *  knows when to wait for the next frame.
         */
        static b8 s_itemFound;

        /**
         * Each time start searching the item, this counter is reset to 0 and be incremented each frame. If
         * it's 0, then the update should not handle the action.
         */
        static u8 s_findingFrameCount;

        /**
         * Used for tracking which renderer is currently focused for the item interaction. INVALID_ID means no renderer is focused.
         */
        static u32 s_focusedRendererId;
    };
} // namespace rpp
#endif