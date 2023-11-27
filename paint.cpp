#include <iostream>
#include <graphics.h>
using namespace std;


void printArray(int n, int* arr) {
	for ( int i = 0 ; i < n ; i++ )
		cout << arr[i] << " ";
	cout << endl;
}


class ColorPallete {
private:
	int boxH = 20;
	int boxW = 20;

	int startx = 40;
	int starty = 100;

	int padding = 5;
	
	int colors[14] = { BLACK, BLUE, GREEN, CYAN,
					 RED, MAGENTA, BROWN, LIGHTBLUE, 
					 LIGHTGREEN, LIGHTCYAN, LIGHTRED, LIGHTMAGENTA, 
					 YELLOW, DARKGRAY };

public:

	void render();
	int getColor(int, int);
};

void ColorPallete::render() {
	int xpos = startx;
	int ypos = starty;
	int cols = 1;

	for ( int i = 0 ; i < sizeof(colors)/sizeof(colors[0]) ; i++ ) {
		int points[10] = { xpos, ypos, 
						xpos+boxW, ypos, 
						xpos+boxW, ypos+boxH, 
						xpos, ypos+boxH, 
						xpos, ypos };

		setcolor(BLACK);
		drawpoly(5, points);
		setfillstyle(SOLID_FILL, colors[i]);
		fillpoly(5, points);

		xpos += boxW + padding;
		cols++;

		if (cols == 3) {
			xpos = startx;
			ypos += boxH + padding;
			cols = 1;
		}

	}
}


int ColorPallete::getColor(int mx, int my) {
	int c = getpixel(mx, my);
	int cols = 2;
	int rows = sizeof(colors)/sizeof(colors[0])/cols + 1;

	// Check if its inside the box
	if ( ( mx > startx && mx < (startx + boxW*cols + padding) ) &&
		( my > starty && my < (starty + (boxH + padding)*rows) ) )
		return c;
	else
		return -1;
}


class Canvas {
private:
	int width, height;
	int startx, starty;
	
	int points[10] = {0};

	int canvasColor = WHITE;

	void initPoints() {
		points[0] = startx; 
		points[1] = starty;
		points[2] = startx + width;
		points[3] = starty;
		points[4] = startx+width;
		points[5] = starty+height;
		points[6] = startx;
		points[7] = starty+height;
		points[8] = startx;
		points[9] = starty;
	}

public:
	Canvas() : width(1024), height(600), startx(100), starty(100) {
		initPoints();
	}
	Canvas(int w, int h) : width(w), height(h) {}

	inline void render(int x = 100, int y = 100) {
		drawpoly(5, points);
		setfillstyle(SOLID_FILL, canvasColor);
		fillpoly(5, points);
	}

	inline void clear() {
		setfillstyle(SOLID_FILL, canvasColor);
		fillpoly(5, points);
	}

	inline bool canDraw(int x, int y) {
		return ( ( x > startx && x < startx + width ) && 
			( y > starty && y < starty + height ) ) 
			? true 
			: false;
	}

	friend class PaintApp;
};


class Pen {
private:
	int penColor;
	int penSize;
	bool penDown;
	bool eraser;

public:
	Pen() : penColor(BLACK), penDown(false), eraser(false), penSize(10) {}

	
	inline void togglePen() { penDown = ( penDown ) ? false : true; }
	inline void toggleEraser(int c) { eraser = (eraser) ? false : true; penColor = ( penColor == c ) ? BLACK : c; }

	inline int getColor() { return penColor; }
	inline void setColor(int c) { penColor = c; }

	void draw(int x, int y) {
		int points[10] = {
			x, y,
			x + penSize, y,
			x + penSize, y + penSize,
			x, y + penSize,
			x, y,
		};

		setcolor(penColor);
		setfillstyle(SOLID_FILL, penColor);

		drawpoly(5, points);
		fillpoly(5, points);
	}

	friend class PaintApp;
};


class PaintApp {
private:
	int width = 1280;
	int height = 720;

	int padding = 5;

	int backgroundColor = DARKGRAY;

	typedef enum status { EXIT, CONTINUE, DRAW, CLEAR_CANVAS } status;

	int mx, my;

	status s = CONTINUE;

	Canvas* canvas;
	ColorPallete* cp;
	Pen* pen;

public:
	PaintApp() {
		canvas = new Canvas;
		cp = new ColorPallete;
		pen = new Pen;
		shapes = new Shapes;
	}
	
	inline void render() {
		int points[] = {
			0, 0,
			width, 0,
			width, height,
			0, height,
			0, 0
		};
		setfillstyle(SOLID_FILL, backgroundColor);
		fillpoly(5, points);
		canvas->render();
		cp->render();
	}
	
	inline void resetColor() { setcolor(canvas->canvasColor); };

	void mainloop() {
		initwindow(width, height, "CG PROJECT : Paint App");

		render();

		do {
			handleMouse();
			handleKeys();
			handleStatus();
		} while( s );

		closegraph();
	}

	void handleKeys() {
		int key = 0;

		if ( kbhit() )
			key = getch();
		
		switch ( key ) {
		case 99:
			s = CLEAR_CANVAS;
			break;

		case 100:
			pen->togglePen();
			break;

		case 101:
			pen->toggleEraser(canvas->canvasColor);
			break;

		case 113:
			s = EXIT;
			break;

		default:
			break;
		}
	}

	void handleStatus() {
		switch ( s ) {
		case CLEAR_CANVAS:
			clearviewport();
			render();
			break;

		case DRAW:
			pen->draw(mx, my);
			break;

		default:
			break;
		}
		if (s != EXIT) s = CONTINUE;
	}

	void handleMouse() {
		bool clicked = ismouseclick(WM_LBUTTONDOWN);
		clearmouseclick(WM_LBUTTONDOWN);

		mx = mousex();
		my = mousey();

		if ( pen->penDown && canvas->canDraw(mx, my) ) {
			s = DRAW;
			return;
		}

		if (clicked) {
			int c = cp->getColor(mx, my);
			if ( c != -1 )
				pen->setColor(c);
		}
	}

};


int main() {
	PaintApp* p = new PaintApp();
	p->mainloop();
	return 0;
}
