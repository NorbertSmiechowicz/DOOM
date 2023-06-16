#include <iostream>
#include <windows.h>
#include <chrono>
#include <thread>

int x = 0;
int y = 0;
int z = 0;

int map[1000][1000];
int screen[3][640];

int unitv[720] = { 1024,0,1023,17,1023,35,1022,53,1021,71,1020,89,1018,107,1016,124,1014,142,1011,160,1008,177,1005,195,1001,212,997,230,993,247,989,265,984,282,979,299,973,316,968,333,
962,350,955,366,949,383,942,400,935,416,928,432,920,448,912,464,904,480,895,496,886,512,877,527,868,542,858,557,848,572,838,587,828,601,817,616,806,630,795,644,
784,658,772,671,760,685,748,698,736,711,724,724,711,736,698,748,685,760,671,772,658,784,644,795,630,806,616,817,601,828,587,838,572,848,557,858,542,868,527,877,
511,886,496,895,480,904,464,912,448,920,432,928,416,935,400,942,383,949,366,955,350,962,333,968,316,973,299,979,282,984,265,989,247,993,230,997,212,1001,195,1005,
177,1008,160,1011,142,1014,124,1016,107,1018,89,1020,71,1021,53,1022,35,1023,17,1023,0,1024,-17,1023,-35,1023,-53,1022,-71,1021,-89,1020,-107,1018,-124,1016,-142,1014,-160,1011,
-177,1008,-195,1005,-212,1001,-230,997,-247,993,-265,989,-282,984,-299,979,-316,973,-333,968,-350,962,-366,955,-383,949,-400,942,-416,935,-432,928,-448,920,-464,912,-480,904,-496,895,
-512,886,-527,877,-542,868,-557,858,-572,848,-587,838,-601,828,-616,817,-630,806,-644,795,-658,784,-671,772,-685,760,-698,748,-711,736,-724,724,-736,711,-748,698,-760,685,-772,671,
-784,658,-795,644,-806,630,-817,616,-828,601,-838,587,-848,572,-858,557,-868,542,-877,527,-886,512,-895,496,-904,480,-912,464,-920,448,-928,432,-935,416,-942,400,-949,383,-955,366,
-962,350,-968,333,-973,316,-979,299,-984,282,-989,265,-993,247,-997,230,-1001,212,-1005,195,-1008,177,-1011,160,-1014,142,-1016,124,-1018,107,-1020,89,-1021,71,-1022,53,-1023,35,-1023,17,
-1024,0,-1023,-17,-1023,-35,-1022,-53,-1021,-71,-1020,-89,-1018,-107,-1016,-124,-1014,-142,-1011,-160,-1008,-177,-1005,-195,-1001,-212,-997,-230,-993,-247,-989,-265,-984,-282,-979,-299,-973,-316,-968,-333,
-962,-350,-955,-366,-949,-383,-942,-400,-935,-416,-928,-432,-920,-448,-912,-464,-904,-480,-895,-496,-886,-511,-877,-527,-868,-542,-858,-557,-848,-572,-838,-587,-828,-601,-817,-616,-806,-630,-795,-644,
-784,-658,-772,-671,-760,-685,-748,-698,-736,-711,-724,-724,-711,-736,-698,-748,-685,-760,-671,-772,-658,-784,-644,-795,-630,-806,-616,-817,-601,-828,-587,-838,-572,-848,-557,-858,-542,-868,-527,-877,
-512,-886,-496,-895,-480,-904,-464,-912,-448,-920,-432,-928,-416,-935,-400,-942,-383,-949,-366,-955,-350,-962,-333,-968,-316,-973,-299,-979,-282,-984,-265,-989,-247,-993,-230,-997,-212,-1001,-195,-1005,
-177,-1008,-160,-1011,-142,-1014,-124,-1016,-107,-1018,-89,-1020,-71,-1021,-53,-1022,-35,-1023,-17,-1023,0,-1024,17,-1023,35,-1023,53,-1022,71,-1021,89,-1020,107,-1018,124,-1016,142,-1014,160,-1011,
177,-1008,195,-1005,212,-1001,230,-997,247,-993,265,-989,282,-984,299,-979,316,-973,333,-968,350,-962,366,-955,383,-949,400,-942,416,-935,432,-928,448,-920,464,-912,480,-904,496,-895,
511,-886,527,-877,542,-868,557,-858,572,-848,587,-838,601,-828,616,-817,630,-806,644,-795,658,-784,671,-772,685,-760,698,-748,711,-736,724,-724,736,-711,748,-698,760,-685,772,-671,
784,-658,795,-644,806,-630,817,-616,828,-601,838,-587,848,-572,858,-557,868,-542,877,-527,886,-512,895,-496,904,-480,912,-464,920,-448,928,-432,935,-416,942,-400,949,-383,955,-366,
962,-350,968,-333,973,-316,979,-299,984,-282,989,-265,993,-247,997,-230,1001,-212,1005,-195,1008,-177,1011,-160,1014,-142,1016,-124,1018,-107,1020,-89,1021,-71,1022,-53,1023,-35,1023,-17};

