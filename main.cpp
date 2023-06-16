#include <iostream>
#include <windows.h>
#include <chrono>
#include <thread>
#include "UNIT_VECTORS_3D.h"

int px = 100 << 10;
int py = 10 << 10;
int pz = 30 << 10;
int pt = 90 * 3;
int pf = 0;

struct bot {
	int x, y, z;
	int l_div2, w_div2, h_div2;
	int id;
	unsigned long long task;
	bot* next;
	bot* prev;
};

int map[800][800][300];
int screen[128 * 72];


void raycast() {

	int theta = (888 + pt) % 1080;
	int phi = (324 + pf) % 360;

	theta = theta + 1080 * phi;
	unsigned int theta1 = theta + 1;
	unsigned int theta2 = theta1 + 1;
	int d = 0;
	unsigned int scrpx = 0;
	unsigned int tmap = 0;
	unsigned int tx=0, ty=0, tz=0;

	for (int i = 0; i < 72; i++) {
		for (int j = 0; j < 128; j++) {
			d = 0;
			tx = px>>1;
			ty = py>>1;
			tz = pz>>1;
			tmap = 0;
			/*
			while (d < 64) {
				tx += unitv[theta];
				ty += unitv[theta1];
				tz += unitv[theta2];
				tmap = (map[tx >> 10][ty >> 10][tz >> 10]) & 0xffffff;
				if (tmap != 0) {
					screen[scrpx] = tmap;
					d = 2000;
				}
				d += 1;
			}
			tx = tx >> 1;
			ty = ty >> 1;
			tz = tz >> 1;
			*/
			while (d < 96) {
				tx += unitv[theta];
				ty += unitv[theta1];
				tz += unitv[theta2];
				tmap = (map[tx >> 9][ty >> 9][tz >> 9]) & 0xffffff;
				if (tmap != 0) {
					screen[scrpx] = tmap;
					d = 2000;
				}
				d += 2;
			}
			tx = tx >> 1;
			ty = ty >> 1;
			tz = tz >> 1;
			while (d < 1400) {
				tx += unitv[theta];
				ty += unitv[theta1];
				tz += unitv[theta2];
				tmap = map[tx >> 8][ty >> 8][tz >> 8] & 0xffffff;
				if (tmap != 0) {
					screen[scrpx] = tmap;
					d = 2000;
				}
				d += 4;
			}
			theta += 3;
			theta = theta % 388800;
			theta1 = theta + 1;
			theta2 = theta1 + 1;
			scrpx++;
		}
		theta += 696;
		theta = theta % 388800;
		theta1 = theta + 1;
		theta2 = theta1 + 1;
	}

}

