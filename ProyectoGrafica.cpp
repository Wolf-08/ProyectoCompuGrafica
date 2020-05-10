/*---------------------------------------------------------*/
/* ----------------   Proyecto Area residencial --------------------------*/
/*-----------------    2020-2   ---------------------------*/
/*------------- Alumnos: Dominguez Cisneros Alexis Saul
						 Montecillo Sandoval Jose Alejandro
						 Ortega Moreno Omar Orlando---------------*/
//#define STB_IMAGE_IMPLEMENTATION
#include <glew.h>
#include <glfw3.h>
#include <stb_image.h>
#include <Windows.h>
#include <MMSystem.h>
#include "camera.h"
#include "Model.h"
#include "Texture.h"

// Other Libs
#include "SOIL2/SOIL2.h"

void resize(GLFWwindow* window, int width, int height);
//void my_input(GLFWwindow *window);
void my_input(GLFWwindow *window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

// settings
// Window size
int SCR_WIDTH = 3800;
int SCR_HEIGHT = 7600;

GLFWmonitor *monitors;
GLuint skyboxVBO, skyboxVAO;

//Camera
Camera camera(glm::vec3(3.0f, 2.0f, 7.0f));
double	lastX = 0.0f,
		lastY = 0.0f;
bool firstMouse = true;

//Timing
double	deltaTime = 0.0f,
		lastFrame = 0.0f;

//Lighting
glm::vec3 lightPosition(0.0f, 8.0f, 5.0f);
glm::vec3 lightDirection(0.0f, -1.0f, 0.0f);

void myData(void);
void display(Shader, Model, Model);
void getResolution(void);
void animate(void);
void LoadTextures(void);
unsigned int generateTextures(char*, bool);

//Texture
unsigned int	t_smile,
				t_toalla,
				t_unam,
				t_white,
				t_panda,
				t_cubo,
				t_caja,
				t_caja_brillo;

//For Sherksss
float rotShrek = 0.0f;
//Variables para animacion de Avion
bool animacion = false;
float movAvion_x = -35.0f,	
	  movAvion_y = 1.5f,
	  movAvion_z = -60.0f,
	  avionRotY = 180.0f;
//Variables para animacion del Lambo
bool lambo = false;
float movAutoX = -25.0f,
	  movAutoY = -1.7f, 
      movAutoZ = -55.0f,
	  radio    =  13.2f,
      rotModel =  0.0f;
//Para modelo virus
float movVirusY =1.0f;

//Animacion globo -11, 2, -10
float
movGlobo_x = -12.f,
movGlobo_y = 1.0f,
movGlobo_z = -10.0f,
orientaG = 0.0f;
bool	animacionG = false,
recorridoG1 = true,
recorridoG2 = false,
recorridoG3 = false,
recorridoG4 = false,
recorridoG5 = false;


void sonido() {

	sndPlaySound("s.wav", SND_ASYNC);

}

bool arriba = false;


void sonido();

unsigned int generateTextures(const char* filename, bool alfa)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	
	unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
	if (data)
	{
		if (alfa)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		return textureID;
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
		return 100;
	}

	stbi_image_free(data);
}

void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;

	lastX = SCR_WIDTH / 2.0f;
	lastY = SCR_HEIGHT / 2.0f;

}

void LoadTextures()
{

	
}

void myData()
{	
	GLfloat skyboxVertices[] = {
		// Positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);

	glBindVertexArray(skyboxVAO);

	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

	// SkyBox
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);

}

int mov_auto = 0,
	mov_avion = 0,
	mov_covid = 0;
float velocidad = 0.5;

