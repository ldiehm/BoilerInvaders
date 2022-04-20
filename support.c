#include "stm32f0xx.h"
#include <stdint.h>
#include <stdlib.h>
#include "lcd.h"
#include "midi.h"
#include "midiplay.h"

static void nano_wait(unsigned int n) {
    asm(    "        mov r0,%0\n"
            "repeat: sub r0,#83\n"
            "        bgt repeat\n" : : "r"(n) : "r0", "cc");
}


// Copy a subset of a large source picture into a smaller destination.
// sx,sy are the offset into the source picture.
void pic_subset(Picture *dst, const Picture *src, int sx, int sy)
{
    int dw = dst->width;
    int dh = dst->height;
    for(int y=0; y<dh; y++) {
        if (y+sy < 0)
            continue;
        if (y+sy >= src->height)
            break;
        for(int x=0; x<dw; x++) {
            if (x+sx < 0)
                continue;
            if (x+sx >= src->width)
                break;
            dst->pix2[dw * y + x] = src->pix2[src->width * (y+sy) + x + sx];
        }
    }
}

// Overlay a picture onto a destination picture.
// xoffset,yoffset are the offset into the destination picture that the
// source picture is placed.
// Any pixel in the source that is the 'transparent' color will not be
// copied.  This defines a color in the source that can be used as a
// transparent color.
void pic_overlay(Picture *dst, int xoffset, int yoffset, const Picture *src, int transparent)
{
    for(int y=0; y<src->height; y++) {
        int dy = y+yoffset;
        if (dy < 0)
            continue;
        if (dy >= dst->height)
            break;
        for(int x=0; x<src->width; x++) {
            int dx = x+xoffset;
            if (dx < 0)
                continue;
            if (dx >= dst->width)
                break;
            unsigned short int p = src->pix2[y*src->width + x];
            if (p != transparent)
                dst->pix2[dy*dst->width + dx] = p;
        }
    }
}

extern const Picture background; // A 240x320 background image
extern const Picture player;
extern const Picture goodBullet;
extern const Picture bulletShadow;
extern const Picture badGuy;

// This C macro will create an array of Picture elements.
// Really, you'll just use it as a pointer to a single Picture
// element with an internal pix2[] array large enough to hold
// an image of the specified size.
// BE CAREFUL HOW LARGE OF A PICTURE YOU TRY TO CREATE:
// A 100x100 picture uses 20000 bytes.  You have 32768 bytes of SRAM.
#define TempPicturePtr(name,width,height) Picture name[(width)*(height)/6+2] = { {width,height,2} }

void erase(int x, int y)
{
    TempPicturePtr(tmp,29,29); // Create a temporary 29x29 image.
    pic_subset(tmp, &background, x-tmp->width/2, y-tmp->height/2); // Copy the background
    //pic_overlay(tmp, 5,5, &player, 0xffff); // Overlay the ball
    LCD_DrawPicture(x-tmp->width/2,y-tmp->height/2, tmp); // Draw
}

void update(int x, int y, int Value)
{
    if (x < 228 && x > 15 && Value == 2) {
        LCD_DrawPicture(x-player.width/2,y-player.height/2, &player); // Draw the player
    }
    else if(Value == 1) {
        LCD_DrawPicture(x-goodBullet.width/2,y-goodBullet.height/2, &goodBullet);
        nano_wait(1000);
    }
    else if(Value == 0) {
        LCD_DrawPicture(x-bulletShadow.width/2,(y)-bulletShadow.height/2, &bulletShadow);
        }
    else if(Value == 3){
        LCD_DrawPicture(x-badGuy.width/2,(y)-badGuy.height/2, &badGuy);
    }
}

