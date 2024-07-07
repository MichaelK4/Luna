# Luna2D Physics Engine

![Alt text](<Luna2D Logo.jpeg>)

## Table of Contents
- [Description](#description)
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
  - [Controls](#controls)
  - [Menu Controls](#menu-controls)
    - [Object Settings](#object-settings)
    - [Custom Object Settings](#custom-object-settings)
    - [Surface Settings](#surface-settings)
- [Project Structure](#project-structure)
- [Measurements and Results for Evaluating the System’s Performance](#measurements-and-results-for-evaluating-the-systems-performance)
- [Recommendations for Further Development](#recommendations-for-further-development)
- [License](#license)

## Description
Luna2D is a 2D physics engine developed using C++ and OpenGL. It simulates basic physical phenomena such as gravity, collision, friction, and rigid body dynamics. This project is part of a bachelor's degree in computer science at Afeka - The Academic College of Engineering in Tel Aviv, and aims to provide an educational tool for understanding physics simulations in two-dimensional space.

## Features
- Simulates gravity, collisions, friction, and rigid body dynamics.
- Integrated with OpenGL for visualization and user interaction.
- Demonstrates fundamental physics principles through realistic simulations.
- Supports elastic collisions and various material properties.
- Interactive UI for modifying simulation settings.

## Installation
1. Clone the repository:
    ```sh
    git clone https://github.com/MichaelK4/Luna2D-Physics-Engine.git
    ```
2. Open `Luna.sln` in Visual Studio.
3. Build the solution.
4. Run the executable from the Visual Studio or navigate to the `Luna2D` folder and run the executable directly.

*Note: Ensure that you are running on Windows (x86).* 

## Usage

### Controls

- **'Space'**: Start the Luna Engine.
- **'Esc'**: Exit the program.
- **Left Click**: Add polygon to scene.
- **Right Click**: Add circle.
- **'M'**: Open menu.
- **'S'**: Stop simulation, **'F'**: Go frame by frame.
- **'C'**: Clear the scene of any object other than static ones.

### Menu Controls
- **'M'**: Close menu.
- **'C'**: Reset to default settings.
- **'U'**: Change surface/object settings.

#### Object Settings
- **'T'**: Switch to custom settings.
- **Left/Right Arrow**: Navigate predefined object settings.

#### Custom Object Settings
- **'T'**: Switch back to predefined settings.
- **'R'**: Adjust Restitution.
- **'D'**: Adjust Dynamic Friction.
- **'S'**: Adjust Static Friction.

#### Surface Settings
- **Left/Right Arrow**: Change the surface material.

## Project Structure
- `main.cpp`: Entry point of the application, sets up the simulation and rendering loop.
- `Vector2.cpp/h`: Implements a 2D vector class with basic vector operations.
- `Mat2.cpp/h`: Implements a 2D matrix class for matrix operations.
- `Material.h`: Defines material properties like restitution and friction.
- `Shape.h`: Abstract base class for different shapes (Circle, Polygon).
- `Manifold.cpp/h`: Handles collision detection and resolution between shapes.
- `Collider.cpp/h`: Manages collision data and interaction between shapes.
- `RigidBody.cpp/h`: Represents physical objects with properties like mass, velocity, and forces.
- `Clock.cpp/h`: Utility class for timing and frame rate control.
- `Scene.cpp/h`: Manages all objects and interactions within the simulation environment.
- `Libs.h`: Includes various utility functions and constants.

## Measurements and Results for Evaluating the System’s Performance
- **Demonstration of Physics Principles**: Accurate representation of mathematical functions and physical interactions.
- **Elastic Collision Implementation**: Mostly accurate collision mechanics with minor visual inconsistencies.
- **Core Components Development**: Stable performance on Windows with no bugs during test sessions.
- **OpenGL Integration**: Effective rendering with minor visual issues when rapidly creating objects.

## Recommendations for Further Development
- Create a website for easier access and complex simulations.
- Improve and make the UI more interactive.
- Develop comprehensive tutorials and documentation.
- Establish a community forum for user support.
- Create an API for the system.
- Enhance data visualization and provide detailed information on physical phenomena.
- Run simulations on powerful servers for better performance.
- Add options for adjusting gravity, air resistance, and realistic weather effects.
- Consider using a more powerful library than OpenGL.
- Prevent object overlap during creation.
- Add a 3D simulation environment option.

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE.txt) file for details. 