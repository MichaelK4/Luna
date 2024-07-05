#include "Libs.h"

//Luna::real restitution; // Restitution (bounciness) of an object
//Luna::real dynamicFriction;// Dynamic friction (less than static friction)
//Luna::real staticFriction; // Static friction (greater than dynamic friction)
//
Luna::Scene scene(1.0f / 60.0f, 10); // Create a scene with a time step of 1/60 and 10 iterations 
//Luna::Clock clock_time; // Create a clock to keep track of the time
//
//bool showLogo = true; // Show the logo at the start of the program
//bool frameStep = false; // Step through the simulation frame by frame
//bool step = false; // Step through the simulation
//bool Menu = false; // Show the menu
//bool custom = false; // Use custom values for restitution, dynamic friction, and static friction
//bool ChangeSurface = false; // Change the surface material instead of the object material
//
//size_t selectedMaterialIndexObject = 0; // Index for the selected material for the object 
//size_t selectedMaterialIndexSurface = 0; // Index for the selected material for the surface 


//// Mouse callback function for handling mouse clicks
//void mouse(int button, int state, int x, int y)
//{
//	if (Menu || showLogo) return; // If the menu is open or logo is showing, don't add objects 
//
//	// Convert the screen coordinates to simulation coordinates
//	Luna::real x_ = x / 10.0f; 
//	Luna::real y_ = y / 10.0f; 
//
//	if (state == GLUT_DOWN)
//	{
//		switch (button)
//		{
//		case GLUT_LEFT_BUTTON:
//		{
//			// Create and add a random polygon
//			Luna::ShapePolygon poly;
//			size_t count = (size_t)Luna::Random(3, MaxPolyCount);
//			Luna::Vector2* vectors = new Luna::Vector2[count];
//			Luna::real radius = Luna::Random(5, 10);
//			for (size_t i = 0; i < count; i++)
//				vectors[i].Set(Luna::Random(-radius, radius), Luna::Random(-radius, radius));
//			poly.Set(vectors, count);
//			Luna::RigidBody* rb = scene.Add(&poly, (size_t)x_, (size_t)y_, selectedMaterialIndexObject);
//			rb->SetOrient(Luna::Random(-PI, PI)); 
//			std::cout << "Poly: Restitution: " 
//					  << rb->GetRestitution() << " Dynamic Friction: " 
//					  << rb->GetDynamicFriction() << " Static Friction: " 
//					  << rb->GetStaticFriction() << std::endl;
//			delete[] vectors;
//		}
//			break;
//		case GLUT_RIGHT_BUTTON:
//		{
//			// Create and add a random circle
//			Luna::ShapeCircle circle(Luna::Random(1.0f, 5.0f));
//			Luna::RigidBody* rb = scene.Add(&circle, (size_t)x_, (size_t)y_, selectedMaterialIndexObject); 
//			std::cout << "Circle: Restitution: "  
//					  << rb->GetRestitution() << " Dynamic Friction: " 
//					  << rb->GetDynamicFriction() << " Static Friction: " 
//					  << rb->GetStaticFriction() << std::endl;
//		}
//			break;
//		}
//	}
//}
//
//void LunaLoop()
//{
//	// Set up the OpenGL clear color and clear the screen and depth buffer
//	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//	// Set up the orthogonal projection
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	gluOrtho2D(0, 80, 60, 0);
//
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//
//	// Render instructional text on the screen
//	scene.StrOnScene(1, 2, "Press 'Esc' to exit");
//	scene.StrOnScene(1, 4, "Left click add a polygon");
//	scene.StrOnScene(1, 6, "Right click add a circle");
//	scene.StrOnScene(1,8, "Press 'M' to open menu");
//	scene.StrOnScene(1, 10, "Press 'S' to start/stop the simulation, 'F' to frame by frame"); 
//	scene.StrOnScene(1, 12, "Press 'C' to clear the scene");
//
//	static Luna::real accumulator = 0.0f; 
//
//	accumulator += clock_time.Elapsed(); // Get the elapsed time since the last frame
//
//	clock_time.Start(); // Restart the clock 
//
//	// acmulator is the time that has passed since the last frame
//	accumulator = Luna::Clamp(accumulator, 0.0f, 0.1f); // Clamp the accumulator to prevent spiral of death
//
//	// If the accumulator is greater than the time step, step the scene
//	while (accumulator >= deltaTime)
//	{
//		if (!frameStep)
//			scene.Step();
//		else
//			if (step)
//			{
//				scene.Step();
//				step = false;
//			}
//
//		accumulator -= deltaTime;
//	}
//
//	clock_time.Stop(); // Stop the clock
//
//	scene.Render(); // Render the scene
//
//	if(!ChangeSurface)
//		scene.DrawMenu(Menu, custom, ChangeSurface, selectedMaterialIndexObject, &restitution, &dynamicFriction, &staticFriction);
//	else
//		scene.DrawMenu(Menu, custom, ChangeSurface, selectedMaterialIndexSurface, &restitution, &dynamicFriction, &staticFriction);
//	
//	glutSwapBuffers(); // Swap the buffers
//}
//
//// Main loop for the program
//void mainLoop()
//{
//	// Clear the screen and depth buffer
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
//
//	if(showLogo)
//		scene.DisplayLogo(); // Display the logo
//	else
//		LunaLoop(); // Display the simulation
//}
//
//void keyboardArrowKeys(int key, int x, int y)
//{
//	if (Menu)
//	{
//		switch (key)
//		{
//		case GLUT_KEY_RIGHT:
//		{
//			if (!ChangeSurface)
//			{
//				if (!custom && !ChangeSurface)
//					selectedMaterialIndexObject = (selectedMaterialIndexObject + 1) % scene.GetmaterialCountObject();
//			}
//			else
//			{
//				custom = false;
//				selectedMaterialIndexSurface = (selectedMaterialIndexSurface + 1) % scene.GetmaterialCountSurface();
//			}
//		}
//			break;
//		case GLUT_KEY_LEFT:
//		{
//			if (!custom && !ChangeSurface)
//			{
//				selectedMaterialIndexObject = (selectedMaterialIndexObject - 1) % scene.GetmaterialCountObject();
//				if (selectedMaterialIndexObject < 0)
//					selectedMaterialIndexObject = scene.GetmaterialCountObject() - 1;
//			}
//			else if (ChangeSurface)
//			{
//				custom = false;
//				selectedMaterialIndexSurface = (selectedMaterialIndexSurface - 1) % scene.GetmaterialCountSurface();
//				if (selectedMaterialIndexSurface < 0)
//					selectedMaterialIndexSurface = scene.GetmaterialCountSurface() - 1;
//			}
//			break;
//		}
//		}
//	}
//}
//
//// Keyboard callback function for menu interactions
//void KeyboardMenu(unsigned char key, int x, int y)
//{
//	switch (key)
//	{
//	case 'r':
//    case 'R':// change restitution
//	{ 
//		if (custom && !ChangeSurface)
//		{
//			restitution += 0.05f;
//			if (restitution > 1.01f)
//				restitution = 0.05f;
//		}
//	}
//		break;
//	case 'd':
//	case 'D': // change dynamic friction
//	{
//		if (custom && !ChangeSurface)
//		{
//			dynamicFriction += 0.05f;
//			if (dynamicFriction > 1.01f)
//				dynamicFriction = 0.05f;
//		}
//	}
//		break;
//	case 'u':
//	case 'U': // toggle between object and surface material
//	{
//		ChangeSurface = !ChangeSurface;
//	}
//		break;
//	case 's':
//	case 'S': // change static friction
//	{
//		if (custom && !ChangeSurface)
//		{
//			staticFriction += 0.05f;
//			if (staticFriction > 1.01f)
//				staticFriction = 0.05f;
//		}
//	}
//		break;
//	case 'c':
//	case 'C': // reset values to default
//	{
//		if (!ChangeSurface)
//		{
//			restitution = 0.2f;
//			dynamicFriction = 0.3f;
//			staticFriction = 0.5f;
//			selectedMaterialIndexObject = 0;
//		}
//		else
//		{
//			selectedMaterialIndexSurface = 0;
//		}
//	}
//		break;
//	case 't':
//	case 'T': // toggle custom values
//	{
//		if (!ChangeSurface)
//			custom = !custom;
//	}
//		break;
//	case 'n': 
//	case 'N': // Select next material
//	{
//		if (!ChangeSurface)
//		{
//			if (!custom && !ChangeSurface)
//				selectedMaterialIndexObject = (selectedMaterialIndexObject + 1) % scene.GetmaterialCountObject();
//		}
//		else
//		{
//			custom = false;
//			selectedMaterialIndexSurface = (selectedMaterialIndexSurface + 1) % scene.GetmaterialCountSurface();
//		}
//	}
//		break;
//	case 'p': 
//	case 'P': // Select previous material
//	{
//		if (!custom && !ChangeSurface)
//		{
//			selectedMaterialIndexObject = (selectedMaterialIndexObject - 1) % scene.GetmaterialCountObject();
//			if (selectedMaterialIndexObject < 0)
//				selectedMaterialIndexObject = scene.GetmaterialCountObject() - 1;
//		}
//		else if (ChangeSurface)
//		{
//			custom = false;
//			selectedMaterialIndexSurface = (selectedMaterialIndexSurface - 1) % scene.GetmaterialCountSurface();
//			if (selectedMaterialIndexSurface < 0)
//				selectedMaterialIndexSurface = scene.GetmaterialCountSurface() - 1;
//		}
//	}
//		break;
//	case 'm':
//	case 'M': // Close the menu
//	{
//		Menu = false;
//		if (!Menu)
//			frameStep = false;
//	}
//	default:
//	{
//		if(key != 'm' && key != 'M')
//			std::cout << "Wrong key pressed..." << std::endl; 
//	}
//		break;
//	}
//}
//
//// General keyboard callback function 
//void keyboard(unsigned char key, int x, int y)
//{
//	if (Menu)
//		KeyboardMenu(key, x, y);
//	else
//	{
//		switch (key)
//		{
//		case ESC: // Exit the program
//			exit(0);
//			break;
//		case 'm':
//		case 'M': // Open/Close the menu
//			if (!showLogo)
//			{
//				Menu = true;
//				frameStep = true; 
//			}
//			break;
//		case 'c':
//		case 'C': // Clear the scene
//			if (!showLogo)
//				scene.Clear();
//			break;
//		case ' ': // Enter Simulation from Intro Logo Screen
//			if (showLogo)
//			{
//				showLogo = false;
//				glutDisplayFunc(LunaLoop);
//				glutPostRedisplay();
//			}
//			break;
//		case 's':
//		case 'S': // Start/Stop the simulation
//			if (!showLogo)
//				frameStep = frameStep ? false : true;
//			break;
//		case 'f':
//		case 'F': // Step through the simulation frame by frame
//			if (!showLogo)
//				step = true;
//			break;
//		default:
//			std::cout << "Wrong key pressed..." << std::endl;
//			break;
//		}
//	}
//}

