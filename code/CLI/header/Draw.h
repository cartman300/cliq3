#pragma once

#ifndef CLIENT
#error Must compile under CLIENT
#endif

namespace CLIq3 {
	public ref class Draw {
	public:

		static void DynamicLight(Vec3 Pos);
	};
}