#ifndef PIXLED_ESP_HPP
#define PIXLED_ESP_HPP

#include <driver/timer.h>
#include "pixled_driver.hpp"
#include "pixled.h"
#include <freertos/task.h>

#define PIXLED_TIMER_DIVIDER 16
#define PIXLED_TIMER_SCALE (TIMER_BASE_CLK / PIXLED_TIMER_DIVIDER)  // convert counter value to seconds

namespace pixled {
	class EspOutput : public api::Output {
		private:
			Strip& strip;
		public:
			EspOutput(Strip& strip) : strip(strip) {}

			void write(const api::Color& color, std::size_t i) override;

	};

	void IRAM_ATTR frame_interrupt_handler(void* param);

	void esp_runtime_main_task(void* param);

	class EspRuntime {
		friend void IRAM_ATTR frame_interrupt_handler(void*);
		friend void esp_runtime_main_task(void*);

		public:
			Runtime runtime;
		private:
			Strip& strip;
			uint8_t fps;
			timer_group_t timer_group;
			timer_idx_t timer_idx;
			xQueueHandle timer_queue;
			TaskHandle_t task_handle;

		public:
			EspRuntime(
				api::Mapping& map,
				api::Output& output,
				api::Function<Color>& animation,
				Strip& strip,
				uint8_t fps,
				timer_group_t timer_group = TIMER_GROUP_0,
				timer_idx_t timer_idx = TIMER_0 
				);

			void start();
			void stop();
	};

}
#endif
