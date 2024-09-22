#pragma once

#include <exception>
#include <chrono>
#include <functional>

namespace Fiea::GameEngine
{
	/** USAGE:
	 *   game() {
	 *       GameClock clock;
	 *       GameTime time = clock.Current();
	 *
	 *       // game loop
	 *       while(1) {
	 *           while(clock.Elapsed(time) < FRAME_TICKS) sleep();
	 * 
	 *           // update the time as we enter a new frame
	 *           clock.Update(time);
	 *
	 *           // update game using the time
	 *           root.Update(time);
	 *       }
	 *  }
	 * 
	 *  Root::Update(const GameTime& time) {
	 *      // for handling typical frame-by-frame updates
	 *      updateSinceLast(time.Frame());
	 * 
	 *      // for handling things scheduled in real time
	 *      processAbsolute(time.Get());
	 * 
	 *      // for handling things scheduled "since boot"
	 *      processSinceStart(time.Game());
	 *  }
 	 */

	class GameClock;

	/**
	 * @brief A simple struct which can provide indirect access to a snapshot of the clock
	 *         information, as well as elapsed frame time
	*/
	struct GameTime final
	{
		friend GameClock;
	public:
		using Millis = long long;

		/**
		 * @brief Milliseconds since epoch
		 * @return milliseconds since epoch
		*/
		inline Millis Get() const { return std::chrono::duration_cast<std::chrono::milliseconds>(_current.time_since_epoch()).count(); };
		/**
		 * @brief Milliseconds since start
		 * @return milliseconds since start
		*/
		inline Millis Game() const { return std::chrono::duration_cast<std::chrono::milliseconds>(_current - _start).count(); };
		/**
		 * @brief Milliseconds since last frame
		 * @return milliseconds since last frame
		*/
		inline Millis Frame() const { return std::chrono::duration_cast<std::chrono::milliseconds>(_current - _last).count(); };

	private:
		GameTime() {}

		std::chrono::high_resolution_clock::time_point _start{};
		std::chrono::high_resolution_clock::time_point _last{};
		std::chrono::high_resolution_clock::time_point _current{};
	};

	/**
	 * @brief A class to wrap the system clock and provide additional functionality
	*/
	class GameClock final
	{
	public:
		/**
		 * @brief Any function which returns a time_point. Provided for use in the
		 *        debug default constructor, so that "fake" time may be maintained by 
		 *        the application, for use in debug code
		*/
		using now_func = std::function<std::chrono::high_resolution_clock::time_point()>;

		GameClock(now_func now = nullptr);
		GameClock(const GameClock&) = default;
		GameClock& operator=(const GameClock&) = default;
		GameClock(GameClock&&) = default;
		GameClock& operator=(GameClock&&) = default;
		~GameClock() = default;

		/**
		 * @brief Return a GameTime struct based on the current clock time
		 * @return a game time struct
		*/
		[[nodiscard]] GameTime Current() const;
		/**
		 * @brief Given a game time struct, how much time has elapsed since it was last updated
		*/
		GameTime::Millis Elapsed(const GameTime& time) const;
		/**
		 * @brief Updates a game time struct, based upon the current clock time
		*/
		void Update(GameTime& time) const;

	private:
		now_func _now = nullptr;

		std::chrono::high_resolution_clock::time_point _startTime;
	};
}