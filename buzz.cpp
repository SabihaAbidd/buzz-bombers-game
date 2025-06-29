//Respective Libraries
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cstdlib>
#include <ctime>

using namespace std;
using namespace sf;

// Global Constants
//Define the game window's resolution.
const int resolutionX = 960;
const int resolutionY = 640;
//Represent the size of each grid unit in pixels.
const int boxPixelsX = 32;
const int boxPixelsY = 32;
//Calculate the number of rows and columns based on the resolution and box size.
const int gameRows = resolutionX / boxPixelsX; // Total rows on grid
const int gameColumns = resolutionY / boxPixelsY; // Total columns on grid

// Function Prototypes
//Declared functions used later for drawing, moving, and updating the game objects.
void drawPlayer(RenderWindow& window, float& player_x, float& player_y, Sprite& playerSprite);

//Function declaration for the movement of bullet
void moveBullet(float& bullet_y, bool& bullet_exists, Clock& bulletClock);

//In order to draw the bullet on the screen
void drawBullet(RenderWindow& window, float& bullet_x, float& bullet_y, Sprite& bulletSprite);

//In order to show a navigable menu on screen
int showMenu(RenderWindow& window, Font& font);

//Initialization of bees
void initializeBees(float beeX[], float beeY[], bool beeActive[], const int beeCount);

//For the movement of bees
void moveBees(float beeX[], float beeY[], bool beeActive[], int beeDirection[], Clock beeStopClocks[], bool beeStopped[], const int beeCount, float honeycombX[], float honeycombY[], bool honeycombActive[], bool flowers[gameRows][gameColumns], float& player_x, float& player_y, int& sprayCans, int& spraysLeft, const int maxSpraysPerCan, float beehiveX[], float beehiveY[], bool beehiveActive[]);

//In order to draw honeycombs on screen
void drawHoneycombs(RenderWindow& window, float honeycombX[], float honeycombY[], bool honeycombActive[], Sprite& yellowHoneycombSprite, const int beeCount);

//In order to draw the respective sprayscans that showcase lives of the respective player
void drawSprayCans(RenderWindow& window, int sprayCans, int spraysLeft, Sprite& sprayCanSprite, const int maxSpraysPerCan);

//Function to check collision between bee and honeycomb
void checkBeeHoneycombCollision(float beeX[], float beeY[], bool beeActive[], int beeDirection[], float honeycombX[], float honeycombY[], bool honeycombActive[], const int beeCount);
//To draw Flowers ( obstacles )
void drawFlowers(RenderWindow& window, Sprite& flowerSprite, bool flowers[gameRows][gameColumns]);

//For pollination of Flowers
void pollinateFlowers(float beeX[], float beeY[], bool beeActive[], const int beeCount, bool flowers[gameRows][gameColumns]);

void drawBees(RenderWindow& window, float beeX[], float beeY[], bool beeActive[], Sprite& beeSprite, const int beeCount);

//In order to move humming Bird
void moveHummingbird(float& hummingbird_x, float& hummingbird_y, int& hummingbirdDirectionX, int& hummingbirdDirectionY, bool& hummingbirdPaused, bool& hummingbirdSick, int& hummingbirdHits, Clock& hummingbirdClock, Clock& hummingbirdPauseClock, Clock& hummingbirdSickClock, float honeycombX[], float honeycombY[], bool honeycombActive[], int& score, const int beeCount);

//In order to draw hummingBird on Screen
void drawHummingbird(RenderWindow& window, float hummingbird_x, float hummingbird_y, Sprite& hummingbirdSprite, bool hummingbirdSick);

//In order to check collision of bullet
void checkBulletCollision(float bullet_x, float bullet_y,float beeX[], float beeY[], bool beeActive[],float honeycombX[], float honeycombY[], bool honeycombActive[],int& score, const int beeCount, bool& bullet_exists,float& hummingbird_x, float& hummingbird_y, bool& hummingbirdSick, int& hummingbirdHits, Clock& hummingbirdSickClock,float beehiveX[], float beehiveY[], bool beehiveActive[]);

//In order to check whether player is surrounded by obstacles
bool isPlayerSurrounded(float player_x, float player_y, bool flowers[gameRows][gameColumns]);

//For Level 2 of the Game
void initializeLevel2Bees(float fastBeeX[], float fastBeeY[], bool fastBeeActive[], int fastBeeDirection[], int fastBeeCount);

void initializeRedHoneycombs(float redHoneycombX[], float redHoneycombY[], bool redHoneycombActive[], int honeycombCount);

void drawFastBees(RenderWindow& window, float fastBeeX[], float fastBeeY[], bool fastBeeActive[], Sprite& fastBeeSprite, int fastBeeCount);

void drawRedHoneycombs(RenderWindow& window, float redHoneycombX[], float redHoneycombY[], bool redHoneycombActive[], Sprite& redHoneycombSprite, int fastBeeCount);

void checkBulletCollisionLevel2( float bullet_x, float bullet_y,float fastBeeX[], float fastBeeY[], bool fastBeeActive[], int fastBeeCount,float redHoneycombX[], float redHoneycombY[], bool redHoneycombActive[], bool& bullet_exists, int& score);

void showLevelTransition(RenderWindow& window, Font& font, int level);
// Helper Functions
float absolute(float value);

bool allBeesInactive(bool beeActive[], int beeCount, bool fastBeeActive[], int fastBeeCount);

void moveFastBees(float fastBeeX[], float fastBeeY[], bool fastBeeActive[], int fastBeeDirection[], const int fastBeeCount);


