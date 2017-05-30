#pragma once

#include <cstddef>

#include <vector>
#include <GL/glew.h>
#include "font.h"
#include "buffers/indexbuffer.h"
#include "maths/maths.h"

#if 0
//#include "../../ext/freetype-gl/freetype-gl.h"
#else
//#include "../../ext/freetype-gl_v2/freetype-gl.h"
#endif

namespace hiraeth {
	namespace graphics {

#define RENDERER_MAX_SPRITES    60000
#define RENDERER_VERTEX_SIZE    sizeof(VertexData)
#define RENDERER_SPRITE_SIZE    RENDERER_VERTEX_SIZE * 4
#define RENDERER_BUFFER_SIZE    RENDERER_SPRITE_SIZE * RENDERER_MAX_SPRITES
#define RENDERER_INDICES_SIZE   RENDERER_MAX_SPRITES * 6
#define RENDERER_MAX_TEXTURES   32

#define SHADER_VERTEX_INDEX		0
#define SHADER_UV_INDEX			1
#define SHADER_TID_INDEX		2
#define SHADER_COLOR_INDEX		3
		 
		struct VertexData
		{
			maths::vec3 vertex;
			maths::vec2 uv;
			float tid;
			unsigned int color;
		};

		class Renderable;
		class Renderer
		{
		private:
			std::vector<maths::mat4> m_TransformationStack;
			const maths::mat4* m_TransformationBack;
			GLuint m_VAO;
			GLuint m_VBO;
			IndexBuffer* m_IBO;
			GLsizei m_IndexCount;
			VertexData* m_Buffer;

			std::vector<GLuint> m_TextureSlots;
		public:
			Renderer();
			~Renderer();
			void push(const maths::mat4& matrix, bool override = false);
			void pop();
			void begin();
			void submit(const Renderable* renderable);
			void submit(const Renderable* renderable, unsigned int blendColor);
			void drawString(const Font& font, const std::string& text, const maths::vec3& position, unsigned int color);
			void end();
			void flush();
		private:
			void init();
		};
	}
}
