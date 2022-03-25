#include "includes.h"
#include <thread>

InterceptionContext context;
InterceptionDevice device;
InterceptionStroke stroke;


void lmb_click() {
	context = interception_create_context();
	interception_set_filter(context, interception_is_mouse, INTERCEPTION_FILTER_MOUSE_ALL);
	while (interception_receive(context, device = interception_wait(context), &stroke, 1) > 0) {
		if (interception_is_mouse(device))
		{
			InterceptionMouseStroke mstroke = InterceptionMouseStroke();
			mstroke.state = InterceptionMouseState::INTERCEPTION_MOUSE_LEFT_BUTTON_DOWN;
			interception_send(context, device, (InterceptionStroke*)&mstroke, 1);
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			mstroke.state = InterceptionMouseState::INTERCEPTION_MOUSE_LEFT_BUTTON_UP;
			interception_send(context, device, (InterceptionStroke*)&mstroke, 1);
			interception_destroy_context(context);
			break;
		}
	}
}



int main()
{	

	Screenshot screeny;
	char* WindowName = (char*)"Grand Theft Auto V";

	SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
	SetConsoleTitle(L"GODv2");
	Capture recorder(WindowName);
	Sleep(1000);

	while (!recorder.isWindowRunning())
	{

		system("CLS");
		std::cout << "Searching for window name '" << WindowName << "'" << std::endl;
		for (int i = 0; i < 3; ++i) {
			std::cout << ".";
			Sleep(1000);
		}

	}

	std::cout << std::endl << WindowName << " found!!!" << std::endl;
	Beep(1000, 250);

	while (GlobalStates::triggerbotenabled) {

		while (!recorder.captureNewFrame(screeny)) Sleep(50);
		if (screeny.triggerBot())
		{
			lmb_click();
		}
		//screeny.drawScreenshot(1, 1);
		if (GetAsyncKeyState(VK_END)) {
			system("pause");
		}
		else if (GetAsyncKeyState(VK_INSERT)) {
			break;
		}
	}

}