void update2(int x, int y)
{
    TempPicturePtr(tmp,29,29); // Create a temporary 29x29 image.
    pic_subset(tmp, &background, x-tmp->width/2, y-tmp->height/2); // Copy the background
    pic_overlay(tmp, 5,5, &player, 0xffff); // Overlay the ball
    LCD_DrawPicture(x-tmp->width/2,y-tmp->height/2, tmp); // Draw
}
//void rocketMan(void)
//{
//    // Draw the background.
//        MIDI_Player *mp = midi_init(midifile);
//        init_tim2(10417);
//
//    int x = 120;
//    int y = 22;
//
//    int right, left, shootah, gbX, gbY;
//    int gbCheck = 0;
//    int bg1, bg2, bg3, bg4, bg5, bg6 = 1;
//    int bgX1 = 90;
//    int bgY1 = 280; //initializing bad guy 1
//
//    int bgX2 = 150;
//    int bgY2 = 280; //initializing bad guy 1
//
//    int bgX3 = 210;
//    int bgY3 = 280; //initializing bad guy 1
//
//    int bgX4 = 60;
//    int bgY4 = 295; //initializing bad guy 1
//
//    int bgX5 = 120;
//    int bgY5 = 295; //initializing bad guy 1
//
//    int bgX6 = 180;
//    int bgY6 = 295; //initializing bad guy 1
//
//    for(;;) {
//        for(int c=0; c<4; c++) {
//
//            nano_wait(7500000); // wait 1 ms
//            right = GPIOC->IDR & 1<<6;
//            left = GPIOC->IDR & 1<<7;
//            shootah = GPIOC->IDR & 1<<8;
//
//            if((shootah && 1<<8) && (gbCheck != 1)) {
//                gbCheck = 1;
//                gbX = x;
//                gbY = 30;
//            }
//
//            if(gbCheck) {
//                update(gbX-2, gbY + 10, 1);
//
//                update(gbX-2, gbY+2, 0);
//
//                gbY += 2;
//            }
//
//            if (right && 1<<7)
//            {
//                //nano_wait(5000000);
//                x++;
//                update(x, y, 2);
//            }
//            else if(left && 1<<6)
//            {
//                right = GPIOC->IDR & 1<<6;
//                left = GPIOC->IDR & 1<<7;
//                shootah = GPIOC->IDR & 1<<8;
//            }
//            if((shootah && 1<<8) && (gbCheck != 1)) {
//                gbCheck = 1;
//                gbX = x;
//                gbY = 30;
//            }
//
//            if(gbCheck) {
//                update(gbX-2, gbY + 10, 1);
//
//                update(gbX-2, gbY+2, 0);
//
//                gbY += 2;
//            }
//
//            if (right && 1<<7)
//            {
//                x++;
//                update(x, y, 2);
//            }
//            else if(left && 1<<6)
//                {
//                //nano_wait(5000000);
//                x--;
//                update(x, y, 2);
//                }
//
//            if(gbY > 310) {
//                gbCheck = 0;
//            }
//
//            if(bg1) {
//                update(x, y, 3);
//            }
//
//            if(bg2) {
//                update(x, y, 3);
//            }
//
//            if(bg3) {
//                update(x, y, 3);
//            }
//
//            if(bg4) {
//                update(x, y, 3);
//            }
//
//            if(bg5) {
//                update(x, y, 3);
//            }
//
//            if(bg6) {
//                update(x, y, 3);
//            }
//
//            if(gbY > 310) {
//                gbCheck = 0;
//            }
//            if(bg1) {
//                update(x, y, 3);
//
//            }
//            if(bg2) {
//                update(x, y, 3);
//
//            }
//            if(bg3) {
//                update(x, y, 3);
//
//            }
//            if(bg4) {
//                update(x, y, 3);
//
//            }
//            if(bg5) {
//                update(x, y, 3);
//
//            }
//            if(bg6) {
//                update(x, y, 3);
//            }
//            if (mp->nexttick == MAXTICKS) {
//                mp = midi_init(midifile);
//            }
//        }
//    }
//}


//here


void rocketMan(void)
{
    // Draw the background.
    MIDI_Player *mp = midi_init(midifile);
    init_tim2(10417);

    int x = 120;
    int y = 22;

    int right, left, shootah, gbX, gbY;
    int gbCheck = 0;
    int bg1, bg2, bg3, bg4, bg5, bg6 = 1;
    int bgX1 = 90;
    int bgY1 = 280; //initializing bad guy 1

    int bgX2 = 150;
    int bgY2 = 280; //initializing bad guy 1

    int bgX3 = 210;
    int bgY3 = 280; //initializing bad guy 1

    int bgX4 = 60;
    int bgY4 = 295; //initializing bad guy 1

    int bgX5 = 120;
    int bgY5 = 295; //initializing bad guy 1

    int bgX6 = 180;
    int bgY6 = 295; //initializing bad guy 1

    for(;;) {
        for(int z=0; z<4; z++) {

            nano_wait(750000); // wait 1 ms
            right = GPIOC->IDR & 1<<6;
            left = GPIOC->IDR & 1<<7;
            shootah = GPIOC->IDR & 1<<8;

            if((shootah && 1<<8) && (gbCheck != 1)) {
                gbCheck = 1;
                gbX = x;
                gbY = 30;
            }

            if(gbCheck) {
                update(gbX-2, gbY + 10, 1);

                update(gbX-2, gbY+2, 0);

                gbY += 2;
            }

            if (right && 1<<7)
            {
                //nano_wait(5000000);
                x++;
                update(x, y, 2);
            }
            else if(left && 1<<6)
                {
                //nano_wait(5000000);
                x--;
                update(x, y, 2);
                }

            if(gbY > 310) {
                gbCheck = 0;
            }
            if(bg1) {
                update(x, y, 3);

            }
            if(bg2) {
                update(x, y, 3);

            }
            if(bg3) {
                update(x, y, 3);

            }
            if(bg4) {
                update(x, y, 3);

            }
            if(bg5) {
                update(x, y, 3);

            }
            if(bg6) {
                update(x, y, 3);

            }
            asm("wfi");
            if (mp->nexttick == MAXTICKS) {
            mp = midi_init(midifile);
            }
        }
    }
}

void generateGame(void) {
    LCD_DrawPicture(0,0,&background);

    update(120,22,2); //initializing spaceship

    update(90, 280, 3); //initializing bad guys
    update(150, 280, 3);
    update(210, 280, 3);
    update(60, 295, 3); //initializing bad guys
    update(120, 295, 3);
    update(180, 295, 3);

}


void basic_drawing(void)
{
    LCD_Clear(0);
    LCD_DrawRectangle(10, 10, 30, 50, GREEN);
    LCD_DrawFillRectangle(50, 10, 70, 50, BLUE);
    LCD_DrawLine(10, 10, 70, 50, RED);
    LCD_Circle(50, 90, 40, 1, CYAN);
    LCD_DrawTriangle(90,10, 120,10, 90,30, YELLOW);
    LCD_DrawFillTriangle(90,90, 120,120, 90,120, GRAY);
    LCD_DrawFillRectangle(10, 140, 120, 159, WHITE);
    LCD_DrawString(20,141, BLACK, WHITE, "Test string!", 16, 0); // opaque background
}

