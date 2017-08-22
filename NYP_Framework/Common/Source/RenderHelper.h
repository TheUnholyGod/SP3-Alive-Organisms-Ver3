#ifndef RENDER_HELPER_H
#define RENDER_HELPER_H

#include "Vertex.h"

class Mesh;
class SpriteAnimation;

class RenderHelper
{
public:
	static void RenderMesh(Mesh* _mesh);
	static void RenderMeshWithLight(Mesh* _mesh);
	static void RenderText(Mesh* _mesh, const std::string& _text, Color _color);
    static void RenderSpriteAnnimation(SpriteAnimation* _mesh);
};

#endif // RENDER_HELPER_H