struct npc {
	unsigned int x;
	unsigned int y;
	int dx;
	int dy;
	unsigned int task;
	int id;
	npc* next;
	npc* prev;
};

int distproj(int d) {
	if (d > 660) {
		return (18895735 - 8951 * d) >> 11; // *2^18*5 = *2^11*640
	}
	if (d > 380) {
		return (39392787 - 40007 * d) >> 11;
	}/*
	if (d > 200) {
		return (76826503 - 150106 * d) >> 11;
	}
	if (d > 100) {
		return (141230080 - 507904 * d) >> 11;
	}
	if (d > 60) {
		return (247726080 - 1572864 * d) >> 11;
	}
	if (d > 40) {
		return (385351680 - 3866624 * d) >> 11;
	}*/
	if (d > 15) {
		return 4608000 / d;
	}
	return 294400;
}

void initMap() {

	x = 50 << 10;
	y = 200 << 10;
	z = 540;

	for (int i = 0; i < 1000; i++) {
		for (int j = 0; j < 1000; j++) {
			map[i][j] = 0;
		}
	}
	for (int i = 5; i < 100; i++) {
		for (int j = 5; j < 995; j++) {
			map[i][j] = 1;
		}
	}
	for (int i = 100; i < 600; i++) {
		for (int j = 400; j < 800; j++) {
			map[i][j] = 1;
		}
	}
	for (int i = 400; i < 440; i++) {
		for (int j = 600; j < 660; j++) {
			map[i][j] = 0;
		}
	}
}

void refresh() {
	int tx;
	int ty;
	int tz = (z + 592) % 720;
	int i;
	for (int t = 0; t < 128; t++) {
		tx = x;
		ty = y;
		i = 1;
		screen[1][t] = 0;
		while (i < 64) {
			tx += unitv[tz];
			ty += unitv[tz | 1];
			switch (map[tx >> 10][ty >> 10])
			{
			case 1:
				break;
			case 0:
				screen[0][t] = i;
				i = 2000;
				break;
			default:
				if (screen[1][t] == 0) screen[1][t] = i;
				break;
			}
			i++;
		}
		tx = tx >> 1;
		ty = ty >> 1;
		while (i < 256) {
			tx += unitv[tz];
			ty += unitv[tz | 1];
			switch (map[tx >> 9][ty >> 9])
			{
			case 1:
				break;
			case 0:
				screen[0][t] = i;
				i = 2000;
				break;
			default:
				if (screen[1][t] == 0) screen[1][t] = i;
				break;
			}
			i += 2;
		}

		tx = tx >> 1;
		ty = ty >> 1;
		while (i < 2000) {
			tx += unitv[tz];
			ty += unitv[tz | 1];
			switch (map[tx >> 8][ty >> 8])
			{
			case 1:
				break;
			case 0:
				screen[0][t] = i;
				i = 2000;
				break;
			default:
				if(screen[1][t]==0) screen[1][t] = i;
				break;
			}
			i += 4;
		}

		tz += 2;
		if (tz >= 720) tz = tz % 720;
	}
}