int main() {
	srand(time(0));

	// Game Window
	RenderWindow window(VideoMode(resolutionX, resolutionY), "Buzz Bombers", Style::Close | Style::Titlebar);
	window.setPosition(Vector2i(500, 200));

	// Font and Menu
	//Loads the game font for displaying text.
        //If the font fails to load, an error message is displayed, and the game terminates.
	Font font;
	if (!font.loadFromFile("Fonts/PressStart2P-Regular.ttf")) {
    	cout<<"Error: Could not load font file!" << endl;
    	return -1;
	}
	int selectedLevel = showMenu(window, font);
	if (selectedLevel == -1) return 0;         //Exit if user selects "Exit"
        
        
 
	//Background Music
	Music bgMusic;
	if (!bgMusic.openFromFile("Music/Music3.ogg")) {
    	cout<<"Error: Could not load music file!" << endl;
    	return -1;
	}
	
	//Load Level 1 Background
	Texture level1BackgroundTexture;
	if (!level1BackgroundTexture.loadFromFile("level1.jpg")) {
    	cout<<"Error: Could not load level1 background texture!" << endl;
    	return -1;
	}
	Sprite level1Background(level1BackgroundTexture);
	bgMusic.setVolume(50);
	bgMusic.setLoop(true);
	bgMusic.play();

	//Player Variables
	//Initializes the player's position near the bottom of the screen.
        //Sets the maximum number of sprays and spray cans the player has.
	float player_x = (gameRows / 2) * boxPixelsX;
	float player_y = (gameColumns - 4) * boxPixelsY;

	const int maxSpraysPerCan = 56;
	const int sprayLevelDecrement = 8;

	//Spray Variables
	int sprayCans = 3;
	int spraysLeft = maxSpraysPerCan;

	Texture playerTexture;
	playerTexture.loadFromFile("Textures/spray.png");
	Sprite playerSprite(playerTexture);

	//Bullet Variables
	float bullet_x = player_x;
	float bullet_y = player_y;
	bool bullet_exists = false;
	Clock bulletClock;
        
        //To load bullet's picture
	Texture bulletTexture;
	bulletTexture.loadFromFile("Textures/bullet.png");
	Sprite bulletSprite(bulletTexture);
	bulletSprite.setScale(3, 3);
        
         //To load spray's picture
	Texture sprayCanTexture;
	sprayCanTexture.loadFromFile("Textures/spray.png");
	Sprite sprayCanSprite(sprayCanTexture);
	sprayCanSprite.setScale(0.5f, 0.5f);
        
	Texture yellowHoneycombTexture;
	if (!yellowHoneycombTexture.loadFromFile("Textures/honeycomb.png")) {
    	cout<<"Error: Could not load yellow honeycomb texture!"<<endl;
    	return -1;
	}
	Sprite yellowHoneycombSprite(yellowHoneycombTexture);

	//Bee Variables
	const int beeCount = 20;
	float beeX[beeCount]{}, beeY[beeCount]{};
	bool beeActive[beeCount]{};
	int beeDirection[beeCount]{};
	Clock beeStopClocks[beeCount];
	bool beeStopped[beeCount]{};
	
	//Initialize bee positions, activity, and directions
	for (int i = 0; i < beeCount; i++) {
    	beeX[i] = rand() % resolutionX;                  //Random initial X position within screen width
    	beeY[i] = rand() % (resolutionY / 2);            //Random initial Y position (upper half of screen)
    	beeActive[i] = true;                             //Initially, all bees are active
    	beeDirection[i] = (rand() % 2 == 0) ? 1 : -1;    //Randomly choose a direction (left or right)
    	beeStopped[i] = false;                           //Initially, bees are not stopped
	}

	initializeBees(beeX, beeY, beeActive, beeCount);
	
	//Arrays for honeycomb management
	float honeycombX[beeCount]{}, honeycombY[beeCount]{};
	
	//Tracks if a honeycomb is active or not
	bool honeycombActive[beeCount]{}; 
	for (int i = 0; i < beeCount; i++) {
    	honeycombX[i] = 0;
    	honeycombY[i] = 0;
    	honeycombActive[i] = false;                      //All honeycombs are inactive at first
	}
	//Generate 3 honeycombs at the start
	for (int i = 0; i < 3; i++) {
    	honeycombX[i] = rand() % (resolutionX - boxPixelsX);
    	honeycombY[i] = rand() % (resolutionY / 2);
    	honeycombActive[i] = true;
	}

	float hummingbird_x, hummingbird_y;
	int hummingbirdDirectionX, hummingbirdDirectionY;
	Clock hummingbirdClock, hummingbirdPauseClock, hummingbirdSickClock;
	bool hummingbirdPaused = false;
	bool hummingbirdSick = false;
	int hummingbirdHits = 0;
	const int maxHummingbirdHits = 3;
	const int hummingbirdPauseDuration = 1000; //milliseconds
	const int hummingbirdSickDuration = 5000; //milliseconds

	// Beehive Variables
	float beehiveX[beeCount]{}, beehiveY[beeCount]{};
	bool beehiveActive[beeCount]{};

	hummingbird_x = rand() % resolutionX;
	hummingbird_y = rand() % resolutionY;
	hummingbirdDirectionX = (rand() % 2 == 0) ? 1 : -1;
	hummingbirdDirectionY = (rand() % 2 == 0) ? 1 : -1;

	Texture hummingbirdTexture;
	hummingbirdTexture.loadFromFile("Textures/hummingbird.png");
	Sprite hummingbirdSprite(hummingbirdTexture);
	hummingbirdSprite.setScale(2.0f, 2.0f);

	Texture beeTexture;
	beeTexture.loadFromFile("Textures/Regular_bee.png");
	Sprite beeSprite(beeTexture);

	//Flower Variables
	bool flowers[gameRows][gameColumns] = { false };
        
        //Level 3 Variables
        const int level3RegularBeeCount = 20;
        const int level3FastBeeCount = 10;
        const int level3HoneycombCount = 15;

        //Regular Bees ( The Worker Bees )
        float level3BeeX[level3RegularBeeCount], level3BeeY[level3RegularBeeCount];
        bool level3BeeActive[level3RegularBeeCount];
        int level3BeeDirection[level3RegularBeeCount];

        //Fast Bees ( The Killer Bees ) ( in Level 2 )
        float level3FastBeeX[level3FastBeeCount], level3FastBeeY[level3FastBeeCount];
        bool level3FastBeeActive[level3FastBeeCount];
        int level3FastBeeDirection[level3FastBeeCount];
 
       //Honeycombs
       float level3HoneycombX[level3HoneycombCount], level3HoneycombY[level3HoneycombCount];
       bool level3HoneycombActive[level3HoneycombCount];
 
	//Flower Texture
	Texture flowerTexture;
	flowerTexture.loadFromFile("Textures/obstacles.png");
	Sprite flowerSprite(flowerTexture);

	//Beehive Texture
	Texture beehiveTexture;
	if (!beehiveTexture.loadFromFile("Textures/hive.png")) {
    	cout<<"Error: Could not load beehive texture!" << endl;
    	return -1;
	}
	Sprite beehiveSprite(beehiveTexture);
	
	//Score
	int score = 0;
	Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(24);
	scoreText.setFillColor(Color::White);
	scoreText.setPosition(10.f, resolutionY - 40.f);
       
        //Level 2 Variables
        const int regularBeeCount = 15;
        const int fastBeeCount = 5;

       //Fast Bees
       float fastBeeX[fastBeeCount], fastBeeY[fastBeeCount];
       bool fastBeeActive[fastBeeCount];
       int fastBeeDirection[fastBeeCount];

      //Red Honeycombs
      float redHoneycombX[fastBeeCount], redHoneycombY[fastBeeCount];
      bool redHoneycombActive[fastBeeCount];

      //Additional Variables
      int level = 1;  //Track the current game level
     //Create Yellow Honeycombs for Pre-Generation
     const int preGeneratedHoneycombCount = 3;
     for (int i = 0; i < preGeneratedHoneycombCount; i++) {
     honeycombX[i] = rand() % (resolutionX - boxPixelsX);
     honeycombY[i] = rand() % (resolutionY / 2);
     honeycombActive[i] = true;
}

    //Fast Bee Texture and Sprite
    Texture fastBeeTexture;
   if (!fastBeeTexture.loadFromFile("Textures/Fast_bee.png")) {
    cout<<"Error: Could not load fast bee texture!"<<endl;
    return -1;
}
   Sprite fastBeeSprite(fastBeeTexture);

   //Red Honeycomb Texture and Sprite
    Texture redHoneycombTexture;
    if (!redHoneycombTexture.loadFromFile("Textures/honeycomb_red.png")) {
    cout<<"Error: Could not load red honeycomb texture!"<<endl;
    return -1;
}

Sprite redHoneycombSprite(redHoneycombTexture);


	while (window.isOpen()) {
    	//Event Handling
    	Event e;
    	while (window.pollEvent(e)) {
        	if (e.type == Event::Closed) 
        	return 0;
    	}

    	//Player Movement
    	//Moves the player left or right when the corresponding keys are pressed.
        //Prevents the player from moving outside the screen boundaries.
    	if (Keyboard::isKeyPressed(Keyboard::Left) && player_x > 0) player_x -= 0.2f;
    	if (Keyboard::isKeyPressed(Keyboard::Right) && player_x < resolutionX - boxPixelsX) player_x += 0.2f;

    	//Bullet Firing
    	if (Keyboard::isKeyPressed(Keyboard::Space) && !bullet_exists && spraysLeft > 0) {
        	bullet_x = player_x + (static_cast<float>(boxPixelsX) / 2) - 12; // Center bullet
        	bullet_y = player_y;
        	bullet_exists = true;
        	bulletClock.restart();
        	spraysLeft--;

        	//Check if we need to switch to a new spray can
        	if (spraysLeft == 0 && sprayCans > 1) {
            	sprayCans--;
            	spraysLeft = maxSpraysPerCan;
        	}
    	}

    	//Bullet Movement
    	if (bullet_exists) moveBullet(bullet_y, bullet_exists, bulletClock);

    	//Collision Check
    	if (bullet_exists) {
        	checkBulletCollision(bullet_x, bullet_y, beeX, beeY, beeActive, honeycombX, honeycombY, honeycombActive, score, beeCount, bullet_exists, hummingbird_x, hummingbird_y, hummingbirdSick, hummingbirdHits, hummingbirdSickClock, beehiveX, beehiveY, beehiveActive);
   	 

    	}
    	//Move the bees
    	moveBees(beeX, beeY, beeActive, beeDirection, beeStopClocks, beeStopped, beeCount, honeycombX, honeycombY, honeycombActive, flowers, player_x, player_y, sprayCans, spraysLeft, maxSpraysPerCan, beehiveX, beehiveY, beehiveActive);

  	 
    	//Move the hummingbird
    	moveHummingbird(hummingbird_x, hummingbird_y, hummingbirdDirectionX,hummingbirdDirectionY, hummingbirdPaused, hummingbirdSick, hummingbirdHits, hummingbirdClock, hummingbirdPauseClock, hummingbirdSickClock, honeycombX, honeycombY, honeycombActive, score, beeCount);
    	
    	//Check if the player is out of spray cans
    	if (sprayCans == 0 && spraysLeft == 0) {
        	Text message("Out of spray cans! Restarting Level 1...", font, 24);
        	message.setFillColor(Color::Red);
        	message.setPosition(200.f, resolutionY / 2.f);
        	window.clear();
        	window.draw(message);
        	window.display();
        	sleep(milliseconds(3000));           //Display the message for 3 seconds
        	main();                              //Restart the game
        	return 0;
    	}
    	                                            //Check if the player is surrounded by flowers
    	if (isPlayerSurrounded(player_x, player_y, flowers)) {
        	Text message("You're surrounded! Restarting Level 1...", font, 24);
        	message.setFillColor(Color::Red);
        	message.setPosition(200.f, resolutionY / 2.f);
        	window.clear();
        	window.draw(message);
        	window.display();
        	sleep(milliseconds(3000));         //Display the message for 3 seconds
        	main();                            //Restart the game
        	return 0;
    	}
    	                                           //Check if All Bees Are Inactive
if (allBeesInactive(beeActive, beeCount, fastBeeActive, fastBeeCount)) {

                                                    //Display Level Transition
    showLevelTransition(window, font, 2);

                                                    //Initialize Level 2 Bees and Honeycombs
    initializeBees(beeX, beeY, beeActive, regularBeeCount);  // Regular Bees
    initializeLevel2Bees(fastBeeX, fastBeeY, fastBeeActive, fastBeeDirection, fastBeeCount);  //Fast Bees
    initializeRedHoneycombs(redHoneycombX, redHoneycombY, redHoneycombActive, fastBeeCount);  //Red Honeycombs
    //Reset Spray Cans and Sprays Left
     sprayCans = 3;
     spraysLeft = maxSpraysPerCan;

    //Pre-Generated Yellow Honeycombs
    for (int i = 0; i < 3; i++) {
        honeycombX[i] = rand() % (resolutionX - boxPixelsX);
        honeycombY[i] = rand() % (resolutionY / 2);
        honeycombActive[i] = true;
    }

    //Update the game state for Level 2
    level = 2;
}

 
    if (bullet_exists && level == 2) {
         checkBulletCollisionLevel2(
        bullet_x, bullet_y,
        fastBeeX, fastBeeY, fastBeeActive, fastBeeCount,
        redHoneycombX, redHoneycombY, redHoneycombActive,
        bullet_exists, score);
    }
    //Check if All Bees Are Inactive (Level 2 Complete)
if (allBeesInactive(beeActive, regularBeeCount, fastBeeActive, fastBeeCount) && level == 2) {
    showLevelTransition(window, font, 3);

    //Initialize Level 3 Bees
    initializeBees(level3BeeX, level3BeeY, level3BeeActive, level3RegularBeeCount);
    initializeLevel2Bees(level3FastBeeX, level3FastBeeY, level3FastBeeActive, level3FastBeeDirection, level3FastBeeCount);

    //Reset Spray Cans and Sprays Left
    sprayCans = 3;
    spraysLeft = maxSpraysPerCan;
    //Initialize Honeycombs
    for (int i = 0; i < level3HoneycombCount; i++) {
        level3HoneycombX[i] = rand() % (resolutionX - boxPixelsX);
        level3HoneycombY[i] = rand() % (resolutionY / 2);
        level3HoneycombActive[i] = true;
    }

    level = 3;
}


    	//Drawing
    	window.clear();
    	// Update and Draw Game Elements for Current Level
if (level == 2) {
   //Calling of Respective Functions to implement Level 2
    moveFastBees(fastBeeX, fastBeeY, fastBeeActive, fastBeeDirection, fastBeeCount);
    drawBees(window, beeX, beeY, beeActive, beeSprite, regularBeeCount);
    drawFastBees(window, fastBeeX, fastBeeY, fastBeeActive, fastBeeSprite, fastBeeCount);
    drawHoneycombs(window, honeycombX, honeycombY, honeycombActive, yellowHoneycombSprite, 9);
    drawRedHoneycombs(window, redHoneycombX, redHoneycombY, redHoneycombActive, redHoneycombSprite, fastBeeCount);

    moveHummingbird(hummingbird_x, hummingbird_y, hummingbirdDirectionX, hummingbirdDirectionY,hummingbirdPaused, hummingbirdSick, hummingbirdHits,hummingbirdClock,hummingbirdPauseClock, hummingbirdSickClock,
    honeycombX, honeycombY, honeycombActive, score, beeCount);

}
if (level == 3) {
    //Calling of Respective Functions to implement Level 3
    moveBees(level3BeeX, level3BeeY, level3BeeActive, level3BeeDirection, beeStopClocks, beeStopped, level3RegularBeeCount, 
             level3HoneycombX, level3HoneycombY, level3HoneycombActive, flowers, player_x, player_y, sprayCans, spraysLeft, maxSpraysPerCan, 
             beehiveX, beehiveY, beehiveActive);

    moveFastBees(level3FastBeeX, level3FastBeeY, level3FastBeeActive, level3FastBeeDirection, level3FastBeeCount);

    drawBees(window, level3BeeX, level3BeeY, level3BeeActive, beeSprite, level3RegularBeeCount);
    drawFastBees(window, level3FastBeeX, level3FastBeeY, level3FastBeeActive, fastBeeSprite, level3FastBeeCount);
    drawHoneycombs(window, level3HoneycombX, level3HoneycombY, level3HoneycombActive, yellowHoneycombSprite, level3HoneycombCount);
}


    	drawPlayer(window, player_x, player_y, playerSprite);
    	if (bullet_exists) drawBullet(window, bullet_x, bullet_y, bulletSprite);
    	drawBees(window, beeX, beeY, beeActive, beeSprite, beeCount);
    	drawHoneycombs(window, honeycombX, honeycombY, honeycombActive, yellowHoneycombSprite, beeCount);
    	drawSprayCans(window, sprayCans, spraysLeft, sprayCanSprite, maxSpraysPerCan);
    	drawFlowers(window, flowerSprite, flowers); // Draw flowers

    	drawHummingbird(window, hummingbird_x, hummingbird_y, hummingbirdSprite, hummingbirdSick);
    	// Draw beehives
    	for (int i = 0; i < beeCount; i++) {
        	if (beehiveActive[i]) {
            	//Draw the beehive
            	beehiveSprite.setPosition(beehiveX[i], beehiveY[i]);
            	window.draw(beehiveSprite);
        	}
    	}
    	//Score Display at the bottom
    	scoreText.setString("Score: " + to_string(score));
    	window.draw(scoreText);

    	window.display();
	}

	return 0;
}

