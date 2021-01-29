#include <iostream>

using namespace std; // 65,66,67,68

const int boardWidth = 14; // eni(ufuqi)
const int boardHeight = 10; // hundurluyu (shaquli)
const int shipTypes = 3; // 3 gur gemi

const char isWATER = 247; // ascii kod
const char isHIT = 'X'; // gemi vuruldugda
const char isSHIP = 'S'; // geminin isharesi
const char isNULL = '0'; // hedef tapilmayanda 

struct point {
	//shebekedeki yer - X(ufuqi) Y(shaquli)

	int X;
	int Y;
};

struct SHIP {
	//geminin adi
	char name;
	//shebekedeki ummumi ballar  

	int length;
	//kordinatlar
	point onGrid[4]; // 0-3 maksumum uzunlug
	//geminin vuruldugunu bildirir(ve ya vurulmadigini)
	bool hitFlag[5];

} ship[shipTypes];

struct PLAYER {
	char grid[boardWidth][boardHeight];
}player[3]; // player's 1 & 2

enum DIRECTION { HORIZONTAL, VERTICAL };
struct PLACESHIPS {
	DIRECTION direction;
	SHIP shipType;
};

bool gameRunning = false;

// funksiyalar
void LoadShips();
void ResetBoard();
void DrawBoard(int);
PLACESHIPS UserInputShipPlacement();
bool UserInputAttack(int&, int&, int);
bool GameOverCheck(int);

int main() {
	LoadShips();
	ResetBoard();

	//gemilerin yerleshdirmek
	//oyunchulara baxilir 
	for (int aplyr = 1; aplyr < 3; aplyr++)
	{
		//gemi tiplerine baxilir yerleshdirilir
		for (int thisShip = 0; thisShip < shipTypes; thisShip++)
		{
			//oyun taxtasi 
			system("cls");
			DrawBoard(aplyr);
			// telimat -----------------||
			cout << "\n"
				<< "Telimat (Player " << aplyr << ")\n\n"
				<< "Gemileri bu formatda yerleshdirmelisiniz:\n"
				<< " (0:shaquli,1:ufuqi), X (ust setir) , Y (sol teref) \n"
				<< "meselen: 0 7 2    Bu X:7 Y:2 kordinatli ufiqi veziyyetdeki geminin kordinatidir.\n\n"
				<< "gemi: " << ship[thisShip].name << " which has a length of " << ship[thisShip].length << "\n"
				<< "Where do you want it placed? ";

			// mushteriden melumatlar alinir (true return edene qeder)
			PLACESHIPS aShip;
			try
			{

				aShip.shipType.onGrid[0].X = -1;
				while (aShip.shipType.onGrid[0].X == -1)
				{
					aShip = UserInputShipPlacement();
				}
			}
			catch (const exception& ex)
			{
				cout << ex.what();
			}


			// "this ship" data = user data  
			aShip.shipType.length = ship[thisShip].length;
			aShip.shipType.name = ship[thisShip].name;

			// gemilerin ilk pointi
			player[aplyr].grid[aShip.shipType.onGrid[0].X][aShip.shipType.onGrid[0].Y] = isSHIP;

			// gemiler yerleshdirilir
			for (int i = 1; i < aShip.shipType.length; i++)
			{
				if (aShip.direction == HORIZONTAL) {
					aShip.shipType.onGrid[i].X = aShip.shipType.onGrid[i - 1].X + 1;
					aShip.shipType.onGrid[i].Y = aShip.shipType.onGrid[i - 1].Y;
				}
				if (aShip.direction == VERTICAL) {
					aShip.shipType.onGrid[i].Y = aShip.shipType.onGrid[i - 1].Y + 1;
					aShip.shipType.onGrid[i].X = aShip.shipType.onGrid[i - 1].X;
				}

				// gemi point
				player[aplyr].grid[aShip.shipType.onGrid[i].X][aShip.shipType.onGrid[i].Y] = isSHIP;
			}
			// her dovrden bir gemi novu
		}
		// her dovrden bir pleyer
	}

	//------------ FINISH ----------

	// oyun bashlanmagha hazirdir
	gameRunning = true;
	int thisPlayer = 1;
	do {
		// player 1 hucum edenden sonra, novbe player 2 de olur
		// player 2 hucum edenden sonra, novbe player 1 de olur


		int enemyPlayer;
		if (thisPlayer == 1)
			enemyPlayer = 2;
		if (thisPlayer == 2)
			enemyPlayer = 1;

		system("cls");
		DrawBoard(enemyPlayer);

		// hucum koordinatlarinin alinmasi
		bool goodInput = false;
		int x, y;
		while (goodInput == false) {
			goodInput = UserInputAttack(x, y, thisPlayer);
		}


		//gemi vars X (isHIT) eks halda 0 (isMISS)
		if (player[enemyPlayer].grid[x][y] == isSHIP) {
			player[enemyPlayer].grid[x][y] = isHIT;
			if (thisPlayer == 1) {
				thisPlayer = 2;
			}
			else {
				thisPlayer = 1;
			}
			if (enemyPlayer == 1) {
				enemyPlayer = 2;
			}
			else {
				enemyPlayer = 1;
			}

		}

		if (player[enemyPlayer].grid[x][y] == isWATER)
			player[enemyPlayer].grid[x][y] = isNULL;

		// Oyunun bitib-bitmediyini yoxlayır
		// 0 qaytarılırsa, hələ heç kim qazanmayıb


		//--------------------------------------
		// true = 1 -- false = 0


		int awin = GameOverCheck(enemyPlayer);

		if (awin == 1) {

			gameRunning = true;
			// geri donmek

		}

		else if (awin == 0) {

			gameRunning = false;

		}



		//------------------------------------------

		if (thisPlayer == 1) {
			thisPlayer = 2;
		}
		else {
			thisPlayer = 1;
		}

	} while (gameRunning);

	system("cls");

	cout << "\n\nCONGRATULATIONS!!!  PLAYER " << thisPlayer << " HAS WON THE GAME!\n\n\n\n";

	system("pause");
	return 0;
}


