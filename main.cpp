#include "raylib.h"
#include <vector>
#include <cstddef>

typedef struct{

	float Lifetime;

}Timer;

void StartTimer(Timer* timer, float lifetime){

	if(timer != NULL) timer->Lifetime = lifetime;

}

void UpdateTimer(Timer* timer){

	if(timer != NULL && timer->Lifetime > 0) timer->Lifetime -= GetFrameTime();

}

bool TimerDone(Timer* timer){

	if(timer != NULL) return timer->Lifetime <=0;

}

void textBlock(const char *text, int framesCounter){
	
	Vector2 blockSize = {640, 180};
	Vector2 blockPos = {320, 520};

	DrawRectangleV(blockPos, blockSize, BLACK);

	DrawText(TextSubtext(text, 0, framesCounter/10), blockPos.x + 10, blockPos.y + 10, 15, WHITE);
	
}

void button(float x, float y, Color color){

	Rectangle button = {x, y, 100, 90};

	DrawRectangleRec(button, color);

}

class Carimbo {
	
	public:

	Vector2 Pos = {20, 540};
	Vector2 Size = {90, 100};

	Texture2D carimboSprite = LoadTexture("sprite/Carimbo.png");
	Rectangle carimboAnim = {0, 0, carimboSprite.width/12.0f, carimboSprite.height};
		
};

class Pacientes {

	public:

		std::vector<const char*> pac1 = {"Marcelo - Olá doutor, estou sentindo meus olhos doloridos e eles ficaram bem\nvermelhos ontem...", "Você - Certamente, irei examinalo...\nCom licença", "Joana - Doutor, preciso de sua ajuda urgentemente, eu não consigo respirar meu\npeito está doendo, estou sentido uma febre horrível!", "Você - Fique calma... Devo te diagnosticar rapidamente\nCom licença.", "Felps - Com licença, e- eu to com muito medo... eu a- acho que estou MORRENDO!", "Você - Por que você diz isso? O que houve com você?!", "Felps - Tem algo estranho no meu olho, é tipo uma bola no meio dele\nque se mexe exatamente pra onde eu OLHO!"};
		std::vector<int> pacSintomas = {1, 0, 1};

		Texture2D marcelo = LoadTexture("sprite/Marcelo.png");
		Texture2D joana = LoadTexture("sprite/Joana.png");
		Texture2D felps = LoadTexture("sprite/Felpo.png");

};

class Diagnostico {

	public:

	Vector2 diagV = {900, -5};
	Texture2D diagT = LoadTexture("sprite/Gaveta.png");

	std::vector<Vector2> sintomasPos = {Vector2{1000, 80}, Vector2{1000, 200}};
	std::vector<const char*> sintomasText = {"Olhos doloridos, vermelhos e \nnariz cheio:", "Olhos doloridos e vermelhos\nSem mais informações:"};
	std::vector<const char*> sintomasText2 = {"Febre constante, dores no \npeito, porém, com falta de ar e\ncoração cheio(ver coração):", "Febre constante, dores no \npeito, porém, coração normal\n(ver coração):"};
	std::vector<const char*> sintomasText3 = {"Mandar embora:", "Vai comprar sementes aianguera\nMuu esse capim ta muito bom\nMuu esse capim ta bom demais"};

	Vector2 sintomasSize = {90, 50};
	
	Rectangle aprove = {780, 550, 110, 60};

};