//Function Definitions

//Purpose : This function draws the player on the game window. The player’s position is updated using player_x and player_y, and the playerSprite is used to display the image.
void drawPlayer(RenderWindow& window, float& player_x, float& player_y, Sprite& playerSprite) {
	playerSprite.setPosition(player_x, player_y);
	window.draw(playerSprite);
}

//Purpose :Draws the bullet on the screen at its current position.
void drawBullet(RenderWindow& window, float& bullet_x, float& bullet_y, Sprite& bulletSprite) {
	bulletSprite.setPosition(bullet_x, bullet_y);
	window.draw(bulletSprite);
}

//Purpose : Places bees at random starting positions and makes them active.
void initializeBees(float beeX[], float beeY[], bool beeActive[], const int beeCount) {
	for (int i = 0; i < beeCount; i++) {
    	beeX[i] = rand() % (resolutionX - boxPixelsX);
    	beeY[i] = rand() % (resolutionY / 3);
    	beeActive[i] = true;
	}
}

//Purpose :Moves bees left or right. If bees hit the screen’s edge, they change direction and drop down. If they reach the bottom, they stop.
void moveBees(float beeX[], float beeY[], bool beeActive[], int beeDirection[], Clock beeStopClocks[], bool beeStopped[], const int beeCount, float honeycombX[], float honeycombY[], bool honeycombActive[], bool flowers[gameRows][gameColumns], float& player_x, float& player_y, int& sprayCans, int& spraysLeft, const int maxSpraysPerCan, float beehiveX[], float beehiveY[], bool beehiveActive[]) {
	//Loop through each bee
	for (int i = 0; i < beeCount; i++) {
    	if (!beeActive[i]) continue;                //Skip inactive bees

    	//Randomly stop bees for a brief duration
    	if (!beeStopped[i] && rand() % 10000 < 1) { //0.01% chance to stop
        	beeStopped[i] = true;
        	beeStopClocks[i].restart();
    	}
    	if (beeStopped[i]) {
        	if (beeStopClocks[i].getElapsedTime().asMilliseconds() > 1000) { // Stop for 1000 milliseconds
            	beeStopped[i] = false;
        	}
        	else {
            	continue; //Skip movement if bee is stopped
        	}
    	}

    	     //Check if the bee hits the screen edges or gets stuck between a honeycomb and the side of the screen
    	if (beeX[i] <= 0 || beeX[i] >= resolutionX - boxPixelsX) {
        	bool stuckBetweenHoneycomb = false;
        	for (int j = 0; j < beeCount; j++) {
            	if (honeycombActive[j] && abs(beeX[i] - honeycombX[j]) < boxPixelsX && abs(beeY[i] - honeycombY[j]) < boxPixelsY) {
                	stuckBetweenHoneycomb = true;
                	break;
            	}
        	}
        	if (stuckBetweenHoneycomb) {
                //Create a beehive if the bee gets stuck between the end of the screen and a honeycomb
            	beehiveX[i] = beeX[i];
            	beehiveY[i] = beeY[i];
            	beehiveActive[i] = true;
            	beeActive[i] = false;                     //Deactivate the bee
        	}
        	else {
            	beeDirection[i] = -beeDirection[i];      //Reverse direction
            	beeY[i] += 40;                  	// Drop to the next tier
        	}
    	}

    	//Moving the bee horizontally
    	beeX[i] += 0.12f * beeDirection[i]; //Adjust the speed with the multiplier

    	//Check if the bee reaches the bottom
    	if (static_cast<int>(beeY[i] / boxPixelsY) == gameColumns - 1) {
        	beeActive[i] = false; //Deactivate the bee
        	//Create flowers at the edges
        	 {
            	int flowerX = static_cast<int>(beeX[i] / boxPixelsX);
            	int flowerY = gameColumns - 1;
            	flowers[flowerX][flowerY] = true;
        	}
    	}
	}

	//Check for collisions with honeycombs
 	checkBeeHoneycombCollision(beeX, beeY, beeActive, beeDirection, honeycombX, honeycombY, honeycombActive, beeCount);

	//Pollinate flowers when bees reach the bottom
 	pollinateFlowers(beeX, beeY, beeActive, beeCount, flowers);
}


