#include <SFML/Graphics.hpp>

#include "defs.h"
#include "Textures.h"

#include <iostream>
#include <string>
#include <limits>
#include "upng.h"

sf::Time TimePerFrame = sf::seconds(1 / FPS);
sf::Image* colorBuffer = nullptr;
sf::Texture* colorBufferTexture = nullptr;
//xsf::Image* wallTexture = nullptr;
sf::Uint32* textures[NUM_RAW_TEXTURES];
sf::Image* image[NUM_TEXTURES];

std::string textureFileNames[NUM_TEXTURES] = {
	"images/redbrick.png",
	"images/purplestone.png",
	"images/mossystone.png",
	"images/graystone.png",
	"images/colorstone.png",
	"images/bluestone.png",
	"images/wood.png",
	"images/eagle.png",
	"images/pikuma.png"
};

//const int map[MAP_NUM_ROWS][MAP_NUM_COLS] = {
	//{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1},
	//{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	//{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	//{1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1},
	//{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	//{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
	//{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
	//{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
	//{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1},
	//{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	//{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	//{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	//{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
//};
static const int map[MAP_NUM_ROWS][MAP_NUM_COLS] = {
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 2, 2, 0, 3, 0, 4, 0, 5, 0, 6, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 5},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 5},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 5},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 5, 5, 5, 5, 5}
};

struct Player
{
	float x{}, y{};
	float width, height{};
	int turnDirection{};	// -1 for left, +1 for right
	int walkDirection{}; // -1 for backwards, +1 for front
	float rotationAngle{};
	float rotationSpeed{};
	float walkSpeed{};
	float turnSpeed{};
} player;

struct Ray
{
	float rayAngle{};
	float wallHitX{}, wallHitY{};
	float distance{};
	bool wasHitVertical{};
	int isRayFacingUp{}, isRayFacingDown{};
	int isRayFacingLeft{}, isRayFacingRight{};
	int wallHitContent{};		// 0, 1, 2, 3, etc.
};

void drawLine(sf::RenderWindow& window, float x0, float y0, float x1, float y1, sf::Color color = sf::Color::White) {

	sf::Vertex line[] = {
		sf::Vertex({x0,y0}),
		sf::Vertex({x1,y1})
	};
	line[0].color = color;
	line[1].color = color;
	window.draw(line,2,sf::LineStrip);
}

class Game
{
public:
	Game()
		: window{ {WINDOW_WIDTH,WINDOW_HEIGHT},"Raycast" },
		player{},
		rays{}
		
	{
		window.setFramerateLimit(30);
		window.setPosition(sf::Vector2i{ window.getPosition().x,0 });
	}
	~Game() { 
		delete colorBuffer;
		delete colorBufferTexture; 
		//xdelete wallTexture;
	}

	void init() {
		player.x = WINDOW_WIDTH / 2;
		player.y = WINDOW_HEIGHT / 2;
		player.width = 1.f;
		player.height = 1.f;
		player.turnDirection = 0;
		player.walkDirection = 0;
		player.rotationAngle =  static_cast<float>(PI) / 2.f;
		player.walkSpeed = 100; //pixels
		player.turnSpeed = 45.f * (static_cast<float>(PI) / 180.f);	// degrees per second in radians (75 was 45)
	
		// allocate the total amount of bytes in memory to color
		colorBuffer = new sf::Image();
		colorBuffer->create(WINDOW_WIDTH, WINDOW_HEIGHT);

		colorBufferTexture = new sf::Texture();
		colorBufferTexture->create(WINDOW_WIDTH , WINDOW_HEIGHT);	

		// allocate the necessary space in memory to hold 64 x 64 uint32 values
		//xwallTexture = new sf::Image();
		//xwallTexture->create(TEX_WIDTH, TEX_HEIGHT);

		// load some textures from the textures.h
		textures[0] = (sf::Uint32*)REDBRICK_TEXTURE;
		textures[1] = (sf::Uint32*)PURPLESTONE_TEXTURE;
		textures[2] = (sf::Uint32*)MOSSYSTONE_TEXTURE;
		textures[3] = (sf::Uint32*)GRAYSTONE_TEXTURE;
		textures[4] = (sf::Uint32*)COLORSTONE_TEXTURE;
		textures[5] = (sf::Uint32*)BLUESTONE_TEXTURE;
		textures[6] = (sf::Uint32*)WOOD_TEXTURE;
		textures[7] = (sf::Uint32*)EAGLE_TEXTURE;


		for (auto i = 0; i < NUM_TEXTURES; ++i) {
			image[i] = new sf::Image();
			image[i]->create(TEX_WIDTH, TEX_HEIGHT);
			image[i]->loadFromFile(textureFileNames[i]);
		}
		//x Custom blue texture create a texture of blue and black lines  
		/*for (int x = 0; x < TEX_WIDTH; ++x) {
			for (int y = 0; y < TEX_HEIGHT; ++y) {
				wallTexture->setPixel(x, y, sf::Color(((x % 8 && y % 8) ? 0x0000FFFF : 0x000000FF)));
			}
		}*/
	}

