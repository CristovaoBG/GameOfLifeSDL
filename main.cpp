#include <stdio.h>
#include <string>

#include "Texture.h"

#define FRAME_NUM 2
#define IMG_BLOCO "BLOCK.bmp"

#define ALTURA_TELA 800
#define LARGURA_TELA 800
#define LINHAS 80
#define COLUNAS 80
#define MORTO 0
#define VIVO 1

struct Cell{
	int x;
	int y;
	short estado;
	short novoEstado;
};

typedef struct Cell Cell;

int DesenhaBloco(int x, int y, int estado);


SDL_Window* gWindow = NULL;		//The window we'll be rendering to
SDL_Renderer* gRenderer = NULL;		//The window renderer


//Walking animation
const int WALKING_ANIMATION_FRAMES = FRAME_NUM;
SDL_Rect gSpriteClips[ WALKING_ANIMATION_FRAMES ];
Texture gSpriteSheetTexture;




void InicializaMatriz(Cell matriz[][COLUNAS]) {
	int i,j;
	for(i=0; i<LINHAS; i++){
	for(j=0; j<COLUNAS; j++){
		matriz[i][j].x = i;
		matriz[i][j].y = j;
		matriz[i][j].estado = MORTO;
		matriz[i][j].novoEstado = MORTO;
		}	
	}

}

void Wait(int milli){
	int ini;
	ini = clock();
	while(clock() - ini < milli * CLOCKS_PER_SEC/1000 );
	return;
}

void Opera(Cell matriz[][COLUNAS]){
	int i,j,k,l;
	int vizinhos = 0;
	int target_x,
	    target_y;

	for(i=0; i<LINHAS; i++){
	for(j=0; j<COLUNAS; j++){
		//varre a vizinhanca
		for(k=0;k<3; k++)
		for(l=0;l<3; l++)
		if (!(k==1 && l==1)){ //exclui a celula em questao
			target_x = i+k-1;
			target_y = j+l-1;
			if(target_x < 0) target_x+= COLUNAS;
			if(target_x >= COLUNAS) target_x-= COLUNAS;
			if(target_y < 0) target_y+= LINHAS;
			if(target_y >= LINHAS) target_y-= LINHAS;

			if(matriz[target_x][target_y].estado == VIVO) 
				vizinhos++;
		}

		if (vizinhos < 2) matriz[i][j].novoEstado = MORTO;
		else if (vizinhos > 3) matriz[i][j].novoEstado = MORTO;
		else if (vizinhos == 3) matriz[i][j].novoEstado = VIVO;
		vizinhos=0;
		
		}	

	}
	for(i=0; i<LINHAS; i++)
	for(j=0; j<COLUNAS; j++)
		matriz[i][j].estado = matriz[i][j].novoEstado;
}


void AtribuiCustomizacao(int *inicial, Cell m[][COLUNAS]){
	int i=0;
	while(inicial[i]!=-1){
		m[inicial[i]][inicial[i+1]].estado = VIVO;
		m[inicial[i]][inicial[i+1]].novoEstado = VIVO;
		i+=2;
	}
}

void Imprime(Cell m[][LINHAS]){
	int i,j;
	/*
	static int t;
	system("clear");
	printf("frame: %d\n",t++);
	*/
	for(i=0; i<LINHAS; i++){
	for(j=0; j<COLUNAS; j++){
		DesenhaBloco(i,j,m[i][j].estado);
		}
	}
}



/*
int main(){

	int inicial[] = {7,7 , 8,7 , 9,7 , 9,6 , 8,5 , -1,-1};
	Cell matriz[LINHAS][COLUNAS];
	InicializaMatriz(matriz);
	AtribuiCustomizacao(inicial, matriz);

	while(1){
		DesenhaBloco(1,1,0);
		draw();
		Wait(1);
	}
	

return 0;
}
*/




bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	SDL_Init( SDL_INIT_VIDEO );


	//Set texture filtering to linear
	if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
	{
		printf( "Warning: Linear texture filtering not enabled!" );
	}

	//Create window
	gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );

	//Create vsynced renderer for window
	gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );

	//Initialize renderer color
	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	IMG_Init( imgFlags );
	return success;
}