//Purpose:Moves the bullet upward if it exists. If the bullet goes off the screen, it disappears.
void moveBullet(float& bullet_y, bool& bullet_exists, Clock& bulletClock) {
	//Move the bullet upwards at a fixed speed
	if (bullet_exists) {
    	bullet_y -= 0.3f; //Adjust speed as needed

    	//If the bullet goes off the screen, reset it
    	if (bullet_y < 0) {
        bullet_exists = false; //Bullet no longer exists
    	}
	}
}

//Purpose: Draws all active bees on the game window.
void drawBees(RenderWindow& window, float beeX[], float beeY[], bool beeActive[], Sprite& beeSprite, const int beeCount) {
	for (int i = 0; i < beeCount; i++) {
    	if (!beeActive[i]) continue;
    	beeSprite.setPosition(beeX[i], beeY[i]);
    	window.draw(beeSprite);
	}
}



void drawHoneycombs(RenderWindow& window, float honeycombX[], float honeycombY[], bool honeycombActive[],
	Sprite& yellowHoneycombSprite, const int beeCount) {
	for (int i = 0; i < beeCount; i++) {
    	if (!honeycombActive[i]) continue;

    	//Draw the yellow honeycomb
    	yellowHoneycombSprite.setPosition(honeycombX[i], honeycombY[i]);
    	window.draw(yellowHoneycombSprite);
	}
}

