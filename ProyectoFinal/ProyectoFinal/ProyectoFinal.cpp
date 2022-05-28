// Std. Includes
#include <string>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "SOIL2/SOIL2.h"
#include "stb_image.h"

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void animacionPuerta();
void animacionLampara();
void animacionAspiradora();
void animacionRefrigerador();
void animacionEstufa();


// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

//Animaciones
bool abrirPuerta = false;
bool cerrarPuerta = false;
bool tirarLampara = false;
bool circuitoAspiradora = false;
bool abrirRefrigerador = false;
bool cerrarRefrigerador = false;
bool abrirEstufa = false;
bool cerrarEstufa = false;

bool recorrido1 = true;
bool recorrido2 = false;
bool recorrido3 = false;
bool recorrido4 = false;

//movimientos
float movAspiradoraX = 0.0;
float movAspiradoraZ = 0.0;

//rotaciones
float rotPuerta = 0.0;
float rotLampara = 0.0;
float rotAspiradora = 0.0;
float rotRefrigerador = 0.0;
float rotEstufa = 0.0;


GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;


int main()
{
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Adaptacion,carga de modelos y camara sintetica", nullptr, nullptr);

    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    // Set the required callback functions
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    // GLFW Options
    //glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    // Define the viewport dimensions
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // OpenGL options
    glEnable(GL_DEPTH_TEST);

    // Setup and compile our shaders
    Shader shader("Shaders/modelLoading.vs", "Shaders/modelLoading.frag");

    // Load models
    //Cuarto Principal
    Model TVRocko((char*)"Models/TVRocko/TVRocko.obj");
    Model SillonGrandeRocko((char*)"Models/SillonGrandeRocko/SILLON_ROCKO.obj");
    Model SillonChicoRocko((char*)"Models/Sillon/SillonChico.obj");
    Model Fachada((char*)"Models/Fachada/Casarocko.obj");
    Model LamparaRocko((char*)"Models/LamparaRocko/Lampara_Rocko.obj");
    Model MesaRocko((char*)"Models/MesaRocko/mesaRocko.obj");
    Model AspiradoraRocko((char*)"Models/AspiradoraRocko/AspiradoraRocko.obj");
    Model CuadroRayoRocko((char*)"Models/CuadrosRocko/CuadroRayoRocko.obj");
    Model CuadroGarabatoRocko((char*)"Models/CuadrosRocko/CuadroGarabatoRocko.obj");
    Model PuertaRocko((char*)"Models/Fachada/PuertaRocko.obj");

    //Cocina
    Model RefriRockoCuerpo((char*)"Models/RefriRocko/RefriRockoCuerpo.obj");
    Model RefriRockoPuerta((char*)"Models/RefriRocko/RefriRockoPuerta.obj");
    Model EstufaRockoCuerpo((char*)"Models/EstufaRocko/EstufaRockoCuerpo.obj");
    Model EstufaRockoPuerta((char*)"Models/EstufaRocko/EstufaRockoPuerta.obj");
    Model MesaCocinaRocko((char*)"Models/MesaCocinaRocko/MesaCocinaRocko.obj");
    Model LavamanosRocko((char*)"Models/LavamanosRocko/LavamanosRocko.obj");
    Model SillaRocko((char*)"Models/SillaRocko/SillaRocko.obj");

    glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);



    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();
        DoMovement();
        animacionPuerta();
        animacionRefrigerador();
        animacionLampara();
        animacionAspiradora();
        animacionEstufa();

        // Clear the colorbuffer
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Use();

        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Draw the loaded model
        glm::mat4 model = glm::mat4(1);
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
       // TVRocko.Draw(shader);
        //SillonGrandeRocko.Draw(shader);
        Fachada.Draw(shader);


        //Lampara
        model = glm::mat4(1);;
        model = glm::translate(model, glm::vec3(0.2f, 0.46f, 0.0f));
        model = glm::scale(model, glm::vec3(0.12f, 0.12f, 0.12f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotLampara), glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        LamparaRocko.Draw(shader);

        ////SillonGrandeRocko
        model = glm::mat4(1);;
        model = glm::translate(model, glm::vec3(-0.38f, 0.445f, 1.3f));
        model = glm::scale(model, glm::vec3(0.12f, 0.12f, 0.15f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        SillonGrandeRocko.Draw(shader);

        //SillonChico
        model = glm::mat4(1);;
        model = glm::translate(model, glm::vec3(0.65f, 0.46f, 0.0f));
        model = glm::scale(model, glm::vec3(0.06f, 0.06f, 0.06f));
        model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        SillonChicoRocko.Draw(shader);


        //TVRocko
        model = glm::mat4(1);;
        model = glm::translate(model, glm::vec3(1.1f, 0.395f, 1.85f));
        model = glm::scale(model, glm::vec3(0.12f, 0.12f, 0.12f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        TVRocko.Draw(shader);

        //mesaRocko
        model = glm::mat4(1);;
        model = glm::translate(model, glm::vec3(-0.00f, 0.30f, 1.35f));
        model = glm::scale(model, glm::vec3(0.18f, 0.18f, 0.18f));
        model = glm::rotate(model, glm::radians(110.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        MesaRocko.Draw(shader);

        //AspiradoraRocko
        model = glm::mat4(1);;
        model = glm::translate(model, glm::vec3(1.7f, 0.40f, 1.4f));
        model = glm::translate(model, glm::vec3(movAspiradoraX, 0, movAspiradoraZ));
        model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotAspiradora), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        AspiradoraRocko.Draw(shader);

        //CuadroRayoRocko
        model = glm::mat4(1);;
        model = glm::translate(model, glm::vec3(-0.50f, 0.86f, -0.065f));
        model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        CuadroRayoRocko.Draw(shader);

        //CuadroGarabatoRocko
        model = glm::mat4(1);;
        model = glm::translate(model, glm::vec3(-0.10f, 0.9f, 2.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        CuadroGarabatoRocko.Draw(shader);

        //PuertaPrincipal
        model = glm::mat4(1);;
        model = glm::translate(model, glm::vec3(1.39f, 0.85f, 2.1f));
        model = glm::rotate(model, glm::radians(rotPuerta), glm::vec3(0.0f, 1.0f, 0.0));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        PuertaRocko.Draw(shader);

        //RefriRockoCuerpo
        model = glm::mat4(1);;
        model = glm::translate(model, glm::vec3(0.4f, 0.80f, -1.25f));
        model = glm::scale(model, glm::vec3(0.18f, 0.18f, 0.18f));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        RefriRockoCuerpo.Draw(shader);

        //RefriRockoPuerta
        model = glm::mat4(1);;
        model = glm::translate(model, glm::vec3(0.4f, 0.80f, -1.25f));
        model = glm::scale(model, glm::vec3(0.18f, 0.18f, 0.18f));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotRefrigerador), glm::vec3(0.0f, 1.0f, 0.0));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        RefriRockoPuerta.Draw(shader);

        //EstufaRockoCuerpo
        model = glm::mat4(1);;
        model = glm::translate(model, glm::vec3(-0.10f, 0.52f, -2.2f));
        model = glm::scale(model, glm::vec3(0.18f, 0.18f, 0.18f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        EstufaRockoCuerpo.Draw(shader);

        //EstufaRockoPuerta
        model = glm::mat4(1);;
        model = glm::translate(model, glm::vec3(-0.10f, 0.52f, -2.2f));
        model = glm::scale(model, glm::vec3(0.18f, 0.18f, 0.18f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(-rotEstufa), glm::vec3(1.0f, 0.0f, 0.0));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        EstufaRockoPuerta.Draw(shader);

        //MesaCocinaRocko
        model = glm::mat4(1);;
        model = glm::translate(model, glm::vec3(1.4f, 0.45f, -1.3f));
        model = glm::scale(model, glm::vec3(0.22f, 0.20f, 0.22f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        MesaCocinaRocko.Draw(shader);

        //Silla1
        model = glm::mat4(1);;
        model = glm::translate(model, glm::vec3(1.55f, 0.45f, -1.58f));
        model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
        model = glm::rotate(model, glm::radians(75.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        SillaRocko.Draw(shader);
        //Silla2
        model = glm::mat4(1);;
        model = glm::translate(model, glm::vec3(1.3f, 0.45f, -1.58f));
        model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
        model = glm::rotate(model, glm::radians(115.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        SillaRocko.Draw(shader);
        //Silla3
        model = glm::mat4(1);;
        model = glm::translate(model, glm::vec3(1.12f, 0.45f, -1.4f));
        model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
        model = glm::rotate(model, glm::radians(150.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        SillaRocko.Draw(shader);

        //LavamanosRocko
        model = glm::mat4(1);;
        model = glm::translate(model, glm::vec3(0.7f, 0.45f, -2.65f));
        model = glm::scale(model, glm::vec3(0.26f, 0.22f, 0.24f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        LavamanosRocko.Draw(shader);

        //PuertaCocina
        model = glm::mat4(1);;
        model = glm::translate(model, glm::vec3(1.93f, 0.85f, -2.84f));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        PuertaRocko.Draw(shader);

        glBindVertexArray(0);
        // Swap the buffers
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}


// Moves/alters the camera positions based on user input
void DoMovement()
{
    // Camera controls
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
    {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }

    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }

    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
    {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }

    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
    {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }

    if (keys[GLFW_KEY_I])
    {
        abrirPuerta = true;
    }
    if (keys[GLFW_KEY_O])
    {
        cerrarPuerta = true;
    }
    if (keys[GLFW_KEY_N])
    {
        abrirRefrigerador = true;
    }
    if (keys[GLFW_KEY_M])
    {
        cerrarRefrigerador = true;
    }
    if (keys[GLFW_KEY_V])
    {
        abrirEstufa = true;
    }
    if (keys[GLFW_KEY_B])
    {
        cerrarEstufa = true;
    }
    if (keys[GLFW_KEY_K])
    {
        tirarLampara = true;
        rotLampara = 0.0f;
    }
    if (keys[GLFW_KEY_L])
    {
        circuitoAspiradora = true;
    }
    if (keys[GLFW_KEY_J])
    {
        circuitoAspiradora = false;

    }

    if (keys[GLFW_KEY_R])
    {
        rotLampara = 0.0f;
        rotPuerta = 0.0;
        rotRefrigerador = 0.0;
        rotEstufa = 0.0;
        circuitoAspiradora = false;
        movAspiradoraX = 0;
        movAspiradoraZ = 0;
        rotAspiradora = 0;
        recorrido1 = true;
        recorrido2 = false;
        recorrido3 = false;
        recorrido4 = false;


    }

}

void animacionPuerta() {
    if (abrirPuerta && rotPuerta<90)
    {
        rotPuerta += 1.0f;
        if (rotPuerta == 90)
        {
            abrirPuerta = false;
        }
    }
    else {
        abrirPuerta = false;
    }
    if (cerrarPuerta && rotPuerta>0)
    {
        rotPuerta -= 1.0f;
        if (rotPuerta == 0)
        {
            cerrarPuerta = false;
        }
    }
    else {
        cerrarPuerta = false;
    }
}

void animacionRefrigerador() {
    if (abrirRefrigerador && rotRefrigerador < 90)
    {
        rotRefrigerador += 1.0f;
        if (rotRefrigerador == 90)
        {
            abrirRefrigerador = false;
        }
    }
    else {
        abrirRefrigerador = false;
    }
    if (cerrarRefrigerador && rotRefrigerador > 0)
    {
        rotRefrigerador -= 1.0f;
        if (rotRefrigerador == 0)
        {
            cerrarRefrigerador = false;
        }
    }
    else {
        cerrarRefrigerador = false;
    }
}

void animacionEstufa() {
    if (abrirEstufa && rotEstufa < 90)
    {
        rotEstufa += 1.0f;
        if (rotEstufa == 90)
        {
            abrirEstufa = false;
        }
    }
    else {
        abrirEstufa = false;
    }
    if (cerrarEstufa && rotEstufa > 0)
    {
        rotEstufa -= 1.0f;
        if (rotEstufa == 0)
        {
            cerrarEstufa = false;
        }
    }
    else {
        cerrarEstufa = false;
    }
}

void animacionLampara() {
    if (tirarLampara && rotLampara > -90)
    {
        rotLampara -= 1.0f;
        if (rotLampara == -90)
        {
            tirarLampara = false;
        }
    }
    else {
        tirarLampara = false;        
    }
  
}

void animacionAspiradora()
{

    //Movimiento de la Aspiradora
    if (circuitoAspiradora)
    {
        if (recorrido1)
        {
            rotAspiradora = 0;
            movAspiradoraZ   -= 0.005f;
            if (movAspiradoraZ < -1)
            {
                recorrido1 = false;
                recorrido2 = true;
            }
        }
        if (recorrido2)
        {
            rotAspiradora = 90;
            movAspiradoraX -= 0.005f;

           /* movAspiradoraX += 0.007071f;
            movAspiradoraZ -= 0.007071f;*/
            if (movAspiradoraX < -1.5)
            {
                recorrido2 = false;
                recorrido3 = true;

            }
        }

        if (recorrido3)
        {
            rotAspiradora = 215;
            movAspiradoraX += 0.002867f;
            movAspiradoraZ += 0.004095f;
            if (movAspiradoraZ > 0)
            {
                recorrido3 = false;
                recorrido4 = true;
            }
        }
        if (recorrido4)
        {
            rotAspiradora = -90;
            movAspiradoraX += 0.005f;
            if (movAspiradoraX > 0)
            {
                recorrido4 = false;
                recorrido1 = true;
            }
        }

    }
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            keys[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            keys[key] = false;
        }
    }

}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}