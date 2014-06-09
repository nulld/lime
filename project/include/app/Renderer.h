#ifndef LIME_APP_RENDERER_H
#define LIME_APP_RENDERER_H


#include <app/Window.h>


namespace lime {
	
	
	class Renderer {
		
		
		public:
			
			Window* currentWindow;
		
		
	};
	
	
	Renderer* CreateRenderer (Window* window);
	
	
}


#endif