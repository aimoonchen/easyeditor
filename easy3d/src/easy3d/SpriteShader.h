#ifndef _E3D_SPRITE_SHADER_H_
#define _E3D_SPRITE_SHADER_H_



#include "Matrix.h"

namespace e3d
{

class SpriteShader : public ee::IShader
{
public:
	SpriteShader();
	virtual ~SpriteShader();

	virtual void Load();
	virtual void Unload();

	virtual void Bind();
	virtual void Unbind();

	virtual void SetProjection(int width, int height);

	virtual void Commit();

	virtual void Reset() {}

	void SetModelView(const mat4& mat);

	int GetTexID() const;
	void SetTexID(int tex);

	void DrawTri(const vec3 vertices[3], const vec2 texcoords[3], int texid);
	void DrawTri(const float* vertices, const float* texcoords, int count, int texid);
	void DrawTriStrip(float* vertices, float* texcoords, int count, int texid);

protected:
	virtual void BindAttrib(GLuint prog);

private:
	void InitBuffers();

	void CopyVertex(const vec3 vertices[3], const vec2 texcoords[3]);
	void CopyVertex(const float* vertices, const float* texcoords, int count);

private:
	GLuint m_model_view, m_projection;

	GLuint m_vertex_buffer;

	int m_count;
	float* m_vb;

	GLuint m_tex;

	mat4 m_mat_modelview, m_mat_projection;

	int m_type;

}; // SpriteShader

}

#endif // _E3D_SPRITE_SHADER_H_