	void renderMap() {
		for (auto i = 0; i < MAP_NUM_ROWS; ++i) {
			for (auto j = 0; j < MAP_NUM_COLS; ++j) {
				auto tileX = j * TILE_SIZE;
				auto tileY = i * TILE_SIZE;
				sf::Color tileColor = map[i][j] != 0 ? sf::Color::White : sf::Color::Black;

				sf::RectangleShape tileRect{ {TILE_SIZE * MINIMAP_SCALE_FACTOR,TILE_SIZE * MINIMAP_SCALE_FACTOR} };
				tileRect.setFillColor(tileColor);	tileRect.setOutlineThickness(1.f);	tileRect.setOutlineColor(sf::Color::Red);
				tileRect.setPosition(static_cast<float>(tileX) * MINIMAP_SCALE_FACTOR, static_cast<float>(tileY) * MINIMAP_SCALE_FACTOR);
			
				window.draw(tileRect);				 
			}
		}
	}

	void renderRays() {
		for (int i = 0; i < NUM_RAYS; i++) {
			drawLine(window, MINIMAP_SCALE_FACTOR * player.x, MINIMAP_SCALE_FACTOR * player.y, MINIMAP_SCALE_FACTOR * rays[i].wallHitX, MINIMAP_SCALE_FACTOR * rays[i].wallHitY, sf::Color::Red);
		}
	}

	void renderPlayer() {
		sf::RectangleShape playerRect{ {player.width * MINIMAP_SCALE_FACTOR, player.height * MINIMAP_SCALE_FACTOR} };
		playerRect.setFillColor(sf::Color::White);
		playerRect.setPosition(player.x * MINIMAP_SCALE_FACTOR, player.y *  MINIMAP_SCALE_FACTOR);
		playerRect.setRotation(player.rotationAngle);

		window.draw(playerRect);
		drawLine(window,
			MINIMAP_SCALE_FACTOR * player.x + (player.width/2),
			MINIMAP_SCALE_FACTOR * player.y + (player.height / 2),
			MINIMAP_SCALE_FACTOR * player.x  + cos(player.rotationAngle) * 40 /*hypotonuse*/ ,
			MINIMAP_SCALE_FACTOR * player.y  + sin(player.rotationAngle) * 40  /*hypotonuse*/,sf::Color{0,225,0});
	}

	float normalizeAngle(float angle) const
	{
		angle = remainder(angle, static_cast<float>(TWO_PI));
		if (angle < 0.f) {
			angle = static_cast<float>(TWO_PI) + angle;
		}
		return angle;
	}

	float distanceBetweenPoints(float x1, float y1, float x2, float y2) const
	{
		return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
	}