bool GameOverCheck(int enemyPLAYER)
{
	if (enemyPLAYER == 1) {
		enemyPLAYER = 2;
	}
	else {
		enemyPLAYER = 1;
	}

	bool winner = true;
	// qarshi terefin lovhesi 

	for (int w = 0; w < boardWidth; w++) {
		for (int h = 0; h < boardHeight; h++) {

			if (player[enemyPLAYER].grid[w][h] == isSHIP)
			{

				return winner;
				break;
			}

		}
	}



	// gemi yoxdursa, oyun bitdi

	return false;
}



bool UserInputAttack(int& x, int& y, int theplayer)
{
	cout << "\nPLAYER " << theplayer << ", HUCUM KOORDINATLARINI DAXIL EDIN: ";
	bool goodInput = false;
	cin >> x >> y;
	if (x < 0 || x >= boardWidth)
		return goodInput;

	if (y < 0 || y >= boardHeight)
		return goodInput;

	goodInput = true;
	return goodInput;
}

PLACESHIPS UserInputShipPlacement()
{
	int d, x, y;
	PLACESHIPS tmp;
	// bad return
	tmp.shipType.onGrid[0].X = -1;
	// 3 int alinir

	cin >> d >> x >> y;


	if (d != 0 && d != 1)
		return tmp;

	if (x < 0 || x >= boardWidth)
		return tmp;

	if (y < 0 || y >= boardHeight)
		return tmp;
	// Good data
	tmp.direction = (DIRECTION)d;
	tmp.shipType.onGrid[0].X = x;
	tmp.shipType.onGrid[0].Y = y;
	return tmp;
}

void LoadShips()
{
	// geminin melumatlari
	// we plan to include in the game
	// data = gemi sayi

	ship[0].name = 'A'; ship[0].length = 2;
	ship[1].name = 'B'; ship[1].length = 3;
	ship[2].name = 'C'; ship[2].length = 4;

}
void ResetBoard()
{
	// her bir oyuncuya baxilir
	for (int plyr = 1; plyr < 3; plyr++)
	{
		// butun shebeke noqteleri = isWATER
		for (int w = 0; w < boardWidth; w++) {
			for (int h = 0; h < boardHeight; ++h) {
				player[plyr].grid[w][h] = isWATER;
			}
		}
		// novbeti player
	}
}

void DrawBoard(int thisPlayer)
{
	//oyuncu uchun lovhe chekir (thisPlayer)
	cout << "PLAYER " << thisPlayer << "'s GAME BOARD\n";
	cout << "----------------------\n";

	// ust setirde (board_width) reqemleri duzur
	cout << "   ";
	for (int w = 0; w < boardWidth; w++) {
		if (w < 10)
			cout << w << "  ";
		else if (w >= 10)
			cout << w << " ";
	}
	cout << "\n";

	// shebekeni(grid) yoxlayir 
	for (int h = 0; h < boardHeight; h++) {
		for (int w = 0; w < boardWidth; w++) {

			// bu ilk shebeke noqtesidirse evvelce nomrelenir
			if (w == 0)
				cout << h << "  ";

			//oyun hele ishlemirse gemiler yerleshdirilir
			// gemiler gosterilir
			if (gameRunning == false)
				cout << player[thisPlayer].grid[w][h] << "  ";
			// gemiler gosterilmir lakin vuruldugu gosterilir
			if (gameRunning == true && player[thisPlayer].grid[w][h] != isSHIP)
			{
				cout << player[thisPlayer].grid[w][h] << "  ";
			}
			else if (gameRunning == true && player[thisPlayer].grid[w][h] == isSHIP)
			{
				cout << isWATER << "  ";
			}
			// eger sona chatdiqsa(siranin) \n
			if (w == boardWidth - 1)
				cout << '\n';
		}
	}
}