void drawSprayCans(RenderWindow& window, int sprayCans, int spraysLeft, Sprite& sprayCanSprite, const int maxSpraysPerCan) {
	for (int i = 0; i < sprayCans - 1; i++) {
    	sprayCanSprite.setPosition(10.f + i * 40.f, resolutionY - 80.f);
    	window.draw(sprayCanSprite);
	}

	//Draw the current spray level
	RectangleShape sprayLevel(Vector2f(30.f, (spraysLeft / (float)maxSpraysPerCan) * 60.f));
	sprayLevel.setFillColor(Color::Green);
	sprayLevel.setPosition(10.f + (sprayCans - 1) * 40.f, resolutionY - 80.f + (60.f - sprayLevel.getSize().y));
	window.draw(sprayLevel);
}

void checkBeeHoneycombCollision(float beeX[], float beeY[], bool beeActive[], int beeDirection[], float honeycombX[], float honeycombY[], bool honeycombActive[], const int beeCount) {
	for (int i = 0; i < beeCount; i++) {
    	if (!beeActive[i]) continue;

    	for (int j = 0; j < beeCount; j++) {
        	if (!honeycombActive[j]) continue;

        	//Check for collision between bee and honeycomb
        	if (abs(beeX[i] - honeycombX[j]) < boxPixelsX && abs(beeY[i] - honeycombY[j]) < boxPixelsY) {
            	beeDirection[i] = -beeDirection[i];      //Reverses the  direction when bee collides with honeycomb
            	beeY[i] += 40;                  	//Drop to the next tier
            	break;                                 //Exit the inner loop once a collision is detected
        	}
    	}
	}
}
void moveHummingbird(float& hummingbird_x, float& hummingbird_y, int& hummingbirdDirectionX, int& hummingbirdDirectionY, bool& hummingbirdPaused, bool& hummingbirdSick, int& hummingbirdHits, Clock& hummingbirdClock, Clock& hummingbirdPauseClock, Clock& hummingbirdSickClock, float honeycombX[], float honeycombY[], bool honeycombActive[], int& score, const int beeCount) {
	if (hummingbirdSick) {
   	 
        	const int hummingbirdSickDuration = 5000; // milliseconds

        	if (hummingbirdSickClock.getElapsedTime().asMilliseconds() > hummingbirdSickDuration) {
        	hummingbirdSick = false;
        	hummingbirdHits = 0;
        	hummingbird_x = rand() % resolutionX;
        	hummingbird_y = rand() % resolutionY;
    	}
    	else {
        	return;
    	}
	}

	if (hummingbirdPaused) {
    	const int hummingbirdPauseDuration = 1000; //milliseconds
    	if (hummingbirdPauseClock.getElapsedTime().asMilliseconds() > hummingbirdPauseDuration) {
      	 
        	hummingbirdPaused = false;
    	}
    	else {
        	return;
    	}
	}

	if (rand() % 100 < 1) {                  //1% chance to change direction ( in order to make the hummingBird change its direction randomly )
    	hummingbirdDirectionX = (rand() % 2 == 0) ? 1 : -1;
    	hummingbirdDirectionY = (rand() % 2 == 0) ? 1 : -1;
    	hummingbirdPauseClock.restart();
    	hummingbirdPaused = true;
	}

	//Increase the speed multiplier here
	float speedMultiplier = 1.0f;     //Adjust this value to increase the speed

	hummingbird_x += speedMultiplier * hummingbirdDirectionX;
	hummingbird_y += speedMultiplier * hummingbirdDirectionY;

	if (hummingbird_x <= 0 || hummingbird_x >= resolutionX - boxPixelsX) {
    	hummingbirdDirectionX = -hummingbirdDirectionX;
	}
	if (hummingbird_y <= 0 || hummingbird_y >= resolutionY - boxPixelsY) {
    	hummingbirdDirectionY = -hummingbirdDirectionY;
	}

	for (int i = 0; i < beeCount; i++) {
    	if (!honeycombActive[i]) continue;

    	if (abs(hummingbird_x - honeycombX[i]) < boxPixelsX && abs(hummingbird_y - honeycombY[i]) < boxPixelsY) {
        	honeycombActive[i] = false;
        	if (honeycombY[i] < static_cast<float>(resolutionY) / 3) {
            	score += 1000;
        	}
        	else if (honeycombY[i] < static_cast<float>(2 * resolutionY) / 3) {
            	score += 800;
        	}
        	else {
            	score += 500;
        	}
    	}
	}
}
void drawHummingbird(RenderWindow& window, float hummingbird_x, float hummingbird_y, Sprite& hummingbirdSprite, bool hummingbirdSick) {
	if (hummingbirdSick) {
    	hummingbirdSprite.setColor(Color::Green);
    	return;
	}
	else {
    	hummingbirdSprite.setColor(Color::White);
	}
	hummingbirdSprite.setPosition(hummingbird_x, hummingbird_y);
	window.draw(hummingbirdSprite);
}

