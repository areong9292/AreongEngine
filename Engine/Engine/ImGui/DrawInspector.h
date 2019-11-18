#ifndef _DRAWINSPECTOR_H_
#define _DRAWINSPECTOR_H_

#include <map>
#include <string>

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "../../../DesignPattern/Transform.h"
#include "../../../DesignPattern/Component.h"
#include "../Material.h"

static void DrawDefaultInspect(const char* name, float width = 0.3f);
static bool DrawTransformInspect(Component* comp);
static bool DrawMaterialInspect(Component* comp);

class Component;
class DrawInspector
{
public :
	DrawInspector();
	~DrawInspector();

	void DrawComponent(Component* comp);

};

#endif // !_DRAWINSPECTOR_H_
