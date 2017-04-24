#include "Game.h"
#include "InputHandler.h"

#include <iostream>
#include <assert.h>

#include "GL\glew.h"
#include "GL\freeglut.h"

#include "IL\il.h"
#include "IL\ilu.h"
#include "IL\ilut.h"

#include "SDL\SDL_mixer.h"

Game::Game()
{
	std::cout << "konstructor Game()" << std::endl;

	screen_width = 1024;
	screen_height = 768;

	running = true;
}


Game::~Game()
{

}

void Game::init()
{
	assert(2 < 5);

	/*
	эт?значен? ст??по умолчани?:
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); - двойно?буффер (рисуно?делает? ?закадровом буфере ?перемещает? на экра?
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	установить размер компоненто?буфера кадр?- 8 би?красного, зеленого, синего ?альф?
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	*/

	// initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) >= 0)
	{
		// SET ATTRIBUTE ONLY after initialize
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1); // on antialiasing sdl
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4); //subsamples for each pixel

		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1); // set to 1 to require hardware acceleration

		SDL_DisplayMode dm;
		SDL_GetDesktopDisplayMode(0, &dm); // if resolution change, this function will return the previous native display mode !!!
		screen_width = 800;// dm.w;
		screen_height = 600; // dm.h;
		std::cout << dm.w << "    " << dm.h << std::endl;

		window = SDL_CreateWindow("Chapter 1",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			screen_width, screen_height,
			SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | 0);

		//SDL_SetWindowOpacity(window, 0.0f); // make window transparent ( prozra4noe )

		if (window != 0)
		{
			glcontext = SDL_GL_CreateContext(window);
			std::cout << "SDL init " << std::endl;
		}
	}
	else
	{
		std::cout << "window create error " << std::endl;
		return; // sdl could not initialize
	}
	SDL_SetWindowFullscreen(window, SDL_WINDOW_OPENGL); 

	glewExperimental = GL_TRUE; //вс?расширен? ?действительным?точкам?вход?буду?выставлены.
	if (glewInit())// if all is init return 0 !! if not init return 1 and if() run
	{
		std::cout << "glew init error !" << std::endl;
	}

	ilInit(); // init image load library
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
	ilEnable(IL_ORIGIN_SET); 
	// OpenGL have the 0.0 coordinate on the bottom y-axis, but images usually have 0.0 at the top of the y-axis.
	ilOriginFunc(IL_ORIGIN_UPPER_LEFT); // 0, 0 textures in upper left side

										// MIX_DEFAULT_FREQUENCY = sample rate = frequensy = speed playing
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4069) == -1)
		cout << "Mixer NOT init !!" << endl;
	Mix_VolumeMusic(1);

	glViewport(0, 0, screen_width, screen_height); // задать прямоугольник окна
	glEnable(GL_DEPTH_TEST); // включаем тест глубин?
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// antialiasing for figures need be do library who created window

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // устанавливае?фоновы?цвет
	glClearDepth(1.0);

	//glMatrixMode(GL_PROJECTION); режи?текуще?матриц?( GL_MODELVIEW- переходи??трехмерный режи?по умолчани?)
	//glLoadIdentity(); // replace the current matrix(modelview ) with the user matrix (GL_PROJECTION)
	//glOrtho(0.0, 920.0, 900.0, 0.0, -1.0, 1.0);  - типа 2?экра?становит?
	//glMatrixMode(GL_MODELVIEW);

	int ff;
	glGetIntegerv(GL_SAMPLE_BUFFERS, &ff); // проверит?чт?включено(ff станет = 1) ?чт?не?ff = 0)?
	//SDL_GL_GetAttribute(SDL_GL_DEPTH_SIZE, &ff);
	std::cout << "GL_CHECK = " << ff << std::endl;


	triangle.init();

}

void Game::handleEvents()
{
	InputHandler::Instance()->updateEvent();
}

void Game::update()
{
	triangle.update();
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	triangle.render();


	glFlush(); // опустошени?буферо? отрисовк?предыдущих команд // не ждет завершен?
	//glFinish(); //ждет пока вс?предыдущие команд?не буду?отрисованы
	SDL_GL_SwapWindow(window);//отобразить только чт?нарисованный кадр на экране.
	 
}

void Game::playSound()
{
	triangle.playSound();
}

void Game::clean()
{
	std::cout << "cleaning " << std::endl;
	running = false;
	SDL_DestroyWindow(window);
	SDL_Quit();

}