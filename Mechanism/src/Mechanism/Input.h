#pragma once
#include "Core.h"

namespace Mechanism
{

	class MECHANISM_API Input
	{

		public:

			//Checks if a key is held down
			static bool IsKeyBeingPressed(int keyCode);

			//Checks if a key was just pressed
			static bool IsKeyPressed(int keyCode);
			
			//Checks if a key was just released
			static bool IsKeyReleased(int keyCode);

			static void Update();

			static void OnKeyDown(int keyCode);
			static void OnKeyUp(int keyCode);

			static const int KEY_UP = 82;
			static const int KEY_DOWN = 81;
			static const int KEY_LEFT = 80;
			static const int KEY_RIGHT = 79;
			static const int KEY_SPACE = 44;

		private:
			
			Input() = delete;
	};

}



