#include "scratch_editor.h"
#include "engine/core/editor/editor.h"
#include "engine/core/main/Engine.h"

namespace Echo
{
#ifdef ECHO_EDITOR_MODE
    ScratchEditor::ScratchEditor(Object* object)
    : ObjectEditor(object)
    {
        m_panel = EchoNew(ScratchEditorPanel(object));
    }
    
    ScratchEditor::~ScratchEditor()
    {
        EchoSafeDelete(m_panel, ScratchEditorPanel);
    }
    
    ImagePtr ScratchEditor::getThumbnail() const
    {
        return Image::loadFromFile(Engine::instance()->getRootPath() + "engine/core/script/scratch/editor/icon/scratch.png");
    }

	void ScratchEditor::onEditorSelectThisNode()
	{
		Editor::instance()->showCenterPanel((QDockWidget*)m_panel->getUiPtr());
	}

	void ScratchEditor::editor_update_self()
	{
		if (m_panel)
		{
			m_panel->update();
		}
	}
#endif
}

