#pragma once
 
template <typename T, typename... Args>
static T call(std::uintptr_t address, Args... args) {
	auto fn = reinterpret_cast<T (*)(Args...)>(address);
	return fn(args...);
}

namespace math {

	struct vec3 {
		float x{}, y{}, z{};

		constexpr vec3() = default;

		constexpr vec3(float x, float y, float z)
			: x(x), y(y), z(z) {}

		vec3 operator+(const vec3 &v) const {
			return vec3(x + v.x, y + v.y, z + v.z);
		}

		vec3 operator-(const vec3 &v) const {
			return vec3(x - v.x, y - v.y, z - v.z);
		}

		vec3 operator*(const vec3 &v) const {
			return vec3(x * v.x, y * v.y, z * v.z);
		}

		// scale
		vec3 operator*(float s) const {
			return {x * s, y * s, z * s};
		}

		float distance(const vec3 v) const {
			float dis_x = x - v.x, dis_y = y - v.y, dis_z = z - v.z;
			return sqrt((dis_x * dis_x) + (dis_y * dis_y) + (dis_z * dis_z) * 1.f);
		}

		float dot(const vec3 &v) const {
			return (x * v.x) + (y * v.y) + (z * v.z);
		}
	};

	struct vec2 {
		float x{}, y{};

		constexpr vec2() = default;

		constexpr vec2(float x, float y)
			: x(x), y(y) {}
	};

	struct matrix4x4 {
		float matrix[16];

		float &operator[](std::size_t i) {
			return matrix[i];
		}

		const float &operator[](std::size_t i) const {
			return matrix[i];
		}
	};
};

namespace structs {

	// local player - <ac_client.exe>+0x18AC00
	struct player {
		char _0x0000[0x4]; //0x0000
		math::vec3 origin1; //0x0004
		char _0x0010[0x18]; //0x0010
		math::vec3 origin2; //0x0028
		math::vec3 view_angles; //0x0034
		char _0x0040[0x36]; //0x0040
		uint8_t alive; // 0x0076
		char _0x0077[0x75]; // 0x0077
		uint32_t current_health; //0x00EC
		char _0x00F0[0x115]; //0x00F0
		char name[15]; // 0x0205
		char _0x0214[0xF8];
		uint32_t team; // 0x030C
	};
	inline player* local_player;

	// <ac_client.exe>+0x18AC04
	struct entity {
		char _0x0000[0x4];
		player* entites[32]; // 0x004
	};
	inline entity* ent;

	// ac_client.exe+0x191ED8
	struct screen_dimensions {
		uint32_t screen_width;	// 0x0000
		uint32_t screen_height; // 0x0004
	};
	inline screen_dimensions* screen;

	// ac_client.exe+0x17DFD0
	struct view_matrix {
		math::matrix4x4 view; // 0x0000
	};
	inline view_matrix* matrix;

	struct traceresult_s {
    	math::vec3 end;
     	bool collided;
	};
};