	void castRay(float rayAngle, int stripId) {
		rayAngle = normalizeAngle(rayAngle);
		const int isRayFacingDown = rayAngle > 0 && rayAngle < PI;		// 0 - 180 degrees
		const int isRayFacingUp = !isRayFacingDown;
		const int isRayFacingRight = rayAngle < 0.5 * PI || rayAngle > 1.5 * PI; 	// < 90 degress or > 270 degrees
		const int isRayFacingLeft = !isRayFacingRight;

		float xintercept = 0.f, yintercept = 0.f;
		float xstep		 = 0.f,  ystep	   = 0.f;

		///////////////////////////////////////////
	   // HORIZONTAL RAY-GRID INTERSECTION CODE
	   ///////////////////////////////////////////
		bool foundHorzWallHit = false;
		float horzWallHitX = 0;
		float horzWallHitY = 0;
		int horzWallContent = 0;

		// Find the y-coordinate of the closest horizontal grid intersection
		yintercept = floor(player.y / TILE_SIZE) * TILE_SIZE;
		yintercept += isRayFacingDown ? TILE_SIZE : 0;

		// Find the x-coordinate of the closest horizontal grid intersection
		xintercept = player.x + (yintercept - player.y) / tan(rayAngle);

		// Calculate the increment xstep and ystep
		ystep = TILE_SIZE;
		ystep *= isRayFacingUp ? -1 : 1;

		xstep = TILE_SIZE / tan(rayAngle);
		xstep *= (isRayFacingLeft && xstep > 0) ? -1 : 1;
		xstep *= (isRayFacingRight && xstep < 0) ? -1 : 1;

		float nextHorzTouchX = xintercept;
		float nextHorzTouchY = yintercept;

		// Increment xstep and ystep until we find a wall
		while (nextHorzTouchX >= 0 && nextHorzTouchX <= WINDOW_WIDTH && nextHorzTouchY >= 0 && nextHorzTouchY <= WINDOW_HEIGHT) {
			// where in the cell
			const float xToCheck = nextHorzTouchX;
			const float yToCheck = nextHorzTouchY + (isRayFacingUp ? - 1 : 0);	// push one pixel up

			if (hasWallAt(static_cast<int>(xToCheck), static_cast<int>(yToCheck))) {
				// there is a wall hit...
				horzWallHitX = nextHorzTouchX;
				horzWallHitY = nextHorzTouchY;
				horzWallContent = map[static_cast<int>(floor(yToCheck)) / TILE_SIZE][static_cast<int>(floor(xToCheck)) / TILE_SIZE];
				foundHorzWallHit = true;
				break;
			}
			else {
				nextHorzTouchX += xstep;
				nextHorzTouchY += ystep;
			}
		}

		///////////////////////////////////////////
		// VERTICAL RAY-GRID INTERSECTION CODE
		///////////////////////////////////////////
		bool foundVertWallHit = false;
		float vertWallHitX = 0;
		float vertWallHitY = 0;
		int vertWallContent = 0;

		// Find the x-coordinate of the closest horizontal grid intersection
		xintercept = floor(player.x / TILE_SIZE) * TILE_SIZE;
		xintercept += isRayFacingRight ? TILE_SIZE : 0;

		// Find the y-coordinate of the closest horizontal grid intersection
		yintercept = player.y + (xintercept - player.x) * tan(rayAngle);

		// Calculate the increment xstep and ystep
		xstep = TILE_SIZE;
		xstep *= isRayFacingLeft ? -1 : 1;

		ystep = TILE_SIZE * tan(rayAngle);
		ystep *= (isRayFacingUp && ystep > 0) ? -1 : 1;
		ystep *= (isRayFacingDown && ystep < 0) ? -1 : 1;

		float nextVertTouchX = xintercept;
		float nextVertTouchY = yintercept;

		// Increment xstep and ystep until we find a wall
		while (nextVertTouchX >= 0 && nextVertTouchX <= WINDOW_WIDTH && nextVertTouchY >= 0 && nextVertTouchY <= WINDOW_HEIGHT) {
			// where in the cell
			const float xToCheck = nextVertTouchX + (isRayFacingLeft ? -1 : 0); // push one pixel up
			const float yToCheck = nextVertTouchY; 

			if (hasWallAt(static_cast<int>(xToCheck), static_cast<int>(yToCheck))) {
				// there is a wall hit...
				vertWallHitX = nextVertTouchX;
				vertWallHitY = nextVertTouchY;
				vertWallContent = map[static_cast<int>(floor(yToCheck)) / TILE_SIZE][static_cast<int>(floor(xToCheck)) / TILE_SIZE];
				foundVertWallHit = true;
				break;
			}
			else {
				nextVertTouchX += xstep;
				nextVertTouchY += ystep;
			}
		}

		// Calculate both horizontal and vertical distances and choose the smallest value
		const float horzHitDistance = (foundHorzWallHit)
			                              ? distanceBetweenPoints(player.x, player.y, horzWallHitX, horzWallHitY)
			                              : FLT_MAX;
		const float vertHitDistance = (foundVertWallHit)
			                              ? distanceBetweenPoints(player.x, player.y, vertWallHitX, vertWallHitY)
			                              : FLT_MAX;

		// only store the smallest distance
		if (vertHitDistance < horzHitDistance) {
			rays[stripId].wallHitX = vertWallHitX;
			rays[stripId].wallHitY = vertWallHitY;
			rays[stripId].distance = vertHitDistance;
			rays[stripId].wallHitContent = vertWallContent;
			rays[stripId].wasHitVertical = true;
		}
		else {
			rays[stripId].wallHitX = horzWallHitX;
			rays[stripId].wallHitY = horzWallHitY;
			rays[stripId].distance = horzHitDistance;
			rays[stripId].wallHitContent = horzWallContent;
			rays[stripId].wasHitVertical = false;
		}

		rays[stripId].rayAngle = rayAngle;
		rays[stripId].isRayFacingDown = isRayFacingDown;
		rays[stripId].isRayFacingUp = isRayFacingUp;
		rays[stripId].isRayFacingLeft = isRayFacingLeft;
		rays[stripId].isRayFacingRight = isRayFacingRight;
	}

