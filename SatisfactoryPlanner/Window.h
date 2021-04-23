#pragma once


struct GLFWwindow;


class [[nodiscard]] Window
{
public:
	[[nodiscard]] constexpr GLFWwindow* GetWindow() const noexcept { return mpWindow; }
	constexpr void SetWindow(GLFWwindow* const pWindow) noexcept { mpWindow = pWindow; }

	[[nodiscard]] bool ShouldClose() const;
	void SwapBuffers() const;


private:
	GLFWwindow* mpWindow = nullptr;
};




void gl_debug_callback(unsigned int source, unsigned int type, unsigned int id, unsigned int severity, int length, const char* message, const void* userData);

Window create_window();

