#include "pixled_esp.hpp"

namespace pixled {
	void EspOutput::write(const api::Color &color, std::size_t i) {
		strip.setRgbPixel(i, color.red(), color.green(), color.blue());
	}

	EspRuntime::EspRuntime(
			api::Mapping& map,
			api::Output& output,
			api::Function<Color>& animation,
			Strip& strip,
			uint8_t fps,
			timer_group_t timer_group,
			timer_idx_t timer_idx
			) : runtime(map, output, animation), strip(strip), fps(fps), timer_group(timer_group), timer_idx(timer_idx) {
		/*
		 * TIMER INITIALIZATION
		 */

		// Timer configuration
		timer_config_t config = {
			.alarm_en = TIMER_ALARM_EN,
			.counter_en = TIMER_PAUSE,
			.intr_type = TIMER_INTR_LEVEL,
			.counter_dir = TIMER_COUNT_UP,
			.auto_reload = TIMER_AUTORELOAD_EN,
			.divider = PIXLED_TIMER_DIVIDER,
		}; // default clock source is APB
		timer_init(timer_group, timer_idx, &config);

		// Timer's counter start from 0, with auto_reload enabled
		timer_set_counter_value(timer_group, timer_idx, 0x00000000ULL);

		// Sets an alarm with a period based on the specified FPS
		timer_set_alarm_value(timer_group, timer_idx, PIXLED_TIMER_SCALE / fps);
		// Enable and register interrupts
		timer_enable_intr(timer_group, timer_idx);
		timer_isr_register(timer_group, timer_idx, frame_interrupt_handler,
				(void *) &runtime, ESP_INTR_FLAG_IRAM, NULL);

		/*
		 * QUEUE INITIALIZATION
		 */
		timer_queue = xQueueCreate(30, 0);
	}

	void EspRuntime::start() {
		xTaskCreate(esp_runtime_main_task, "esp_runtime_main_task", 2048, this, 5, &task_handle);
		timer_start(timer_group, timer_idx);
	}

	void EspRuntime::stop() {
		timer_pause(timer_group, timer_idx);
		vTaskDelete(task_handle);
	}

	void IRAM_ATTR frame_interrupt_handler(void* param) {
		EspRuntime* esp_runtime = (EspRuntime*) param;
		timer_spinlock_take(esp_runtime->timer_group);


		timer_group_clr_intr_status_in_isr(esp_runtime->timer_group, esp_runtime->timer_idx);

		// After the alarm has been triggered
		// we need enable it again, so it is triggered the next time */
		timer_group_enable_alarm_in_isr(esp_runtime->timer_group, esp_runtime->timer_idx);

		// Now just send the event data back to the main program task <]
		if(!xQueueIsQueueFullFromISR(esp_runtime->timer_queue))
			xQueueSendFromISR(esp_runtime->timer_queue, NULL, NULL);

		timer_spinlock_give(esp_runtime->timer_group);
	}

	void esp_runtime_main_task(void* param) {
		EspRuntime* esp_runtime = (EspRuntime*) param;
		ESP_LOGI("PIXLED_ESP", "Esp Pixled Runtime main task launched");
		while (1) {
			if(xQueueIsQueueFullFromISR(esp_runtime->timer_queue))
				ESP_LOGW("PIXLED_ESP", "Framerate overflow");
			if(xQueueReceive(esp_runtime->timer_queue, NULL, portMAX_DELAY)) {
				ESP_LOGD("PIXLED_ESP", "Compute frame %lu", esp_runtime->runtime.time());
				esp_runtime->runtime.next();
				esp_runtime->strip.show();
			}
		}
	}
}