bool loadMedia(){
	gSpriteSheetTexture.loadFromFile( IMG_BLOCO );

	gSpriteClips[ 0 ].x =   0;
	gSpriteClips[ 0 ].y =   0;
	gSpriteClips[ 0 ].w =  10;
	gSpriteClips[ 0 ].h =  10;

	gSpriteClips[ 1 ].x =  10;
	gSpriteClips[ 1 ].y =   0;
	gSpriteClips[ 1 ].w =  10;
	gSpriteClips[ 1 ].h =  10;

}


void close()
{
	//Free loaded images
	gSpriteSheetTexture.free();

	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

int DesenhaBloco(int x, int y, int estado){
	SDL_Rect* currentClip = &gSpriteClips[ 1 ];
	currentClip = &gSpriteClips[ (estado%2)+1 / FRAME_NUM ];
	gSpriteSheetTexture.render( x*10, y*10, currentClip );
}

int SetStartConfig(Cell matriz[][COLUNAS]){
	SDL_Event e;
	bool desenhando = true;
	bool clicando = false;
	int quit = false;
	int mouse_x, mouse_y;
	int target_x, target_y;

	do{
		SDL_PollEvent( &e );
		if (e.type != SDL_KEYDOWN ) break;
	}while (e.key.keysym.sym == SDLK_RETURN );

	do{
		SDL_PollEvent( &e ) ;
		if (e.type == SDL_MOUSEBUTTONDOWN){
			clicando = true;
			desenhando = (e.button.button == SDL_BUTTON_LEFT)? true : false;	
		}
		else if (e.type == SDL_MOUSEBUTTONUP){
			clicando = false;
		}
		else if (e.type == SDL_KEYDOWN){
			if (e.key.keysym.sym == SDLK_SPACE || e.key.keysym.sym == SDLK_DOWN){
				quit = true; 	
			}
			else if (e.key.keysym.sym == SDLK_ESCAPE) {
				return 1;	
			}
		}
		else if (e.type == SDL_QUIT){
			return 1;		
		}
		if (clicando){
			SDL_GetMouseState( &mouse_x, &mouse_y );
			target_x = mouse_x/10;
			target_y = mouse_y/10;
			if(target_x<LINHAS && target_y<COLUNAS && target_x > 0 && target_y > 0){
				matriz[target_x][target_y].estado = desenhando? VIVO : MORTO;			
			}
		}
		//Clear screen
		SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
		SDL_RenderClear( gRenderer );

		Imprime(matriz);

		//Update screen
		SDL_RenderPresent( gRenderer );

	}while (!quit);


return 0;
}

int main() {


	init();

	loadMedia();

	bool quit = false;		//Main loop flag
	bool pause = false;
	bool reset = false;
	SDL_Event e;			//Event handler

	int frame = 0;			//Current animation frame

	SDL_Rect* currentClip = &gSpriteClips[ 1 ];

	//int inicial[] = {7,7 , 8,7 , 9,7 , 9,6 , 8,5 , -1,-1};
	Cell matriz[LINHAS][COLUNAS]; 

	do{ //reset loop

		InicializaMatriz(matriz);
		//AtribuiCustomizacao(inicial, matriz);

		system("clear");
		printf("desenhe\n");
		//espera para o enter nao interferir?
		Wait(100);

		quit = SetStartConfig(matriz);

		system("clear");
		printf("going..\n");

		while(!quit) {
		

			while( SDL_PollEvent( &e ) != 0 || pause){	//Handle events on queue
				if( e.type == SDL_QUIT) {
					quit = true;
					reset = false;
					break;
					}
				else if( e.type == SDL_KEYDOWN ) {
					if (e.key.keysym.sym == SDLK_RETURN){
						reset = true;
						quit = true;
						break;
						}
					else if (e.key.keysym.sym == SDLK_p){
						pause = (pause==true)? false: true;
						}
					else if (e.key.keysym.sym == SDLK_ESCAPE){
						quit = true;
						reset = false;
						break;
					}
				}
			}

			//Wait(1);

			//Clear screen
			SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xF0 );
			SDL_RenderClear( gRenderer );

			Imprime(matriz);
			Wait(100);
			Opera(matriz);

			//Update screen
			SDL_RenderPresent( gRenderer );

		} //while (!quit)

	} while (reset);


	//Free resources and close SDL
	close();


	return 0;
}









