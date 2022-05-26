//���������� ����������� ����������
#include <SFML/Graphics.hpp>
#include <cmath>
#include <ctime>
#include <cstdlib>
//���������� ����������� ������������ ���
using namespace sf;
using namespace std;

int main()
{
	srand((time(NULL)));

	// ���������� ����������� ���������
	const float pi = 3.14159f; // ����� ��
	const int gameWidth = 800; // ������ ����
	const int gameHeight = 600; // ������ ����
	Vector2f paddleSize(25, 100); // ������ �������
	float ballRadius = 10.f; //������ ����

	// ������ ���� ��� ����
	RenderWindow window(VideoMode(gameWidth, gameHeight, 32), "Ping-Pong Game",
		Style::Titlebar | Style::Close);
	window.setVerticalSyncEnabled(true); // ������������ �������������

	// �������� ����� �������
	RectangleShape leftPaddle; // ��������� ����� �������
	leftPaddle.setSize(paddleSize - Vector2f(3, 3)); // ������
	leftPaddle.setOutlineThickness(3); // ������� �������
	leftPaddle.setOutlineColor(Color::Black); // ���� �������
	leftPaddle.setFillColor(Color(100, 100, 200)); // �������� ����
	leftPaddle.setOrigin(paddleSize / 2.f);

	// �������� ������ �������
	RectangleShape rightPaddle; // ��������� ������ �������
	rightPaddle.setSize(paddleSize - Vector2f(3, 3)); // ������
	rightPaddle.setOutlineThickness(3);  // ������� �������
	rightPaddle.setOutlineColor(Color::Black); // ���� �������
	rightPaddle.setFillColor(Color(200, 100, 100)); // �������� ����
	rightPaddle.setOrigin(paddleSize / 2.f); 

	// �������� ����
	CircleShape ball; // ��������� ����
	ball.setRadius(ballRadius - 3); // ������ ����
	ball.setOutlineThickness(3); // ������� �������
	ball.setOutlineColor(Color::Black); // ���� �������
	ball.setFillColor(Color::Yellow); // �������� ����
	ball.setOrigin(ballRadius / 2, ballRadius / 2); //����������

	// ������������� ������ � ����
	Font font; // ��������� ������
	if (!font.loadFromFile("resources/QuicksandBold.ttf")) 
		return EXIT_FAILURE; // ������� ��� �������� ������

	// ������������� �������������� ����
	Text pauseMessage; // ��������� ��� ������
	pauseMessage.setFont(font); // ��������� ������
	pauseMessage.setCharacterSize(40); // ������ ������
	pauseMessage.setPosition(160.f, 230.f); // ���������� 
	pauseMessage.setFillColor(Color::White); // ���� ������
	pauseMessage.setString("This is Ping-Pong Game!\n  Press 'Space' to Start."); // ���������� ������

	// ����������� ������������� �������
	Clock AITimer; // ��������� ��� ��������� �������
	const Time AITime = seconds(0.1f); // ��������� ��� �������� �������
	const float paddleSpeed = 400.f; // �������� �������
	float rightPaddleSpeed = 0.f; // �������� ������� ����������
	const float ballSpeed = 400.f; // �������� ����
	float ballAngle = 0.f; // ���� �����

	Clock clock; 
	bool isPlaying = false; 
	while (window.isOpen()) // ���� ������� ����
	{
		// ��������� ������� 
		Event event; // ��������� �������
		while (window.pollEvent(event)) // ������� ����
		{
			// �������, ���� ���� ������� ��� ������ ������� Esc
			if ((event.type == Event::Closed) ||
				((event.type == Event::KeyPressed) && (event.key.code == Keyboard::Escape)))
			{
				window.close(); // ���� �����������
				break; 
			}

			// �������, ���� ������ ������� Space(������ ����)
			if ((event.type == Event::KeyPressed) && (event.key.code == Keyboard::Space))
			{
				if (!isPlaying)
				{
					// ������ ����� ��� �������������
					isPlaying = true;
					clock.restart();

					// ����� ������� ������� � ����
					leftPaddle.setPosition(10 + paddleSize.x / 2, gameHeight / 2);
					rightPaddle.setPosition(gameWidth - 10 - paddleSize.x / 2, gameHeight / 2);
					ball.setPosition(gameWidth / 2, gameHeight / 2);
				}
			}
		}

		if (isPlaying)
		{
			float deltaTime = clock.restart().asSeconds();

			// ����������� ����� �������
			if (Keyboard::isKeyPressed(Keyboard::Up) && // ��������� ������ �����
				(leftPaddle.getPosition().y - paddleSize.y / 2 > 5.f))
			{
				leftPaddle.move(0.f, -paddleSpeed * deltaTime);
			}
			if (Keyboard::isKeyPressed(Keyboard::Down) && // ��������� ������ ����
				(leftPaddle.getPosition().y + paddleSize.y / 2 < gameHeight - 5.f))
			{
				leftPaddle.move(0.f, paddleSpeed * deltaTime);
			}

			// ����������� ������� ����������
			if (((rightPaddleSpeed < 0.f) && (rightPaddle.getPosition().y - paddleSize.y / 2 > 5.f)) ||
				((rightPaddleSpeed > 0.f) && (rightPaddle.getPosition().y + paddleSize.y / 2 < gameHeight - 5.f)))
			{
				rightPaddle.move(0.f, rightPaddleSpeed * deltaTime);
			}

			// ����������� ������� ���������� � ����������� � ������������ ����
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

			// ����������� ����
			float factor = ballSpeed * deltaTime;
			ball.move(cos(ballAngle) * factor, sin(ballAngle) * factor);

			// �������� ������������ ���� � �������
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

			// �������� ������������ ����� ��������� � �����
			// ����� �������
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

			// ������ �������
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

		// ���� ����
		window.clear(Color(50, 50, 50));

		if (isPlaying) 
		{
			// ����������� ������� � ����
			window.draw(leftPaddle);
			window.draw(rightPaddle);
			window.draw(ball);
		}
		else
		{
			// ����������� ���� �����
			window.draw(pauseMessage);
		}
		window.display(); // ����������� �� �����
	}
	return 0;
}