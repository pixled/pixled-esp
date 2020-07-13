#include "pixled_driver.hpp"
#include "pixled/api/output.h"
#include "pixled/api/pixel.h"

namespace pixled {
	class EspOutput : public api::Output {
		private:
			pixled::Strip& strip;
		public:
			EspOutput(pixled::Strip& strip) : strip(strip) {}

			void write(const api::Color& color, std::size_t i) override;

	};
}