//Purpose: Creates flowers when bees reach the bottom row.
void pollinateFlowers(float beeX[], float beeY[], bool beeActive[], const int beeCount, bool flowers[gameRows][gameColumns]) {
	for (int i = 0; i < beeCount; i++) {
    	if (!beeActive[i]) continue;

    	//Check if the bee reaches the bottom row
    	
    	if (static_cast<int>(beeY[i] / boxPixelsY) == gameColumns - 1) {
        	beeActive[i] = false; // Deactivate the bee

        	//Pollinate flowers at the bottom row
        	
        	int flowerX = static_cast<int>(beeX[i] / boxPixelsX);
        	int flowerY = gameColumns - 1;

        	//Create a flower at the bee's position
        	flowers[flowerX][flowerY] = true;
        	cout<<"Flower created at ("<<flowerX << ", " <<flowerY <<")" << endl;

    	}
	}
}
//Pupose : To Draw The Flowers 
void drawFlowers(RenderWindow& window, Sprite& flowerSprite, bool flowers[gameRows][gameColumns]) {
	for (int x = 0; x < gameRows; x++) {
    	for (int y = 0; y < gameColumns; y++) {
        	if (flowers[x][y]) {
            	flowerSprite.setPosition(x * boxPixelsX, y * boxPixelsY);
            	window.draw(flowerSprite);
        	}
    	}
	}
}

