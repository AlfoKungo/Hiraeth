#pragma once

#include "layer.h"
#include "../batchrenderer2d.h"

namespace hiraeth {
	namespace graphics {

		class TileLayer : public Layer
		{
		public:
			TileLayer(Shader* shader);
			virtual ~TileLayer();

			void update();
			void clear() { m_Renderables.clear(); }
		};

	}
}