void shoot(npc* p) {
	int tx = x>>1;
	int ty = y>>1;
	npc* temp = 0;
	int id = 0;
	int ttx = 0;
	int tty = 0;
	for (int i = 0; i < 400; i++) {
		tx += unitv[z];
		ty += unitv[z | 1];
		switch (map[tx >> 9][ty >> 9])
		{
		case 1:
			break;
		case 0:
			return;
		default:
			tx = tx >> 9;
			ty = ty >> 9;
			id = map[tx][ty];
			temp = p + id - 1;
			(temp)->prev->next = (temp)->next;
			if(temp->next != NULL) (temp)->next->prev = (temp)->prev;
			temp->task = 0;
			temp->x = 0;
			temp->y = 0;
			tx = tx < 6 ? 0 : tx - 6;
			ttx = tx > 994 ? 1000 : tx + 12;
			ty = ty < 6 ? 0 : ty - 6;
			tty = ty > 994 ? 1000 : ty + 12;
			for (int ii = tx; ii < ttx; ii++)
			{
				for (int jj = ty; jj < tty; jj++)
				{
					map[ii][jj] = map[ii][jj] == id ? 1 : map[ii][jj];
				}
			}
			return;
		 break;
		}
	}
}

void move(npc* p, int* trigger) {
	if (GetKeyState('W') & 0x8000)
	{
		x += unitv[z];
		if (map[x >> 10][y >> 10] != 1) {
			x -= unitv[z];
		}
		y += unitv[z + 1];
		if (map[x >> 10][y >> 10] != 1) {
			y -= unitv[z + 1];
		}
	}
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
	}

	if (x < 4096) x = 4096;
	if (x > (996 << 10)) x = (996 << 10);
	if (y < 4096) y = 4096;
	if (y > (996 << 10)) y = (996 << 10);

	if (GetKeyState(VK_RIGHT) & 0x8000)
	{
		z += 6;
		z = z % 720;
	}
	if (GetKeyState(VK_LEFT) & 0x8000)
	{
		z += 714;
		z = z % 720;
	}
	if ((GetKeyState(VK_UP) & 0x8000)&&*trigger==0)
	{
		shoot(p);
		*trigger = 21;
	}
}

void movenpc(npc* p) {
	if (p->task == 0) {
		int dirx = 0;
		int diry = 1;
		int mean;
		p->dx = (x >> 10) - p->x;
		p->dy = (y >> 10) - p->y;
		p->task = 1;
		if (p->dx < 0) {
			dirx = 2;
			p->dx = -p->dx;
		}
		if (p->dy < 0) {
			diry = 3;
			p->dy = -p->dy;
			p->task = 3;
		}
		mean = p->dx;
		for (int i = 0; i < 15; i++) {
			p->task = p->task << 2;
			if (mean > 0) {
				mean -= p->dy;
				p->task += dirx;
			}
			else {
				mean += p->dx;
				p->task += diry;
			}
		}
	}
	switch (p->task & 3) {
	case 0:
		p->task = p->task >> 2;
		if (p->x > 995) return;
		if (map[p->x + 4][p->y + 3] != 1) return;
		if (map[p->x + 4][p->y - 3] != 1) return;
		p->x++;
		for (int i = p->y - 2; i < p->y + 3; i++) {
			map[p->x - 3][i] = 1;
			map[p->x + 2][i] = p->id;
		}
		break;
	case 1:
		p->task = p->task >> 2;
		if (p->y > 995) return;
		if (map[p->x + 3][p->y + 4] != 1) return;
		if (map[p->x - 3][p->y + 4] != 1) return;
		p->y++;
		for (int i = p->x - 2; i < p->x + 3; i++) {
			map[i][p->y - 3] = 1;
			map[i][p->y + 2] = p->id;
		}
		break;
	case 2:
		p->task = p->task >> 2;
		if (p->x < 5) return;
		if (map[p->x - 4][p->y + 3] != 1) return;
		if (map[p->x - 4][p->y - 3] != 1) return;
		p->x--;
		for (int i = p->y - 2; i < p->y + 3; i++) {
			map[p->x + 3][i] = 1;
			map[p->x - 2][i] = p->id;
		}
		break;
	case 3:
		p->task = p->task >> 2;
		if (p->y < 5) return;
		if (map[p->x + 3][p->y - 4] != 1) return;
		if (map[p->x - 3][p->y - 4] != 1) return;
		p->y--;
		for (int i = p->x - 2; i < p->x + 3; i++) {
			map[i][p->y + 3] = 1;
			map[i][p->y - 2] = p->id;
		}
		break;
	}
}

