/*
 * Snake2d.cpp
 *
 *  Created on: Jul 25, 2016
 *      Author: kenny
 */
#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#include <list>
#include <iostream>
//#include <thread>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <sstream>
int foodx, foody;
int xdim, ydim;
int windowX, windowY;
float menuy;
int stepTime;
enum Direction { up, right, down, left };
enum Status {alive, dead, complete};
Status s;
Direction d;//, prevdir;
time_t start;
struct Pair{
	Pair(int X, int Y){
		x = X;
		y = Y;
	}
	int x;
	int y;
};
std::list<Pair> snake;

bool isSnake(int x, int y){
	for(std::list<Pair>::iterator i = snake.begin(); i != snake.end(); i++){
		if((*i).x == x && (*i).y == y){
			return true;
		}
	}
	return false;
}

void printToScreen( float x, float y, std::string s)
{
int i;
glColor3f(1.0f, 1.0f, 1.0f);
glRasterPos2f( x, y); // location to start printing text
for( i=0; i < static_cast<int>(s.size()); i++) // loop until i is greater then l
{
glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, s[i]); // Print a character on the screen
}

}

/* Handler for window-repaint event. Call back when the window first appears and
   whenever the window needs to be re-painted. */
void display() {

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_QUADS);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2f(-1.0f, 1.0f);    // x, y
	glVertex2f(-1.0f, 1.0f  - menuy);
	glVertex2f(1.0f, 1.0f - menuy);
	glVertex2f(1.0f, 1.0f);
	glEnd();
	float xspace = 2.0f / (windowX >= windowY ? xdim : ydim);
	float yspace = (2.0f - menuy) / (windowX >= windowY ? xdim : ydim);
	//TODO add gameover and game complete using status s
	for(int i = 0; i < xdim; i++){
		for(int j = 0; j < ydim; j++){
			glBegin(GL_QUADS);
			if(i == foodx && j == foody){
				glColor3f(1.0f, 0.0f, 0.0f);
			}
			else if(isSnake(i,j)){
				glColor3f(0.0f, 1.0f, 0.0f);
				//std::cout << "here" << std::endl;
			}
			else{
				glColor3f(0.0f, 0.0f, 1.0f);
			}
			/*
			std::cout << xdim<< "," << ydim << ",";
std::cout << xspace << "," << yspace << ",";
std::cout << -1.0f + i * xspace;
std::cout << ",";
std::cout << 1.0f - j * yspace <<std::endl;
*/
			glVertex2f(-1.0f + i * xspace, 1.0f - menuy - j * yspace);    // x, y
			glVertex2f(-1.0f + i * xspace, 1.0f - menuy - (j + 1) * yspace);
			glVertex2f(-1.0f + (i+1) * xspace, 1.0f - menuy - (j + 1) * yspace);
			glVertex2f(-1.0f + (i+1) * xspace, 1.0f - menuy - j * yspace);
			glEnd();
			glLineWidth(.1f);
			glColor3f(0.0, 0.0, 0.0);
			glBegin(GL_LINE_STRIP);
			glVertex2f(-1.0f + i * xspace, 1.0f - menuy - j * yspace);    // x, y
			glVertex2f(-1.0f + i * xspace, 1.0f - menuy - (j + 1) * yspace);
			glVertex2f(-1.0f + (i+1) * xspace, 1.0f - menuy - (j + 1) * yspace);
			glVertex2f(-1.0f + (i+1) * xspace, 1.0f - menuy - j * yspace);
			glVertex2f(-1.0f + i * xspace, 1.0f - menuy - j * yspace);    // x, y
			glEnd();

			glEnd();
		}
	}
           // Clear the color buffer (background)

   // Draw a Red 1x1 Square centered at origin
                 // Each set of 4 vertices form a quad
       // Red

time_t temp = time(NULL);

char buffer[256];  // make sure this is big enough!!!
snprintf(buffer, sizeof(buffer), "%g", difftime(temp, start));


std::ostringstream strs();

std::string str(buffer);


printToScreen(-1.0f ,1.0f - menuy, str);
	if(s == dead){
		//TODO figure out a better way to center text.
		printToScreen(-0.5f + menuy ,0.0f - (menuy / 2) - menuy, std::string("Game over"));
		printToScreen(-0.5f ,0.0f - (menuy / 2) - 2 * menuy, std::string("Space to Restart"));
	}
	if(s == complete){
			printToScreen(-0.75f + menuy ,0.0f - (menuy / 2) - menuy, std::string("You Win!"));
			printToScreen(-0.75f ,0.0f - (menuy / 2) - 2 * menuy, std::string("Space to Restart"));
	}
