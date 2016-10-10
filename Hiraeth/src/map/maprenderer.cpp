#include "maprenderer.h"
#include "maprenderable.h"

namespace hiraeth {
	namespace map {

		//MapRenderer::MapRenderer(const std::string& filename) 
		//	: m_Tex(filename)
		MapRenderer::MapRenderer(graphics::Texture tex) 
			: m_Tex(m_Tex)
		{
				m_TransformationStack.push_back(maths::mat4::identity());
				m_TransformationBack = &m_TransformationStack.back();
			init();
		}

		MapRenderer::~MapRenderer() 
		{
			delete m_IBO;
			glDeleteBuffers(1, &m_VBO);
		}

		void MapRenderer::init()
		{
			glGenVertexArrays(1, &m_VAO);
			glGenBuffers(1, &m_VBO);

			glBindVertexArray(m_VAO);
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
			glBufferData(GL_ARRAY_BUFFER, MAP_RENDERER_BUFFER_SIZE, NULL, GL_DYNAMIC_DRAW);

			glEnableVertexAttribArray(MAP_SHADER_VERTEX_INDEX);
			glEnableVertexAttribArray(MAP_SHADER_UV_INDEX);
			glEnableVertexAttribArray(MAP_SHADER_COLOR_INDEX);

			glVertexAttribPointer(MAP_SHADER_VERTEX_INDEX, 3, GL_FLOAT, GL_FALSE, MAP_RENDERER_VERTEX_SIZE, (const GLvoid*) 0);
			glVertexAttribPointer(MAP_SHADER_UV_INDEX, 2, GL_FLOAT, GL_FALSE, MAP_RENDERER_VERTEX_SIZE, (const GLvoid*)(offsetof(VertexData, VertexData::uv)));
			glVertexAttribPointer(MAP_SHADER_COLOR_INDEX, 4, GL_UNSIGNED_BYTE, GL_TRUE, MAP_RENDERER_VERTEX_SIZE, (const GLvoid*)(offsetof(VertexData, VertexData::color)));

			glBindBuffer(GL_ARRAY_BUFFER, 0);

			GLuint* indices = new GLuint[MAP_RENDERER_INDICES_SIZE];

			int offset = 0;
			for (int i = 0; i < MAP_RENDERER_INDICES_SIZE; i += 6)
			{
				indices[  i  ] = offset + 0;
				indices[i + 1] = offset + 1;
				indices[i + 2] = offset + 2;

				indices[i + 3] = offset + 2;
				indices[i + 4] = offset + 3;
				indices[i + 5] = offset + 0;

				offset += 4;
			}
			m_IBO = new graphics::IndexBuffer(indices, MAP_RENDERER_INDICES_SIZE);

			glBindVertexArray(0);
		}

		void MapRenderer::begin()
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
			m_Buffer = (VertexData*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		}

		void  MapRenderer::submit(const MapRenderable* renderable)
		{
			const maths::vec3& position = renderable->getPosition();
			const maths::vec2& size = renderable->getSize();
			const maths::vec4& color = renderable->getColor();
			const std::vector<maths::vec2>& uv = renderable->getUV();

			unsigned int c = 0;

			int r = color.x * 255.0f;
			int g = color.y * 255.0f;
			int b = color.z * 255.0f;
			int a = color.w * 255.0f;
			c = a << 24 | b << 16 | g << 8 | r;


			m_Buffer->vertex = *m_TransformationBack * position;
			m_Buffer->uv = uv[0];
			m_Buffer->color = c;
			m_Buffer++;

			m_Buffer->vertex = *m_TransformationBack * maths::vec3(position.x, position.y + size.y, 0);
			m_Buffer->uv = uv[1];
			m_Buffer->color = c;
			m_Buffer++;

			m_Buffer->vertex = *m_TransformationBack * maths::vec3(position.x + size.x, position.y + size.y, 0);
			m_Buffer->uv = uv[2];
			m_Buffer->color = c;
			m_Buffer++;

			m_Buffer->vertex = *m_TransformationBack * maths::vec3(position.x + size.x, position.y, 0);
			m_Buffer->uv = uv[3];
			m_Buffer->color = c;
			m_Buffer++;

			m_IndexCount += 6;
		}

		void MapRenderer::end()
		{
			glUnmapBuffer(GL_ARRAY_BUFFER);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		void MapRenderer::flush()
		{
			//glActiveTexture(GL_TEXTURE0);
				//glBindTexture(GL_TEXTURE_2D, m_Tex->getID());

			glBindVertexArray(m_VAO);
			m_IBO->bind();
			
			glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, NULL);

			m_IBO->unbind();
			glBindVertexArray(0);
			m_IndexCount = 0;
		}
	}
}