void animate(void)
{
	if (mov_covid == 0)
	{
		movVirusY += velocidad - 0.4f;;
		if (movVirusY >= 5.0f)
			mov_covid = 1;

	}
	if (mov_covid == 1) {
		movVirusY -= velocidad - 0.4f;
		if (movVirusY <= 3.0f)
			mov_covid = 0;
	}
	if (lambo) {
		if (mov_auto == 0) {
			movAutoZ += velocidad;
			if (movAutoZ >= -35.50f)
				mov_auto = 1;
		}
		if (mov_auto == 1) {
			rotModel = -90;
			movAutoX += velocidad;
			if (movAutoX >= 39.0f)
				mov_auto = 2;
		}
		 if(mov_auto == 2){
			 rotModel = 0;
			 movAutoZ += velocidad;
			if (movAutoZ >= 23.5f)
		    	mov_auto = 3;
		}
		
		 if (mov_auto == 3) {
			rotModel = 90.0f;
			movAutoX -= velocidad;
			if (movAutoX <= -2.0f)
				mov_auto = 4;
		}
		
		if (mov_auto == 4){
			rotModel += 3.0f;
			movAutoX = (radio * cos(glm::radians(rotModel)) -2.0f);
			movAutoZ = 23.0f + (radio * sin(glm::radians(rotModel))-radio);
			if (rotModel == 360.0f) {
				rotModel = 0.0f;
				mov_auto = 4;
			}
		}


	 }
	if (animacion) {
		if (mov_avion == 0)
		{
			//avionRotY = 0.0f;
			movAvion_y += velocidad;
			if (movAvion_y >= 35.0f) {
				mov_avion = 1;
			}
		}

		if (mov_avion == 1)
		{
			movAvion_z += velocidad;
			//avionRotY = 90.0f;
			if (movAvion_z >= 35.0f)
				mov_avion = 2;
		}

		if (mov_avion == 2)
		{
			movAvion_x += velocidad;
			avionRotY = -90.0f;
			if (movAvion_x >= 40.0f)
				mov_avion = 3;
		}  

		if (mov_avion == 3)
		{
			movAvion_z -= velocidad;
			avionRotY = -180.0f;
			if (movAvion_z <= -60.0f)
				mov_avion = 4;
		}

		if (mov_avion == 4)
		{
			movAvion_x -= velocidad;
			avionRotY = 90.0f;
			if (movAvion_x <= -35.0f)
				mov_avion = 5;
		}

		if (mov_avion == 5)
		{
			avionRotY = 180.0f;
			//if (movAvion_z <= 30.0f)
			mov_avion = 1;
		}
	}

	if (animacionG)
	{
		//animacion del Globo
		if (recorridoG1)
		{
			movGlobo_y += 0.3f;//velocidad
			//orientaG = 180.0f;//giro
			if (movGlobo_y > 0.6f)//cuando se mueva hasta..
			{
				recorridoG1 = false;//deja el recorrido1
				recorridoG2 = true;//comienza el recorrido 2
			}
		}

		if (recorridoG2)
		{
			movGlobo_y += 0.15;
			movGlobo_x += 0.25;
			orientaG = 30.96f;
			if (movGlobo_x > 1.5f)
			{
				recorridoG2 = false;
				recorridoG3 = true;
			}
		}

		if (recorridoG3)
		{
			movGlobo_y += 0.15;
			movGlobo_x -= 0.25;
			orientaG = 30.96f;
			if (movGlobo_x < -2.0f)
			{
				recorridoG3 = false;
				recorridoG4 = true;
			}
		}
		if (recorridoG4)
		{
			movGlobo_y += 0.15;
			movGlobo_x += 0.25;
			orientaG = 30.96f;
			if (movGlobo_x > 4.0f)
			{
				recorridoG4 = false;
				recorridoG5 = true;
			}
		}
		if (recorridoG5)
		{
			movGlobo_y += 0.15;
			movGlobo_x -= 0.25;
			orientaG = 30.96f;
			if (movGlobo_x < -2.0f)
			{
				recorridoG5 = false;
				recorridoG1 = true;
			}
		}
	}
	
	printf("movX %f  \n ", movAutoX );
	//printf("movY %f  \n ", movAutoY); 
	printf("movZ %f  \n ", movAutoZ); 

}

