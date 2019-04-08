#ifndef VELOCITY_TRACKER_HPP
#define VELOCITY_TRACKER_HPP

#include </home/izzy/ml/darknet/src/yolo_v2_class.hpp>

namespace terraclear
{
	class velocity_tracker
	{
		public:
			velocity_tracker(uint32_t fps, uint32_t id);
			uint32_t get_id();
			void update_position(uint32_t ypos);
			uint32_t find_velocity();
			void reset_anchor();
		private:
			std::map<uint32_t,uint32_t> _frame_locations;
			uint32_t _tracker_id;
			uint32_t _fps;
			uint32_t _current_frame_number;
	}
}