	void castAllRays() {
		// start first ray subtracting half of our FOV
		float rayAngle = player.rotationAngle - (static_cast<float>(FOV_ANGLE) / 2.f);

		for (int stripid = 0; stripid < NUM_RAYS; ++stripid) {
			castRay(rayAngle, stripid);
			rayAngle += float(FOV_ANGLE / NUM_RAYS);  // by 1
		}
	}

	void movePlayer(float dt) {
		player.rotationAngle += player.turnDirection * player.turnSpeed * dt;
		const float moveStep = player.walkDirection * player.walkSpeed * dt;

		const float newPlayerX = player.x + cos(player.rotationAngle) * moveStep;
		const float newPlayerY = player.y + sin(player.rotationAngle) * moveStep;

		if (!hasWallAt(static_cast<int>(newPlayerX), static_cast<int>(newPlayerY))) {
			player.x = newPlayerX;
			player.y = newPlayerY;
		}
	}

	void processInput() {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Escape)
					window.close();
				else if (event.key.code == sf::Keyboard::W)
					player.walkDirection = +1;
				else if (event.key.code == sf::Keyboard::A)
					player.turnDirection = -1;
				else if (event.key.code == sf::Keyboard::S)
					player.walkDirection = -1;
				else if (event.key.code == sf::Keyboard::D)
					player.turnDirection = +1;
			}
			else if (event.type == sf::Event::KeyReleased) {
				{
					if (event.key.code == sf::Keyboard::W)
						player.walkDirection = 0;
					else if (event.key.code == sf::Keyboard::A)
						player.turnDirection = 0;
					else if (event.key.code == sf::Keyboard::S)
						player.walkDirection = 0;
					else if (event.key.code == sf::Keyboard::D)
						player.turnDirection = 0;
				}
			}
		}
	}

	void update(float dt) {

		movePlayer(dt);
		castAllRays();
	}

	void clearColorBuffer(sf::Uint8 color) {
		for (int x = 0; x < WINDOW_WIDTH; ++x) {
			for (int y = 0; y < WINDOW_HEIGHT; ++y) {
				if(x == y)
				{
					/*	sf::Uint8 r = (rgbaPixel & 0x00ff0000) >> 16;
						sf::Uint8 g = (rgbaPixel & 0x0000ff00) >> 8;
						sf::Uint8 b = (rgbaPixel & 0x000000ff);
						sf::Uint8 a = (rgbaPixel & 0xff000000) >> 24;*/
					
					/*const sf::Uint32 rgbaPixel = ((pixel & 0xff000000) >> 24) | ((pixel & 0x00ffffff) << 8);		// or to an RGBA uint32: */

					drawPixel(x, y,color);
				}
				else {
					drawPixel(x, y, 0xFFFFFFFF);

				}
			}
		}
	}

	void drawPixel(int x, int y, sf::Uint32 color) {
		const sf::Uint32 rgbaPixel = ((color & 0xff000000) >> 24) | ((color & 0x00ffffff) << 8);		// or to an RGBA uint32:

		colorBuffer->setPixel(x,y,sf::Color(rgbaPixel));
	}
	void drawPixel(int x, int y, sf::Color color) {
		colorBuffer->setPixel(x, y, color);
	}
	// Change the color intensity based on a factor value between 0 and 1
	void changeColorIntensity(sf::Uint32* color, float factor) {
		sf::Uint32 a = (*color & 0xFF000000);
		sf::Uint32 r = (*color & 0x00FF0000) * factor;
		sf::Uint32 g = (*color & 0x0000FF00) * factor;
		sf::Uint32 b = (*color & 0x000000FF) * factor;

		*color = a | (r & 0x00FF0000) | (g & 0x0000FF00) | (b & 0x000000FF);
	}

	void generate3DProjection() {

		for (int x = 0; x < NUM_RAYS; x++) {
			const float perpDistance = rays[x].distance * cos(rays[x].rayAngle - player.rotationAngle);
			const float projectedWallHeight = (TILE_SIZE / perpDistance) * DIST_PROJ_PLANE;

			const int wallStripHeight = static_cast<int>(projectedWallHeight);

			int wallTopPixel = (WINDOW_HEIGHT / 2) - (wallStripHeight / 2);
			wallTopPixel = wallTopPixel < 0 ? 0 : wallTopPixel;

			int wallBottomPixel = (WINDOW_HEIGHT / 2) + (wallStripHeight / 2);
			wallBottomPixel = wallBottomPixel > WINDOW_HEIGHT ? WINDOW_HEIGHT : wallBottomPixel;

			// set the color of the ceiling
			for (int y = 0; y < wallTopPixel; ++y)
				drawPixel(x, y, 0xFF333333);

			// calculate textureOffsetX
			int textureOffsetX;
			if (rays[x].wasHitVertical)
				textureOffsetX = static_cast<int>(rays[x].wallHitY) % TILE_SIZE;		// perform offset for the vertical hit
			else
				textureOffsetX = static_cast<int>(rays[x].wallHitX) % TILE_SIZE;		// perform offset for the horizonal hit

			// get the correct texture id number from the map content
			const int texNum = rays[x].wallHitContent - 1;


			// render the wall from wallTopPixel to wallBottomPixel
			for (int y = wallTopPixel; y < wallBottomPixel; y++) {
				auto distanceFromTop = (y + wallStripHeight / 2.f) - (WINDOW_HEIGHT / 2.f);
				// calculate of textureOffsetY
				const int textureOffsetY = distanceFromTop * (static_cast<float>(TEX_HEIGHT) / wallStripHeight);

				// set the color of the wall based on the color of the texture
				//xsf::Color* texelColor = &wallTexture->getPixel(textureOffsetX, textureOffsetY);
				//x sf::Uint32 texelColor = textures[texNum][(TEX_WIDTH * textureOffsetY) + textureOffsetX];
				auto texelColor = image[texNum]->getPixel(textureOffsetX, textureOffsetY);
				//xdrawPixel(x, y, (rays[x].wasHitVertical ? 0xFFFFFFFF : 0xCCCCCCFF));		// solid color (no texture)
				drawPixel(x, y, texelColor);
			}

			// set the color of the floor
			for (int y = wallBottomPixel; y < WINDOW_HEIGHT; y++)
				drawPixel(x, y, 0xFF777777);

		} 
	}
	
	void renderColorBuffer() {
		
		colorBufferTexture->loadFromImage(*colorBuffer);
		const sf::Sprite sprite(*colorBufferTexture);
		window.draw(sprite);
	}
	void draw() 
	{
		window.clear();

		renderColorBuffer();
		clearColorBuffer(static_cast<sf::Uint8>(0xFF000000));
		// creates mock '3D projection'
		generate3DProjection();

		// display mini-map
		renderMap();
		renderRays();
		renderPlayer();

		window.display();
	}

	bool hasWallAt(int x, int y) const
	{
		if (x < 0 || x > WINDOW_WIDTH || y < 0 || y > WINDOW_HEIGHT) {
			return true;
		}

		const int mapGridIndexX = static_cast<int>(floor(x / TILE_SIZE));
		const int mapGridIndexY = static_cast<int>(floor(y / TILE_SIZE));
		return map[mapGridIndexY][mapGridIndexX] != 0;
	}

	int getWallContentAt(int x, int y) const
	{
		if (x < 0 || x > WINDOW_WIDTH || y < 0 || y > WINDOW_HEIGHT) {
			return 0;
		}
		const int mapGridIndexX = static_cast<int>(floor(x / TILE_SIZE));
		const int mapGridIndexY = static_cast<int>(floor(y / TILE_SIZE));
		return map[mapGridIndexY][mapGridIndexX];
	}

	void run()
	{
		init();
		sf::Time timeSinceLastUpdate = sf::Time::Zero;

		while (window.isOpen())
		{
			sf::Time dt = clock.restart();
			timeSinceLastUpdate += dt;
			while (timeSinceLastUpdate > TimePerFrame) {
					timeSinceLastUpdate -= dt;

					processInput();
					update(dt.asSeconds());
			}
			draw();
		}
	}
private:
	sf::RenderWindow window;
	sf::Clock clock;
	Player player;
	Ray rays[NUM_RAYS];

};

int main() {
	Game game;
	game.run();
}