void flybot(bot* p) {
	if (p->task == 1) {

		int dx, dy, dz;
		dx = (px >> 10) - p->x;
		dy = (py >> 10) - p->y;
		dz = (pz >> 10) - p->z;
		int diff[3] = { dx,dy,dz };
		int min = 0;
		char dirx = 0, diry = 2, dirz = 4;

		p->task = p->task << 3;

		if (dx < 0) {
			dx = -dx;
			dirx = 1;
		}
		if (dy < 0) {
			dy = -dy;
			diry = 3;
		}
		if (dz < 0) {
			dz = -dz;
			dirz = 5;
		}

		min = dx < dy ? dx : dy;
		min = min < dz ? min : dz;
		min = min < 1 ? 1 : min;

		for (int i = 0; i < 20; i++) {
			if ((diff[0] <= 0) && (diff[1] <= 0) && (diff[2] <= 0)) {
				diff[0] += dx;
				diff[1] += dy;
				diff[2] += dz;
			}
			p->task = p->task << 3;
			if (diff[0] > diff[1]) {
				if (diff[1] > diff[2]) {
					diff[0] -= min;
					p->task |= dirx;
				}
				else {
					diff[2] -= min;
					p->task |= dirz;
				}
			}
			else if (diff[1] > diff[2]) {
				diff[1] -= min;
				p->task |= diry;
			}
			else {
				diff[2] -= min;
				p->task |= dirz;
			}
		}

	}
	switch (p->task & 7) {
	case 0:
		p->task = p->task >> 3;
		p->x+=2;
		p->x += p->l_div2;
		for (int i = p->y - p->w_div2; i < p->y + p->w_div2; i++) {
			for (int j = p->z - p->h_div2; j < p->z + p->h_div2; j++) {
				if (map[p->x][i][j] != 0) {
					p->x-=2;
					p->x -= p->l_div2;
					return;
				}
			}
		}
		p->x--;
		p->x -= p->l_div2;
		return;
	case 1:
		p->task = p->task >> 3;
		p->x-=2;
		p->x -= p->l_div2;
		for (int i = p->y - p->w_div2; i < p->y + p->w_div2; i++) {
			for (int j = p->z - p->h_div2; j < p->z + p->h_div2; j++) {
				if (map[p->x][i][j] != 0) {
					p->x+=2;
					p->x += p->l_div2;
					return;
				}
			}
		}
		p->x++;
		p->x += p->l_div2;
		return;
	case 2:
		p->task = p->task >> 3;
		p->y+=2;
		p->y += p->w_div2;
		for (int i = p->z - p->h_div2; i < p->z + p->h_div2; i++) {
			for (int j = p->x - p->l_div2; j < p->x + p->l_div2; j++) {
				if (map[j][p->y][i] != 0) {
					p->y-=2;
					p->y -= p->w_div2;
					return;
				}
			}
		}
		p->y--;
		p->y -= p->w_div2;
		return;
	case 3:
		p->task = p->task >> 3;
		p->y-=2;
		p->y -= p->w_div2;
		for (int i = p->z - p->h_div2; i < p->z + p->h_div2; i++) {
			for (int j = p->x - p->l_div2; j < p->x + p->l_div2; j++) {
				if (map[j][p->y][i] != 0) {
					p->y+=2;
					p->y += p->w_div2;
					return;
				}
			}
		}
		p->y++;
		p->y += p->w_div2;
		return;
	case 4:
		p->task = p->task >> 3;
		p->z+=2;
		p->z += p->h_div2;
		for (int i = p->x - p->l_div2; i < p->x + p->l_div2; i++) {
			for (int j = p->y - p->w_div2; j < p->y + p->w_div2; j++) {
				if (map[i][j][p->z] != 0) {
					p->z-=2;
					p->z -= p->h_div2;
					return;
				}
			}
		}
		p->z--;
		p->z -= p->h_div2;
		return;
	case 5:
		p->task = p->task >> 3;
		p->z-=2;
		p->z -= p->h_div2;
		for (int i = p->x - p->l_div2; i < p->x + p->l_div2; i++) {
			for (int j = p->y - p->w_div2; j < p->y + p->w_div2; j++) {
				if (map[i][j][p->z] != 0) {
					p->z+=2;
					p->z += p->h_div2;
					return;
				}
			}
		}
		p->z++;
		p->z += p->h_div2;
		return;
	default:
		p->task = p->task >> 3;
		return;
	}
}

void erasebot(bot* p) {
	for (int i = p->x - p->l_div2; i < p->x + p->l_div2; i++) {
		for (int j = p->y - p->w_div2; j < p->y + p->w_div2; j++) {
			for (int k = p->z - p->h_div2; k < p->z + p->h_div2; k++) {
				if ((map[i][j][k] & 0xFF000000) == p->id) map[i][j][k] = 0;
			}
		}
	}
}

void drawbot(bot* p) {
	for (int i = p->x - p->l_div2; i < p->x + p->l_div2; i++) {
		for (int j = p->y - p->w_div2; j < p->y + p->w_div2; j++) {
			for (int k = p->z - p->h_div2; k < p->z + p->h_div2; k++) {
				map[i][j][k] = p->id | 0xFFFFFF;
			}
		}
	}
}