void display(Shader shader, Shader skyboxShader, GLuint skybox, std::vector<Model> modelArr)
{
	shader.use();

	//Setup Advanced Lights
	shader.setVec3("viewPos", camera.Position);
	shader.setVec3("dirLight.direction", lightDirection);
	shader.setVec3("dirLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
	shader.setVec3("dirLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
	shader.setVec3("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));

	shader.setVec3("pointLight[0].position", lightPosition);
	shader.setVec3("pointLight[0].ambient", glm::vec3(1.0f, 1.0f, 1.0f));
	shader.setVec3("pointLight[0].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
	shader.setVec3("pointLight[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));
	shader.setFloat("pointLight[0].constant", 1.0f); 
	shader.setFloat("pointLight[0].linear", 0.009f);  
	shader.setFloat("pointLight[0].quadratic", 0.0032f); 

	shader.setVec3("pointLight[1].position", glm::vec3(0.0, 0.0f, 0.0f));
	shader.setVec3("pointLight[1].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
	shader.setVec3("pointLight[1].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
	shader.setVec3("pointLight[1].specular", glm::vec3(0.0f, 0.0f, 0.0f));
	shader.setFloat("pointLight[1].constant", 1.0f);
	shader.setFloat("pointLight[1].linear", 0.009f);
	shader.setFloat("pointLight[1].quadratic", 0.032f);

	shader.setFloat("material_shininess", 32.0f);

	// create transformations and Projection
	glm::mat4 origin = glm::mat4(1.0f);
	glm::mat4 model = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 view = glm::mat4(1.0f);		//Use this matrix for ALL models
	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection

	//Use "projection" to include Camera
	projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 150.0f);
	view = camera.GetViewMatrix();

	// pass them to the shaders
	shader.setMat4("model", model);
	shader.setMat4("view", view);
	shader.setMat4("projection", projection);

	/*LISTA ORDEN ARREGLO
	modelPiso -> 0, modelAlberca -> 1, modelCasa -> 2, modelEdificio -> 3, modelBuilding -> 4, modelEdificio2 -> 5, modelResidencia -> 6,
	modelHouseF -> 7, modelArbol -> 8, modelArbol1 -> 9, modelArbol2 -> 10, modelPalmera1 -> 11, modelLampara -> 12, modelArbolSaul -> 13, 
	modelSauce -> 14, modelPlanta -> 15, modelFuente -> 16, modelShrek -> 17, modelAvion -> 18, modelPlataforma -> 19,modelLambo ->20
	modelLlantas -> 21, modelVirus ->22, modelGlobo -> 23, modelNiña -> 24
	*/ /*EL 6 Y EL 9 NO SIRVEN*/

	model = glm::mat4(1.0f);
	origin = glm::mat4(1.0f);
	//Temporal para manipulacion del modelo Lambo
	glm::mat4 tmp = glm::mat4(1.0f);
	//Temporal para el covid
	glm::mat4 tmp1 = glm::mat4(1.0f);




	model = glm::translate(origin, glm::vec3(0.0f, -1.8f, -1.0f));
	model = glm::scale(model, glm::vec3(0.025f, 0.025f, 0.025f));
	shader.setMat4("model", model);
	modelArr.at(0).Draw(shader);

	glm::mat4 modelAlberca = glm::translate(origin, glm::vec3(57, -1.7, 38));
	modelAlberca = glm::scale(modelAlberca, glm::vec3(.1f, .1f, .1f));
	shader.setMat4("model", modelAlberca);
	modelArr.at(1).Draw(shader);

	glm::mat4 modelCasa = glm::translate(origin, glm::vec3(0.0f, -1.7f, -27.0f));
	modelCasa = glm::scale(modelCasa, glm::vec3(2.0f, 6.0f, 2.0f));
	shader.setMat4("model", modelCasa);
	modelArr.at(2).Draw(shader);

	glm::mat4 modelCasa1 = glm::translate(origin, glm::vec3(19.5f, -1.7f, -27.0f));
	modelCasa1 = glm::scale(modelCasa1, glm::vec3(2.0f, 6.0f, 2.0f));
	shader.setMat4("model", modelCasa1);
	modelArr.at(2).Draw(shader);

	glm::mat4 modelCasa2 = glm::translate(origin, glm::vec3(-19.5f, -1.7f, -27.0f));
	modelCasa2 = glm::scale(modelCasa2, glm::vec3(2.0f, 6.0f, 2.0f));
	shader.setMat4("model", modelCasa2);
	modelArr.at(2).Draw(shader);

	glm::mat4 modelCasa3 = glm::translate(origin, glm::vec3(50.0f, -1.7f, 1.0f));
	modelCasa3 = glm::rotate(modelCasa3, glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	modelCasa3 = glm::scale(modelCasa3, glm::vec3(2.0f, 6.0f, 2.0f));
	shader.setMat4("model", modelCasa3);
	modelArr.at(2).Draw(shader);

	glm::mat4 modelCasa7 = glm::translate(origin, glm::vec3(50.0f, -1.7f, -19.0f));
	modelCasa7 = glm::rotate(modelCasa7, glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	modelCasa7 = glm::scale(modelCasa7, glm::vec3(2.0f, 6.0f, 2.0f));
	shader.setMat4("model", modelCasa7);
	modelArr.at(2).Draw(shader);

	glm::mat4 modelCasa4 = glm::translate(origin, glm::vec3(0.0f, -1.7f, -48.0f));
	modelCasa4 = glm::scale(modelCasa4, glm::vec3(2.0f, 6.0f, 2.0f));
	shader.setMat4("model", modelCasa4);
	modelArr.at(2).Draw(shader);

	glm::mat4 modelCasa5 = glm::translate(origin, glm::vec3(20.0f, -1.7f, -48.0f));
	modelCasa5 = glm::scale(modelCasa5, glm::vec3(2.0f, 6.0f, 2.0f));
	shader.setMat4("model", modelCasa5);
	modelArr.at(2).Draw(shader);

	glm::mat4 modelCasa6 = glm::translate(origin, glm::vec3(-20.0f, -1.7f, -48.0f));
	modelCasa6 = glm::scale(modelCasa6, glm::vec3(2.0f, 6.0f, 2.0f));
	shader.setMat4("model", modelCasa6);
	modelArr.at(2).Draw(shader);

	glm::mat4 modelEdificio2 = glm::translate(origin, glm::vec3(0, 8.2, 65));
	modelEdificio2 = glm::rotate(modelEdificio2, glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	modelEdificio2 = glm::scale(modelEdificio2, glm::vec3(8.0f, 8.0f, 8.0f));
	shader.setMat4("model", modelEdificio2);
	modelArr.at(5).Draw(shader);

	glm::mat4 modelEdificio3 = glm::translate(origin, glm::vec3(-15, 8.2, 65));
	modelEdificio3 = glm::rotate(modelEdificio3, glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	modelEdificio3 = glm::scale(modelEdificio3, glm::vec3(8.0f, 8.0f, 8.0f));
	shader.setMat4("model", modelEdificio3);
	modelArr.at(5).Draw(shader);

	glm::mat4 modelEdificio5 = glm::translate(origin, glm::vec3(-30, 8.2, 65));
	modelEdificio5 = glm::rotate(modelEdificio5, glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	modelEdificio5 = glm::scale(modelEdificio5, glm::vec3(8.0f, 8.0f, 8.0f));
	shader.setMat4("model", modelEdificio5);
	modelArr.at(5).Draw(shader);

	glm::mat4 modelEdificio4 = glm::translate(origin, glm::vec3(15, 8.2, 65));
	modelEdificio4 = glm::rotate(modelEdificio4, glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	modelEdificio4 = glm::scale(modelEdificio4, glm::vec3(8.0f, 8.0f, 8.0f));
	shader.setMat4("model", modelEdificio4);
	modelArr.at(5).Draw(shader);

	glm::mat4 modelEdificio6 = glm::translate(origin, glm::vec3(30, 8.2, 65));
	modelEdificio6 = glm::rotate(modelEdificio6, glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	modelEdificio6 = glm::scale(modelEdificio6, glm::vec3(8.0f, 8.0f, 8.0f));
	shader.setMat4("model", modelEdificio6);
	modelArr.at(5).Draw(shader);

	glm::mat4 modelEdificio7 = glm::translate(origin, glm::vec3(45, 8.2, 65));
	modelEdificio7 = glm::rotate(modelEdificio7, glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	modelEdificio7 = glm::scale(modelEdificio7, glm::vec3(8.0f, 8.0f, 8.0f));
	shader.setMat4("model", modelEdificio7);
	modelArr.at(5).Draw(shader);

	glm::mat4 modelHouseF = glm::translate(origin, glm::vec3(-14.0f, -2.5f, -5.0f));
	modelHouseF = glm::rotate(modelHouseF, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelHouseF = glm::scale(modelHouseF, glm::vec3(4.5f, 4.5f, 4.5f));
	shader.setMat4("model", modelHouseF);
	modelArr.at(7).Draw(shader);

	glm::mat4 modelHouseF1 = glm::translate(origin, glm::vec3(-14.0f, -2.5f, 15.0f));
	modelHouseF1 = glm::rotate(modelHouseF1, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelHouseF1 = glm::scale(modelHouseF1, glm::vec3(4.5f, 4.5f, 4.5f));
	shader.setMat4("model", modelHouseF1);
	modelArr.at(7).Draw(shader);

	glm::mat4 modelHouseF2 = glm::translate(origin, glm::vec3(-14.0f, -2.5f, 35.0f));
	modelHouseF2 = glm::rotate(modelHouseF2, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelHouseF2 = glm::scale(modelHouseF2, glm::vec3(4.5f, 4.5f, 4.5f));
	shader.setMat4("model", modelHouseF2);
	modelArr.at(7).Draw(shader);

	glm::mat4 modelHouseF3 = glm::translate(origin, glm::vec3(-14.0f, -2.5f, 55.0f));
	modelHouseF3 = glm::rotate(modelHouseF3, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelHouseF3 = glm::scale(modelHouseF3, glm::vec3(4.5f, 4.5f, 4.5f));
	shader.setMat4("model", modelHouseF3);
	modelArr.at(7).Draw(shader);

	glm::mat4 modelArbol = glm::translate(origin, glm::vec3(0, -1.8, -30));
	modelArbol = glm::scale(modelArbol, glm::vec3(1.0f, 1.0f, 1.0f));
	shader.setMat4("model", modelArbol);
	modelArr.at(8).Draw(shader);

	glm::mat4 modelArbol1 = glm::translate(origin, glm::vec3(0, -1.8, -17));
	modelArbol1 = glm::scale(modelArbol1, glm::vec3(1.0f, 1.0f, 1.0f));
	shader.setMat4("model", modelArbol1);
	modelArr.at(8).Draw(shader);

	glm::mat4 copyModelArbol = glm::translate(origin, glm::vec3(14, -1.8, -17));
	copyModelArbol = glm::scale(copyModelArbol, glm::vec3(1.0f, 1.0f, 1.0f));
	shader.setMat4("model", copyModelArbol);
	modelArr.at(8).Draw(shader);

	glm::mat4 modelArbolGnomo = glm::translate(origin, glm::vec3(-50, 5.7, -45));
	modelArbolGnomo = glm::scale(modelArbolGnomo, glm::vec3(6.0f, 6.0f, 6.0f));
	shader.setMat4("model", modelArbolGnomo);
	modelArr.at(10).Draw(shader);

	glm::mat4 modelArbolGnomo1 = glm::translate(origin, glm::vec3(-65, 5.7, -50));
	modelArbolGnomo1 = glm::scale(modelArbolGnomo1, glm::vec3(6.0f, 6.0f, 6.0f));
	shader.setMat4("model", modelArbolGnomo1);
	modelArr.at(10).Draw(shader);

	glm::mat4 modelArbolGnomo2 = glm::translate(origin, glm::vec3(-60, 5.7, -40));
	modelArbolGnomo2 = glm::scale(modelArbolGnomo2, glm::vec3(6.0f, 6.0f, 6.0f));
	shader.setMat4("model", modelArbolGnomo2);
	modelArr.at(10).Draw(shader);

	glm::mat4 modelArbolGnomo3 = glm::translate(origin, glm::vec3(-60, 5.7, -58));
	modelArbolGnomo3 = glm::scale(modelArbolGnomo3, glm::vec3(6.0f, 6.0f, 6.0f));
	shader.setMat4("model", modelArbolGnomo3);
	modelArr.at(10).Draw(shader);

	glm::mat4 modelPalmerita = glm::translate(origin, glm::vec3(15, 2.7, 0));
	modelPalmerita = glm::scale(modelPalmerita, glm::vec3(5, 5, 5));
	shader.setMat4("model", modelPalmerita);
	modelArr.at(11).Draw(shader);

	glm::mat4 modelPalmerita1 = glm::translate(origin, glm::vec3(15, 2.7, 5));
	modelPalmerita1 = glm::scale(modelPalmerita1, glm::vec3(5, 5, 5));
	shader.setMat4("model", modelPalmerita1);
	modelArr.at(11).Draw(shader);

	glm::mat4 modelPalmerita2 = glm::translate(origin, glm::vec3(22, 2.7, 3));
	modelPalmerita2 = glm::scale(modelPalmerita2, glm::vec3(5, 5, 5));
	shader.setMat4("model", modelPalmerita2);
	modelArr.at(11).Draw(shader);

	glm::mat4 modelPalmerita3 = glm::translate(origin, glm::vec3(-18, 2.7, 5));
	modelPalmerita3 = glm::scale(modelPalmerita3, glm::vec3(5, 5, 5));
	shader.setMat4("model", modelPalmerita3);
	modelArr.at(11).Draw(shader);

	glm::mat4 modelPalmerita4 = glm::translate(origin, glm::vec3(-18, 2.7, -10));
	modelPalmerita4 = glm::scale(modelPalmerita4, glm::vec3(5, 5, 5));
	shader.setMat4("model", modelPalmerita4);
	modelArr.at(11).Draw(shader);

	glm::mat4 modelLampara = glm::translate(origin, glm::vec3(-11, 2, 12));
	modelLampara = glm::scale(modelLampara, glm::vec3(3.0f, 3.0f, 3.0f));
	shader.setMat4("model", modelLampara);
	modelArr.at(12).Draw(shader);

	glm::mat4 modelLampara1 = glm::translate(origin, glm::vec3(-11, 2, -10));
	modelLampara1 = glm::scale(modelLampara1, glm::vec3(3.0f, 3.0f, 3.0f));
	shader.setMat4("model", modelLampara1);
	modelArr.at(12).Draw(shader);

	glm::mat4 modelLampara2 = glm::translate(origin, glm::vec3(19, 2, -10));
	modelLampara2 = glm::scale(modelLampara2, glm::vec3(3.0f, 3.0f, 3.0f));
	shader.setMat4("model", modelLampara2);
	modelArr.at(12).Draw(shader);

	glm::mat4 modelLampara3 = glm::translate(origin, glm::vec3(19, 2, 12));
	modelLampara3 = glm::scale(modelLampara3, glm::vec3(3.0f, 3.0f, 3.0f));
	shader.setMat4("model", modelLampara3);
	modelArr.at(12).Draw(shader);

	glm::mat4 modelLampara4 = glm::translate(origin, glm::vec3(-17.5, 2, -18.5));
	modelLampara4 = glm::scale(modelLampara4, glm::vec3(3.0f, 3.0f, 3.0f));
	shader.setMat4("model", modelLampara4);
	modelArr.at(12).Draw(shader);

	glm::mat4 modelLampara5 = glm::translate(origin, glm::vec3(-7.5, 2, -18.5));
	modelLampara5 = glm::scale(modelLampara5, glm::vec3(3.0f, 3.0f, 3.0f));
	shader.setMat4("model", modelLampara5);
	modelArr.at(12).Draw(shader);

	glm::mat4 modelLampara6 = glm::translate(origin, glm::vec3(2.5, 2, -18.5));
	modelLampara6 = glm::scale(modelLampara6, glm::vec3(3.0f, 3.0f, 3.0f));
	shader.setMat4("model", modelLampara6);
	modelArr.at(12).Draw(shader);

	glm::mat4 modelLampara7 = glm::translate(origin, glm::vec3(12.5, 2, -18.5));
	modelLampara7 = glm::scale(modelLampara7, glm::vec3(3.0f, 3.0f, 3.0f));
	shader.setMat4("model", modelLampara7);
	modelArr.at(12).Draw(shader);

	glm::mat4 modelLampara8 = glm::translate(origin, glm::vec3(22.5, 2, -18.5));
	modelLampara8 = glm::scale(modelLampara8, glm::vec3(3.0f, 3.0f, 3.0f));
	shader.setMat4("model", modelLampara8);
	modelArr.at(12).Draw(shader);

	glm::mat4 modelLampara9 = glm::translate(origin, glm::vec3(32.5, 2, -18.5));
	modelLampara9 = glm::scale(modelLampara9, glm::vec3(3.0f, 3.0f, 3.0f));
	shader.setMat4("model", modelLampara9);
	modelArr.at(12).Draw(shader);

	glm::mat4 modelLamparaAtras4 = glm::translate(origin, glm::vec3(-17.5, 2, -38.5));
	modelLamparaAtras4 = glm::scale(modelLamparaAtras4, glm::vec3(3.0f, 3.0f, 3.0f));
	shader.setMat4("model", modelLamparaAtras4);
	modelArr.at(12).Draw(shader);

	glm::mat4 modelLamparaAtras5 = glm::translate(origin, glm::vec3(-7.5, 2, -38.5));
	modelLamparaAtras5 = glm::scale(modelLamparaAtras5, glm::vec3(3.0f, 3.0f, 3.0f));
	shader.setMat4("model", modelLamparaAtras5);
	modelArr.at(12).Draw(shader);

	glm::mat4 modelLamparaAtras6 = glm::translate(origin, glm::vec3(2.5, 2, -38.5));
	modelLamparaAtras6 = glm::scale(modelLamparaAtras6, glm::vec3(3.0f, 3.0f, 3.0f));
	shader.setMat4("model", modelLamparaAtras6);
	modelArr.at(12).Draw(shader);

	glm::mat4 modelLamparaAtras7 = glm::translate(origin, glm::vec3(12.5, 2, -38.5));
	modelLamparaAtras7 = glm::scale(modelLamparaAtras7, glm::vec3(3.0f, 3.0f, 3.0f));
	shader.setMat4("model", modelLamparaAtras7);
	modelArr.at(12).Draw(shader);

	glm::mat4 modelLamparaAtras8 = glm::translate(origin, glm::vec3(22.5, 2, -38.5));
	modelLamparaAtras8 = glm::scale(modelLamparaAtras8, glm::vec3(3.0f, 3.0f, 3.0f));
	shader.setMat4("model", modelLamparaAtras8);
	modelArr.at(12).Draw(shader);

	glm::mat4 modelLamparaAtras9 = glm::translate(origin, glm::vec3(32.5, 2, -38.5));
	modelLamparaAtras9 = glm::scale(modelLamparaAtras9, glm::vec3(3.0f, 3.0f, 3.0f));
	shader.setMat4("model", modelLamparaAtras9);
	modelArr.at(12).Draw(shader);

	glm::mat4 modelFuente = glm::translate(origin, glm::vec3(-7.5f, -2.0f, 22.5f));
	modelFuente = glm::scale(modelFuente, glm::vec3(0.1f, 0.1f, 0.1f));
	shader.setMat4("model", modelFuente);
	modelArr.at(16).Draw(shader);

	glm::mat4 modelArbolSaul = glm::translate(origin, glm::vec3(50, -2, 23.0));
	modelArbolSaul = glm::scale(modelArbolSaul, glm::vec3(0.1f, 0.1f, 0.1f));
	shader.setMat4("model", modelArbolSaul);
	modelArr.at(13).Draw(shader);

	glm::mat4 modelSauce = glm::translate(origin, glm::vec3(50, -2, -40));
	modelSauce = glm::scale(modelSauce, glm::vec3(.25f, .35f, .25f));
	shader.setMat4("model", modelSauce);
	modelArr.at(14).Draw(shader);

	glm::mat4 modelPlanta = glm::translate(origin, glm::vec3(-37, 2.3, 25));
	modelPlanta = glm::scale(modelPlanta, glm::vec3(2.0f, 2.0f, 2.0f));
	shader.setMat4("model", modelPlanta);
	modelArr.at(15).Draw(shader);

	glm::mat4 modelPlanta2 = glm::translate(origin, glm::vec3(-37, 2.3, -3));
	modelPlanta2 = glm::scale(modelPlanta2, glm::vec3(2.0f, 2.0f, 2.0f));
	shader.setMat4("model", modelPlanta2);
	modelArr.at(15).Draw(shader);

	glm::mat4 modelPlanta3 = glm::translate(origin, glm::vec3(-37, 2.3, 5));
	modelPlanta3 = glm::scale(modelPlanta3, glm::vec3(2.0f, 2.0f, 2.0f));
	shader.setMat4("model", modelPlanta3);
	modelArr.at(15).Draw(shader);

	glm::mat4 modelCasaLamp4 = glm::translate(origin, glm::vec3(-40, 2, -20));
	modelCasaLamp4 = glm::scale(modelCasaLamp4, glm::vec3(3.0f, 3.0f, 3.0f));
	shader.setMat4("model", modelCasaLamp4);
	modelArr.at(12).Draw(shader);

	glm::mat4 modelCasaLamp5 = glm::translate(origin, glm::vec3(-40, 2, 0));
	modelCasaLamp5 = glm::scale(modelCasaLamp5, glm::vec3(3.0f, 3.0f, 3.0f));
	shader.setMat4("model", modelCasaLamp5);
	modelArr.at(12).Draw(shader);

	glm::mat4 modelCasaLamp6 = glm::translate(origin, glm::vec3(-40, 2, 20));
	modelCasaLamp6 = glm::scale(modelCasaLamp6, glm::vec3(3.0f, 3.0f, 3.0f));
	shader.setMat4("model", modelCasaLamp6);
	modelArr.at(12).Draw(shader);

	glm::mat4 modelCasaLamp7 = glm::translate(origin, glm::vec3(-40, 2, 40));
	modelCasaLamp7 = glm::scale(modelCasaLamp7, glm::vec3(3.0f, 3.0f, 3.0f));
	shader.setMat4("model", modelCasaLamp7);
	modelArr.at(12).Draw(shader);

	glm::mat4 modelCasaLamp8 = glm::translate(origin, glm::vec3(9, 2, 48));
	modelCasaLamp8 = glm::scale(modelCasaLamp8, glm::vec3(3.0f, 3.0f, 3.0f));
	shader.setMat4("model", modelCasaLamp8);
	modelArr.at(12).Draw(shader);

	glm::mat4 modelCasaLamp9 = glm::translate(origin, glm::vec3(24, 2, 48));
	modelCasaLamp9 = glm::scale(modelCasaLamp9, glm::vec3(3.0f, 3.0f, 3.0f));
	shader.setMat4("model", modelCasaLamp9);
	modelArr.at(12).Draw(shader);

	glm::mat4 modelCasaLamp10 = glm::translate(origin, glm::vec3(-6, 2, 48));
	modelCasaLamp10 = glm::scale(modelCasaLamp10, glm::vec3(3.0f, 3.0f, 3.0f));
	shader.setMat4("model", modelCasaLamp10);
	modelArr.at(12).Draw(shader);

	glm::mat4 modelCasaLamp11 = glm::translate(origin, glm::vec3(-21, 2, 48));
	modelCasaLamp11 = glm::scale(modelCasaLamp11, glm::vec3(3.0f, 3.0f, 3.0f));
	shader.setMat4("model", modelCasaLamp11);
	modelArr.at(12).Draw(shader);

	glm::mat4 modelAvion = glm::scale(modelAvion, glm::vec3(0.5f, 0.5f, 0.5f));
	modelAvion = glm::translate(origin, glm::vec3(movAvion_x, movAvion_y, movAvion_z));
	modelAvion = glm::rotate(modelAvion, glm::radians(avionRotY), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", modelAvion);
	modelArr.at(18).Draw(shader);

	glm::mat4 modelPlataforma = glm::scale(modelPlataforma, glm::vec3(1.0f, 1.0f, 1.0f));
	modelAvion = glm::translate(origin, glm::vec3(-35.0f, -0.5f ,-55.0f ));
	shader.setMat4("model", modelAvion);
	modelArr.at(19).Draw(shader);
	//Lambo
	glm::mat4 modelLambo = glm::translate(origin, glm::vec3(movAutoX, movAutoY,movAutoZ));
	modelLambo = glm::rotate(modelLambo, glm::radians(-rotModel - 1800.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	tmp = modelLambo = glm::scale(modelLambo, glm::vec3(1.5f, 1.5f, 1.5f));
	modelLambo = glm::scale(modelLambo, glm::vec3(0.01f, 0.01f, 0.01f));
	shader.setMat4("model", modelLambo);
	modelArr.at(20).Draw(shader);

	glm::mat4 modelLlantas = glm::translate(tmp, glm::vec3(0.85f, 0.25f, 1.29f));
	modelLlantas = glm::scale(modelLlantas, glm::vec3(0.01f, 0.01f, 0.01f));
	shader.setMat4("model", modelLlantas);
	modelArr.at(21).Draw(shader);	//Izq delantera

	modelLlantas = glm::translate(tmp, glm::vec3(-0.85f, 0.25f, 1.29f));
	modelLlantas = glm::scale(modelLlantas, glm::vec3(0.01f, 0.01f, 0.01f));
	modelLlantas = glm::rotate(modelLlantas, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", modelLlantas);
	modelArr.at(21).Draw(shader);	//Der delantera

	modelLlantas = glm::translate(tmp, glm::vec3(-0.85f, 0.25f, -1.45f));
	modelLlantas = glm::scale(modelLlantas, glm::vec3(0.01f, 0.01f, 0.01f));
	modelLlantas = glm::rotate(modelLlantas, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", modelLlantas);
	modelArr.at(21).Draw(shader); //Der trasera

	modelLlantas = glm::translate(tmp, glm::vec3(0.85f, 0.25f, -1.45f));
	modelLlantas = glm::scale(modelLlantas, glm::vec3(0.01f, 0.01f, 0.01f));
	shader.setMat4("model", modelLlantas);
	modelArr.at(21).Draw(shader);	//Izq trase
	//modelShrek = glm::scale(origin, glm::vec3(0.4f, 0.4f, 0.4f));
	glm::mat4 modelShrek = glm::translate(origin, glm::vec3(-1.0f, 9.0f, 7.5f));
	modelShrek = glm::rotate(modelShrek,glm::radians(rotShrek),glm::vec3(0.0f,1.0f,0.0f));
	shader.setMat4("model", modelShrek);
	modelArr.at(17).Draw(shader);

	float movX = 5.0f,
		  movZ = 5.0f;
	glm::mat4 modelVirus = glm::translate(origin, glm::vec3(-20.0f, 0.0f, 0.0f));
	//tmp1 = modelVirus;
	tmp = modelVirus;
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			modelVirus = glm::translate(modelVirus, glm::vec3(0.0f, movVirusY,movZ));
			shader.setMat4("model", modelVirus);
			modelArr.at(22).Draw(shader);
			//modelVirus = glm::translate(tmp, glm::vec3(movX, movVirusY, 0.0f));

		}

		tmp = glm::translate(tmp, glm::vec3(movX,0.0f, 0.0f));
		modelVirus = tmp;
	}

	//globo
	glm::mat4
		modelGlobo = glm::rotate(modelGlobo, glm::radians(0.0f), glm::vec3(0.0f, 0.3f, 0.0f));
	modelGlobo = glm::translate(glm::mat4(1.0f), glm::vec3(movGlobo_x, movGlobo_y, movGlobo_z));
	modelGlobo = glm::scale(modelGlobo, glm::vec3(0.5f, 0.5f, 0.5f));
	shader.setMat4("model", modelGlobo);
	modelArr.at(23).Draw(shader);
	//niña
	glm::mat4
	modelNiña = glm::translate(glm::mat4(1.0f), glm::vec3(-12.7f, -1.7f, -8.5f));
	modelNiña = glm::rotate(modelNiña, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelNiña = glm::scale(modelNiña, glm::vec3(0.02f, 0.02f, 0.02f));
	shader.setMat4("model", modelNiña);
	modelArr.at(24).Draw(shader);


	// Draw skybox as last
	glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
	skyboxShader.use();
	view = glm::mat4(glm::mat3(camera.GetViewMatrix()));	// Remove any translation component of the view matrix

	skyboxShader.setMat4("view", view);
	skyboxShader.setMat4("projection", projection);

	// skybox cube
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // Set depth function back to default
}

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    /*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Proyecto Final CGEIHC", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
	glfwSetWindowPos(window, 0, 30);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, resize);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, my_input);

	//To Enable capture of our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	glewInit();


	//Mis funciones
	//Datos a utilizar
	LoadTextures();
	myData();
	glEnable(GL_DEPTH_TEST);
	
	//For Models
	Shader modelShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights.fs");
	//For Primitives
	//Shader primitivasShader("shaders/shader_texture_color.vs", "shaders/shader_texture_color.fs");
	Shader SkyBoxshader("Shaders/SkyBox.vs", "Shaders/SkyBox.frag");
	
	/*Carga de Modelos*/
	Model modelPiso = ((char *)"Models/piso/piso.obj");
	Model modelAlberca = ((char *)"Models/alberca/pool.obj");
	Model modelCasa = ((char *)"Models/depas/casa.obj");
	Model modelBuilding = ((char *)"Models/edificio/Building.obj");
	Model modelEdificio = ((char *)"Models/Edificio/Building.obj");
	Model modelEdificio2 = ((char *)"Models/edificio02/edificio2.obj");
	Model modelResidencia = ((char *)"Models/Edificio2/residencia.obj");
	Model modelHouseF = ((char *)"Models/house/house.obj");
	Model modelArbol = ((char *)"Models/arbol/arbol.obj");
	Model modelArbol1 = ((char *)"Models/arbol1/arbol1.obj");
	Model modelArbolGnomo = ((char *)"Models/arbol2/arbol2.obj");
	Model modelPalmerita = ((char *)"Models/palmera1/palmera1.obj");
	Model modelLampara = ((char *)"Models/lampara/lampara.obj");
	Model modelShrek = ((char *)"Models/Shrek/Shrek.obj");
	Model modelArbolSaul = ((char *)"Models/arbol3/arbol3.obj");
	Model modelSauce = ((char *)"Models/arbol4/arbol4.obj");
	Model modelPlanta = ((char *)"Models/planta/planta.obj");
	Model modelFuente = ((char *)"Models/fuente/fuente.obj");
	Model modelAvion = ((char *)"Models/avion/avion.obj");
	Model modelPlataforma = ((char *)"Models/plataforma/plataforma.obj");
	Model modelLambo = ((char *)"Models/Lambo/carroseria.obj");
	Model modelLlantas = ((char *)"Models/Lambo/Wheel.obj");
	Model modelVirus = ((char *) "Models/virus/covid.obj");
	Model modelGlobo = ((char *)"Models/globo/globo.obj");
	Model modelNiña = ((char *)"Models/niña/niña.obj");
	/*Array para los modelos*/
	std::vector<Model> modelArr;
	modelArr.push_back(modelPiso); //0
	modelArr.push_back(modelAlberca); //1
	modelArr.push_back(modelCasa); //2
	modelArr.push_back(modelEdificio); //3
	modelArr.push_back(modelBuilding); //4
	modelArr.push_back(modelEdificio2); //5
	modelArr.push_back(modelResidencia); //6 -> no sirve
	modelArr.push_back(modelHouseF); //7
	modelArr.push_back(modelArbol); //8
	modelArr.push_back(modelArbol1); //9 -> no sirve
	modelArr.push_back(modelArbolGnomo); //10
	modelArr.push_back(modelPalmerita); //11
	modelArr.push_back(modelLampara); //12
	modelArr.push_back(modelArbolSaul); //13
	modelArr.push_back(modelSauce); // 14
	modelArr.push_back(modelPlanta); // 15
	modelArr.push_back(modelFuente); // 16
	/******************DESPUES DE AQUI VAN SUS MODELOS***********************/
	modelArr.push_back(modelShrek); //17
	modelArr.push_back(modelAvion); // 18
	modelArr.push_back(modelPlataforma); // 19
	modelArr.push_back(modelLambo); // 20
	modelArr.push_back(modelLlantas); // 21
	modelArr.push_back(modelVirus); // 22
	modelArr.push_back(modelGlobo); // 23
	modelArr.push_back(modelNiña); // 24


	// Load textures
	vector<const GLchar*> faces;
	/*SKYBOX VALENCIA*/
	/*faces.push_back("SkyBox/right.tga");
	faces.push_back("SkyBox/left.tga");
	faces.push_back("SkyBox/top.tga");
	faces.push_back("SkyBox/bottom.tga");
	faces.push_back("SkyBox/back.tga");
	faces.push_back("SkyBox/front.tga");*/
	/*OTRO*/
	/*faces.push_back("SkyBox/Others/purplenebula_rt.tga");
	faces.push_back("SkyBox/Others/purplenebula_lf.tga");
	faces.push_back("SkyBox/Others/purplenebula_up.tga");
	faces.push_back("SkyBox/Others/purplenebula_dn.tga");
	faces.push_back("SkyBox/Others/purplenebula_bk.tga");
	faces.push_back("SkyBox/Others/purplenebula_ft.tga");*/

	faces.push_back("SkyBox/Others/posx.jpg");
	faces.push_back("SkyBox/Others/negx.jpg");
	faces.push_back("SkyBox/Others/posy.jpg");
	faces.push_back("SkyBox/Others/negy.jpg");
	faces.push_back("SkyBox/Others/negz.jpg");
	faces.push_back("SkyBox/Others/posz.jpg");


	GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);
    
	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection
	projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	// render loop
    // While the windows is not closed
    while (!glfwWindowShouldClose(window))
    {
		// per-frame time logic
		// --------------------
		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
        // input
        // -----
        //my_input(window);
		animate();
		//for sherk
		rotShrek += 1.0;
        // render
        // Backgound color
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//display(modelShader, ourModel, llantasModel);
		display(modelShader, SkyBoxshader, cubemapTexture, modelArr);
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &skyboxVBO);

    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);


	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		lightPosition.z -=0.5f;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		lightPosition.z += 0.5f;

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		animacion = !animacion;
		////animacion ^= true;
		//if(movAuto_z >= 10.0f)
		//	movAuto_z = 0.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		lambo = !lambo;

	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
	{
		//animacionG ^= true; -11, 2, -10
		animacionG = !animacionG;
		movGlobo_x = -12.0f;
		movGlobo_y = 1.0f;
		movGlobo_z = -10.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		sonido();
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void resize(GLFWwindow* window, int width, int height)
{
    // Set the Viewport to the size of the created window
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	double xoffset = xpos - lastX;
	double yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}