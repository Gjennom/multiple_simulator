#pragma once

#include <iostream>
#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"
#include <fstream>
#include <vector>
#include <unordered_map>

/*	This program will use raylib to implement circuits in a 
 *	was that looks exactly like multisim.
 *
 *	Important features:
 *		UI in the style of multisim
 *		Agilent DMM, full functionality and appearence
 *		The ability to build and simulate simple resistive
 *			circuits, with voltage sources, current sources,
 *			and dependant sources of both kinds
 *		The ability to build and simulate logic circuits
 *
 *	Less important features:
 *		Logic to analog circuit conversion
 *		Some sort of logic auto-generation based either
 *			on a truth table or a boolean expression
 *		Algabraic logic circuit simplification
 *		Non resistive circuit elements (i.e. capacitors, transistors)
 *		The ability to save and open files
 *		Other stuff too maybe idk (animations?)
 *	
 *	Structure:
 *		UI:
 *			All components are sprites with a rotation
 *			There are two visual parts: The "canvas",
 *			where components are added and we can zoom in / out
 *			/ move around to see stuff, and the "interface";
 *			where we have buttons and can drag and drop components
 *			from.
 *
 *			There will be some "mobile interfaces", like the
 *			agilent DMM, that can be moved around the screen,
 *			and will have interactable buttons that can be pressed.
 *
 *		Simulation:
 *			0) Will check to see if circuit has changed since
 *			last simulated, if so:
 *			1) Will search nodes and make a list of essential
 *			nodes (3+ connections), reference will be grounded
 *			2) Will account for dependant sources and other
 *			special cases before applying KCL
 *			3) Will solve system of equations using linear
 *			algebra
 *			4) Will solve voltages across each component,
 *			and current through each component
 *			5) Will display result to DMM if / where connected
 *
 *		Storage:
 *			Files will be stored as plain text, bc idk how to
 *				do binary encoding
 *			Format will be:
 *				1) list of variables and the resistances / 
 *				other qualities associated with them
 *				e.g.:
 *					R1 10 ohm 100 50 N
 *					R2 40 ohm x y S
 *					R3 5 kohm x y E
 *					S1 5 volt x y W // last letter == 
 *						orientation in cardinal dir,
 *						+ is west for s1
 *
 *				2) then the list of line segments that 
 *				form the connectors:
 *					L1 1 50 100 100
 *					L2 x1 x2 y1 y2
 *
 *				3) then a list of nodes / connections from
 *				either node to line segment or component
 *				to line segment:
 *					N1 100 50
 *					N2 x y
 *
 *			Reading from file:
 *				1) make a vector to store a list of 
 *				each type of component, e.g. a vector
 *				for resistors, a vector for sources, a vector
 *				for line segments, and a vector for nodes.
 *				2) solve systems of line segments to
 *				map nodes to the components they connect to:
 *					{N1, S1+, R1} // connects positive of
 *					S1 to R1
 *				3) Draws everything
 *
 *
 *		Building:
 *			Will be a list of components on LHS
 *			Drag and drop components to place
 *			Variable values of components(voltage and such)
 *				can be edited manually like text
 *			There will be a component struct that contains things
 *				like:
 *				Any and all variable values
 *				Voltage across
 *				Current through
 *				Orientation
 *				Node / line segment connections
 *				X position
 *				Y position
 *
 */

/*
int resistor_width = 60;
int resistor_height = 12;
int source_width = 36;
int source_height = 48;
int node_radius = 5;
int connector_width = 2;
*/

enum Orientation{
	NORTH,
	SOUTH,
	EAST,
	WEST
};

struct Resistor{
	int x; // center x of sprite
	int y; // center y of sprite
	Orientation orientation; 
	float resistance;
};

struct Source{
	int x; // center x of sprite
	int y; // center y of sprite
	Orientation orientation; // direction of positive terminal
};

struct Node{
	int x;
	int y;
	float voltage;
};

struct Connection{
	int start_x;
	int end_x;
	int start_y;
	int end_y;
	float current;
};

struct Region{
	int x_top;
	int x_bottom;
	int y_top;
	int y_bottom;
};


bool is_cursor_in_x(const Region & region);

void get_input(Camera2D & camera, const std::unordered_map<std::string, Region>& Regions);

