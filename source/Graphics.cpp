#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include "Main.h"
#include "Engine3D.h"
#include "Constants.h"
#include "Graphics.h"

void SWAP(int x, int y){ 
    (x)=(x)^(y); (y)=(x)^(y); (x)=(x)^(y); 
}


void drawTriangle(triangle &tri, vec3d &color){
    SDL_SetRenderDrawColor(gRenderer, color.x, color.y, color.z, 255);
    SDL_RenderDrawLine( gRenderer, (int)tri.points[0].x, (int)tri.points[0].y, (int)tri.points[1].x, (int)tri.points[1].y);
    SDL_RenderDrawLine( gRenderer, (int)tri.points[1].x, (int)tri.points[1].y, (int)tri.points[2].x, (int)tri.points[2].y);
    SDL_RenderDrawLine( gRenderer, (int)tri.points[2].x, (int)tri.points[2].y, (int)tri.points[0].x, (int)tri.points[0].y);
}

void fillTriangle(triangle &tri, vec3d &color){
    uint8_t t1x,t2x,y,minx,maxx,t1xp,t2xp;
	bool changed1 = false;
	bool changed2 = false;
	int8_t signx1,signx2,dx1,dy1,dx2,dy2;
	uint8_t e1,e2;
    // Sort vertices
	if ((int)tri.points[0].y>(int)tri.points[1].y) { SWAP((int)tri.points[0].y,(int)tri.points[1].y); SWAP((int)tri.points[0].x,(int)tri.points[1].x); }
	if ((int)tri.points[0].y>(int)tri.points[2].y) { SWAP((int)tri.points[0].y,(int)tri.points[2].y); SWAP((int)tri.points[0].x,x3); }
	if ((int)tri.points[1].y>(int)tri.points[2].y) { SWAP((int)tri.points[1].y,(int)tri.points[2].y); SWAP((int)tri.points[1].x,x3); }

	t1x=t2x=(int)tri.points[0].x; y=(int)tri.points[0].y;   // Starting points

	dx1 = (int8_t)((int)tri.points[1].x - (int)tri.points[0].x); if(dx1<0) { dx1=-dx1; signx1=-1; } else signx1=1;
	dy1 = (int8_t)((int)tri.points[1].y - (int)tri.points[0].y);
 
	dx2 = (int8_t)(x3 - (int)tri.points[0].x); if(dx2<0) { dx2=-dx2; signx2=-1; } else signx2=1;
	dy2 = (int8_t)((int)tri.points[2].y - (int)tri.points[0].y);
	
	if (dy1 > dx1) {   // swap values
        SWAP(dx1,dy1);
		changed1 = true;
	}
	if (dy2 > dx2) {   // swap values
        SWAP(dy2,dx2);
		changed2 = true;
	}
	
	e2 = (uint8_t)(dx2>>1);
    // Flat top, just process the second half
    if((int)tri.points[0].y==(int)tri.points[1].y) goto next;
    e1 = (uint8_t)(dx1>>1);
	
	for (uint8_t i = 0; i < dx1;) {
		t1xp=0; t2xp=0;
		if(t1x<t2x) { minx=t1x; maxx=t2x; }
		else		{ minx=t2x; maxx=t1x; }
        // process first line until y value is about to change
		while(i<dx1) {
			i++;			
			e1 += dy1;
	   	   	while (e1 >= dx1) {
				e1 -= dx1;
   	   	   	   if (changed1) t1xp=signx1;//t1x += signx1;
				else          goto next1;
			}
			if (changed1) break;
			else t1x += signx1;
		}
	// Move line
	next1:
        // process second line until y value is about to change
		while (1) {
			e2 += dy2;		
			while (e2 >= dx2) {
				e2 -= dx2;
				if (changed2) t2xp=signx2;//t2x += signx2;
				else          goto next2;
			}
			if (changed2)     break;
			else              t2x += signx2;
		}
	next2:
		if(minx>t1x) minx=t1x; if(minx>t2x) minx=t2x;
		if(maxx<t1x) maxx=t1x; if(maxx<t2x) maxx=t2x;
	   	lcd_hline(minx, maxx, y);    // Draw line from min to max points found on the y
		// Now increase y
		if(!changed1) t1x += signx1;
		t1x+=t1xp;
		if(!changed2) t2x += signx2;
		t2x+=t2xp;
    	y += 1;
		if(y==(int)tri.points[1].y) break;
		
   }
	next:
	// Second half
	dx1 = (int8_t)(x3 - (int)tri.points[1].x); if(dx1<0) { dx1=-dx1; signx1=-1; } else signx1=1;
	dy1 = (int8_t)((int)tri.points[2].y - (int)tri.points[1].y);
	t1x=(int)tri.points[1].x;
 
	if (dy1 > dx1) {   // swap values
        SWAP(dy1,dx1);
		changed1 = true;
	} else changed1=false;
	
	e1 = (uint8_t)(dx1>>1);
	
	for (uint8_t i = 0; i<=dx1; i++) {
		t1xp=0; t2xp=0;
		if(t1x<t2x) { minx=t1x; maxx=t2x; }
		else		{ minx=t2x; maxx=t1x; }
	    // process first line until y value is about to change
		while(i<dx1) {
    		e1 += dy1;
	   	   	while (e1 >= dx1) {
				e1 -= dx1;
   	   	   	   	if (changed1) { t1xp=signx1; break; }//t1x += signx1;
				else          goto next3;
			}
			if (changed1) break;
			else   	   	  t1x += signx1;
			if(i<dx1) i++;
		}
	next3:
        // process second line until y value is about to change
		while (t2x!=x3) {
			e2 += dy2;
	   	   	while (e2 >= dx2) {
				e2 -= dx2;
				if(changed2) t2xp=signx2;
				else          goto next4;
			}
			if (changed2)     break;
			else              t2x += signx2;
		}	   	   
	next4:

		if(minx>t1x) minx=t1x; if(minx>t2x) minx=t2x;
		if(maxx<t1x) maxx=t1x; if(maxx<t2x) maxx=t2x;
	   	lcd_hline(minx, maxx, y);    // Draw line from min to max points found on the y
		// Now increase y
		if(!changed1) t1x += signx1;
		t1x+=t1xp;
		if(!changed2) t2x += signx2;
		t2x+=t2xp;
    	y += 1;
		if(y>(int)tri.points[2].y) return;
	}
}