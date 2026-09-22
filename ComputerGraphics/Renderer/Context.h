#pragma once

// forward declare for GLFW
struct GLFWwindow;

namespace aie
{
	class Context
	{
		GLFWwindow* Window = nullptr;

	public:
		bool Init(int Width, int Height, const char* Title);
		void Tick();
		void Clear();
		void Term();

		bool ShouldClose() const;
	};
}