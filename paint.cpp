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

	int startx = 50;
	int starty = 100;

	int padding = 5;

	int colors[MAXCOLORS+1] = { BLACK, BLUE, GREEN, CYAN,
							  RED, MAGENTA, BROWN, LIGHTGRAY,
							  DARKGRAY, LIGHTBLUE, LIGHTGREEN, LIGHTCYAN,
							  LIGHTRED, LIGHTMAGENTA, YELLOW, WHITE };

public:

	void render();
	inline int getColor(int, int);
	int clickEvent(int, int);
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

int ColorPallete::clickEvent(int mx, int my) {
	return getColor(mx, my);
}

inline int ColorPallete::getColor(int mx, int my) {
	return getpixel(mx, my);
}


class Canvas {
private:
	int width, height;
	int startx, starty;
	
	int points[10] = {0};

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
	}

	inline void clear() {
		setfillstyle(SOLID_FILL, BLACK);
		fillpoly(5, points);
	}

	friend class PaintApp;
};


class Pen {
private:
	int penColor;
	int penSize;

public:
	bool penDown;
	bool eraser;

	Pen() : penColor(WHITE), penDown(false), eraser(false), penSize(10) {}

	
	inline void togglePen() { penDown = ( penDown ) ? false : true; }
	inline void toggleEraser() { eraser = (eraser) ? false : true; penColor = ( penColor == BLACK ) ? WHITE : BLACK; }

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
};


class PaintApp {
private:
	int padding = 5;

	typedef enum status { EXIT, CONTINUE, CLEAR_CANVAS } status;

	status s = CONTINUE;

	Canvas* canvas;
	ColorPallete* cp;

	Pen* pen;

public:
	PaintApp() {
		canvas = new Canvas();
		cp = new ColorPallete();
		pen = new Pen();
	}
	
	inline void render() {
		canvas->render();
		cp->render();
	}
	
	inline void resetColor() { setcolor(WHITE); };

	void mainloop() {
		initwindow(1280, 720, "CG PROJECT : Paint App");

		render();

		do {
			int mx, my;

			mx = mousex();
			my = mousey();


			if ( pen->penDown && ( mx > canvas->startx && mx < canvas->startx + canvas->width ) && ( my > canvas->starty && my < canvas->starty + canvas->height ) ) {
				pen->draw(mx, my);
			}

			handleKeys();
			handleStatus();
			setcolor(WHITE);
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
			pen->toggleEraser();
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
			canvas->clear();
			s = CONTINUE;
			break;

		default:
			break;
		}
	}

};


int main() {
	PaintApp* p = new PaintApp();
	p->mainloop();
	return 0;
}
