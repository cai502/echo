#include "ShaderDataModel.h"
#include "Data/DataAny.h"
#include "NodeTreePanel.h"

namespace DataFlowProgramming
{
	ShaderDataModel::ShaderDataModel()
		: NodeDataModel()
	{
		static Echo::ui32 id = 0;
		m_id = id++;
	}

	bool ShaderDataModel::onNodePressed() 
	{ 
		if (m_shaderNode)
		{
			Studio::NodeTreePanel::instance()->onEditObject(m_shaderNode);

			return true;
		}

		return false; 
	}

	Echo::String ShaderDataModel::getVariableName() const
	{ 
		return getDefaultVariableName();
	}

	Echo::String ShaderDataModel::getDefaultVariableName() const
	{
		Echo::String variableName = name().toStdString().c_str() + Echo::StringUtil::Format("_%d", m_id);
		variableName = Echo::StringUtil::Replace(variableName, " ", "");

		return variableName;
	}

	unsigned int ShaderDataModel::nPorts(PortType portType) const
	{
		switch (portType)
		{
		case PortType::In: return m_inputs.size();
		case PortType::Out:return m_outputs.size();
		default:           return 0;
		}
	}

	NodeDataType ShaderDataModel::dataType(PortType portType, PortIndex portIndex) const
	{
		if (portType == PortType::In)
		{
			return m_inputDataTypes[portIndex];
		}
		else if (portType == PortType::Out)
		{
			return m_outputs[portIndex]->type();
		}

		return NodeDataType{ "invalid", "invalid" };
	}

	std::shared_ptr<NodeData> ShaderDataModel::outData(PortIndex portIndex)
	{
		return m_outputs[portIndex];
	}

	bool ShaderDataModel::checkValidation()
	{
		m_modelValidationState = NodeValidationState::Valid;
		m_modelValidationError = QStringLiteral("");

		// check 
		if (m_inputDataTypes.size() != m_inputs.size())
		{
			m_modelValidationState = NodeValidationState::Error;
			m_modelValidationError = QStringLiteral("Inputs count error");

			return false;
		}

		// input type check
		for (size_t i = 0; i < m_inputDataTypes.size(); i++)
		{
			if (m_inputs[i] && m_inputs[i]->type().id != m_inputDataTypes[i].id)
			{
				m_modelValidationState = NodeValidationState::Error;
				m_modelValidationError = Echo::StringUtil::Format("Input [%d] type error", i).c_str();

				return false;
			}
		}

		// check invalid input
		for (size_t i = 0; i < m_inputs.size(); i++)
		{
			if (m_inputs[i] && m_inputs[i]->type().id == "invalid")
			{
				m_modelValidationState = NodeValidationState::Error;
				m_modelValidationError = Echo::StringUtil::Format("Input [%d] is invalid", i).c_str();

				return false;
			}
		}

		// check invalid output
		for (size_t i = 0; i < m_inputs.size(); i++)
		{
			if (m_inputs[i] && m_inputs[i]->type().id == "invalid")
			{
				m_modelValidationState = NodeValidationState::Error;
				m_modelValidationError = Echo::StringUtil::Format("Input [%d] is invalid", i).c_str();

				return false;
			}
			else if (m_inputs[i] && m_inputs[i]->type().id == "any")
			{
				std::shared_ptr<ShaderData> internalData = DataAny::getInternalData(m_inputs[i]);
				if (internalData && internalData->type().id == "invalid")
				{
					m_modelValidationState = NodeValidationState::Error;
					m_modelValidationError = Echo::StringUtil::Format("Input [%d] is invalid", i).c_str();

					return false;
				}
			}
		}

		return true;
	}

	void ShaderDataModel::saveShaderNode(QJsonObject& p) const
	{
		if (m_shaderNode)
		{
			Echo::PropertyInfos properties;
			Echo::Class::getPropertys(m_shaderNode->getClassName(), m_shaderNode, properties, 3, true);

			for (Echo::PropertyInfo* prop : properties)
			{
				Echo::Variant variant;
				Echo::Class::getPropertyValue(m_shaderNode, prop->m_name, variant);

				p[prop->m_name.c_str()] = variant.toString().c_str();
			}
		}
	}

	void ShaderDataModel::restoreShaderNode(QJsonObject const& p)
	{
		if (m_shaderNode)
		{
			Echo::PropertyInfos properties;
			Echo::Class::getPropertys(m_shaderNode->getClassName(), m_shaderNode, properties, 3, true);

			for (Echo::PropertyInfo* prop : properties)
			{
				QJsonValue v = p[prop->m_name.c_str()];
				if (!v.isUndefined())
				{
					Echo::String value = v.toString().toStdString().c_str();

					Echo::Variant variant;
					variant.fromString(prop->m_type, value);
					Echo::Class::setPropertyValue(m_shaderNode, prop->m_name, variant);
				}
			}

			// sync data types
			m_inputDataTypes.clear();
			Echo::ShaderNode::DataTypes inputTypes = m_shaderNode->getInputDataTypes();
			for (const Echo::ShaderNode::DataType& type : inputTypes)
			{
				m_inputDataTypes.push_back({type.m_type, type.m_name});
			}

			m_inputs.resize(m_inputDataTypes.size());
		}
	}

	bool ShaderDataModel::generateCode(Echo::ShaderCompiler& compiler)
	{
		if (m_shaderNode)
		{
			m_shaderNode->generateCode(compiler);

			return true;
		}

		return false;
	}
}