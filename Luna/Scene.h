#ifndef SCENE_H
#define SCENE_H
#include "Mat2.h"
#include "Clock.h"

namespace Luna
{
	class Scene
	{
	private:
		real deltaTime; //time between frames
		size_t iterations; //number of iterations for the physics engine
		std::vector<RigidBody*> bodies; //list of rigid bodies in the scene
		std::vector<Manifold> contacts; //list of contacts in the scene
		GLuint logoTexture; //texture for the logo

		real restitution; // Restitution (bounciness) of an object
		real dynamicFriction;// Dynamic friction (less than static friction)
		real staticFriction; // Static friction (greater than dynamic friction)

		Clock clock_time; // Create a clock to keep track of the time  

		bool showLogo = true; // Show the logo at the start of the program
		bool frameStep = false; // Step through the simulation frame by frame
		bool step = false; // Step through the simulation
		bool Menu = false; // Show the menu
		bool custom = false; // Use custom values for restitution, dynamic friction, and static friction
		bool ChangeSurface = false; // Change the surface material instead of the object material
		bool clickedCustomParameters = false; // Clicked the custom parameters button

		short selectedMaterialIndexObject = 0; // Index for the selected material for the object
		short selectedMaterialIndexSurface = 0; // Index for the selected material for the surface

	public:
		Scene(); //constructor
		Scene(real deltaTime, size_t iterations); //constructor
		~Scene(); //destructor

		RigidBody* surfaceRB; //rigid body for the surface 

		int materialCountObject = 0;  
		int materialCountSurface = 0; 
		std::vector<Material*> objectMaterials; 
		std::vector<Material*> surfaceMaterials; 

		void Step(); //step the physics engine
		void Render(); //render the scene
		RigidBody* Add(Shape* shape, size_t x, size_t y); //add a rigid body to the scene
		RigidBody* Add(Shape* shape, size_t x, size_t y, size_t i);
		void Clear(); //clear the scene

		std::vector<RigidBody*> GetBodies() { return bodies; } //get the list of rigid bodies

		void StrOnScene(int x, int y, const char* s, bool isSim); //draw a string on the scene
		void DrawMenu(/*bool menu, bool custom, bool surface, size_t selectedMaterialIndex,*/ real* restitution, real* dynamicFriction, real* staticFriction); //draw the menu 
		void LoadLogoTexture(const char* filename); //load the logo texture 
		unsigned char* LoadBMP(const char* filename, int& width, int& height); //load a BMP file 
		void DisplayLogo(); //display the logo

		GLuint GetLogoTexture() { return logoTexture; } //get the logo texture

		void ShowOnDisplay(); //show the scene on the display
		void DisplayHelpMenu(); //display the help menu  
		void Init(); //initialize OpenGL
		
		void AddMaterialObject(const std::string& name, real restitution, real dynamicFriction, real staticFriction); // add a material to the object 
		void AddMaterialSurface(const std::string& name, real restitution, real dynamicFriction, real staticFriction); // add a material to the surface
		void AddMaterials(); // add materials to the scene
		
		// get the number of materials for the object 
		short GetmaterialCountObject() { return materialCountObject; }

		// get the number of materials for the surface
		short GetmaterialCountSurface(){ return materialCountSurface; }

		bool getIsShowLogo() { return showLogo; } //get the value of showLogo

		void setShowLogo(bool l) { this->showLogo = l; } //set the value of showLogo

		// Mouse callback function for handling mouse clicks
		void mouse(int button, int state, int x, int y);
		// Luna loop for the program
		void LunaLoop();
		// Main loop for the program
		void mainLoop();
		void keyboardArrowKeys(int key, int x, int y);
		// Keyboard callback function for menu interactions
		void KeyboardMenu(unsigned char key, int x, int y);
		// General keyboard callback function 
		void keyboard(unsigned char key, int x, int y);
	}; 
}

#endif // ! SCENE_H