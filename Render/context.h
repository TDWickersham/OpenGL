#pragma once

class context
{
	struct GLFWwindow * window;

public:
	void init();
	void tick();
	void term();
	void clear();

	bool shouldClose();
};