#pragma once

namespace sn {
	template <typename T>
	class Singleton {
	public:
		Singleton(const Singleton&) = delete;
		Singleton& operator = (const Singleton&) = delete;

		static T& GetInstance() {
			static T s_instance;
			return s_instance;
		}

	protected:
		Singleton() {}
	};
}