void move() {
	if (GetKeyState('W') & 0x8000)
	{
		px += unitv[pt + pf * 1080]>>1;
		if (map[px >> 10][py >> 10][pz >> 10] != 0) {
			px -= unitv[pt + pf * 1080]>>1;
		}
		py += unitv[pt + pf * 1080 + 1]>>1;
		if (map[px >> 10][py >> 10][pz >> 10] != 0) {
			py -= unitv[pt + pf * 1080 + 1]>>1;
		}
		pz += unitv[pt + pf * 1080 + 2]>>1;
		if (map[px >> 10][py >> 10][pz >> 10] != 0) {
			pz -= unitv[pt + pf * 1080 + 2]>>1;
		}
	}
	if (GetKeyState('S') & 0x8000)
	{
		px -= unitv[pt + pf * 1080] >> 1;
		if (map[px >> 10][py >> 10][pz >> 10] != 0) {
			px += unitv[pt + pf * 1080] >> 1;
		}
		py -= unitv[pt + pf * 1080 + 1] >> 1;
		if (map[px >> 10][py >> 10][pz >> 10] != 0) {
			py += unitv[pt + pf * 1080 + 1] >> 1;
		}
		pz -= unitv[pt + pf * 1080 + 2] >> 1;
		if (map[px >> 10][py >> 10][pz >> 10] != 0) {
			pz += unitv[pt + pf * 1080 + 2] >> 1;
		}
	}
	if (GetKeyState('A') & 0x8000)
	{
		px -= unitv[pt + pf * 1080 + 1] >> 1;
		if (map[px >> 10][py >> 10][pz >> 10] != 0) {
			px += unitv[pt + pf * 1080 + 1] >> 1;
		}
		py += unitv[pt + pf * 1080] >> 1;
		if (map[px >> 10][py >> 10][pz >> 10] != 0) {
			py -= unitv[pt + pf * 1080] >> 1;
		}
	}
	if (GetKeyState('D') & 0x8000)
	{
		px += unitv[pt + pf * 1080+1] >> 1;
		if (map[px >> 10][py >> 10][pz >> 10] != 0) {
			px -= unitv[pt + pf * 1080+1] >> 1;
		}
		py -= unitv[pt + pf * 1080] >> 1;
		if (map[px >> 10][py >> 10][pz >> 10] != 0) {
			py += unitv[pt + pf * 1080] >> 1;
		}
	}
	if (GetKeyState(VK_LEFT) & 0x8000) {
		pt += 6;
		pt = pt % 1080;
	}
	if (GetKeyState(VK_RIGHT) & 0x8000) {
		pt += 1074;
		pt = pt % 1080;
	}
	if (GetKeyState(VK_UP) & 0x8000) {
		pf += 2;
		pf = pf % 360;
	}
	if (GetKeyState(VK_DOWN) & 0x8000) {
		pf += 358;
		pf = pf % 360;
	}
	/*
	if (GetKeyState('S') & 0x8000)
	{
		x -= unitv[z];
		if (map[x >> 10][y >> 10] != 1) {
			x += unitv[z];
		}
		y -= unitv[z + 1];
		if (map[x >> 10][y >> 10] != 1) {
			y += unitv[z + 1];
		}
	}
	if (GetKeyState('A') & 0x8000)
	{
		x += unitv[z + 1];
		if (map[x >> 10][y >> 10] != 1) {
			x -= unitv[z + 1];
		}
		y -= unitv[z];
		if (map[x >> 10][y >> 10] != 1) {
			y += unitv[z];
		}
	}
	if (GetKeyState('D') & 0x8000)
	{
		x -= unitv[z + 1];
		if (map[x >> 10][y >> 10] != 1) {
			x += unitv[z + 1];
		}
		y += unitv[z];
		if (map[x >> 10][y >> 10] != 1) {
			y -= unitv[z];
		}
	}*/

}

