#pragma once

#include "../base/text.h"

namespace Echo
{
#ifdef ECHO_EDITOR_MODE
	class UiTextEditor : public ObjectEditor
	{
	public:
		UiTextEditor(Object* object);
		virtual ~UiTextEditor();

		// get camera2d icon, used for editor
		virtual ImagePtr getThumbnail() const override;

	private:
	};

#endif
}