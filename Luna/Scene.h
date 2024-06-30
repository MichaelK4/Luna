#ifndef  SCENE_H
#define SCENE_H
//#include "Shape.h"
#include "Mat2.h"

namespace Luna
{


	class Scene
	{
	private:
		real deltaTime; //time between frames
		uint iterations; //number of iterations for the physics engine
		std::vector<RigidBody*> bodies; //list of rigid bodies in the scene
		std::vector<Manifold> contacts; //list of contacts in the scene
		GLuint logoTexture; //texture for the logo

	public:
		Scene();
		Scene(real deltaTime, uint iterations);
		~Scene();

		RigidBody* surfaceRB; //rigid body for the surface

		//struct Material 
		//{
		//	std::string name; 
		//	real restitution; 
		//	real dynamicFriction; 
		//	real staticFriction; 
		//}; 

		//Material materials[]; 
		int materialCountObject = 0;  
		int materialCountSurface = 0; 
		std::vector<Material*> objectMaterials; 
		std::vector<Material*> surfaceMaterials; 

		void Step(); //step the physics engine
		void Render(); //render the scene
		RigidBody* Add(Shape* shape, uint x, uint y); //add a rigid body to the scene
		RigidBody* Add(Shape* shape, uint x, uint y, size_t i);
		//SoftBody* AddSoft(Shape* shape, uint x, uint y);
		void Clear(); //clear the scene

		std::vector<RigidBody*> GetBodies() { return bodies; } //get the list of rigid bodies

		void StrOnScene(int x, int y, const char* s); //draw a string on the scene
		void DrawMenu(bool menu, bool custom, bool surface, int selectedMaterialIndex, real* restitution, real* dynamicFriction, real* staticFriction); //draw the menu
		void LoadLogoTexture(const char* filename); //load the logo texture 
		unsigned char* LoadBMP(const char* filename, int& width, int& height); //load a BMP file
		void DisplayLogo(); //display the logo

		GLuint GetLogoTexture() { return logoTexture; } //get the logo texture

		void ShowOnDisplay(); //show the scene on the display
		void DisplayHelpMenu(); //display the help menu 
		void Init(); //initialize OpenGL
		
		void AddMaterialObject(const std::string& name, real restitution, real dynamicFriction, real staticFriction);
		void AddMaterialSurface(const std::string& name, real restitution, real dynamicFriction, real staticFriction);
		
		int GetmaterialCountObject()    
		{
			return materialCountObject;  
		}

		int GetmaterialCountSurface()  
		{
			return materialCountSurface;  
		}

		void AddMaterials();
	}; 
}

#endif // ! SCENE_H



