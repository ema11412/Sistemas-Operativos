#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

#include "../../include/graphics.h"


/*
 * Loads a BMP image into a texture on the rendering device
 * @param file The BMP image file to load
 * @param ren The renderer to load the texture onto
 * @return the loaded texture, or NULL if something went wrong.
 */
SDL_Texture *loadTexture(const char *file, SDL_Renderer *ren) {
    SDL_Texture *texture = IMG_LoadTexture(ren, file);
    //If the loading went ok, convert to texture and return the texture
    if (texture == NULL) {
        printf("LoadTexture error: %s\n", SDL_GetError());
    }
    return texture;
}

/*
 * Draw an SDL_Texture to an SDL_Renderer at position x, y, with some desired
 * width and height
 * @param tex The source texture we want to draw
 * @param ren The renderer we want to draw too
 * @param x The x coordinate to draw too
 * @param y The y coordinate to draw too
 */

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h) {
    //Setup the destination rectangle to be at the position we want
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    dst.w = w;
    dst.h = h;
    //Query the texture to get its width and height to use
    SDL_RenderCopy(ren, tex, NULL, &dst);
}

/*
 * Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
 * the texture's width and height
 * @param tex The source texture we want to draw
 * @param rend The renderer we want to draw too
 * @param x The x coordinate to draw too
 * @param y The y coordinate to draw too
 */
void renderTextureFull(SDL_Texture *tex, SDL_Renderer *ren, int x, int y) {
    int w, h;
    SDL_QueryTexture(tex, NULL, NULL, &w, &h);
    renderTexture(tex, ren, x, y, w, h);
}

/**
* Draw an SDL_Texture to an SDL_Renderer at some destination rect
* taking a clip of the texture if desired
* @param tex The source texture we want to draw
* @param ren The renderer we want to draw to
* @param dst The destination rectangle to render the texture to
* @param clip The sub-section of the texture to draw (clipping rect)
*		default of nullptr draws the entire texture
*/
void renderTextureClip(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect dst, SDL_Rect *clip) {
    SDL_RenderCopy(ren, tex, clip, &dst);
}

/**
* Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
* the texture's width and height and taking a clip of the texture if desired
* If a clip is passed, the clip's width and height will be used instead of
*	the texture's
* @param tex The source texture we want to draw
* @param ren The renderer we want to draw to
* @param x The x coordinate to draw to
* @param y The y coordinate to draw to
* @param clip The sub-section of the texture to draw (clipping rect)
*		default of nullptr draws the entire texture
*/
void renderTextureSheet(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, SDL_Rect *clip) {
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    if (clip != NULL) {
        dst.w = w;
        dst.h = w;
    } else {
        SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
    }
    renderTextureClip(tex, ren, dst, clip);
}

