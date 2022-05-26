//подключаем необходимые библиотеки
#include <SFML/Graphics.hpp>
#include <cmath>
#include <ctime>
#include <cstdlib>
//подключаем необходимые пространства имён
using namespace sf;
using namespace std;

int main()
{
	srand((time(NULL)));

	// определяем необходимые константы
	const float pi = 3.14159f; // число пи
	const int gameWidth = 800; // ширина окна
	const int gameHeight = 600; // высота окна
	Vector2f paddleSize(25, 100); // размер ракеток
	float ballRadius = 10.f; //радиус шара

	// Создаём окно для игры
	RenderWindow window(VideoMode(gameWidth, gameHeight, 32), "Ping-Pong Game",
		Style::Titlebar | Style::Close);
	window.setVerticalSyncEnabled(true); // вертикальная синхронизация

	// Создание левой ракетки
	RectangleShape leftPaddle; // экземлпяр левой ракетки
	leftPaddle.setSize(paddleSize - Vector2f(3, 3)); // размер
	leftPaddle.setOutlineThickness(3); // толщина контура
	leftPaddle.setOutlineColor(Color::Black); // цвет контура
	leftPaddle.setFillColor(Color(100, 100, 200)); // основной цвет
	leftPaddle.setOrigin(paddleSize / 2.f);

	// Создание правой ракетки
	RectangleShape rightPaddle; // экземпляр правой ракетки
	rightPaddle.setSize(paddleSize - Vector2f(3, 3)); // размер
	rightPaddle.setOutlineThickness(3);  // толщина контура
	rightPaddle.setOutlineColor(Color::Black); // цвет контура
	rightPaddle.setFillColor(Color(200, 100, 100)); // основной цвет
	rightPaddle.setOrigin(paddleSize / 2.f); 

	// Создание мяча
	CircleShape ball; // экземпляр мяча
	ball.setRadius(ballRadius - 3); // радиус мяча
	ball.setOutlineThickness(3); // толщина контура
	ball.setOutlineColor(Color::Black); // цвет контура
	ball.setFillColor(Color::Yellow); // основной цвет
	ball.setOrigin(ballRadius / 2, ballRadius / 2); //координаты

	// Инициализация шрифта в игре
	Font font; // экземлпяр шрифта
	if (!font.loadFromFile("resources/QuicksandBold.ttf")) 
		return EXIT_FAILURE; // условие для загрузки шрифта

	// Инициализация привественного окна
	Text pauseMessage; // экземпляр для текста
	pauseMessage.setFont(font); // установка шрифта
	pauseMessage.setCharacterSize(40); // размер шрифта
	pauseMessage.setPosition(160.f, 230.f); // координаты 
	pauseMessage.setFillColor(Color::White); // цвет шрифта
	pauseMessage.setString("This is Ping-Pong Game!\n  Press 'Space' to Start."); // содержимое текста

	// Определение характеристик ракеток
	Clock AITimer; // экземпляр для измерения времени
	const Time AITime = seconds(0.1f); // экземпляр для значения времени
	const float paddleSpeed = 400.f; // скорость ракеток
	float rightPaddleSpeed = 0.f; // скорость ракетки противника
	const float ballSpeed = 400.f; // скорость мяча
	float ballAngle = 0.f; // угол полёта

	Clock clock; 
	bool isPlaying = false; 
	while (window.isOpen()) // пока открыто окно
	{
		// Обработка событий 
		Event event; // экземпляр событий
		while (window.pollEvent(event)) // события окна
		{
			// события, если окно закрыто или нажата клавиша Esc
			if ((event.type == Event::Closed) ||
				((event.type == Event::KeyPressed) && (event.key.code == Keyboard::Escape)))
			{
				window.close(); // окно закрывается
				break; 
			}

			// события, если нажата клавиша Space(начать игру)
			if ((event.type == Event::KeyPressed) && (event.key.code == Keyboard::Space))
			{
				if (!isPlaying)
				{
					// начать снова или перезапустить
					isPlaying = true;
					clock.restart();

					// сброс позиций ракеток и шара
					leftPaddle.setPosition(10 + paddleSize.x / 2, gameHeight / 2);
					rightPaddle.setPosition(gameWidth - 10 - paddleSize.x / 2, gameHeight / 2);
					ball.setPosition(gameWidth / 2, gameHeight / 2);
				}
			}
		}

		if (isPlaying)
		{
			float deltaTime = clock.restart().asSeconds();

			// перемещение нашей ракетки
			if (Keyboard::isKeyPressed(Keyboard::Up) && // обработка кнопки вверх
				(leftPaddle.getPosition().y - paddleSize.y / 2 > 5.f))
			{
				leftPaddle.move(0.f, -paddleSpeed * deltaTime);
			}
			if (Keyboard::isKeyPressed(Keyboard::Down) && // обработка кнопки вниз
				(leftPaddle.getPosition().y + paddleSize.y / 2 < gameHeight - 5.f))
			{
				leftPaddle.move(0.f, paddleSpeed * deltaTime);
			}

			// перемещение ракетки противника
			if (((rightPaddleSpeed < 0.f) && (rightPaddle.getPosition().y - paddleSize.y / 2 > 5.f)) ||
				((rightPaddleSpeed > 0.f) && (rightPaddle.getPosition().y + paddleSize.y / 2 < gameHeight - 5.f)))
			{
				rightPaddle.move(0.f, rightPaddleSpeed * deltaTime);
			}

			// перемещение ракетки противника в соотвествии с направлением мяча
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

			// перемещение мяча
			float factor = ballSpeed * deltaTime;
			ball.move(cos(ballAngle) * factor, sin(ballAngle) * factor);

			// проверка столкновений мяча и экраном
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

			// проверка столкновений между ракетками и мячом
			// левая ракетка
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

			// правая ракетка
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

		// цвет фона
		window.clear(Color(50, 50, 50));

		if (isPlaying) 
		{
			// отображение ракеток и мяча
			window.draw(leftPaddle);
			window.draw(rightPaddle);
			window.draw(ball);
		}
		else
		{
			// отображение окна паузы
			window.draw(pauseMessage);
		}
		window.display(); // отображение на экран
	}
	return 0;
}