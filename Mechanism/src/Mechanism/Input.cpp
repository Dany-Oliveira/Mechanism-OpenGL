#include "Input.h"
#include <SDL3/SDL.h>
#include <unordered_set>

namespace Mechanism
{

	static std::unordered_set<int> heldKey;
	static std::unordered_set<int> pressedKey;
	static std::unordered_set<int> releasedKey;
	static std::unordered_set<int> previousHeldKey;


	
	bool Input::IsKeyBeingPressed(int keyCode)
	{
		return heldKey.find(keyCode) != heldKey.end();
	}

	bool Input::IsKeyPressed(int keyCode)
	{
		return pressedKey.find(keyCode) != pressedKey.end();
	}

	bool Input::IsKeyReleased(int keyCode)
	{
		return releasedKey.find(keyCode) != releasedKey.end();
	}

	void Input::Update()
	{
		// Update held keys
		pressedKey.clear();
		releasedKey.clear();
	}

	void Input::OnKeyDown(int keyCode)
	{
		// If the key was not already held, mark it as pressed
		if (heldKey.find(keyCode) == heldKey.end())
		{
			pressedKey.insert(keyCode);
		}
		// Mark the key as held
		heldKey.insert(keyCode);
	}

	void Input::OnKeyUp(int keyCode)
	{
		// Remove from held keys
		heldKey.erase(keyCode);
		// Add to released keys
		releasedKey.insert(keyCode);
	}

}