//Purpose : Checks if the bullet has hit: A bee → Deactivates the bee and creates a honeycomb. A honeycomb → Deactivates the honeycomb.A beehive → Deactivates the beehive.
void checkBulletCollision(float bullet_x, float bullet_y,
	float beeX[], float beeY[], bool beeActive[],
	float honeycombX[], float honeycombY[], bool honeycombActive[],
	int& score, const int beeCount, bool& bullet_exists,
	float& hummingbird_x, float& hummingbird_y, bool& hummingbirdSick, int& hummingbirdHits, Clock& hummingbirdSickClock,
	float beehiveX[], float beehiveY[], bool beehiveActive[]) {

	for (int i = 0; i < beeCount; i++) {
    	if (!beeActive[i]) continue;

    	//Check for collision between bullet and bee
    	if (abs(bullet_x - beeX[i]) < boxPixelsX && abs(bullet_y - beeY[i]) < boxPixelsY) {
        	beeActive[i] = false;    //Deactivate the bee
        	score += 100;     	//Award points for hitting the bee

        	//Create a yellow honeycomb at the bee's position
        	honeycombX[i] = beeX[i];
        	honeycombY[i] = beeY[i];
        	honeycombActive[i] = true;

        	bullet_exists = false;         //Deactivate the bullet
        	return;                        //Exit the function after collision
    	}
	}

	//Check for collision between bullet and honeycomb
	for (int i = 0; i < beeCount; i++) {
    	if (!honeycombActive[i]) continue;

    	if (abs(bullet_x - honeycombX[i]) < boxPixelsX && abs(bullet_y - honeycombY[i]) < boxPixelsY) {
        	honeycombActive[i] = false;    //Deactivate the honeycomb
        	bullet_exists = false;        //Deactivate the bullet
        	return;                      //Exit the function after collision
    	}
	}

	//Check for collision between bullet and beehive
	for (int i = 0; i < beeCount; i++) {
    	if (!beehiveActive[i]) continue;

    	if (abs(bullet_x - beehiveX[i]) < boxPixelsX && abs(bullet_y - beehiveY[i]) < boxPixelsY) {
        	beehiveActive[i] = false;          //Deactivate the beehive
        	bullet_exists = false;             //Deactivate the bullet
        	return;                            //Exit the function after collision
    	}
	}

	//Check for collision between bullet and hummingbird
	if (abs(bullet_x - hummingbird_x) < boxPixelsX && abs(bullet_y - hummingbird_y) < boxPixelsY) {
    	hummingbirdHits++;
    	int maxHummingbirdHits = 3;
    	if (hummingbirdHits >= maxHummingbirdHits) {
        	hummingbirdSick = true;
        	hummingbirdSickClock.restart();
    	}
    	bullet_exists = false; //Deactivate the bullet
	}
}
bool isPlayerSurrounded(float player_x, float player_y, bool flowers[gameRows][gameColumns]) {
	int playerRow = static_cast<int>(player_x / boxPixelsX);
	int playerCol = static_cast<int>(player_y / boxPixelsY);

	//Check if the player is surrounded by flowers on all four sides
	bool leftBlocked = (playerRow > 0) && flowers[playerRow - 1][playerCol];
	bool rightBlocked = (playerRow < gameRows - 1) && flowers[playerRow + 1][playerCol];
	bool topBlocked = (playerCol > 0) && flowers[playerRow][playerCol - 1];
	bool bottomBlocked = (playerCol < gameColumns - 1) && flowers[playerRow][playerCol + 1];

	return leftBlocked && rightBlocked && topBlocked && bottomBlocked;
}


void drawFastBees(RenderWindow& window, float fastBeeX[], float fastBeeY[], bool fastBeeActive[], Sprite& fastBeeSprite, int fastBeeCount) {
    for (int i = 0; i < fastBeeCount; i++) {
        if (!fastBeeActive[i]) continue;

        fastBeeSprite.setPosition(fastBeeX[i], fastBeeY[i]);
        window.draw(fastBeeSprite);
    }
}
//Purpose: As soon as a level is completed , it shows a window for the next level.
void showLevelTransition(RenderWindow& window, Font& font, int level) {
    Text levelMessage;
    levelMessage.setFont(font);
    levelMessage.setCharacterSize(40);
    levelMessage.setFillColor(Color::Yellow);
    levelMessage.setString("Level " + to_string(level) + " Starting...");
    levelMessage.setPosition(200.f, resolutionY / 2.f);

    window.clear();
    window.draw(levelMessage);
    window.display();
    sleep(milliseconds(3000));  //Pause for 3 Seconds
}

//An alternative of using the absolute function
float absolute(float value) {
    return (value < 0) ? -value : value;
}
//Checks whether the bees are active or not .
bool allBeesInactive(bool beeActive[], int beeCount, bool fastBeeActive[], int fastBeeCount) {
    for (int i = 0; i < beeCount; i++) if (beeActive[i]) return false;
    for (int i = 0; i < fastBeeCount; i++) if (fastBeeActive[i]) return false;
    return true;
}