void updateFrame(COLORREF* frameBuffer) {
	int walldwn;
	int wallup;
	int npcdwn;
	int npchat;
	int wi = 0;
	int twi = 0;
	for (int i = 0; i < 128; i++) {

		walldwn = 153600 + distproj(screen[0][i]);
		walldwn = walldwn > 307200 ? 307200 : walldwn;
		wallup = 307200 - walldwn;

		npcdwn = screen[1][i] != 0 ? 153600 + distproj(screen[1][i]) : -1;
		npchat = npcdwn == -1 ? -1 : 360*640 - (npcdwn>>1);

		npcdwn = npcdwn > 307200 ? 307200 : npcdwn;
		npchat = npchat < 0 ? 0 : npchat;

		for (int locali = 0; locali < 5; locali++) {
			wi = twi;
			if (npchat < wallup) {
				while (wi < npchat) {
					frameBuffer[wi] = 0;
					wi += 640;
				}
				while (wi < npcdwn) {
					frameBuffer[wi] = 0xF0F0F0;
					wi += 640;
				}
			}
			while (wi < wallup) {
				frameBuffer[wi] = 0;
				wi += 640;
			}
			while (wi < npchat) {
				frameBuffer[wi] = 0x808080;
				wi += 640;
			}
			while (wi < npcdwn) {
				frameBuffer[wi] = 0xF0F0F0;
				wi += 640;
			}
			while (wi < walldwn) {
				frameBuffer[wi] = 0x808080;
				wi += 640;
			}
			while (wi < 640 * 480) {
				frameBuffer[wi] = 0;
				wi += 640;
			}
			twi++;
		}
	}

	frameBuffer[640 * 240 + 316] = 0xFFFFFF;
	frameBuffer[640 * 240 + 317] = 0xFFFFFF;
	frameBuffer[640 * 240 + 318] = 0xFFFFFF;
	frameBuffer[640 * 240 + 322] = 0xFFFFFF;
	frameBuffer[640 * 240 + 323] = 0xFFFFFF;
	frameBuffer[640 * 240 + 324] = 0xFFFFFF;

	frameBuffer[640 * 236 + 320] = 0xFFFFFF;
	frameBuffer[640 * 237 + 320] = 0xFFFFFF;
	frameBuffer[640 * 238 + 320] = 0xFFFFFF;
	frameBuffer[640 * 242 + 320] = 0xFFFFFF;
	frameBuffer[640 * 243 + 320] = 0xFFFFFF;
	frameBuffer[640 * 244 + 320] = 0xFFFFFF;
}

int main() {

	initMap();

	npc stack[6];

	stack[5].next = NULL;
	stack[5].prev = &stack[4];
	stack[5].x = 90;
	stack[5].y = 30;
	stack[5].task = 0;
	stack[5].id = 6;

	stack[0].next = &stack[1];
	stack[0].prev = NULL;
	stack[0].x = 10;
	stack[0].y = 0;
	stack[0].task = 0;
	stack[0].id = 1;

	for (int i = 1; i < 5; i++) {
		stack[i].next = &stack[i + 1];
		stack[i].prev = &stack[i - 1];
		stack[i].x = 10 + 20 * (i-1);
		stack[i].y = 30;
		stack[i].task = 0;
		stack[i].id = i+1;
	}

	npc* next = stack[0].next;

	int walldwn = 0;
	int wallup = 0;
	int npcdwn = 0;
	int npcup = 0;
	int wi = 0;
	int trigger = 0;

	HDC hdc = GetDC(GetConsoleWindow());
	HDC buf = CreateCompatibleDC(hdc);
	COLORREF* frameBuffer = (COLORREF*) calloc(640 * 480, sizeof(COLORREF));
	HBITMAP hbitmap;
	std::chrono::high_resolution_clock::time_point t1;
	std::chrono::high_resolution_clock::time_point t2;

	while (1) {
		t1 = std::chrono::high_resolution_clock::now();
		
		refresh();

		updateFrame(frameBuffer);

		hbitmap = CreateBitmap(640, 480, 1, 32, (void*)frameBuffer);
		SelectObject(buf, hbitmap);
		BitBlt(hdc, 0, 0, 640, 480, buf, 0, 0, SRCCOPY);	
		
		while (next != NULL) {
			movenpc(next);
			next = next->next;
		}
		next = stack[0].next;

		if (trigger > 0) trigger--;
		move(stack,&trigger);

		std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
		std::this_thread::sleep_for(std::chrono::microseconds(16666) - (t2 - t1));
	}
	DeleteObject(hbitmap);
	return 0;
}