/**
* Render the message we want to display to a texture for drawing
* @param message The message we want to display
* @param fontFile The font we want to use to render the text
* @param color The color we want the text to be
* @param fontSize The size we want the font to be
* @param renderer The renderer to load the texture in
* @return An SDL_Texture containing the rendered message, or nullptr if something went wrong
*/
SDL_Texture *renderText(char *message, char *fontFile, SDL_Color color, int fontSize, SDL_Renderer *renderer) {
    //Open the font
    TTF_Font *font = TTF_OpenFont(fontFile, fontSize);
    if (font == NULL) {
        printf("TTF_OpenFont error: %s\n", SDL_GetError());
        return NULL;
    }
    //We need to first render to a surface as that's what TTF_RenderText
    //returns, then load that surface into a texture
    SDL_Surface *surf = TTF_RenderText_Blended(font, message, color);
    if (surf == NULL) {
        TTF_CloseFont(font);
        printf("TTF_RenderText error: %s\n", SDL_GetError());
        return NULL;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
    if (texture == NULL) {
        printf("CreateTexture error: %s\n", SDL_GetError());
    }
    //Clean up the surface and font
    SDL_FreeSurface(surf);
    TTF_CloseFont(font);
    return texture;
}

/**
* Calc the rectangle of any Texture for collisions
* @param Tex The texture to calculate de rect
* @param x upper left corner x coordenate of the texture
* @param y upper left corner y coordenate of the texture
* @return A bounding box of the texture.
*/
SDL_Rect getTextureRect(SDL_Texture *Tex, int x, int y) {
    SDL_Rect temp;
    int w, h;
    SDL_QueryTexture(Tex, NULL, NULL, &w, &h);

    temp.x = x;
    temp.y = y;
    temp.w = w;
    temp.h = h;
    return temp;
}

void loadClips(SDL_Rect clips[3], int type, int num, int w, int h) {
    for (int i = 0; i < 3; ++i) {
        clips[i].x = (i + 3 * (num - 1)) * w;
        clips[i].y = (4 * (type - 1)) * h;
        clips[i].w = w;
        clips[i].h = h;
    }
}

int configWindow(int *baseVel, int *maxTemp, int *exitProgram, int *modeManual) {

    //Opening a Window
    SDL_Window *config = SDL_CreateWindow("Crazy Ants", 1366 / 2 - 150, 200, 320, 400, SDL_WINDOW_SHOWN);
    if (config == NULL) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        return 1;
    }

    //Creating a renderer
    SDL_Renderer *configRen = SDL_CreateRenderer(config, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (configRen == NULL) {
        SDL_DestroyWindow(config);
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        return 1;
    }

    char velText[5];
    char maxTempText[5];
    int isButton1 = 0;
    int isButton2 = 0;

    SDL_itoa(*baseVel, velText, 10);
    SDL_itoa(*maxTemp, maxTempText, 10);

    SDL_Color fontColor = {0, 0, 0, 255};

    //Load Textures & Fonts
    char *bgPath = "../assets/images/menubg.png";
    char *button1Path = "../assets/images/manualBtn.png";
    char *button1_selPath = "../assets/images/manualSel.png";
    char *button2Path = "../assets/images/autoBtn.png";
    char *button2_selPath = "../assets/images/autoSelectBtn.png";
    char *buttonleft = "../assets/images/l.png";
    char *buttonright = "../assets/images/r.png";
    char *buttonOk = "../assets/images/start.png";
    char *buttonCancel = "../assets/images/exit.png";
    char *mainFontPath = "../assets/fonts/font.ttf";


    SDL_Texture *BG = loadTexture(bgPath, configRen);
    SDL_Texture *Button1 = loadTexture(isButton1 ? button1Path : button1_selPath, configRen);;
    SDL_Texture *Button2 = loadTexture(isButton2 ? button2Path : button2_selPath, configRen);;
    SDL_Texture *Left = loadTexture(buttonleft, configRen);
    SDL_Texture *Right = loadTexture(buttonright, configRen);
    SDL_Texture *Ok = loadTexture(buttonOk, configRen);
    SDL_Texture *Cancel = loadTexture(buttonCancel, configRen);
    SDL_Texture *selection = renderText("Seleccione un modo", mainFontPath, fontColor, 14, configRen);
    SDL_Texture *velSelText = renderText("Velocidad base", mainFontPath, fontColor, 10, configRen);
    SDL_Texture *maxTempTitle = renderText("Tiempo maximo", mainFontPath, fontColor, 10, configRen);
    SDL_Texture *velSel = renderText(velText, mainFontPath, fontColor, 13, configRen);
    SDL_Texture *maxTempSel = renderText(maxTempText, mainFontPath, fontColor, 13, configRen);

    SDL_Rect Button1Rect = getTextureRect(Button1, 15, 75);
    SDL_Rect Button2Rect = getTextureRect(Button2, 175, 75);
    SDL_Rect ButtonLeftRect = getTextureRect(Left, 80, 210);
    SDL_Rect ButtonRightRect = getTextureRect(Right, 190, 210);
    SDL_Rect ButtonLeftRect2 = getTextureRect(Left, 80, 290);

    SDL_Rect ButtonRightRect2 = getTextureRect(Right, 190, 290);
    SDL_Rect ButtonOkRect = getTextureRect(Ok, 30, 335);
    SDL_Rect ButtonCancelRect = getTextureRect(Cancel, 180, 335);
    SDL_Rect mouseRect;
    SDL_Event e;
    int mouseX, mouseY;
    int exitWin = 0;
    while (!exitWin) {
        Button1 = loadTexture(isButton1 ? button1_selPath : button1Path, configRen);
        Button2 = loadTexture(isButton2 ? button2_selPath : button2Path, configRen);
        while (SDL_PollEvent(&e)) {
            //If user closes the window
            if (e.type == SDL_QUIT) {
                *exitProgram = 1;
                exitWin = 1;
                return 1;
            }
            if ((e.type == SDL_MOUSEBUTTONDOWN) && SDL_BUTTON(SDL_BUTTON_LEFT)) {
                SDL_GetMouseState(&mouseX, &mouseY);
                mouseRect.x = mouseX;
                mouseRect.y = mouseY;
                mouseRect.w = mouseRect.h = 1;
                if (SDL_HasIntersection(&mouseRect, &Button1Rect)) {
                    *modeManual = 1;
                    isButton1 = 1;
                    isButton2 = 0;
                } else if (SDL_HasIntersection(&mouseRect, &Button2Rect)) {
                    *modeManual = 0;
                    isButton2 = 1;
                    isButton1 = 0;
                } else if (SDL_HasIntersection(&mouseRect, &ButtonLeftRect)) {
                    if (*baseVel > 5)
                        *baseVel -= 5;
                    SDL_itoa(*baseVel, velText, 10);
                    velSel = renderText(velText, mainFontPath, fontColor, 14, configRen);
                } else if (SDL_HasIntersection(&mouseRect, &ButtonRightRect)) {
                    if (*baseVel < 20)
                        *baseVel += 5;
                    SDL_itoa(*baseVel, velText, 10);
                    velSel = renderText(velText, mainFontPath, fontColor, 14, configRen);
                } else if (SDL_HasIntersection(&mouseRect, &ButtonLeftRect2)) {
                    if (*maxTemp > 5)
                        *maxTemp -= 5;
                    SDL_itoa(*maxTemp, maxTempText, 10);
                    maxTempSel = renderText(maxTempText, mainFontPath, fontColor, 14, configRen);
                } else if (SDL_HasIntersection(&mouseRect, &ButtonRightRect2)) {
                    if (*maxTemp < 120)
                        *maxTemp += 5;
                    SDL_itoa(*maxTemp, maxTempText, 10);
                    maxTempSel = renderText(maxTempText, mainFontPath, fontColor, 14, configRen);
                } else if (SDL_HasIntersection(&mouseRect, &ButtonOkRect)) {
                    if (isButton2 || isButton1) {
                        exitWin = 1;
                    }
                } else if (SDL_HasIntersection(&mouseRect, &ButtonCancelRect)) {
                    exitWin = 1;
                    return 1;
                }
            }
        }
        renderTexture(BG, configRen, 0, 0, 400, 400);

        renderTextureFull(selection, configRen, 10, 50);
        renderTextureFull(Button1, configRen, Button1Rect.x, Button1Rect.y);
        renderTextureFull(Button2, configRen, Button2Rect.x, Button2Rect.y);

        renderTextureFull(velSelText, configRen, 100, 170);
        renderTextureFull(Left, configRen, ButtonLeftRect.x, ButtonLeftRect.y);
        renderTextureFull(Right, configRen, ButtonRightRect.x, ButtonRightRect.y);
        renderTextureFull(velSel, configRen, 150, 220);

        renderTextureFull(maxTempTitle, configRen, 60, 260);
        renderTextureFull(Left, configRen, ButtonLeftRect2.x, ButtonLeftRect2.y);
        renderTextureFull(Right, configRen, ButtonRightRect2.x, ButtonRightRect2.y);
        renderTextureFull(maxTempSel, configRen, 150, 300);

        renderTextureFull(Ok, configRen, ButtonOkRect.x, ButtonOkRect.y);
        renderTextureFull(Cancel, configRen, ButtonCancelRect.x, ButtonCancelRect.y);

        //Update the screen
        SDL_RenderPresent(configRen);
        SDL_Delay(30);
    }
    SDL_DestroyTexture(BG);
    SDL_DestroyTexture(Button1);
    SDL_DestroyTexture(Button2);
    SDL_DestroyTexture(Left);
    SDL_DestroyTexture(Right);
    SDL_DestroyTexture(Ok);
    SDL_DestroyTexture(Cancel);
    SDL_DestroyTexture(selection);
    SDL_DestroyTexture(velSelText);
    SDL_DestroyTexture(velSel);
    SDL_DestroyTexture(maxTempTitle);
    SDL_DestroyTexture(maxTempSel);
    SDL_DestroyWindow(config);
    SDL_DestroyRenderer(configRen);

    exitProgram = 0;
    return 0;
}