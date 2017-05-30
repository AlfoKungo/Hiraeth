#pragma once

#include "layer.h"
#include "../renderer.h"

/** This class is currently unused */


namespace hiraeth {
	namespace graphics {

		class TileLayer : public Layer<Renderable>
		{
		public:
			TileLayer(Shader* shader);
			virtual ~TileLayer();

			void update();
		};

	}
}
