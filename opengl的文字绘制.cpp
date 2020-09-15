#include"utils.h"
#include <windows.h>


//Methdo to reshape the game is the screen size changes
void reshape(int w, int h) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glOrtho(0, 750, 750, 0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

bool save_json(Json::Value data, char* name)
{
	Json::StyledWriter writer;
	ofstream os;
	os.open(name);
	os << writer.write(data);
	os.close();
}

Json::Value read_json(char* name)
{
	ifstream is(name, ios::binary);
	if (!is.is_open())
	{
		cout << "open json file failed" << endl;
		return -1;
	}

	Json::Reader reader;
	Json::Value root;



	if (reader.parse(is, root))
	{
	}
	is.close();
	return root;
}



//Method to check if the food has been eaten
bool foodEaten(int x, int y, float pacmanX, float pacmanY) 
{
	if (x >= pacmanX - 16.0 * cos(359 * M_PI / 180.0) && x <= pacmanX + 16.0 * cos(359 * M_PI / 180.0)) {
		if (y >= pacmanY - 16.0 * cos(359 * M_PI / 180.0) && y <= pacmanY + 16.0 * cos(359 * M_PI / 180.0)) {
			return true;
		}
	}
	return false;
}

void setpoint(int x0, int y0, int x, int y)//同时绘制八个点(对称)
{
	glColor3f(1.0f, 1.0f, 0.0);
	glVertex2f((x0 + x), (y0 + y));
	glVertex2f((x0 + y), (y0 + x));
	glVertex2f((x0 + y), (y0 - x));
	glVertex2f((x0 + x), (y0 - y));
	glVertex2f((x0 - x), (y0 - y));
	glVertex2f((x0 - y), (y0 - x));
	glVertex2f((x0 - y), (y0 + x));
	glVertex2f((x0 - x), (y0 + y));
}

void midpointcircle(int x0, int y0, int r)
{
	int x = 0;
	int y = r;
	int d = 1 - r;    
	//glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_POLYGON);
	setpoint(x0, y0, x, y);
	while (x < y)
	{
		if (d < 0)
		{
			d += 2 * x + 3;
		}
		else
		{
			d += 2 * (x - y) + 5;
			y--;
		}
		x++;
		setpoint(x0, y0, x, y);
	}
	glEnd();
	glFlush();
}

//Method to draw the pacman character through consicutive circle algorithm
void drawPacman(float positionX, float positionY, float rotation,int size) 
{
	GLfloat curSizeLine = 3;
	glLineWidth(curSizeLine);
	int x, y;
	glBegin(GL_LINES);
	glColor3f(1.0, 1.0, 0.3);
	for (int k = 0; k < size; k++) {
		x = (float)k / 2.0 * cos((30 + 90 * rotation) * M_PI / 180.0) + (positionX * squareSize);
		y = (float)k / 2.0 * sin((30 + 90 * rotation) * M_PI / 180.0) + (positionY * squareSize);
		for (int i = 30; i < 330; i++) {
			glVertex2f(x, y);
			x = (float)k / 2.0 * cos((i + 90 * rotation) * M_PI / 180.0) + (positionX * squareSize);
			y = (float)k / 2.0 * sin((i + 90 * rotation) * M_PI / 180.0) + (positionY * squareSize);
			glVertex2f(x, y);
		}
	}
	glEnd();
}

//Method to draw the monster character through consecutive circles algorithm
void drawMonster(float positionX, float positionY, float r, float g, float b) 
{
	GLfloat curSizeLine = 3;
	glLineWidth(curSizeLine);
	int x, y;
	glBegin(GL_LINES);
	glColor3f(r, g, b);
	//draw the head
	for (int k = 0; k < 32; k++) {
		x = (float)k / 2.0 * cos(360 * M_PI / 180.0) + (positionX * squareSize);
		y = (float)k / 2.0 * sin(360 * M_PI / 180.0) + (positionY * squareSize);
		for (int i = 180; i <= 360; i++) {
			glVertex2f(x, y);
			x = (float)k / 2.0 * cos(i * M_PI / 180.0) + (positionX * squareSize);
			y = (float)k / 2.0 * sin(i * M_PI / 180.0) + (positionY * squareSize);
			glVertex2f(x, y);
		}
	}
	glEnd();
	//draw body

	glRectf((positionX * squareSize) - 17, positionY * squareSize, (positionX * squareSize) + 15, (positionY * squareSize) + 15);
	glPointSize(3);
	glBegin(GL_POINTS);

	glColor3f(0, 0., 0.);
	////draw eyes and legs
	glVertex2f((positionX * squareSize) - 11, (positionY * squareSize) + 14); //legs
	glVertex2f((positionX * squareSize) - 1, (positionY * squareSize) + 14); //legs
	glVertex2f((positionX * squareSize) + 8, (positionY * squareSize) + 14); //legs

	glColor3f(1, 1., 1.);
	glVertex2f((positionX * squareSize) + 4, (positionY * squareSize) - 3); //eyes
	glVertex2f((positionX * squareSize) - 7, (positionY * squareSize) - 3); //eyes 
	glEnd();

	glPointSize(3);
	glBegin(GL_POINTS);
	glColor3f(0, 0., 1.);
	glVertex2f((positionX * squareSize) + 5, (positionY * squareSize) - 1); //eyes
	glVertex2f((positionX * squareSize) - 7, (positionY * squareSize) - 1); //eyes 
	glEnd();
}

void selectFont(int size, int charset, const char* face)
{
	HFONT hFont = CreateFontA(size, 0, 0, 0, FW_MEDIUM, 0, 0, 0,
		charset, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, face);
	HFONT hOldFont = (HFONT)SelectObject(wglGetCurrentDC(), hFont);
	DeleteObject(hOldFont);
}


void drawString(const char* str, float positionX, float positionY, float r, float g, float b) 
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


		static int isFirstCall = 1;
		static GLuint lists;
		//selectFont(30, DEFAULT_CHARSET, "Tohama");
	glPushMatrix();
	//glRotated(45, 0, 10, 0);
	//	glColor3f(r,g,b);
	//	glRasterPos2f(positionX, positionY);



	//	for (int i = 0; i < strlen(str); i++)
	//		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *(str + i));
	QString text = QString::number(5);
	QFont font;
	font.setPointSize(9);
	QFontMetrics fm(font);
	QPixmap pix(fm.width(text), fm.height());
	QRect rect(0, 0, fm.width(text), fm.height());
	pix.fill(Qt::transparent);
	QPainter painter(&pix);
	painter.setFont(font);
	painter.setPen(Qt::red);
	painter.drawText(rect, text);
	glPopMatrix();

}