glFlush();  // Render now
}

void idle(int val){
	//std::cout <<"idle" <<std::endl;
//	while(1){
		//sleep(1);
		Pair temp(snake.front().x, snake.front().y);
		switch(d){
		case up:
			temp.y--;
			break;
		case right:
			temp.x++;
			break;
		case down:
			temp.y++;
			break;
		case left:
			temp.x--;
			break;
		}
		if(isSnake(temp.x, temp.y)){
			s = dead;
			glutPostRedisplay();
			return;
		}
		if(temp.x < 0 || temp.x >= xdim || temp.y < 0 || temp.y >= ydim){
			s = dead;
			glutPostRedisplay();
			return;
		}

		snake.push_front(temp);
		if(foodx == temp.x && foody ==temp.y){
			if(snake.size() == xdim * ydim){
				s = complete;
				glutPostRedisplay();
				return;
			}
			while(isSnake(foodx, foody)){
				foodx = std::rand() % xdim;
				foody = std::rand() % ydim;
			}
		}
		else{
			snake.pop_back();
		}
		glutTimerFunc(val, idle, val);
		glutPostRedisplay();
//	}
}

void reset(){
	std::srand(time(NULL));
		d = up;
		//prevdir = down;
		s = alive;
		menuy = .1f; //TODO actually calculate how tall menu should be
		xdim = 15;
		ydim = 15;
		foodx = std::rand() % xdim;
		foody = std::rand() % xdim;
	windowX = 500;
	windowY = 500;
	windowY += static_cast<int>(windowY * menuy);
	stepTime = 500;
	start = time(NULL);
		while(isSnake(foodx, foody)){
						foodx = rand() % xdim;
						foody = rand() % ydim;
		}
		snake.clear();
		snake.push_back(Pair(xdim / 2, ydim / 2));
		glutTimerFunc(stepTime, idle, stepTime);
		glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y){
    float alpha = 1.0;
    switch(key){
    	case 'w':
    		d = up;
    		break;
    	case 's':
    		d = down;
    		break;
    	case 'a':
    		d = left;
    		break;
    	case 'd':
    		d = right;
    		break;
    	case ' ':
    		if(s != alive){
    			reset();
    		}
    		break;

    	}
}

void SpecialInput(int key, int x, int y){
	switch(key){
	case GLUT_KEY_UP:
		d = up;
		break;
	case GLUT_KEY_DOWN:
		d = down;
		break;
	case GLUT_KEY_LEFT:
		d = left;
		break;
	case GLUT_KEY_RIGHT:
		d = right;
		break;
	}
}

void reshape(int width, int height){
	windowX = 500;
	windowY = 500;
	windowY += static_cast<int>(windowY * menuy);
}



/* Main function: GLUT runs as a console application starting at main()  */
main(int argc, char** argv){
	int window1,window2;
	std::srand(time(NULL));
	d = up;
	s = alive;
	menuy = .1f; //TODO actually calculate how tall menu should be
	xdim = 15;
	ydim = 15;
	foodx = std::rand() % xdim;
	foody = std::rand() % xdim;
windowX = 500;
windowY = 500;
windowY += static_cast<int>(windowY * menuy);
stepTime = 500;
start = time(NULL);
	while(isSnake(foodx, foody)){
					foodx = rand() % xdim;
					foody = rand() % ydim;
	}

	snake.push_back(Pair(xdim / 2, ydim / 2));

   glutInit(&argc, argv);
   // Initialize GLUT
   //window1 = glutCreateWindow("Snake options");
   glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
   glutInitWindowSize(windowX, windowY);   // Set the window's initial width & height
   window2 = glutCreateWindow("Snake"); // Create a window with the given title
   //std::thread thread(idle);
   glutIdleFunc(NULL);
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutSpecialFunc(SpecialInput);
   glutTimerFunc(stepTime, idle, stepTime);
   glutDisplayFunc(display); // Register display callback handler for window re-paint
   glutMainLoop();           // Enter the event-processing loop
   return 0;
}





