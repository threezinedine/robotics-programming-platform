#pragma once
#include "modules/modules.h"

namespace rpp
{
    /**
     * Each component from variable, function, constants, code blocks will be represented as a block in the language.
     * Each block will be drawn in the editor as a visual block that can be connected to other blocks to form a program.
     */
    class Block
    {
    public:
        Block();
        virtual ~Block();

    public:
        /**
         * Updates the block logic.
         * @param delta Time elapsed since the last frame.
         */
        void Update(f32 delta);

        /**
         * Draws the block in the editor.
         *
         * @param isEditorWindow If TRUE, the block is drawn in the editor window context, so that just drawing the child blocks is enough.
         * If FALSE, the block is drawn in the main editor context, the child blocks are not drawn automatically, only its representation.
         */
        void Draw(b8 isEditorWindow = FALSE);

    protected:
        /**
         * Implements this method to customize how the block is updated.
         * @param delta Time elapsed since the last frame.
         */
        virtual void UpdateImpl(f32 delta);

        /**
         * Implements this method to customize how the block is drawn in the editor.
         * @note This is the rendering method for itself only, not contains the child blocks drawing logic.
         */
        virtual void DrawImpl();

    private:
        Array<Block *> m_childBlocks; ///< If the current block is opened in the editor window -> these blocks are drawn as logic.
    };
} // namespace rpp