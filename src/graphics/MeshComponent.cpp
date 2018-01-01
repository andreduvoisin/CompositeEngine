#include "MeshComponent.h"

#include "Mesh.h"
#include "Texture.h"
#include "TextureManager.h"

#include <GL\glew.h>

namespace CE
{
	MeshComponent::MeshComponent(Meshes* meshes)
		: m_meshes(meshes)
	{

	}

	void MeshComponent::Draw(
		GLuint g_vbo,
		GLuint g_ibo,
		GLuint g_diffuseTextureID,
		GLuint g_diffuseTextureLocation,
		GLuint g_diffuseTextureUnit)
	{
		for (int i = 0; i < m_meshes->size(); ++i)
		{
			DrawMesh(
				m_meshes->at(i),
				g_vbo,
				g_ibo,
				g_diffuseTextureID,
				g_diffuseTextureLocation,
				g_diffuseTextureUnit);
		}
	}

	void MeshComponent::DrawMesh(
		const Mesh& mesh,
		GLuint g_vbo,
		GLuint g_ibo,
		GLuint g_diffuseTextureID,
		GLuint g_diffuseTextureLocation,
		GLuint g_diffuseTextureUnit)
	{
		glBindBuffer(GL_ARRAY_BUFFER, g_vbo);
		glBufferData(GL_ARRAY_BUFFER, mesh.m_vertices.size() * sizeof(CE::Vertex), mesh.m_vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.m_indices.size() * sizeof(unsigned int), mesh.m_indices.data(), GL_STATIC_DRAW);

		Texture* texture = TextureManager::Get().GetTexture(mesh.m_diffuseMapName.c_str());

		// TODO: How much of this has to be done every Draw() call?
		glBindTexture(GL_TEXTURE_2D, g_diffuseTextureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		unsigned int glChannels = texture->channels == 3 ? GL_RGB : GL_RGBA;
		glTexImage2D(GL_TEXTURE_2D, 0, glChannels, texture->width, texture->height, 0, glChannels, GL_UNSIGNED_BYTE, texture->data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glUniform1i(g_diffuseTextureLocation, g_diffuseTextureUnit);

		glDrawElements(GL_TRIANGLES, mesh.m_indices.size(), GL_UNSIGNED_INT, NULL);
	}
}
