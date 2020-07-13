#include "pixled_esp.hpp"

namespace pixled {
	void EspOutput::write(const api::Color &color, std::size_t i) {
		strip.setRgbPixel(i, color.red(), color.green(), color.blue());
	}
}
