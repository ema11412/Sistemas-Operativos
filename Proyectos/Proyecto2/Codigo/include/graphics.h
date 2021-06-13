#ifndef _GRAPHICS_H
#define _GRAPHICS_H


SDL_Texture *loadTexture(const char *file, SDL_Renderer *ren);

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h);

void renderTextureFull(SDL_Texture *tex, SDL_Renderer *ren, int x, int y);

void renderTextureClip(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect dst, SDL_Rect *clip);

void renderTextureSheet(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, SDL_Rect *clip);

SDL_Texture *renderText(char *message, char *fontFile, SDL_Color color, int fontSize, SDL_Renderer *renderer);

SDL_Rect getTextureRect(SDL_Texture *Tex, int x, int y);

void loadClips(SDL_Rect clips[3], int type, int num, int w, int h);

int configWindow(int *baseVel, int *maxTemp, int *exitProgram, int *modeManual);

#endif 