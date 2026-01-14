#pragma once

#include <iostream>

#ifdef MECHANISM_PLATFORM_WINDOWS

extern Mechanism::Application* Mechanism::CreateApplication();

int main(int argc, char** argv)
{
	//Create the Mechanism application by calling the CreateApplication function that is defined in the client (Xennon)
	
	std::cout <<R"(
___  ___          _                 _                    _____ _             _   _                      
|  \/  |         | |               (_)                  /  ___| |           | | (_)                     
| .  . | ___  ___| |__   __ _ _ __  _ ___ _ __ ___      \ `--.| |_ __ _ _ __| |_ _ _ __   __ _          
| |\/| |/ _ \/ __| '_ \ / _` | '_ \| / __| '_ ` _ \      `--. \ __/ _` | '__| __| | '_ \ / _` |         
| |  | |  __/ (__| | | | (_| | | | | \__ \ | | | | |    /\__/ / || (_| | |  | |_| | | | | (_| |   _ _ _ 
\_|  |_/\___|\___|_| |_|\__,_|_| |_|_|___/_| |_| |_|    \____/ \__\__,_|_|   \__|_|_| |_|\__, |  (_|_|_)
                                                                                          __/ |         
                                                                                         |___/          
   
)" << std::endl;

	auto app = Mechanism::CreateApplication();
	app->Run();
	delete app;


}

#endif 
