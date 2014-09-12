#pragma once

#ifndef SERVER
#error Must compile under SERVER
#endif

using namespace System;

namespace CLIq3 {
	public ref class ConColor {
	public:
		literal String^ Black = "^0";
		literal String^ Red = "^1";
		literal String^ Green = "^2";
		literal String^ Yellow = "^3";
		literal String^ Blue = "^4";
		literal String^ Cyan = "^5";
		literal String^ Magenta = "^6";
		literal String^ White = "^7";
	};
}