void initmap() {
	
	for (int i = 4; i < 196; i++) {
		for (int j = 4; j < 196; j++) {
			for (int k = 4; k < 196; k++) {
				map[i][j][k] = 0;
			}
		}
	}

	for (int i = 0; i < 200; i++) {
		for (int j = 0; j < 200; j++) {
			for (int k = 0; k < 5; k++) {
				map[i][j][k] = 0x0000af;
			}
		}
	}
	for (int i = 0; i < 200; i++) {
		for (int j = 0; j < 200; j++) {
			for (int k = 195; k < 200; k++) {
				map[i][j][k] = 0x00aF00;
			}
		}
	}
	for (int i = 0; i < 200; i++) {
		for (int j = 0; j < 5; j++) {
			for (int k = 0; k < 200; k++) {
				map[i][j][k] = 0xaF0000;
			}
		}
	}
	for (int i = 0; i < 200; i++) {
		for (int j = 195; j < 200; j++) {
			for (int k = 0; k < 200; k++) {
				map[i][j][k] = 0xafaf00;
			}
		}
	}
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 200; j++) {
			for (int k = 0; k < 200; k++) {
				map[i][j][k] = 0xaf00af;
			}
		}
	}
	for (int i = 195; i < 200; i++) {
		for (int j = 0; j < 200; j++) {
			for (int k = 0; k < 200; k++) {
				map[i][j][k] = 0x00afaf;
			}
		}
	}
	
	/*
	for (int i = 4; i < 196; i++) {
		for (int j = 4; j < 796; j++) {
			for (int k = 4; k < 196; k++) {
				map[i][j][k] = 0;
			}
		}
	}
	for (int i = 0; i < 200; i++) {
		for (int j = 0; j < 800; j++) {
			for (int k = 0; k < 5; k++) {
				map[i][j][k] = 1;
			}
		}
	}
	for (int i = 0; i < 200; i++) {
		for (int j = 0; j < 800; j++) {
			for (int k = 195; k < 200; k++) {
				map[i][j][k] = 1;
			}
		}
	}
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 800; j++) {
			for (int k = 0; k < 200; k++) {
				map[i][j][k] = 1;
			}
		}
	}
	for (int i = 195; i < 200; i++) {
		for (int j = 0; j < 800; j++) {
			for (int k = 0; k < 200; k++) {
				map[i][j][k] = 1;
			}
		}
	}
	for (int i = 0; i < 200; i++) {
		for (int j = 795; j < 800; j++) {
			for (int k = 0; k < 200; k++) {
				map[i][j][k] = 1;
			}
		}
	}
	for (int i = 0; i < 200; i++) {
		for (int j = 0; j < 5; j++) {
			for (int k = 0; k < 200; k++) {
				map[i][j][k] = 1;
			}
		}
	}
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 800; j++) {
			for (int k = 50; k < 150; k++) {
				map[i][j][k] = 0xa0a0a0;
			}
		}
	}
	for (int i = 195; i < 200; i++) {
		for (int j = 0; j < 800; j++) {
			for (int k = 50; k < 150; k++) {
				map[i][j][k] = 0xa0a0a0;
			}
		}
	}
	for (int i = 0; i < 200; i++) {
		for (int j = 795; j < 800; j++) {
			for (int k = 50; k < 150; k++) {
				map[i][j][k] = 0xa0a0a0;
			}
		}
	}
	for (int i = 0; i < 200; i++) {
		for (int j = 0; j < 5; j++) {
			for (int k = 50; k < 150; k++) {
				map[i][j][k] = 0xa0a0a0;
			}
		}
	}
	*/
}

int main() {

	initmap();

	bot bots[2];
	bots[0].prev = NULL;
	bots[0].next = &bots[1];
	bot* nextbot;

	bots[1].prev = bots;
	bots[1].next = NULL;
	bots[1].x = 50;
	bots[1].y = 50;
	bots[1].z = 50;
	bots[1].l_div2 = 2;
	bots[1].w_div2 = 2;
	bots[1].h_div2 = 2;
	bots[1].id = 1 << 24;
	bots[1].task = 1;

	HDC hdc = GetDC(GetConsoleWindow());
	HDC buf = CreateCompatibleDC(hdc);
	COLORREF* frameBuffer = (COLORREF*)calloc(128 * 72 * 25, sizeof(COLORREF));
	HBITMAP hbitmap;

	std::chrono::high_resolution_clock::time_point t1;
	std::chrono::high_resolution_clock::time_point t2;

	while (1) {
		t1 = std::chrono::high_resolution_clock::now();

		raycast();

		int i5 = 0;
		int is = 128*72-1;
		for (int h = 0; h < 72; h++) {
			for (int i = 0; i < 128; i++) {
				for (int ii = 0; ii < 5; ii++) {
					frameBuffer[i5] = screen[is];
					frameBuffer[i5 + 128 * 5] = screen[is];
					frameBuffer[i5 + 128 * 5 * 2] = screen[is];
					frameBuffer[i5 + 128 * 5 * 3] = screen[is];
					frameBuffer[i5 + 128 * 5 * 4] = screen[is];
					i5++;
				}
				is--;
			}
			i5 += 128 * 5 * 4;
		}

		move();

		nextbot = bots->next;
		while (nextbot != NULL) {
			erasebot(nextbot);
			flybot(nextbot);
			drawbot(nextbot);
			nextbot = nextbot->next;
		}
		

		hbitmap = CreateBitmap(128 * 5, 72 * 5, 1, 32, (void*)frameBuffer);
		SelectObject(buf, hbitmap);
		BitBlt(hdc, 0, 0, 128 * 5, 72 * 5, buf, 0, 0, SRCCOPY);

		std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
		std::this_thread::sleep_for(std::chrono::microseconds(16666) - (t2 - t1));
	}


	DeleteObject(hbitmap);
	while (1);
	return 0;

}