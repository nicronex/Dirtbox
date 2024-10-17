#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SFML/Audio.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <random>

#include "window/window.hpp"
#include "camera/camera.hpp"
#include "texture/texture.hpp"
#include "shader/shader.hpp"
#include "mesh/mesh.hpp"
#include "model/model.hpp"
#include "chunk/chunk.hpp"
#include "player/player.hpp"
#include "framebuffer/framebuffer.hpp"
#include "sprite2d/sprite2d.hpp"
#include "animation/animation.hpp"
#include "bilboard2d/bilboard2d.hpp"
#include "enemy/enemy.hpp"


struct SunLight
{
	static glm::vec3 _ambient;
	static glm::vec3 _diffuse;
	static glm::vec3 _specular;
	static glm::vec3 _direction;
	SunLight()
	{
		
	}
};

glm::vec3 SunLight::_ambient;
glm::vec3 SunLight::_diffuse;
glm::vec3 SunLight::_specular;
glm::vec3 SunLight::_direction;

class Light
{
private:
	glm::vec3 _ambient;
	glm::vec3 _diffuse;
	glm::vec3 _specular;
	    
    float _constant;
    float _linear;
    float _quadratic;  
	float _strength;
	
    glm::vec3 _position;

	static std::vector<Light*> _lights;
	
public:
	Light(
		glm::vec3 ambient, 
		glm::vec3 diffuse, 
		glm::vec3 specular, 
		float constant, 
		float linear, 
		float quadratic
	) :
		_ambient(ambient), 
		_diffuse(diffuse), 
		_specular(specular), 
		_constant(constant), 
		_linear(linear), 
		_quadratic(quadratic)
	{
		_lights.push_back(this);
		_strength = 1.0f;
	}

	void setConstant(float constant)
	{
		_constant = constant;
	}	

	void setLinear(float linear)
	{
		_constant = linear;
	}	

	void setQuadratic(float quadratic)
	{
		_quadratic = quadratic;
	}
	
	void setPosition(glm::vec3 position)
	{
		_position = position;
	}

	void setStrength(float strength)
	{
		_strength = strength;
	}
	
	glm::vec3 getAmbient() const
	{
		return _ambient;
	}

	glm::vec3 getDiffuse() const
	{
		return _diffuse;
	}

	glm::vec3 getSpecular() const
	{
		return _specular;
	}

	float getConstant() const
	{
		return _constant;
	}

	float getLinear() const
	{
		return _linear;
	}

	float getQuadratic() const
	{
		return _quadratic;
	}
	
	glm::vec3 getPosition() const
	{
		return _position;
	}

	float getStrength() const
	{
		return _strength;
	}
	
	static void draw(Model model)
	{
		model.setVector3f("viewPos", Camera::position);

		model.setVector3f("dirLight.ambient", SunLight::_ambient);
		model.setVector3f("dirLight.diffuse", SunLight::_diffuse);
		model.setVector3f("dirLight.specular", SunLight::_specular);
		model.setVector3f("dirLight.direction", SunLight::_direction);
		
		for(unsigned int i = 0; i < _lights.size(); i++)
		{
			auto light = _lights[i];

			std::string uniformName = "lights[" + std::to_string(i) + "].";
			model.setVector3f(uniformName + "position", light->getPosition());
			model.setVector3f(uniformName + "ambient", light->getAmbient());
			model.setVector3f(uniformName + "diffuse", light->getDiffuse());
			model.setVector3f(uniformName + "specular", light->getSpecular());
			model.setFloat(uniformName + "constant", light->getConstant());
			model.setFloat(uniformName + "linear", light->getLinear());
			model.setFloat(uniformName + "quadratic", light->getQuadratic());
			model.setFloat(uniformName + "strength", light->getStrength());
		}
		
		model.draw();
	}

	static unsigned int getLightsAmount()
	{
		return _lights.size();
	}
};

std::vector<Light*> Light::_lights;

#define RGB2VEC3(r,g,b) glm::vec3(1.0f/255.0f*float(r),1.0f/255.0f*float(g),1.0f/255.0f*float(b))
 
