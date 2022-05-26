//ïîäêëþ÷àåì íåîáõîäèìûå áèáëèîòåê
#include <SFML/Graphics.hpp>
#include <cmath>
#include <ctime>
#include <cstdlib>
//ïîäêëþ÷àåì íåîáõîäèìûå ïðîñòðàíñòâà èì¸í
using namespace sf;
using namespace std;

int main()
{
	srand((time(NULL)));

	// îïðåäåëÿåì íåîáõîäèìûå êîíñòàíòû
	const float pi = 3.14159f; // ÷èñëî ïè
	const int gameWidth = 800; // øèðèíà îêíà
	const int gameHeight = 600; // âûñîòà îêíà
	Vector2f paddleSize(25, 100); // ðàçìåð ðàêåòîê
	float ballRadius = 10.f; //ðàäèóñ øàðà

	// Ñîçäà¸ì îêíî äëÿ èãðû
	RenderWindow window(VideoMode(gameWidth, gameHeight, 32), "Ping-Pong Game",
		Style::Titlebar | Style::Close);
	window.setVerticalSyncEnabled(true); // âåðòèêàëüíàÿ ñèíõðîíèçàöèÿ

	// Ñîçäàíèå ëåâîé ðàêåòêè
	RectangleShape leftPaddle; // ýêçåìëïÿð ëåâîé ðàêåòêè
	leftPaddle.setSize(paddleSize - Vector2f(3, 3)); // ðàçìåð
	leftPaddle.setOutlineThickness(3); // òîëùèíà êîíòóðà
	leftPaddle.setOutlineColor(Color::Black); // öâåò êîíòóðà
	leftPaddle.setFillColor(Color(100, 100, 200)); // îñíîâíîé öâåò
	leftPaddle.setOrigin(paddleSize / 2.f);

	// Ñîçäàíèå ïðàâîé ðàêåòêè
	RectangleShape rightPaddle; // ýêçåìïëÿð ïðàâîé ðàêåòêè
	rightPaddle.setSize(paddleSize - Vector2f(3, 3)); // ðàçìåð
	rightPaddle.setOutlineThickness(3);  // òîëùèíà êîíòóðà
	rightPaddle.setOutlineColor(Color::Black); // öâåò êîíòóðà
	rightPaddle.setFillColor(Color(200, 100, 100)); // îñíîâíîé öâåò
	rightPaddle.setOrigin(paddleSize / 2.f); 

	// Ñîçäàíèå ìÿ÷à
	CircleShape ball; // ýêçåìïëÿð ìÿ÷à
	ball.setRadius(ballRadius - 3); // ðàäèóñ ìÿ÷à
	ball.setOutlineThickness(3); // òîëùèíà êîíòóðà
	ball.setOutlineColor(Color::Black); // öâåò êîíòóðà
	ball.setFillColor(Color::Yellow); // îñíîâíîé öâåò
	ball.setOrigin(ballRadius / 2, ballRadius / 2); //êîîðäèíàòû

	// Èíèöèàëèçàöèÿ øðèôòà â èãðå
	Font font; // ýêçåìëïÿð øðèôòà
	if (!font.loadFromFile("resources/QuicksandBold.ttf")) 
		return EXIT_FAILURE; // óñëîâèå äëÿ çàãðóçêè øðèôòà

	// Èíèöèàëèçàöèÿ ïðèâåñòâåííîãî îêíà
	Text pauseMessage; // ýêçåìïëÿð äëÿ òåêñòà
	pauseMessage.setFont(font); // óñòàíîâêà øðèôòà
	pauseMessage.setCharacterSize(40); // ðàçìåð øðèôòà
	pauseMessage.setPosition(160.f, 230.f); // êîîðäèíàòû 
	pauseMessage.setFillColor(Color::White); // öâåò øðèôòà
	pauseMessage.setString("This is Ping-Pong Game!\n  Press 'Space' to Start."); // ñîäåðæèìîå òåêñòà

	// Îïðåäåëåíèå õàðàêòåðèñòèê ðàêåòîê
	Clock AITimer; // ýêçåìïëÿð äëÿ èçìåðåíèÿ âðåìåíè
	const Time AITime = seconds(0.1f); // ýêçåìïëÿð äëÿ çíà÷åíèÿ âðåìåíè
	const float paddleSpeed = 400.f; // ñêîðîñòü ðàêåòîê
	float rightPaddleSpeed = 0.f; // ñêîðîñòü ðàêåòêè ïðîòèâíèêà
	const float ballSpeed = 400.f; // ñêîðîñòü ìÿ÷à
	float ballAngle = 0.f; // óãîë ïîë¸òà

	Clock clock; 
	bool isPlaying = false; 
	while (window.isOpen()) // ïîêà îòêðûòî îêíî
	{
		// Îáðàáîòêà ñîáûòèé 
		Event event; // ýêçåìïëÿð ñîáûòèé
		while (window.pollEvent(event)) // ñîáûòèÿ îêíà
		{
			// ñîáûòèÿ, åñëè îêíî çàêðûòî èëè íàæàòà êëàâèøà Esc
			if ((event.type == Event::Closed) ||
				((event.type == Event::KeyPressed) && (event.key.code == Keyboard::Escape)))
			{
				window.close(); // îêíî çàêðûâàåòñÿ
				break; 
			}

			// ñîáûòèÿ, åñëè íàæàòà êëàâèøà Space(íà÷àòü èãðó)
			if ((event.type == Event::KeyPressed) && (event.key.code == Keyboard::Space))
			{
				if (!isPlaying)
				{
					// íà÷àòü ñíîâà èëè ïåðåçàïóñòèòü
					isPlaying = true;
					clock.restart();

					// ñáðîñ ïîçèöèé ðàêåòîê è øàðà
					leftPaddle.setPosition(10 + paddleSize.x / 2, gameHeight / 2);
					rightPaddle.setPosition(gameWidth - 10 - paddleSize.x / 2, gameHeight / 2);
					ball.setPosition(gameWidth / 2, gameHeight / 2);
				}
			}
		}

		if (isPlaying)
		{
			float deltaTime = clock.restart().asSeconds();

			// ïåðåìåùåíèå íàøåé ðàêåòêè
			if (Keyboard::isKeyPressed(Keyboard::Up) && // îáðàáîòêà êíîïêè ââåðõ
				(leftPaddle.getPosition().y - paddleSize.y / 2 > 5.f))
			{
				leftPaddle.move(0.f, -paddleSpeed * deltaTime);
			}
			if (Keyboard::isKeyPressed(Keyboard::Down) && // îáðàáîòêà êíîïêè âíèç
				(leftPaddle.getPosition().y + paddleSize.y / 2 < gameHeight - 5.f))
			{
				leftPaddle.move(0.f, paddleSpeed * deltaTime);
			}

			// ïåðåìåùåíèå ðàêåòêè ïðîòèâíèêà
			if (((rightPaddleSpeed < 0.f) && (rightPaddle.getPosition().y - paddleSize.y / 2 > 5.f)) ||
				((rightPaddleSpeed > 0.f) && (rightPaddle.getPosition().y + paddleSize.y / 2 < gameHeight - 5.f)))
			{
				rightPaddle.move(0.f, rightPaddleSpeed * deltaTime);
			}

			// ïåðåìåùåíèå ðàêåòêè ïðîòèâíèêà â ñîîòâåñòâèè ñ íàïðàâëåíèåì ìÿ÷à
			if (AITimer.getElapsedTime() > AITime)
			{
				AITimer.restart();
				if (ball.getPosition().y + ballRadius > rightPaddle.getPosition().y + paddleSize.y / 2)
					rightPaddleSpeed = paddleSpeed;
				else if (ball.getPosition().y - ballRadius < rightPaddle.getPosition().y - paddleSize.y / 2)
					rightPaddleSpeed = -paddleSpeed;
				else
					rightPaddleSpeed = 0.f;
			}

			// ïåðåìåùåíèå ìÿ÷à
			float factor = ballSpeed * deltaTime;
			ball.move(cos(ballAngle) * factor, sin(ballAngle) * factor);

			// ïðîâåðêà ñòîëêíîâåíèé ìÿ÷à è ýêðàíîì
			if (ball.getPosition().x - ballRadius < 0.f) 
			{
				isPlaying = false;
				pauseMessage.setString("You lost!\nPress 'Space' to restart or\n'Esc' to exit");
			}
			if (ball.getPosition().x + ballRadius > gameWidth) 
			{
				isPlaying = false;
				pauseMessage.setString("You won!\nPress 'Space' to restart or\n'Esc' to exit");
			}
			if (ball.getPosition().y - ballRadius < 0.f)
			{
				
				ballAngle = -ballAngle;
				ball.setPosition(ball.getPosition().x, ballRadius + 0.1f);
			}
			if (ball.getPosition().y + ballRadius > gameHeight)
			{
				
				ballAngle = -ballAngle;
				ball.setPosition(ball.getPosition().x, gameHeight - ballRadius - 0.1f);
			}

			// ïðîâåðêà ñòîëêíîâåíèé ìåæäó ðàêåòêàìè è ìÿ÷îì
			// ëåâàÿ ðàêåòêà
			if (ball.getPosition().x - ballRadius < leftPaddle.getPosition().x + paddleSize.x / 2 &&
				ball.getPosition().x - ballRadius > leftPaddle.getPosition().x &&
				ball.getPosition().y + ballRadius >= leftPaddle.getPosition().y - paddleSize.y / 2 &&
				ball.getPosition().y - ballRadius <= leftPaddle.getPosition().y + paddleSize.y / 2)
			{
				if (ball.getPosition().y > leftPaddle.getPosition().y)
					ballAngle = pi - ballAngle + (rand() % 20) * pi / 180;
				else
					ballAngle = pi - ballAngle - (rand() % 20) * pi / 180;

			
				ball.setPosition(leftPaddle.getPosition().x + ballRadius + paddleSize.x / 2 + 0.1f, ball.getPosition().y);
			}

			// ïðàâàÿ ðàêåòêà
			if (ball.getPosition().x + ballRadius > rightPaddle.getPosition().x - paddleSize.x / 2 &&
				ball.getPosition().x + ballRadius < rightPaddle.getPosition().x &&
				ball.getPosition().y + ballRadius >= rightPaddle.getPosition().y - paddleSize.y / 2 &&
				ball.getPosition().y - ballRadius <= rightPaddle.getPosition().y + paddleSize.y / 2)
			{
				if (ball.getPosition().y > rightPaddle.getPosition().y)
					ballAngle = pi - ballAngle + (rand() % 20) * pi / 180;
				else
					ballAngle = pi - ballAngle - (rand() % 20) * pi / 180;

				
				ball.setPosition(rightPaddle.getPosition().x - ballRadius - paddleSize.x / 2 - 0.1f, ball.getPosition().y);
			}
		}

		// öâåò ôîíà
		window.clear(Color(50, 50, 50));

		if (isPlaying) 
		{
			// îòîáðàæåíèå ðàêåòîê è ìÿ÷à
			window.draw(leftPaddle);
			window.draw(rightPaddle);
			window.draw(ball);
		}
		else
		{
			// îòîáðàæåíèå îêíà ïàóçû
			window.draw(pauseMessage);
		}
		window.display(); // îòîáðàæåíèå íà ýêðàí
	}
	return 0;
}