void mainLoop()
{
	scene.mainLoop();
}

void keyboardArrowKeys(int key, int x, int y)
{
	scene.keyboardArrowKeys(key, x, y); 
}

void LunaLoop() 
{
	scene.LunaLoop();
}

void keyboard(unsigned char key, int x, int y)
{
	if (scene.getIsShowLogo() && key == ' ')
	{
		scene.setShowLogo(false);  
		glutDisplayFunc(LunaLoop);
		glutPostRedisplay();
	}
	else
		scene.keyboard(key, x, y); 
}

void mouse(int button, int state, int x, int y) 
{
	scene.mouse(button, state, x, y);  
} 

// Main function
int main(int argc, char* argv[]) 
{
	glutInit(&argc, argv); // Initialize GLUT
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE); // Set up a basic display buffer (double buffered)

	// Set window position and size
	int screenWidth = glutGet(GLUT_SCREEN_WIDTH);
	int screenHeight = glutGet(GLUT_SCREEN_HEIGHT); 
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(350, 100);
	glutCreateWindow("Luna2D - Physics Engine"); // Set the window title

	srand(time(NULL)); // Seed the random number generator 

	scene.LoadLogoTexture("Luna2D-Logo.bmp"); // Load the logo texture

	// Set up GLUT callback functions
	glutDisplayFunc(mainLoop);  
	glutKeyboardFunc(keyboard); 
	glutSpecialFunc(keyboardArrowKeys);
	glutMouseFunc(mouse);
	glutIdleFunc(mainLoop); 

	// Set up OpenGL projection
	glMatrixMode(GL_PROJECTION); 
	glPushMatrix(); 
	glLoadIdentity(); 
	gluOrtho2D(0, 80, 60, 0); 
	glMatrixMode(GL_MODELVIEW); 
	glPushMatrix(); 
	glLoadIdentity();

	scene.Init(); // Set up OpenGL

	scene.ShowOnDisplay();
	
	glutMainLoop(); // Enter the GLUT main loop

	scene.Clear(); // Clean up the program 
	return 0; 
}