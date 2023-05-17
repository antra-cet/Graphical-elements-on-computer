#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Ski/Ski.h"

namespace Ski_Utils {

	struct skier {
		glm::vec3 inSpacePosition;

		float speed, score;
		float angle;
		bool hit;
	};

	struct randObjects {
		glm::vec3 inSpacePosition;
		bool render;
		bool touched;
	};

}   // namespace Ski_Utils