int main(){
	
	bool getCarimbo = false;	
	bool carimbar = false;

	bool menu = true;

	bool pause = false;
	bool falando = false;
	bool receita = false;
	bool ending = false;
	bool transition = false;

	const int Swidth = 1280;
	const int Sheight = 720;

	Color sintomaCor = BLACK;

	InitWindow(Swidth, Sheight, "Carimbo");
	InitAudioDevice();
	SetTargetFPS(60);

	Carimbo carimbo;
	Pacientes pacientes;
	Diagnostico diagnostico;

	Vector2 mousePos = GetMousePosition();
	
	Rectangle endButton = {595, 380, 100, 90};
	Rectangle transButton = {595, 380, 100, 90};
	Rectangle carimboBounds = {carimbo.Pos.x, carimbo.Pos.y - 120, 330, 340};

	Texture2D background = LoadTexture("sprite/Sala.png");
	Texture2D receitaS = LoadTexture("sprite/papelhehe.png");
	
	Vector2 receitaPos = {610, 180};
	
	Vector2 carimPos = {0, 0};
	Vector2 carimSize = {carimbo.Size.x, carimbo.Size.y/2.0f};
	Rectangle carimBounds = {carimPos.x, carimPos.y, carimSize.x, carimSize.y};

	int idx = 0;
	int pac = 0;
	int pacAtual = pac+1;
	int sintomaSel = -1;
	int textCounter = 0;
	int animCounter = 0;
	int animFrame = 0;

	Music music = LoadMusicStream("sprite/musica.mp3");
	PlayMusicStream(music);
	music.looping = true;
	SetMusicVolume(music, 0.2);

	Sound carimboSound = LoadSound("sprite/Carimbo.wav");
	Sound clickSound = LoadSound("sprite/Click.wav");	

	Timer carimbadaTimer = {0};
	float carimbadaLife = 0.6f;

	while(!WindowShouldClose()){

		UpdateMusicStream(music);

		if(IsKeyPressed(KEY_TAB)) pause = !pause;
		if(IsKeyPressed(KEY_F)) ToggleFullscreen();

		if(menu){

			if(IsKeyPressed(KEY_ENTER)){

				falando = true;
				menu = false;
				PlaySound(clickSound);
			
			}

		}else if(!menu){

		if(!pause){

		if(!falando){	

		mousePos = GetMousePosition();
		
		getCarimbo = true;

		if(animFrame != 12) animCounter++;

		if(CheckCollisionPointRec(mousePos, carimboBounds)){
			
			if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
	
				HideCursor();

				carimbo.Pos.x = mousePos.x - 45;
				carimbo.Pos.y = mousePos.y - 50;		

				carimboBounds = {carimbo.Pos.x - 90, carimbo.Pos.y - 140, 330, 340};
			}	
		
			if(IsKeyPressed(KEY_C) && !carimbar){
							
				carimPos = {mousePos.x - 15, mousePos.y + 25};
				carimBounds = {carimPos.x, carimPos.y, carimSize.x, carimSize.y};
				StartTimer(&carimbadaTimer, carimbadaLife);
				PlaySound(carimboSound);

			}
			
			for(int i = 0; i < diagnostico.sintomasPos.size(); i++){
				if(CheckCollisionRecs(carimBounds, Rectangle{diagnostico.sintomasPos[i].x, diagnostico.sintomasPos[i].y, diagnostico.sintomasSize.x, diagnostico.sintomasSize.y})){	
				
					sintomaSel = i;
			
				}

			}


					if(sintomaSel == pacientes.pacSintomas[pac]){
	
						receita = true;
	
					}

			if(receita){

				if(CheckCollisionRecs(carimBounds, diagnostico.aprove)){

					transition = true;
					falando = false;
					getCarimbo = false;
					receita = false;

				}
			}

			if(transition){
				
				if(pacAtual == 3){

						ending = true;
						transition = false;

				}

				if(CheckCollisionRecs(carimBounds, transButton)){

					pacAtual++;
					pac++;
					//idx++;
					falando = true;
					getCarimbo = false;
					receita = false;
					transition = false;

				}	

			}
			

		}else{	
		
			ShowCursor();
			carimbo.Pos = {20, 540};
			carimboBounds = {carimbo.Pos.x - 130, carimbo.Pos.y - 140, 330, 340};

		}

		}

		UpdateTimer(&carimbadaTimer);

		if(TimerDone(&carimbadaTimer)){
			
			sintomaSel = -1;
			carimbar = false;
		
		}

		textCounter += 8;	

		}

		}

		if(ending){
			
			falando = false;
			receita = false;
			//getCarimbo = false;
			if(CheckCollisionRecs(carimBounds, endButton)){

				CloseWindow();

			}

		}

		if(animCounter >= 0.5 && !TimerDone(&carimbadaTimer)){

				animCounter = 0;
				animFrame++;	

				carimbo.carimboAnim.x = (float)animFrame*(float)carimbo.carimboSprite.width/12.0f;
		}

		if(TimerDone(&carimbadaTimer)){

			animFrame = 0;
			carimbo.carimboAnim.x = (float)animFrame*(float)carimbo.carimboSprite.width/12.0f;

		}

		BeginDrawing();
		DrawTexture(background, 0, 0, WHITE);	
		DrawFPS(10, 10);

		if(menu){

			DrawText("Dia de Doutor", 640 - MeasureText("Dia de Doutor", 50)/2, 200, 50, BLACK);
			DrawText("Pressione enter para começar", 640 - MeasureText("Pressione enter para começar", 20)/2, 700, 20, BLACK);

		}

		if(pacAtual == 1 && !ending && !menu && !transition){

			DrawTexture(pacientes.marcelo, 0, 0, WHITE);

		}else if(pacAtual == 2 && !ending && !menu && !transition){

			DrawTexture(pacientes.joana, 0, 0, WHITE);

		}else if(pacAtual == 3 && !ending && !menu && !transition){

			DrawTexture(pacientes.felps, 0, 0, WHITE);

		}

		if(!falando && !receita && !ending && !menu && !transition){

		DrawTextureV(diagnostico.diagT, diagnostico.diagV, WHITE);

		for(int i = 0; i < diagnostico.sintomasPos.size(); i++){

			if(i == sintomaSel){
			
				sintomaCor = RED;

			}else sintomaCor = BLACK;

			if(pacAtual == 1){

				//DrawTexture(pacientes.marcelo, 0, 0, WHITE);
				DrawText(diagnostico.sintomasText[i], diagnostico.sintomasPos[i].x, diagnostico.sintomasPos[i].y - 60, 17, BLACK);

			}else if(pacAtual == 2){
				
				//DrawTexture(pacientes.joana, 0, 0, WHITE);
				DrawText(diagnostico.sintomasText2[i], diagnostico.sintomasPos[i].x, diagnostico.sintomasPos[i].y - 60, 17, BLACK);

			}else{

				//DrawTexture(pacientes.felps, 0, 0, WHITE);
				DrawText(diagnostico.sintomasText3[i], diagnostico.sintomasPos[i].x, diagnostico.sintomasPos[i].y - 60, 17, BLACK);

			} 
			DrawRectangleV(diagnostico.sintomasPos[i], diagnostico.sintomasSize, sintomaCor);

			
		
		}

		}

		if(receita){

			DrawTextureV(receitaS, receitaPos, WHITE);
			//DrawRectangleRec(diagnostico.aprove, BLACK);
			//DrawTextureV(receitaS)

		}

		if(falando){	

			if(pacAtual == 1){

				//DrawTexture(pacientes.marcelo, 0, 0, WHITE);
				if(idx == 2){

					falando = false;

				}

			}else if(pacAtual == 2){

				//DrawTexture(pacientes.joana, 0, 0, WHITE);
				if(idx == 4){

					falando = false;

				}


			}else if(idx == pacientes.pac1.size()){

				idx = 0;
				falando = false;
			}

			textBlock(pacientes.pac1[idx], textCounter);
			DrawText("[espaço]", 640 - MeasureText("[espaço]", 15)/2, 660, 15, WHITE);

			if(IsKeyPressed(KEY_SPACE)){ 

				PlaySound(clickSound);
				idx++;
				textCounter = 0;
			}	

		}

		if(ending){
			
			DrawRectangle(630 - MeasureText("Parabéns, você conseguiu diagnosticar todos os pacientes!", 40)/2, 290, MeasureText("Parabéns, você conseguiu diagnosticar todos os pacientes!", 40) + 20, 60, BLACK);
			DrawText("Parabéns, você conseguiu diagnosticar todos os pacientes!", 640 - MeasureText("Parabéns, você conseguiu diagnosticar todos os pacientes!", 40)/2, 300, 40, WHITE);
			button(595, 380, BLACK);

		}

		if(transition){
			
			DrawRectangle(630 - MeasureText("Parabéns, você conseguiu diagnosticar seu paciente!", 40)/2, 290, MeasureText("Parabéns, você conseguiu diagnosticar seu paciente!", 40) + 20, 60, BLACK);
			DrawText("Parabéns, você conseguiu diagnosticar seu paciente!", 640 - MeasureText("Parabéns, você conseguiu diagnosticar seu paciente!", 40)/2, 300, 40, WHITE);
			button(595, 380, BLACK);

		}


					
		if(getCarimbo){
			
			if(!TimerDone(&carimbadaTimer)){	

				DrawRectangleV(carimPos, carimSize, GREEN);	
	
			}

			//DrawRectangleRec(carimboBounds, BLUE);
			//DrawRectangleV(carimbo.Pos, carimbo.Size, BLACK);
			DrawTextureRec(carimbo.carimboSprite, carimbo.carimboAnim, carimbo.Pos, WHITE);
			DrawText("C para CARIMBAR", 640 - MeasureText("C para CARIMBAR", 20)/2, 660, 20, WHITE);
			
		}
	

		//DrawText(TextFormat("i= %d", idx), 20, 30 ,20, BLACK);
		//DrawText(TextFormat("atual= %d", pacAtual), 20, 40 ,20, BLACK);
		EndDrawing();
	
	}

	UnloadMusicStream(music);
	CloseAudioDevice();
		
	CloseWindow();
	

return 0;
}

