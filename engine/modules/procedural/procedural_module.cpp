#include "procedural_module.h"
#include "procedural_texture.h"
#include "procedural_geometry.h"
#include "geometry/node/connect/pg_connect.h"
#include "geometry/node/primitive/pg_sphere.h"
#include "geometry/node/primitive/pg_grid.h"
#include "geometry/node/primitive/pg_box.h"
#include "editor/procedural_geometry_editor.h"
#include "editor/procedural_texture_editor.h"
#include "procedural_depend_graph.h"

namespace Echo
{
	DECLARE_MODULE(ProceduralModule)

	ProceduralModule::ProceduralModule()
	{

	}

	ProceduralModule* ProceduralModule::instance()
	{
		static ProceduralModule* inst = EchoNew(ProceduralModule);
		return inst;
	}

	void ProceduralModule::bindMethods()
	{

	}

	void ProceduralModule::registerTypes()
	{
		Class::registerType<ProceduralTexture>();
        Class::registerType<ProceduralGeometry>();

		Class::registerType<PGNode>();
		Class::registerType<PGConnnect>();
		Class::registerType<PGSphere>();
		Class::registerType<PGGrid>();
		Class::registerType<PGBox>();

		CLASS_REGISTER_EDITOR(ProceduralTexture, ProceduralTextureEditor)
		CLASS_REGISTER_EDITOR(ProceduralGeometry, ProceduralGeometryEditor)
	}
}