int randint(int min, int max)
{
	return (rand() % (max - min)) + min;
}


int main()
{
	float mapWidth = 100.0f;
	float mapDepth = 100.0f;
	
	int windowW = 1024;
	int windowH = 1024;
	std::string windowName = "EngineGLFW C++";
    GLFWwindow* window = initWindow(windowW, windowH, windowName);
	if(!window)
	{
		std::cout << "initWindow() error\n";
		glfwTerminate();
	}

	unsigned int playerScore = 0;
	
	// depth test
	glEnable(GL_DEPTH_TEST);
	// multisample
	glEnable(GL_MULTISAMPLE);
	// stencil buffer (for outlining)
    // clear color
	glClearColor(0.4, 0.5, 0.9, 1.0);

	Camera::setProperties(windowW, windowH, 75.0f, 0.01f, 500.0f);
	Camera::setPosition(glm::vec3(0.0f, 0.0f, 1.0f));
	Sprite2D::init();
	
	Bilboard2D::init();

	float loadingScreenProgress = 0.0f;
	Texture loadingScreenTexture = Texture::loadFromFile("textures/loadingScreen.png");
	Shader loadingScreenShader = Shader::loadFromFile("shaders/loadingScreen.vs", "shaders/loadingScreen.fs");
	Sprite2D loadingScreenSprite(loadingScreenShader);
	loadingScreenSprite.setScale(windowW, windowH);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	loadingScreenProgress += 0.05f;
	loadingScreenSprite.setTexture("texture0", loadingScreenTexture);
	loadingScreenSprite.setFloat("progress", loadingScreenProgress);
	loadingScreenSprite.draw();
	glfwSwapBuffers(window);
	glfwPollEvents();

	sf::Music music;
	if (!music.openFromFile("sounds/Omission.ogg"))
	{
		std::cout << "Failed to load Omission.ogg\n";
	}
	music.setLoop(true);

	// LOADING SCREEN PROGRESS
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	loadingScreenProgress += 0.05f;
	loadingScreenSprite.setTexture("texture0", loadingScreenTexture);
	loadingScreenSprite.setFloat("progress", loadingScreenProgress);
	loadingScreenSprite.draw();
	glfwSwapBuffers(window);
	glfwPollEvents();
		
	sf::SoundBuffer ak47_shoot_sb;
    if (!ak47_shoot_sb.loadFromFile("sounds/ak47_shoot.ogg"))
    {
    	std::cout << "Failed to load ak47_shoot.ogg\n";
    }	
    sf::Sound ak47_shoot_sound;
    ak47_shoot_sound.setBuffer(ak47_shoot_sb);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	loadingScreenProgress += 0.05f;
	loadingScreenSprite.setTexture("texture0", loadingScreenTexture);
	loadingScreenSprite.setFloat("progress", loadingScreenProgress);
	loadingScreenSprite.draw();
	glfwSwapBuffers(window);
	glfwPollEvents();
		
	std::vector<glm::vec4> solidObjects;

	Shader shader = Shader::loadFromFile("shaders/textureShader.vs", "shaders/textureShader.fs");
	Shader shaderLight = Shader::loadFromFile("shaders/lightShader.vs", "shaders/lightShader.fs");
	Shader shader2D = Shader::loadFromFile("shaders/sprite2D.vs", "shaders/sprite2D.fs");
	Shader shaderGrass = Shader::loadFromFile("shaders/grassShader.vs", "shaders/grassShader.fs");
	Shader shaderMultisample = Shader::loadFromFile("shaders/multisample.vs", "shaders/multisample.fs");
	Shader shaderBilboard = Shader::loadFromFile("shaders/bilboard2D.vs", "shaders/bilboard2D.fs");
	Shader shaderOutline = Shader::loadFromFile("shaders/outline.vs", "shaders/outline.fs");
	Shader shaderSpecular = Shader::loadFromFile("shaders/textureShaderSpecular.vs", "shaders/textureShaderSpecular.fs");
	Shader shaderDamage = Shader::loadFromFile("shaders/damage2D.vs", "shaders/damage2D.fs");

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	loadingScreenProgress += 0.05f;
	loadingScreenSprite.setTexture("texture0", loadingScreenTexture);
	loadingScreenSprite.setFloat("progress", loadingScreenProgress);
	loadingScreenSprite.draw();
	glfwSwapBuffers(window);
	glfwPollEvents();

	Texture restartButtonTexture = Texture::loadFromFile("textures/restart.png");
	Texture restartButtonSelectedTexture = Texture::loadFromFile("textures/restartSelected.png");
	Texture gunTexture = Texture::loadFromFile("textures/ak47_3.png");
	Texture gunTexture_specular = Texture::loadFromFile("textures/ak47_3_specular.png");
	Texture planeTexture0 = Texture::loadFromFile("textures/grass.jpeg");
	Texture planeTexture1 = Texture::loadFromFile("textures/grass1.png");
	Texture planeTexture2 = Texture::loadFromFile("textures/grass2.png");
	Texture sunTexture = Texture::loadFromFile("textures/sun.png");
	
	std::vector<Texture> numbersTextures;
	for(int i = 0; i <= 9; i++)
	{
		Texture texture = Texture::loadFromFile("textures/num" + std::to_string(i) + ".png");
		numbersTextures.push_back(texture);
	}
	Texture scoreTextTexture = Texture::loadFromFile("textures/score.png");
	
	std::vector<std::string> enemyTexturesPaths = 
	{
		"textures/b_enemy0.png",
		"textures/b_enemy1.png",
		"textures/b_enemy2.png",
		"textures/b_enemy3.png",
		"textures/b_enemy4.png"
	};
	std::vector<Texture> enemyTextures;
	for(auto& path: enemyTexturesPaths)
	{
		auto texture = Texture::loadFromFile(path);
		enemyTextures.push_back(texture);
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	loadingScreenProgress += 0.05f;
	loadingScreenSprite.setTexture("texture0", loadingScreenTexture);
	loadingScreenSprite.setFloat("progress", loadingScreenProgress);
	loadingScreenSprite.draw();
	glfwSwapBuffers(window);
	glfwPollEvents();
	
	Sprite2D restartButton(shader2D);
	restartButton.setPosition(windowW * 0.5f - 256.0f * 0.5f, windowH * 0.5f - 128.0f * 0.5f);
	restartButton.setScale(256.0f, 128.0f);
		
	Mesh houseMesh = Mesh::loadFromFile("models/3floor.obj");
	Texture houseTexture = Texture::loadFromFile("textures/house.png");
	Texture houseTexture_specular = Texture::loadFromFile("textures/house_specular.png");
	Model houseModel(houseMesh, shaderLight);
	houseModel.setScale(1.5f, 1.5f, 1.5f);
	houseModel.setPosition(0.0f, 4.475219 * 1.5f, 0.0f);
	auto houseSolidRect = glm::vec4(-16.320005f - 0.03f, -3.496396f - 0.03f, 16.320005f * 2.0f + 0.06f, 3.496396f * 2.0f + 0.06f);
	houseSolidRect *= 1.5f;
	solidObjects.push_back(houseSolidRect);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	loadingScreenProgress += 0.05f;
	loadingScreenSprite.setTexture("texture0", loadingScreenTexture);
	loadingScreenSprite.setFloat("progress", loadingScreenProgress);
	loadingScreenSprite.draw();
	glfwSwapBuffers(window);
	glfwPollEvents();
		
	Mesh gunMesh = Mesh::loadFromFile("models/ak47_2.obj");
	Model gunModel(gunMesh, shaderLight);
	gunModel.setPosition(-5.0f, 0.0f, 15.0f);
	gunModel.setScale(1.3f, 1.3f, 1.3f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	loadingScreenProgress += 0.05f;
	loadingScreenSprite.setTexture("texture0", loadingScreenTexture);
	loadingScreenSprite.setFloat("progress", loadingScreenProgress);
	loadingScreenSprite.draw();
	glfwSwapBuffers(window);
	glfwPollEvents();
	
	Mesh planeMesh = Mesh::loadFromFile("models/plane.obj");
	Model planeModel(planeMesh, shaderLight);
	planeModel.setScale(mapWidth, 1.0f, mapDepth);

	// LOADING SCREEN PROGRESS
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	loadingScreenProgress += 0.05f;
	loadingScreenSprite.setTexture("texture0", loadingScreenTexture);
	loadingScreenSprite.setFloat("progress", loadingScreenProgress);
	loadingScreenSprite.draw();
	glfwSwapBuffers(window);
	glfwPollEvents();
	
	Bilboard2D bilboardEnemy(shaderBilboard);
	bilboardEnemy.setScale(3.0f, 3.0f);	
		
	std::vector<Enemy> enemies;
	for(int i = 0; i < 3; i++)
	{
		Enemy enemy(bilboardEnemy, enemyTextures[randint(0, enemyTextures.size())], 
									glm::vec3(randint(0, mapWidth), 0.0f, randint(0, mapDepth)));
		enemies.push_back(enemy);
	}
	
	bool cursorCaptured = false;

	// LOADING SCREEN PROGRESS
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	loadingScreenProgress += 0.05f;
	loadingScreenSprite.setTexture("texture0", loadingScreenTexture);
	loadingScreenSprite.setFloat("progress", loadingScreenProgress);
	loadingScreenSprite.draw();
	glfwSwapBuffers(window);
	glfwPollEvents();
	
	glm::vec3 sunPosition(5.0f, 10.0f, 10.0f);

	Sprite2D screenSprite(shader2D);
	screenSprite.setScale(windowW + 20.0f, windowH + 20.0f);
	Framebuffer gunFramebuffer = Framebuffer::createColorFramebuffer(windowW, windowH);
	Framebuffer screenFramebuffer = Framebuffer::createColorFramebuffer(windowW, windowH);

	Sprite2D numSprite(shader2D);
	numSprite.setScale(128.0f, 128.0f);
	Sprite2D scoreTextSprite(shader2D);
	scoreTextSprite.setScale(256.0f, 128.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	loadingScreenProgress += 0.05f;
	loadingScreenSprite.setTexture("texture0", loadingScreenTexture);
	loadingScreenSprite.setFloat("progress", loadingScreenProgress);
	loadingScreenSprite.draw();
	glfwSwapBuffers(window);
	glfwPollEvents();
		
	SunLight::_ambient = RGB2VEC3(128 + 32, 128 + 32, 128 + 32);
	SunLight::_diffuse = RGB2VEC3(128 + 64, 128 + 64, 128 + 64);
	SunLight::_specular = RGB2VEC3(128 + 64, 128 + 64, 128 + 64);
	SunLight::_direction = glm::normalize(glm::vec3(1.0f, -1.0f, 0.5f));
	
	Light sunLight(
		glm::vec3(0.5f, 0.5f, 0.4f),//RGB2VEC3(128 + 64, 128 + 64, 128 + 64), 
		glm::vec3(1.0f, 1.0f, 1.0f),//RGB2VEC3(128 + 32, 128 + 32, 128 + 32), 
		glm::vec3(1.0f, 1.0f, 1.0f),//RGB2VEC3(255, 255, 255),
		0.1f,
        0.03f,
        0.032f
   	);
	sunLight.setPosition(glm::vec3(5.0f, 20.0f, 10.0f));

	Bilboard2D bilboardSun(shaderBilboard);
	bilboardSun.setScale(3.0f, 3.0f);
	bilboardSun.setPosition(-SunLight::_direction * 100.0f);
	
	Light muzzleLight(
		glm::vec3(0.5f, 0.5f, 0.4f),//RGB2VEC3(128 + 64, 128 + 32, 128 + 64), 
		glm::vec3(0.5f, 0.5f, 0.4f),//RGB2VEC3(128 + 32, 128, 128 + 32), 
		glm::vec3(0.5f, 0.5f, 0.4f),//RGB2VEC3(255, 128 + 64, 255), 
		0.8f,
        0.4f,
        0.032f
	);
	muzzleLight.setPosition(glm::vec3(5.0f, 20.0f, 10.0f));

	Player player(glm::vec3(10.0f, 10.0f, 10.0f), 1.8f);
	player.setPhysicProperties(8.0f, 5.5f, 9.5f);

	// LOADING SCREEN PROGRESS
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	loadingScreenProgress += 0.05f;
	loadingScreenSprite.setTexture("texture0", loadingScreenTexture);
	loadingScreenSprite.setFloat("progress", loadingScreenProgress);
	loadingScreenSprite.draw();
	glfwSwapBuffers(window);
	glfwPollEvents();
	
	Animation<glm::vec3> gunSwitchAimAnimation(0.25f);
	Animation<float> gunSwitchAimFovAnimation(0.25f);
	Animation<glm::vec3> gunShootAnimation(0.1f);
	gunShootAnimation.setLoop(true);
	Animation<float> gunShootRecoilFactorAnimation(0.15f);
	gunShootRecoilFactorAnimation.setLoop(true);
	Animation<float> gunMoveBobAnimation(1.0f);
	gunMoveBobAnimation.setLoop(true);
	Animation<glm::vec3> gunFallBobAnimation(0.2f);
	gunFallBobAnimation.setLoop(true);
	Animation<float> playerFallBobAnimation(0.3f);
	playerFallBobAnimation.setLoop(true);
	Animation<float> playerGotDamageAnimation(0.25f);
	playerGotDamageAnimation.setLoop(true);
	Animation<float> buttonRestartDelay(1.0f);

	Animation<float> muzzleFlashAnimation(0.05f);
	muzzleFlashAnimation.setLoop(true);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	loadingScreenProgress += 0.05f;
	loadingScreenSprite.setTexture("texture0", loadingScreenTexture);
	loadingScreenSprite.setFloat("progress", loadingScreenProgress);
	loadingScreenSprite.draw();
	glfwSwapBuffers(window);
	glfwPollEvents();
		
	//                 //
	// MAIN LOOP STUFF //
	//                 //
	float delta = 0.0f;
	double lastTime = 0.0f;
	double time = glfwGetTime();

	bool mouseLeftClick = false;
	bool mouseRightClick = false;
	bool mouseLeftHold = false;
	bool mouseRightHold = false;

	bool restartScreen = false;
	glm::vec3 lastCameraFront;
	glm::vec3 lastCameraPosition;
	Animation<glm::vec3> restartScreenAnimation(10.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	loadingScreenProgress += 0.05f;
	loadingScreenSprite.setTexture("texture0", loadingScreenTexture);
	loadingScreenSprite.setFloat("progress", loadingScreenProgress);
	loadingScreenSprite.draw();
	glfwSwapBuffers(window);
	glfwPollEvents();
	
	music.play();
	
   	while(!glfwWindowShouldClose(window))
	{
		lastTime = time;
		time = glfwGetTime();
		delta = time - lastTime;

		// stable 60 FPS
		//std::cout << "FPS: " << 1.0f / delta << std::endl;
		
		double mouseX, mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);
		
	   	if(cursorCaptured)
	   	{
	   		if(!restartScreen)
	   		{
		   		auto playerPosition = player.getPosition();
				if(playerPosition.x < -mapWidth)
				{
					playerPosition.x = -mapWidth;
				}
				else if(playerPosition.x > mapWidth)
				{
					playerPosition.x = mapWidth;
				}
				if(playerPosition.z < -mapDepth)
				{
					playerPosition.z = -mapDepth;
				}
				else if(playerPosition.z > mapDepth)
				{
					playerPosition.z = mapDepth;
				}
				player.setPosition(playerPosition);
		   		if(player.updateKeyboard(delta, window, solidObjects))
		   		{
		   			if(!gunMoveBobAnimation.isPlaying())
		   			{
		   				gunMoveBobAnimation.restart();
		   			}
		   		}
		   		else
		   		{
		   			if(gunMoveBobAnimation.isPlaying())
		   			{
			   			gunMoveBobAnimation.setReverse(true);
			   		}
		   		}
		   		if(player.isJustFalled())
		   		{
		   			gunFallBobAnimation.restart();
		   			playerFallBobAnimation.restart();
		   		}
		   		
		   		gunShootRecoilFactorAnimation.update(0.0f, 2.23f, delta);
		   		player.updateMouse(mouseX, mouseY, glm::vec3(0.0f, 0.0f, gunShootRecoilFactorAnimation.getValue()));
		   	
		   		if(glfwGetKey(window, GLFW_KEY_ESCAPE))
		   		{
		   			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		   			cursorCaptured = false;
		   		}	   		

		   		if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		   		{
		   			if(!mouseLeftClick || (mouseLeftHold && !gunShootAnimation.isPlaying()))
		   			{
		   				muzzleFlashAnimation.restart();
		   				
		   				ak47_shoot_sound.setPitch(1.0f + randint(0, 10) * 0.0025f);
		   				ak47_shoot_sound.setVolume(75.0f + randint(0, 10) * 0.5f);
		   				ak47_shoot_sound.play();
		   				
		   				mouseLeftClick = true;
		   				mouseLeftHold = true;

						gunShootAnimation.restart();
						gunShootRecoilFactorAnimation.restart();
											
		   				glm::vec3 bulletDir = Camera::front;
						glm::vec3 bulletPos = Camera::position;
						bool hit = false;
						for(int i = 0; i < 100; i++)
						{
							for(auto& solid: solidObjects)
							{
								if(pointInsideRect(bulletPos.x, bulletPos.z, solid))
								{
									goto didntHit;
								}
							} 

							for(auto& enemy: enemies)
							{
								if(pointInsideCircle(bulletPos.x, bulletPos.y, bulletPos.z, enemy.getPosition(), 0.5f * enemy.getScale().x))
								{
									enemy.addHP(-(float)randint(1, 4));
									hit = true;
									goto didntHit;
								}
							}
							bulletPos += bulletDir;
						}
						didntHit:;
		   			}
		   		}
		   		if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
		   		{
		   			if(mouseLeftClick)
		   			{
		   				mouseLeftClick = false;
		   				mouseLeftHold = false;
					}
		   		}
		   		if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		   		{
		   			if(!mouseRightClick)
		   			{
		   				mouseRightClick = true;
		   				mouseRightHold = true;

		   				gunSwitchAimFovAnimation.setReverse(false);
		   				gunSwitchAimFovAnimation.restart();
						gunSwitchAimAnimation.setReverse(false);
	   					gunSwitchAimAnimation.restart();
		   			}
		   		}
		   		if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
		   		{
		   			if(mouseRightClick)
		   			{
		   				mouseRightClick = false;
		   				mouseRightHold = false;

		   				gunSwitchAimFovAnimation.setReverse(true);
		   				gunSwitchAimFovAnimation.restart();
						gunSwitchAimAnimation.setReverse(true);
	   					gunSwitchAimAnimation.restart();
					}
		   		}
		   	}
	   	}
	   	else
	   	{
	   		if(!restartScreen)
	   		{
		   		if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		    	{
		    		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		    		Camera::setMouseLast(mouseX, mouseY);
		    		cursorCaptured = true;
		    	}
		    }
		    else
		    {
		    	if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		    	{
		    		if(!buttonRestartDelay.isPlaying() && mouseX > restartButton.getPosition().x 
					&& mouseX < restartButton.getPosition().x + restartButton.getScale().x 
					&& mouseY > restartButton.getPosition().y
					&& mouseY < restartButton.getPosition().y + restartButton.getScale().y
					)
					{
						restartScreen = false;
						
						Camera::setPosition(lastCameraPosition);
						Camera::setFront(glm::vec3(
							(float)randint(0, 10) * 0.1f, 
							0.0f, 
							(float)randint(0, 10) * 0.1f
							)
						);

						player.setHP(10.0f);
						playerScore = 0;

						music.stop();
						music.play();
						
						enemies.clear();
						for(int i = 0; i < 3; i++)
						{
							Enemy enemy(bilboardEnemy, enemyTextures[randint(0, enemyTextures.size())], 
														glm::vec3(randint(0, mapWidth), 0.0f, randint(0, mapDepth)));
							enemies.push_back(enemy);
						}

	
						glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
						Camera::setMouseLast(mouseX, mouseY);
						cursorCaptured = true;
					}
		    	}
		    }
	   	}
	   	if(player.getHP() <= 0.0)
	   	{
		   	if(!restartScreen)
	   		{
	   			restartScreen = true;
	   			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	   			cursorCaptured = false;
	   			lastCameraFront = Camera::front;
	   			lastCameraPosition = Camera::position;
	   			restartScreenAnimation.restart();
	   			buttonRestartDelay.restart();
	   		}
	   		buttonRestartDelay.update(0.0f, 0.0f, delta);
	   		Camera::setFront(restartScreenAnimation.update(lastCameraFront, glm::vec3(0.0f, -1.0f, 0.0f), delta));
	   		Camera::setPosition(restartScreenAnimation.update(lastCameraPosition, glm::vec3(lastCameraPosition.x, 30.0f, lastCameraPosition.z), delta));
  		}

		gunFramebuffer.bind();
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glEnable(GL_DEPTH_TEST);
			
			glm::vec3 gunPos = Camera::position;
			glm::vec3 gunFront = Camera::front;
			glm::vec3 cameraRight = glm::normalize(glm::vec3(-gunFront.z, 0.0f, gunFront.x));
			glm::vec3 cameraFront = glm::normalize(gunFront);
			glm::vec3 cameraDown = glm::cross(cameraRight, cameraFront);
			glm::vec3 gunPosHold = gunPos + cameraRight * 0.25f + cameraFront * 0.5f - cameraDown * 0.25f;
			glm::vec3 gunPosAim = gunPos + cameraFront * 0.175f - cameraDown * 0.1f;

			Camera::setFov(gunSwitchAimFovAnimation.update(75.0f, 45.0f, delta));
			gunPos = gunSwitchAimAnimation.update(gunPosHold, gunPosAim, delta);
			gunPos += gunShootAnimation.update(glm::vec3(0.0f, 0.0f, 0.0f), -cameraFront * 0.05f, delta);
			gunPos += gunFallBobAnimation.update(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -0.006f, 0.0f), delta);
			gunModel.setPosition(gunPos);
			
			gunModel.setRotation(Camera::rotation.x, -Camera::rotation.y, Camera::rotation.z);

			muzzleLight.setStrength(muzzleFlashAnimation.update(0.0f, 0.25f, delta));
			muzzleLight.setPosition(gunPos + Camera::front * 0.5f);
			
			//testAnim.update(delta);

			// draw to stencil buffer
			        
			gunModel.setTexture("diffuse", gunTexture);
			gunModel.setTexture("specular", gunTexture_specular);
			Light::draw(gunModel);

			// dont write to stentil buffer //
	        glEnable(GL_DEPTH_TEST);
						
		screenFramebuffer.bind();
			glClearColor(0.4, 0.5, 0.9, 1.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			glEnable(GL_DEPTH_TEST);

			houseModel.setTexture("diffuse", houseTexture);
			houseModel.setTexture("specular", houseTexture_specular);
			Light::draw(houseModel);
						
			planeModel.setTexture("diffuse", planeTexture0);
			planeModel.setTexture("specular", planeTexture0);
			Light::draw(planeModel);
			

			for(auto& enemy: enemies)
			{
				enemy.draw();
			}
			if(!restartScreen)
			{
				for(auto& enemy: enemies)
				{
					if(enemy.getHP() <= 0.0f)
					{
						{
							Enemy enemy(bilboardEnemy, enemyTextures[randint(0, enemyTextures.size())], 
								glm::vec3(randint(0, mapWidth), 0.0f, randint(0, mapDepth)));
							enemies.push_back(enemy);
						}
						if(randint(0, 100) > 75)
						{
							{
								Enemy enemy(bilboardEnemy, enemyTextures[randint(0, enemyTextures.size())], 
									glm::vec3(randint(0, mapWidth), 0.0f, randint(0, mapDepth)));
								enemies.push_back(enemy);
							}
						}
						playerScore++;
					}
					if(pointInsideCircle(player.getPosition().x, 0.0f, player.getPosition().z, enemy.getPosition(), 0.5f * enemy.getScale().x))
					{
						if(!playerGotDamageAnimation.isPlaying())
						{
							playerGotDamageAnimation.restart();
							player.addHP(-1.0f);
						}
					}

					auto enemyOldPos = enemy.getPosition();
					auto enemyPos = enemy.getPosition();
					auto enemyDir = player.getPosition() - enemyPos;
					enemyDir.y = 0.0f;
					enemyDir = glm::normalize(enemyDir);
					enemyPos += player.getWalkSpeed() * 0.7f * delta * enemyDir;

					for(auto& solid: solidObjects)
					{
						if(pointInsideRect(enemyPos.x, enemyOldPos.z, solid))
						{
							enemyPos.x = enemyOldPos.x;
						}
						if(pointInsideRect(enemyOldPos.x, enemyPos.z, solid))
						{
							enemyPos.z = enemyOldPos.z;
						}
					} 
											
					enemy.setPosition(enemyPos);
				}
			}

			enemies.erase(
				std::remove_if(
					enemies.begin(), enemies.end(),
					[](Enemy enemy) 
					{ 
						return enemy.getHP() <= 0.0f; 
					}
				),
				enemies.end()
			);

			if(!restartScreen)
			{
				screenSprite.setPosition(glm::vec2(0.0f, 1.0f) * gunMoveBobAnimation.update(0.0f, 5.0f, delta));
				screenSprite.setTexture("texture0", gunFramebuffer.getTexture());
				screenSprite.draw();
			}
			else if(!buttonRestartDelay.isPlaying())
			{
				buttonRestartDelay.update(0.0f, 0.0f, delta);
				if(!buttonRestartDelay.isPlaying())
				{
					if(mouseX > restartButton.getPosition().x 
					&& mouseX < restartButton.getPosition().x + restartButton.getScale().x 
					&& mouseY > restartButton.getPosition().y
					&& mouseY < restartButton.getPosition().y + restartButton.getScale().y * 0.5f
					)
					{
						restartButton.setTexture("texture0", restartButtonSelectedTexture);
					}
					else
					{
						restartButton.setTexture("texture0", restartButtonTexture);
					}
					restartButton.draw();
				}
			}
						
			bilboardSun.setTexture("texture0", sunTexture);
			bilboardSun.setPosition(player.getPosition() - SunLight::_direction * 75.0f);
			bilboardSun.draw();
			
		Framebuffer::unbind();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);
		screenSprite.setPosition(-10.0f, -10.0f + 6.0f * std::sin(playerFallBobAnimation.update(0.0f, 6.0f, delta) * 0.1f));
		screenSprite.setTexture("texture0", screenFramebuffer.getTexture());
		shaderDamage.use();
		shaderDamage.setFloat("anim", playerGotDamageAnimation.update(0.0f, 0.9f, delta));
		screenSprite.draw(shaderDamage);

		std::vector<unsigned int> playerScoreDigits;
		for(unsigned int tempPlayerScore = playerScore; tempPlayerScore; tempPlayerScore /= 10)
		{
			playerScoreDigits.push_back(tempPlayerScore % 10);
		}
		std::reverse(playerScoreDigits.begin(), playerScoreDigits.end());
		if(playerScoreDigits.size() == 0)
		{
			playerScoreDigits.push_back(0);
		}
		
		for(unsigned int i = 0; i < playerScoreDigits.size(); i++)
		{
			unsigned int digit = playerScoreDigits[i];
			numSprite.setTexture("texture0", numbersTextures[digit]);
			numSprite.setPosition(128.0f + 64.0f + 64.0f + i * 96.0f, 32.0f);
			numSprite.draw();
		}

		scoreTextSprite.setTexture("texture0", scoreTextTexture);
		scoreTextSprite.setPosition(10.0f, 32.0f);
		scoreTextSprite.draw();
		
		
   		glfwSwapBuffers(window);
   		glfwPollEvents();
   	}

	glfwDestroyWindow(window);
   	glfwTerminate();
}
