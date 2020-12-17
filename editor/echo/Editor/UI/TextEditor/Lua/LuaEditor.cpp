#include "ProjectMgr.h"
#include <QFileDialog>
#include "MainWindow.h"
#include "LuaEditor.h"
#include "Studio.h"
#include <engine/core/util/PathUtil.h>
#include <engine/core/io/IO.h>
#include "XmlSyntaxHighLighter.h"
#include "TextSyntaxHighLighter.h"

namespace Studio
{
	LuaEditor::LuaEditor(QWidget* parent)
		: QWidget(parent)
	{
		setupUi( this);

		// connections
		QObject::connect(m_plainTextEdit, SIGNAL(textChanged()), this, SLOT(onTextChanged()));
	}

	LuaEditor::~LuaEditor()
	{
		EchoSafeDelete(m_syntaxHighLighter, SyntaxHighLighter);
	}

	void LuaEditor::open(const Echo::String& fullPath)
	{
		m_pathName = fullPath;
		Echo::String content = Echo::IO::instance()->loadFileToString(fullPath);
		if (!content.empty())
		{
			m_plainTextEdit->setPlainText(content.c_str());
			m_content = m_plainTextEdit->toPlainText().toStdString().c_str();

			updateTitle();
		}

		// syntax high lighter
		m_syntaxHighLighter = nullptr;
		if(Echo::PathUtil::GetFileExt(fullPath, true) == ".lua")
			m_syntaxHighLighter = EchoNew(LuaSyntaxHighLighter(m_plainTextEdit->document()));
		else if (Echo::PathUtil::GetFileExt(fullPath, true) == ".xml")
			m_syntaxHighLighter = EchoNew(XmlSyntaxHighLighter(m_plainTextEdit->document()));
		else
			m_syntaxHighLighter = EchoNew(TextSyntaxHighLighter(m_plainTextEdit->document()));

		m_plainTextEdit->setSyntaxHighter(m_syntaxHighLighter);
	}

	const Echo::String& LuaEditor::getFilePath()
	{
		return m_pathName;
	}

	void LuaEditor::onTextChanged()
	{
		updateTitle();
	}

	void LuaEditor::save()
	{
		m_content = m_plainTextEdit->toPlainText().toStdString().c_str();
		Echo::IO::instance()->saveStringToFile(m_pathName, m_content);

		updateTitle();
	}

	void LuaEditor::updateTitle()
	{
		Echo::String currentContent = m_plainTextEdit->toPlainText().toStdString().c_str();
		if (m_content == currentContent)
		{
			setWindowTitle( m_pathName.c_str());
		}
		else
		{
			setWindowTitle((m_pathName + "*").c_str());
		}
        
        emit titleChanged(this);
	}
}
