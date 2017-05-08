#pragma once

#include "layer.h"
#include "../batchrenderer2d.h"

/** This class is currently unused */


namespace hiraeth {
	namespace graphics {

		class TileLayer : public Layer<Renderable2D>
		{
		public:
			TileLayer(Shader* shader);
			virtual ~TileLayer();

			void update();
		};

	}
}