//Function Definitions For Level 2 
void initializeLevel2Bees(float* fastBeeX, float* fastBeeY, bool* fastBeeActive, int* fastBeeDirection, int fastBeeCount) {
    for (int i = 0; i < fastBeeCount; i++) {
        fastBeeX[i] = rand() % (resolutionX - boxPixelsX);
        fastBeeY[i] = rand() % (resolutionY / 3);          //Top third of the screen
        fastBeeActive[i] = true;
        fastBeeDirection[i] = (rand() % 2 == 0) ? 1 : -1;  //Random direction
    }
}
void initializeRedHoneycombs(float* redHoneycombX, float* redHoneycombY, bool* redHoneycombActive, int honeycombCount) {
    for (int i = 0; i < honeycombCount; i++) {
        redHoneycombX[i] = rand() % (resolutionX - boxPixelsX);    //Random X position
        redHoneycombY[i] = rand() % (resolutionY / 2);            //Top half of the screen
        redHoneycombActive[i] = true;                             //Activate the honeycomb
    }
}
void drawRedHoneycombs(RenderWindow& window, float redHoneycombX[], float redHoneycombY[], bool redHoneycombActive[], Sprite& redHoneycombSprite, int fastBeeCount) {
    for (int i = 0; i < fastBeeCount; i++) {
        if (!redHoneycombActive[i]) continue;

        redHoneycombSprite.setPosition(redHoneycombX[i], redHoneycombY[i]);
        window.draw(redHoneycombSprite);
    }
}

void moveFastBees(float fastBeeX[], float fastBeeY[], bool fastBeeActive[], int fastBeeDirection[], const int fastBeeCount) {
    for (int i = 0; i < fastBeeCount; i++) {
        if (!fastBeeActive[i]) continue;

        //Move horizontally
        fastBeeX[i] += 0.1f * fastBeeDirection[i];        //Increased speed for fast bees than regular bees

        //Check screen edges for reversal
        if (fastBeeX[i] <= 0 || fastBeeX[i] >= resolutionX - boxPixelsX) {
            fastBeeDirection[i] = -fastBeeDirection[i];  //Reverses the  direction
            fastBeeY[i] += 40;                           //Drop down one tier when hitting the edge
        }

        //Deactivate if reaching the bottom of the screen
        if (fastBeeY[i] >= resolutionY - boxPixelsY) {
            fastBeeActive[i] = false;                  //Remove the bee from play
        }
    }
}
//Purpose : Checks collision for Bullets in Level 2
void checkBulletCollisionLevel2(float bullet_x, float bullet_y,float fastBeeX[], float fastBeeY[], bool fastBeeActive[], int fastBeeCount,float redHoneycombX[], float redHoneycombY[], bool redHoneycombActive[],bool& bullet_exists, int& score) {
    //Check Collision with Fast Bees
    for (int i = 0; i < fastBeeCount; i++) {
        if (!fastBeeActive[i]) continue;

        if (absolute(bullet_x - fastBeeX[i]) < boxPixelsX && absolute(bullet_y - fastBeeY[i]) < boxPixelsY) {
            fastBeeActive[i] = false;  //Deactivate Fast Bee
            score += 1000;             //Award Points for Hitting Bee

            //Create Red Honeycomb at Fast Bee's Position
            redHoneycombX[i] = fastBeeX[i];
            redHoneycombY[i] = fastBeeY[i];
            redHoneycombActive[i] = true;

            bullet_exists = false;  //Deactivate Bullet
            return;                //Exit after collision
        }
    }

           //Check Collision with Red Honeycombs
    for (int i = 0; i < fastBeeCount; i++) {
        if (!redHoneycombActive[i]) continue;

        if (absolute(bullet_x - redHoneycombX[i]) < boxPixelsX && absolute(bullet_y - redHoneycombY[i]) < boxPixelsY) {
            redHoneycombActive[i] = false;  //Deactivate Red Honeycomb
            bullet_exists = false;          //Deactivate Bullet
            return;                         //Exit after collision
        }
    }
}

//Declaration Of function that showcases a navigable menu at the beginning of the game.
int showMenu(RenderWindow& window, Font& font) {
	// Load background
	Texture backgroundTexture;
	Sprite background;
	if (!backgroundTexture.loadFromFile("background.jpeg")) {
    	cerr << "Error loading background texture!" << endl;
    	return -1;                   //Exit if texture cannot be loaded
	}
	background.setTexture(backgroundTexture);
	background.setPosition(0, 0);
        
	//To Display the Title
	Text title("Buzz Bombers", font, 40);
	title.setFillColor(Color::Yellow);
	title.setPosition(320, 100);

	//Menu options
	Text options[2];
	options[0] = Text("1. Start Game (Level 1)", font, 20);
	options[1] = Text("2. Exit", font, 20);

	for (int i = 0; i < 2; i++) {
    	options[i].setFillColor(Color::Blue);
    	options[i].setPosition(320, 200 + i * 50);
	}

	int selectedOption = 0;                           //Keep track of which option is selected
	options[selectedOption].setFillColor(Color::Red); //Highlight the first option

	while (window.isOpen()) {
    	Event e;
    	while (window.pollEvent(e)) {
        	if (e.type == Event::Closed) {
            	window.close();
            	return -1;                                //Exit the application
        	}
        	if (e.type == Event::KeyPressed) {
                 //Arrow key navigation
            	if (e.key.code == Keyboard::Up || e.key.code == Keyboard::Down) {
                	options[selectedOption].setFillColor(Color::Blue);         //Reset current color
                	selectedOption = (selectedOption + 1) % 2;                //Toggle between options
                	options[selectedOption].setFillColor(Color::Red);        //Highlight new selection
            	}
            	//Select option
            	if (e.key.code == Keyboard::Enter) {
                	return selectedOption + 1; //Return 1 for Level 1, 2 for Exit
            	}
            	//Direct key press (optional for numbers)
            	if (e.key.code == Keyboard::Num1) return 1;
            	if (e.key.code == Keyboard::Num2) return 2;
        	}
    	}

    	//Render the navigable menu
    	window.clear();
    	window.draw(background);
    	window.draw(title);
    	for (int i = 0; i < 2; i++) {
        	window.draw(options[i]);
    	}
    	window.display();
	}
	return -1;
}
