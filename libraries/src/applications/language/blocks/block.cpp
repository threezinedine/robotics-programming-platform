#include "applications/language/blocks/blocks.h"

namespace rpp
{
    Block::Block()
    {
        RPP_PROFILE_SCOPE();
    }

    Block::~Block()
    {
        RPP_PROFILE_SCOPE();
    }

    void Block::Update(f32 delta)
    {
        RPP_PROFILE_SCOPE();
        UpdateImpl(delta);
    }

    void Block::UpdateImpl(f32 delta)
    {
        RPP_PROFILE_SCOPE();
    }

    void Block::Draw(b8 isEditorWindow)
    {
        RPP_PROFILE_SCOPE();

        if (!isEditorWindow)
        {
            DrawImpl();
        }
        else
        {
            u32 childBlockCount = m_childBlocks.Size();
            for (u32 childBlockIndex = 0; childBlockIndex < childBlockCount; ++childBlockIndex)
            {
                m_childBlocks[childBlockIndex]->Draw(FALSE);
            }
        }
    }

    void Block::DrawImpl()
    {
        RPP_PROFILE_SCOPE();
    }
} // namespace rpp
