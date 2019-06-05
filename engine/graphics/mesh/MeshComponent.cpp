#include "MeshComponent.h"

#include "Mesh.h"
#include "graphics/texture/Texture.h"
#include "graphics/texture/TextureManager.h"

#include <GL/glew.h>

namespace CE
{
	MeshComponent::MeshComponent(Meshes* meshes, Textures* textures)
		: m_meshes(meshes)
		, m_textures(textures)
	{

	}

	void MeshComponent::Draw(
		GLuint g_vbo,
		GLuint g_ibo,
		GLuint g_diffuseTextureID,
		GLuint g_diffuseTextureLocation,
		GLuint g_diffuseTextureUnit)
	{
    nVertices = 0;
    nIndices = 0;

		for (size_t i = 0; i < m_meshes->size(); ++i)
		{
			DrawMesh(
				m_meshes->at(i),
				g_vbo,
				g_ibo,
				g_diffuseTextureID,
				g_diffuseTextureLocation,
				g_diffuseTextureUnit);
		}

//    printf("MeshComponent drawn.  nVerts:  %zd  nIndices:  %zd \n", nVertices, nIndices);
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
		glBufferData(GL_ARRAY_BUFFER, mesh.m_vertices.size() * sizeof(CE::Vertex1P1UV4J), mesh.m_vertices.data(), GL_STATIC_DRAW);
    nVertices += mesh.m_vertices.size();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.m_indices.size() * sizeof(unsigned int), mesh.m_indices.data(), GL_STATIC_DRAW);
    nIndices += mesh.m_indices.size();

		const Texture& texture = (*m_textures)[mesh.m_diffuseIndex];

		// TODO: How much of this has to be done every Draw() call?
		glBindTexture(GL_TEXTURE_2D, g_diffuseTextureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		unsigned int glChannels = texture.channels == 3 ? GL_RGB : GL_RGBA;
		glTexImage2D(GL_TEXTURE_2D, 0, glChannels, texture.width, texture.height, 0, glChannels, GL_UNSIGNED_BYTE, texture.data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glUniform1i(g_diffuseTextureLocation, g_diffuseTextureUnit);

		glDrawElements(GL_TRIANGLES, (GLsizei) mesh.m_indices.size(), GL_UNSIGNED_INT, NULL);
	}
}
