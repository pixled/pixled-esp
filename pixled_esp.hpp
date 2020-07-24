#ifndef PIXLED_ESP_HPP
#define PIXLED_ESP_HPP

#include "pixled_driver.hpp"
#include "pixled.h"

namespace pixled {
	class EspOutput : public api::Output {
		private:
			pixled::Strip& strip;
		public:
			EspOutput(pixled::Strip& strip) : strip(strip) {}

			void write(const api::Color& color, std::size_t i) override;

	};
}
#endif
